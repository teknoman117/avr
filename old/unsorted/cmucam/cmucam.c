/*
 *      cmucam.c
 *      
 *      Copyright 2010 Nathaniel Robert Lewis <linux.robotdude@gmail.com>
 *      
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *      
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *      
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 *
 *      This code controls the CMUcam version 1
 *    
 */

#include "cmucam.h"
#include <iusart0.h>
#include <util/delay.h>

#define CMUCAM_TRACKING_FLAG  0x80
#define CMUCAM_POLL_FLAG           0x40
#define CMUCAM_CENTROID_FLAG  0x20
#ifndef NULL
#define NULL 0
#endif

unsigned char cam_flags = 0;
unsigned char bytes[2];

void init_cmucam() {
	init_iusart0(32, DB8 | P_N | SB1);
	cam_flags = 0;
	writeStringi0("RS\r");   // Reset the camera
	_delay_ms(100);
	writeStringi0("RM 7\r"); // Set the comms mode to binary tx + rx and suppress Ack + NCK
	_delay_ms(100);
	flush_buffer0();         // Flush any extraneous data from the camera
}

void _cmucam_command(unsigned char c1, unsigned char c2, unsigned char argc, unsigned char *argv) {
    txbi0(c1);
    txbi0(c2);
    txbi0(argc);
    writei0(argv, argc);
}

void set_cmucam_light(unsigned char state) {
    // Send a light command
    bytes[0] = (state > 2) ? 2 : state;
    _cmucam_command('L', '1', 1, bytes);
    while(rxbi0() != ':') DELAY_ACTION;
}

void set_cmucam_servo(unsigned char position) {
    bytes[0] = (position > 128) ? 128 : position;
    _cmucam_command('S', '1', 1, bytes);
    while(rxbi0() != ':')  DELAY_ACTION;
}

void set_cmucam_poll_mode(unsigned char state) {
    if(state) cam_flags |= CMUCAM_POLL_FLAG;
    else cam_flags &= ~CMUCAM_POLL_FLAG;
    bytes[0] = (cam_flags & CMUCAM_POLL_FLAG) ? 1 : 0;
    _cmucam_command('P', 'M', 1, bytes);
    while(rxbi0() != ':')  DELAY_ACTION;
}

void set_cmucam_centroid_mode(unsigned char state) {
    if(state) cam_flags |= CMUCAM_CENTROID_FLAG;
    else cam_flags &= ~CMUCAM_CENTROID_FLAG;
    bytes[0] = (cam_flags & CMUCAM_CENTROID_FLAG) ? 1 : 0;
    _cmucam_command('M', 'M', 1, bytes);
    while(rxbi0() != ':') DELAY_ACTION;
}

void set_cmucam_contrast(unsigned char value) {
    bytes[0] = 5;
    bytes[1] = value;
    _cmucam_command('C', 'R', 2, bytes);
    while(rxbi0() != ':')  DELAY_ACTION;
}

void set_cmucam_brightness(unsigned char value) {
    bytes[0] = 6;
    bytes[1] = value;
    _cmucam_command('C', 'R', 2, bytes);
    while(rxbi0() != ':')  DELAY_ACTION;
}

void set_cmucam_colormode(unsigned char mode) {
    bytes[0] = 18;
    bytes[1] = mode;
    _cmucam_command('C', 'R', 2, bytes);
    while(rxbi0() != ':') DELAY_ACTION;
}

void set_cmucam_fps(unsigned char speed) {
    bytes[0] = 17;
    bytes[1] = speed;
    _cmucam_command('C', 'R', 2, bytes);
    while(rxbi0() != ':') DELAY_ACTION;
}

void set_cmucam_autogain(unsigned char on) {
    bytes[0] = 19;
    bytes[1] = on ? 33 : 32;
    _cmucam_command('C', 'R', 2, bytes);
    while(rxbi0() != ':') DELAY_ACTION;
}

void set_cmucam_switching_mode(unsigned char state) {
    // Send a light command
    bytes[0] = state ? 1 : 0;
    _cmucam_command('S', 'M', 1, bytes);
    while(rxbi0() != ':') DELAY_ACTION;
}

void set_cmucam_window(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2) {
    // Set the tracking window
	txbi0('S');
	txbi0('W');
	txbi0(4);
	txbi0(x1);
	txbi0(y1);
	txbi0(x2);
	txbi0(y2);
}

void get_cmucam_mean(struct cam_s_packet *packet) {
    // Check poll flag, we need it on for this operation
    if(!(cam_flags & CMUCAM_POLL_FLAG)) set_cmucam_poll_mode(ON);
    
    // Issue the get mean command
    _cmucam_command('G', 'M', 0, NULL);
    
    // Wait for the statistics packet to be returned and then dump the colon
    while(!cmucam_fetch_s_packet(packet)) DELAY_ACTION;
    while(rxbi0() != ':') DELAY_ACTION;
}

void cmucam_start_tracking_mean(void) {
    // Check poll mode flag, if it is set, clear it - we want continuous tracking
    if(cam_flags & CMUCAM_POLL_FLAG) set_cmucam_poll_mode(OFF);
	cam_flags |= CMUCAM_TRACKING_FLAG;
    
    // Issue a tracking command
    _cmucam_command('G', 'M', 0, NULL);
}

void cmucam_start_tracking_object(struct cam_s_packet *packet) {
     // Check poll mode flag, if it is set, clear it - we want continuous tracking
    if(cam_flags & CMUCAM_POLL_FLAG) set_cmucam_poll_mode(OFF);
	cam_flags |= CMUCAM_TRACKING_FLAG;
    
    // Issue the track window command
    _cmucam_command('T', 'W', 0, NULL);
    
    // Get the first data packet, will be a statistic packet
    while(!cmucam_fetch_s_packet(packet)) DELAY_ACTION;
}

void cmucam_start_tracking_color(unsigned char rm, unsigned char rn, unsigned char gm, unsigned char gn, unsigned char bm, unsigned char bn) {
    // Check poll mode flag, if it is set, clear it - we want continuous tracking
    if(cam_flags & CMUCAM_POLL_FLAG) set_cmucam_poll_mode(OFF);

	// Flag the tracking bit
	cam_flags |= CMUCAM_TRACKING_FLAG;
	txbi0('T');
	txbi0('C');
	txbi0(6);
	txbi0(rm);
	txbi0(rn);
	txbi0(gm);
	txbi0(gn);
	txbi0(bm);
	txbi0(bn);
	// We have begun to track the color
}

void cmucam_stop_tracking() {
    cam_flags &= ~CMUCAM_TRACKING_FLAG;
    txbi0(13);  // Stops tracking
    while(rxbi0() != ':') DELAY_ACTION;  // Dump any remaining tracking data
}

unsigned char cmucam_query_packet(unsigned block) {
    if(bytesInBuf0() < 2) return 0;
    if(peeki0(0) != 255) return 0;  /* Error has occurred, no aligned packets */
    char type = peeki0(1); // Get the packet type
    if(type == 'C') {
        if(bytesInBuf0() < 8) return 0;
        else return C_PACKET;
    } else if(type == 'M') {
        if(bytesInBuf0() < 10) return 0;
        else return M_PACKET;
    } else if(type == 'N') {
        if(bytesInBuf0() < 11) return 0;
        else return N_PACKET;
    } else if(type == 'S') {
        if(bytesInBuf0() < 8) return 0;
        else return S_PACKET;
    }
    return 0;
}

unsigned char cmucam_fetch_c_packet(struct cam_c_packet *packet) {
    // Make sure we have a packet and that it is a C packet
    unsigned char type = cmucam_query_packet(0);
    if(!type || type != C_PACKET) return 0;
    
    // Fetch the data
    rxbi0(); rxbi0();  // Dump the alignment byte and the type byte
    packet->x1 = rxbi0();
    packet->y1 = rxbi0();
    packet->x2 = rxbi0();
    packet->y2 = rxbi0();
    packet->pixels = rxbi0();
    packet->confidence = rxbi0();
    
    // Return success
    return 1;
}

unsigned char cmucam_fetch_m_packet(struct cam_m_packet *packet) {
    // Make sure we have a packet and that it is a C packet
    unsigned char type = cmucam_query_packet(0);
    if(!type || type != M_PACKET) return 0;
    
    // Fetch the data
    rxbi0(); rxbi0();  // Dump the alignment byte and the type byte
    packet->mx = rxbi0();
    packet->my = rxbi0();
    packet->x1 = rxbi0();
    packet->y1 = rxbi0();
    packet->x2 = rxbi0();
    packet->y2 = rxbi0();
    packet->pixels = rxbi0();
    packet->confidence = rxbi0();
    
    // Return success
    return 1;
}

unsigned char cmucam_fetch_s_packet(struct cam_s_packet *packet) {
    // Make sure we have a packet and that it is a C packet
    unsigned char type = cmucam_query_packet(0);
    if(!type || type != S_PACKET) return 0;
    
    // Fetch the data
    rxbi0(); rxbi0();  // Dump the alignment byte and the type byte
    packet->r_mean = rxbi0();
    packet->g_mean = rxbi0();
    packet->b_mean = rxbi0();
    packet->r_deviation = rxbi0();
    packet->g_deviation = rxbi0();
    packet->b_deviation = rxbi0();
    
    // Return success
    return 1;
}
