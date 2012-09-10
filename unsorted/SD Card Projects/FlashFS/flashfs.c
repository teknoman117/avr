#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <alloca.h>
#include "spi.h"
#include "mmc.h"
#include "stdusart0.h"

#define FA_READ 0
#define FA_WRITE 1
#define FA_OPEN 0
#define FA_NEW 2

/*
 * File descriptor - 32 bytes
 * 0 - 3 - unsigned integer - start page
 * 4 - 7 - unsigned integer - end page
 * 8 - 11 - signed integer - size
 */

uint32_t cpointer = 0;
uint8_t window[512];

int put_flashfile( char c, FILE* f ) {
    int *p = &((int*)(f->udata))[0];   //get current address
    if( f->len > *p + 127 ) {
        mmc_readblock( *p, window );
        int cntr;
        for(cntr=0;cntr<128;cntr++) window[cntr] = f->buf[cntr];
        mmc_writeblock( *p, window );
        *p += 128;
    }
    f->buf[f->len++ % 128] = c;

    return 0;
}

int get_flashfile( FILE* f ) {
    int *p = &((int*)(f->udata))[0];   //get current address
    if( f->len == f->size ) return EOF;
    if( f->len > *p + 127 ) {
        mmc_readpartialblock( *p + 128, 128, (uint8_t*)f->buf );
        *p += 128;
    }
    return f->buf[f->len++ % 128];
}

FILE* open_flashfile( char* name, char mode ) {
    FILE* f = (FILE*) alloca(sizeof(FILE));
    f->udata = alloca(sizeof(int)*3);
    uint32_t a, b;
    uint32_t spage = 0;
    uint32_t epage = 0;
    int32_t size = 0;
    uint8_t *c = (uint8_t*) window;
    for(a=0;a<65536;a+=512) {
        mmc_readblock( a, window );
        for(b=0;b<512;b+=32) {
            spage = *(((uint32_t*)(c)));
            epage = *(((int32_t*)(c + 4)));
            size = *(((int32_t*)(c + 8)));
            if(!strcmp(((char*)(c+12)), name)) {
                f->len = 0;
                f->size = size;
            }
            c += 32;
        }
    }
    ((uint32_t*)(f->udata))[0] =
    return f;
}

void close_flashfile( FILE* f ) {

}

int main( void ) {

    return 0;
}
