#include <stdio.h>
#include <util/delay.h>

#include "cmucam.h"
#include "fakeuart.h"

void cmucam_blink() {
	    set_cmucam_light(1);
	    _delay_ms(100);
	    set_cmucam_light(0);
	    _delay_ms(100);
}

int main() {
	/* Hardware initalization */
	initFakeUart();
	fdevopen((void*)txbs, (void*)rxbs);
	printf("CMUcam Demo\r\n");
	
	// CMUCam setup
	init_cmucam();
	cmucam_blink();
	
	// Set the TC command to return central coordinates as well
	set_cmucam_centroid_mode(ON);
	cmucam_blink();
	
	// Turn off the autogain
	set_cmucam_autogain(OFF);
	cmucam_blink();
	
	// Set color mode to RGB without white balance
	set_cmucam_colormode(RGB_WHITE_BALANCE_OFF);
	cmucam_blink();
	
	// The tracking commands will alternate between tracking and statistic packets
	set_cmucam_switching_mode(ON);
	cmucam_blink();
	
	// Set the frames per second
	set_cmucam_fps(FPS_4);
	cmucam_blink();	
	
	// Set the light to default operation (on = tracking; off = not tracking)
	set_cmucam_light(2);
	
	// Loop forever (to analyze data response)
	//struct cam_m_packet packet;
	struct cam_s_packet stats;
	struct cam_m_packet location; 
	unsigned char packet_type = 0;
	while(1) {
	    // Start Tracking an object (uses TW command)
	    cmucam_start_tracking_object(&stats);
	    
	    // Loop until a keystroke
	    while(bytesInBufferS() == 0) {
	        // Block until a packet is received
	        while(!(packet_type = cmucam_query_packet(0)));
	        
	        // Check the packet type
	        if(packet_type == S_PACKET) {
	            // If the packet was a statistics packet; dump the statistics
	            cmucam_fetch_s_packet(&stats);
	            if(bytesInTXBufferS() < 100)
	                printf("S %u %u %u %u %u %u\r\n", stats.r_mean, stats.g_mean, stats.b_mean, stats.r_deviation, stats.g_deviation, stats.b_deviation);
	        } else if(packet_type == M_PACKET) {
	            // If the packet was a tracking packet; dump the tracking data
	        	cmucam_fetch_m_packet(&location);
	        	if(bytesInTXBufferS() < 100)
	                printf("M (%u, %u) ({%u, %u}, {%u, %u}) %u %u\r\n", location.mx, location.my, location.x1, location.y1, location.x2, location.y2, location.pixels, location.confidence);
	        }
	    }
	    
	    // Read the keystroke and stop tracking
	    rxbs();
	    cmucam_stop_tracking();
	    
	    // Wati for another to start again
	    rxbs();
	}
	return 0;
}
