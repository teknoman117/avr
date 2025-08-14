enum {ON,OFF,NULL};

uint16_t counttopulse[2];
uint16_t reload;
uint16_t counter[2];
uint8_t plsref[2];


void initservos( uint16_t reloadtime, uint8_t servo1en, uint8_t servo2en );
void setservo1 ( uint16_t cycles1 );
void setservo2 ( uint16_t cycles2 );