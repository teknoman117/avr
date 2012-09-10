/****************************************************************************
 Title  :   C Test program for the LCD FUNCTIONS library (test_lcd.c)
 Author:    Chris efstathiou hendrix@otenet.gr
 Date:      1/Jul/2002
 Software:  AVR-GCC with AVR-AS
 Target:    any AVR device
 Comments:  This software is FREE.

*****************************************************************************/
#define  F_CPU   3686400L

#include <avr/io.h>
#include "lcd_io.h"


void main(void)
{
  
 /* Since we also test the eeprom DONT! forget to write the test_lcd.eep to AVR's eeprom 
     otherwise the lcd will not show the eeprom_string                                       */
  static  unsigned char eeprom_string[]__attribute__((section(".eeprom")))={"lcd_puts_e testing"};
  unsigned char ram_string[]={"DONE TESTING..."};
  unsigned int pos=0;

   lcd_init();         /* lcd(s) is(are) initialized and if more than 1, lcd unit 0 gets active */

   lcd_clrscr();
   lcd_gotoxy(0,0);  lcd_puts_P("NOT VISIBLE LINE");
   lcd_clrline(0);   lcd_puts_P( "Hello World\nthis is line 2" );

#if LCD_AUTO_LINE_FEED == 1
   lcd_gotoxy(0,2);  lcd_puts_e(eeprom_string); lcd_puts(ram_string);  
#elif LCD_AUTO_LINE_FEED == 0
/*---------------------------------------------------------------------------------------------------*/
/*
   I puted this command here to test the correct saving of the lcd x,y coordinates
   between lcd unit switching, if you select mode 7 
*/
   lcd_gotoxy(0,2);
/*---------------------------------------------------------------------------------------------------*/
#if   LCD_IO_MODE == 7
   select_lcd(LCD_1);
   lcd_clrscr();
   lcd_clrline(0);   lcd_puts_P( "This is display #2" );
   select_lcd(LCD_0);
#endif
/*---------------------------------------------------------------------------------------------------*/
   lcd_puts_e(eeprom_string);
   lcd_gotoxy(0,3); lcd_puts(ram_string);  
#endif /* #elif LCD_AUTO_LINE_FEED == 0 */

   lcd_gotoxy(16,3); lcd_puti(pos,2);  pos=lcd_getxy();
   lcd_gotoxy(16,3); lcd_puti(pos,2);  
  
}
/*######################################################################################################*/
/*                                         T H E   E N D                                                */
/*######################################################################################################*/

