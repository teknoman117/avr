#include <stdint.h>
uint16_t cnt;
void    initPWM( void);
void    initPWM08( void );
void    initPWM09( void );
void    initPWM10( void ); 
void    setCHA( uint16_t valueA );
void    setCHB( uint16_t valueB );
void    setCHA08( uint16_t valueA ); //sets 8 bit value
void    setCHB08( uint16_t valueB );
void    setCHA09( uint16_t valueA); //sets 9 bit value
void    setCHB09( uint16_t valueB);
void    setCHA10( uint16_t valueA); //sets 10 bit value
void    setCHB10( uint16_t valueB);
