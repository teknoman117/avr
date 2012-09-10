#ifndef _BOOTLOADER_CONFIG_H
#define _BOOTLOADER_CONFIG_H

#include <avr/version.h>

/* check for avr-libc version */
#if __AVR_LIBC_VERSION__ < 10402UL
#error newer libc version (>= 1.4.2) needed!
#endif

/* cpu specific configuration registers */
#define _TIMSK_TIMER1 TIMSK1
#define _UDRIE_UART0 UDRIE0
#define _TXEN_UART0 TXEN
#define _RXEN_UART0 RXEN
#define _RXCIE_UART0 RXCIE0
#define _UBRRH_UART0 UBRRH
#define _UBRRL_UART0 UBRRL
#define _UCSRA_UART0 UCSRA
#define _UCSRB_UART0 UCSRB
#define _UCSRC_UART0 UCSRC
#define _UCSZ0_UART0 UCSZ00
#define _UCSZ1_UART0 UCSZ01
#define _SIG_UART_RECV_UART0 SIG_USART_RECV
#define _SIG_UART_DATA_UART0 SIG_USART_DATA
#define _UDR_UART0 UDR
#define _UDRE_UART0 UDRE
#define _RXC_UART0 RXC
#define _IVREG MCUCR
#define _TIFR_TIMER1 TIFR

#define BOOTLOADER_ENTRY_CHAR 'p'

#define BOOTLOADER_SUCCESS_CHAR 'S'

/* uart configuration */
#define UART_UCSRC (_BV(URSEL) | _BV(UCSZ0) | _BV(UCSZ1))

/* buffer load configuration */
#define BLOCKSIZE SPM_PAGESIZE

/* use 8 or 16 bit counter, according to the page size of the target device */
#if SPM_PAGESIZE < 256
#   define BUF_T uint8_t
#else
#   define BUF_T uint16_t
#endif

#endif
