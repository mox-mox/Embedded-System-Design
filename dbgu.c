/*
** dbgu.c
*/

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <board.h>
#include <dbgu.h>

#define DBGU_RX_BUFFER_SIZE	64

char dbgu_rx_buffer[DBGU_RX_BUFFER_SIZE];
uint32_t dbgu_rx_buffer_fill = 0;

void dbgu_init(void)
{
	/* Open PIO for DBGU */
	AT91F_DBGU_CfgPIO();

	/* Configure DBGU */
	AT91F_US_Configure(
		(AT91PS_USART)AT91C_BASE_DBGU,	// DBGU base address
		MCK,
		AT91C_US_ASYNC_MODE,			// Mode Register to be programmed
		AT91C_DBGU_BAUD,				// Baudrate to be programmed
		0);								// Timeguard to be programmed

	/* Enable Transmitter & receivier */
	AT91C_BASE_DBGU->DBGU_CR = AT91C_US_RXEN | AT91C_US_TXEN;
	
	/* clear rx buffer */
	dbgu_rx_buffer_fill = 0;
}

void dbgu_putc(char c)
{
	if(c == '\n') dbgu_putc('\r');

	while(!AT91F_US_TxReady((AT91PS_USART)AT91C_BASE_DBGU));
	AT91F_US_PutChar((AT91PS_USART)AT91C_BASE_DBGU, c);
}

void dbgu_puts(const char *s)
{
	while(*s) {
		dbgu_putc(*s++);
	}
}

int dbgu_getc(char *c)
{
	if(!AT91F_US_RxReady((AT91PS_USART)AT91C_BASE_DBGU)) {
		return 0;
	}

	*c = AT91F_US_GetChar((AT91PS_USART)AT91C_BASE_DBGU);
	return 1;
}

void dbgu_tick(void)
{
	for(;;) {
		char c;
		if(!dbgu_getc(&c)) return;
		if(c == 10) continue;
		
		if(c == 8) {
			if(dbgu_rx_buffer_fill > 0) {
				dbgu_rx_buffer_fill--;
				dbgu_putc(8);
				dbgu_putc(' ');
				dbgu_putc(8);
			}
		}
		else if(c == 13) {
			dbgu_putc(10);
			dbgu_putc(13);
			dbgu_rx_buffer[dbgu_rx_buffer_fill] = 0;
			dbgu_command(dbgu_rx_buffer);
			dbgu_rx_buffer_fill = 0;
		}
		else {
			if(dbgu_rx_buffer_fill < (DBGU_RX_BUFFER_SIZE-1)) {
				dbgu_rx_buffer[dbgu_rx_buffer_fill++] = c;
				dbgu_putc(c);
			}
		}
	}
}

void dbgu_clear(void)
{
	dbgu_putc(27);
	dbgu_putc('[');
	dbgu_putc('2');
	dbgu_putc('J');
}

