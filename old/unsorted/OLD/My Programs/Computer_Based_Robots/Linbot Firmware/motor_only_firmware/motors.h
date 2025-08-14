#include <stdint.h>

void initMotors (void);
void fwd (uint8_t pwr);
void bwd (uint8_t pwr);
void lft (uint8_t pwr);
void rgh (uint8_t pwr);
void slft (uint8_t pwr);
void srgh (uint8_t pwr);
void blft (uint8_t pwr);
void brgh (uint8_t pwr);
void stop (void);
int isMoving (void);
 