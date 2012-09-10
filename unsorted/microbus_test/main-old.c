/* Have the sprintf() + writeStringi0() statements to a printf() statement using a bitbang system */

#include <avr/interrupt.h>

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <crc32.h>
#include <eeprom.h>
#include <iusart0.h>
#include <timer0ctc.h>

#define PACKET_TYPE_TOKENSEND  0x0f
#define PACKET_TYPE_TOKENACPT  0x1e
#define PACKET_TYPE_USER       0x2d
#define PACKET_TYPE_REGISTER   0x3c
#define PACKET_TYPE_COREACTION 0x4b
#define PACKET_TYPE_RESET      0x5a

typedef struct {
    uint8_t sender;
    uint8_t receiver;
    uint8_t datasize;
    uint8_t data[246];   // this makes the packet 256 bytes, use 118 for 128 byte packet (allocation)
    uint32_t crc32;
    uint8_t type;
    void *nextPacket;
} Packet;

// Packet receiving vriables
Packet *tmpPacket = NULL;
uint8_t downloadStage = 0;
uint8_t dataStage = 0;

// Buffers and State
Packet *packetBuffer = NULL;
Packet *txPacketBuffer = NULL;
uint32_t systemTimeMS = 0;
uint32_t nextTime = 0;

// Token System
uint8_t hasToken = 0x00;   // 0 - no token, 1 - has token, 2 - attempting to get rid of token
uint8_t myAddress = 0x01;
uint8_t nextAddress = 0x00;
uint8_t addressTries = 0x00;
uint8_t hasSetNextAddress = 0x00;

// Constants
const char *friendlyName = "A Demo Microbus Node\0";

ISR(TIMER0_COMPA_vect, ISR_NOBLOCK) {  // Increases system time every ms
    systemTimeMS++;
}

// Packet Operations
void transmitPacket (Packet *packet) {
    txbi0(packet->type);  
    txbi0(packet->sender);
    txbi0(packet->receiver);
    txbi0(packet->datasize);
    if(packet->datasize > 0) {
        writei0(packet->data, packet->datasize);
        packet->crc32 = generate_crc32(0x00000000, packet->data, packet->datasize);
        txbi0((uint8_t)((packet->crc32 >> 24) & 0x000000FF)); 
        txbi0((uint8_t)((packet->crc32 >> 16) & 0x000000FF));
        txbi0((uint8_t)((packet->crc32 >> 8) & 0x000000FF));  
        txbi0((uint8_t)(packet->crc32 & 0x000000FF)); 
    }
}

// Microbus Operations
int interpret_packet( Packet *packet ) {
    /* This code segment interprets packets */ 
    Packet *testPacket = (Packet *)malloc(sizeof(Packet));
    testPacket->datasize = 16;
    testPacket->sender = myAddress;
    testPacket->receiver = 0;
    testPacket->nextPacket = NULL;
    testPacket->type = PACKET_TYPE_USER;
    int c = 0;
    for(c=0;c<16;c++) testPacket->data[c] = (rand() % 94) + 32;

    if(!txPacketBuffer) txPacketBuffer = testPacket;
    else {
        Packet *aPacket = txPacketBuffer;
        while(aPacket->nextPacket) aPacket = aPacket->nextPacket;
        aPacket->nextPacket = testPacket;
    }
    return 0;
}

void idle_action( void ) {
    /* This action runs when no other action is */
}
 
void microbus_main (void) {
    // Queue the registration packet
    Packet *registerPacket = (Packet *)malloc(sizeof(Packet));
    registerPacket->sender = myAddress;
    registerPacket->receiver = 0; // Master
    registerPacket->datasize = strlen(friendlyName) + 4;
    registerPacket->type = PACKET_TYPE_REGISTER;
    registerPacket->data[0] = eeprom_read(0x0000); // Get serial #
    registerPacket->data[1] = eeprom_read(0x0001);
    registerPacket->data[2] = eeprom_read(0x0002);
    registerPacket->data[3] = eeprom_read(0x0003);
    strcpy(&registerPacket->data[4], friendlyName);
    txPacketBuffer = registerPacket;
    // Start Microbus
    flush_buffer0();
    //uint32_t aInt = 0;
    while (1) {
        /* Check for download data */
        while(bytesInBuf0() > 0) {
            switch (downloadStage) {
                case 0: // make packet of type
                    tmpPacket = (Packet *) malloc(sizeof(Packet));
                    tmpPacket->nextPacket = NULL;
                    tmpPacket->type = rxbi0();
                    tmpPacket->crc32 = 0;
                    downloadStage = 1;
                    dataStage = 0;
                    break;
                case 1: // get sender
                    tmpPacket->sender = rxbi0();
                    downloadStage = 2;
                    break;
                case 2: // get receiver
                    tmpPacket->receiver = rxbi0();
                    downloadStage = 3;
                    break;
                case 3: // get datasize
                    tmpPacket->datasize = rxbi0();
                    if(!tmpPacket->datasize) {   
                        downloadStage = 0;
                        // Is packet for me?
                        if(tmpPacket->receiver == myAddress) {
                            if(tmpPacket->type == PACKET_TYPE_TOKENSEND) {
                                // Reply to Transfer (use temp packet as not to allocate more)
                                tmpPacket->type = PACKET_TYPE_TOKENACPT;
                                tmpPacket->receiver = tmpPacket->sender;
                                tmpPacket->sender = myAddress;
                                transmitPacket(tmpPacket);
                                // Finally destroy it
                                free(tmpPacket);
                                hasToken = 0x01;
                            } else if(tmpPacket->type == PACKET_TYPE_TOKENACPT) {
                                if(hasToken == 0x02) {
                                    nextAddress = tmpPacket->sender;
                                    hasSetNextAddress = 1;
                                    hasToken = 0x00;
                                }
                                free(tmpPacket);
                            } else {
                                Packet *aPacket = packetBuffer;
                                if(aPacket == NULL) packetBuffer = tmpPacket;
                                else {
                                    while(aPacket->nextPacket) aPacket = (Packet *) aPacket->nextPacket;
                                    aPacket->nextPacket = tmpPacket;
                                }
                            }
                        } else free(tmpPacket); // If not, destroy packet 
                    } else downloadStage = 4;
                    break;
                case 4: // get data
                    tmpPacket->data[dataStage++] = rxbi0();
                    if(dataStage >= tmpPacket->datasize) {
                        downloadStage = 5;
                        dataStage = 0;
                    }
                    break;
                case 5: // get checksum
                    // Download a byte
                    tmpPacket->crc32 |= rxbi0();
                    dataStage++;
                    // Check for checksum completion
                    if(dataStage >= 4) {
                        dataStage = downloadStage = 0;
                        // Is packet for me?
                        if(tmpPacket->receiver == myAddress) {
                            uint32_t verify = generate_crc32(0x00000000, tmpPacket->data, tmpPacket->datasize);
                            // If the CRC32 verifies, add it to buffer
                            if(verify == tmpPacket->crc32) {
                                Packet *aPacket = packetBuffer;
                                if(aPacket == NULL) packetBuffer = tmpPacket;
                                else {
                                    while(aPacket->nextPacket) aPacket = (Packet *) aPacket->nextPacket;
                                    aPacket->nextPacket = tmpPacket;
                                }
                            } else free(tmpPacket); // If not, destroy packet
                        } else free(tmpPacket); // If not, destroy packet
                    } else tmpPacket->crc32 <<= 8;
                    break;
                default: break;
            }
        }
        /* Check whether we need to send token */
        if((hasToken == 0x02) && (systemTimeMS >= nextTime)) {
            Packet *tokenPacket = (Packet *) malloc (sizeof(Packet));
            tokenPacket->type = PACKET_TYPE_TOKENSEND;
            tokenPacket->sender = myAddress;
            tokenPacket->receiver = addressTries++;
            tokenPacket->datasize = 0;
            transmitPacket(tokenPacket);
            free(tokenPacket);
            nextTime = systemTimeMS + 10;
        } else if(txPacketBuffer && (hasToken == 0x01)) {
            // Transmit a packet if we have the token
            Packet *txPacket = txPacketBuffer;
            transmitPacket(txPacket);
            txPacketBuffer = txPacketBuffer->nextPacket;
            free(txPacket);
            // If no packets left, give up token
            if(!txPacketBuffer) {
                nextTime = 0;
                hasToken = 0x02;
                if(hasSetNextAddress) addressTries = nextAddress;
                else addressTries = myAddress + 1;
            }
            continue;
        } else if (hasToken == 0x01) {  
            nextTime = 0;
            hasToken = 0x02;
            if(hasSetNextAddress) addressTries = nextAddress;
            else addressTries = myAddress + 1;
        }
        /* Check for packets to interpret */
        if(packetBuffer) {
            Packet *packet = packetBuffer;
            switch (packet->type) {
                case PACKET_TYPE_USER:
                    interpret_packet(packet);
                    break;  
                case PACKET_TYPE_REGISTER:
                    break;  
                default: break;                 
            }
            packetBuffer = (Packet *) packet->nextPacket;
            free(packet);
            continue;
        }
        /* Run an idle action */
        idle_action();
    }
}

/* Initalize the AVR & Microbus network */
int main ( int argc, char** argv ) {
    init_iusart0(/*BAUD(57600)*/ 10, DB8 | P_N | SB1);  // 2 is 230.4 kBaud at 11.0592 MHz
    init_timer0_ctc(T0_PRESCALER_256, PIN_DISCONNECT, PIN_DISCONNECT);
    set_ocr0a(43);  // 78 - 1 ms @ 20 Mhz
    enable_interrupt_t0a();
    microbus_main();
    while (1) ;
    return 0;
}
