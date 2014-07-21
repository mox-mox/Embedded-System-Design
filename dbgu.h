/*
** dbgu.h
*/

#ifndef _DBGU_H_
#define _DBGU_H_

/// Standard operating mode (asynchronous, 8bit, no parity, 1 stop bit)
#define DBGU_STANDARD           AT91C_US_PAR_NONE

void dbgu_init(void);

void dbgu_putc(char c);
void dbgu_puts(const char *s);

int dbgu_getc(char *c);

void dbgu_tick(void);
void dbgu_command(const char *cmd);

void dbgu_clear(void);

//------------------------------------------------------------------------------
//         Global functions
//------------------------------------------------------------------------------

extern void DBGU_Configure(
    unsigned int mode,
    unsigned int baudrate,
    unsigned int mck);

extern unsigned char DBGU_GetChar(void);

extern void DBGU_PutChar(unsigned char c);

extern unsigned int DBGU_IsRxReady(void);

#endif

