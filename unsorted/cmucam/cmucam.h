/*
 *      cmucam.h
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
 *      This header controls the CMUcam version 1
 *    
 */

#ifndef _CMUCAM_H_
#define _CMUCAM_H_

#define ON 1
#define OFF 0

#define YUV_WHITE_BALANCE_ON 36
#define YUV_WHITE_BALANCE_OFF 32
#define RGB_WHITE_BALANCE_ON 44
#define RGB_WHITE_BALANCE_OFF 40

#define FPS_17 2
#define FPS_13 3
#define FPS_11 4
#define FPS_9   5
#define FPS_8   6
#define FPS_7   7
#define FPS_6   8
#define FPS_5   10
#define FPS_4   12

#define NO_PACKET 0
#define C_PACKET 1
#define M_PACKET 2
#define N_PACKET 3
#define S_PACKET 4

#define DELAY_ACTION { __asm__ ("nop"); }  // this action is used to delay, in avr-theads, use avr_thread_yield() instead of the no operation function

// This structure defines a CMUCam 'C' type packet
struct cam_c_packet {
    unsigned char x1;
    unsigned char x2;
    unsigned char y1;
    unsigned char y2;
    unsigned char pixels;
    unsigned char confidence;
};

struct cam_m_packet {
    unsigned char mx;
    unsigned char my;
    unsigned char x1;
    unsigned char x2;
    unsigned char y1;
    unsigned char y2;
    unsigned char pixels;
    unsigned char confidence;
};

struct cam_n_packet {
    unsigned char spos;
    unsigned char mx;
    unsigned char my;
    unsigned char x1;
    unsigned char x2;
    unsigned char y1;
    unsigned char y2;
    unsigned char pixels;
    unsigned char confidence;
};

struct cam_s_packet {
    unsigned char r_mean;
    unsigned char g_mean;
    unsigned char b_mean;
    unsigned char r_deviation;
    unsigned char g_deviation;
    unsigned char b_deviation;
};

// Initalizes the cmucam driver
void init_cmucam( void );

// This command sends an instruction packet to the CMUCam
void _cmucam_command(unsigned char c1, unsigned char c2, unsigned char argc, unsigned char *argv);

// These commands set different modes of the camera
void set_cmucam_light(unsigned char state);
void set_cmucam_servo(unsigned char position);
void set_cmucam_poll_mode(unsigned char state);
void set_cmucam_centroid_mode(unsigned char state);
void set_cmucam_contrast(unsigned char value);
void set_cmucam_brightness(unsigned char value) ;
void set_cmucam_colormode(unsigned char mode);
void set_cmucam_fps(unsigned char speed);
void set_cmucam_autogain(unsigned char on);
void set_cmucam_switching_mode(unsigned char state);
void set_cmucam_window(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);

// These operations deal with NON CONTINUOUS tracking
void get_cmucam_mean(struct cam_s_packet *packet); // fetch mean data
void set_cmucam_track_colors(unsigned char rm, unsigned char rn, unsigned char gm, unsigned char gn, unsigned char bm, unsigned char bn); // uses TC
void get_cmucam_track_colors(struct cam_s_packet *packet); // uses TW

// These operations deal with CONTINUOUS tracking
void cmucam_start_tracking_mean(void); // mean
void cmucam_start_tracking_object(struct cam_s_packet *packet); // track window - returns the statistics packet it dumps first
void cmucam_start_tracking_color(unsigned char rm, unsigned char rn, unsigned char gm, unsigned char gn, unsigned char bm, unsigned char bn); // color
void cmucam_stop_tracking(); // stop tracking

// Returns 0 for no packet or the type.  Using avrthread, use  "while(!(type = query_packet(0)) avr_thread_yeild();" to allow other threads to proceed while a packet is queuing from the camera
unsigned char cmucam_query_packet(unsigned block);  // Block keeps the function stopped until a packet is available.  

// These commands fetch data packets from the buffer, they error if there is no frame available
unsigned char cmucam_fetch_c_packet(struct cam_c_packet *packet);
unsigned char cmucam_fetch_m_packet(struct cam_m_packet *packet);
unsigned char cmucam_fetch_n_packet(struct cam_n_packet *packet);
unsigned char cmucam_fetch_s_packet(struct cam_s_packet *packet);

#endif
