#define MCU_FREQ 8000000UL
uint16_t convert;
uint16_t loop_count;

void     initADC( void );
uint16_t  ADC10bit( uint8_t channel );
uint16_t  ADC08bit( uint8_t channel );
void     DELAY_US( uint16_t microseconds);