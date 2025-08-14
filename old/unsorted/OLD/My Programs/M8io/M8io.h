#define I 0
#define O 1
uint8_t prtb;
uint8_t prtc;
uint8_t prtd;
uint8_t dirb;
uint8_t dirc;
uint8_t dird;

void   initIO( void );
void   setIOB(uint8_t pin , uint8_t direction);
void   setIOC(uint8_t pin , uint8_t direction);
void   setIOD(uint8_t pin , uint8_t direction);
void   setStateB(uint8_t pin , uint8_t state);
void   setStateC(uint8_t pin , uint8_t state);
void   setStateD(uint8_t pin , uint8_t state);
uint8_t getStateB(uint8_t pin);
uint8_t getStateC(uint8_t pin);
uint8_t getStateD(uint8_t pin);
