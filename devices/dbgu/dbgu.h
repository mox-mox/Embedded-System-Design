//*----------------------------------------------------------------------------
//*         ATMEL Microcontroller Software Support  -  ROUSSET  -
//*----------------------------------------------------------------------------
//* The software is delivered "AS IS" without warranty or condition of any
//* kind, either express, implied or statutory. This includes without
//* limitation any warranty or condition with respect to merchantability or
//* fitness for any particular purpose, or against the infringements of
//* intellectual property rights of others.
//*----------------------------------------------------------------------------
//* File Name           : Debug.h
//* Object              : Debug menu
//* Creation            : JPP   02/Jul/2004
//*----------------------------------------------------------------------------

#ifndef dbgu_h_
#define dbgu_h_

#ifndef AT91C_DBGU_BAUD
#define AT91C_DBGU_BAUD 115200
//	#define AT91C_DBGU_BAUD 57600
#endif

// mthomas: set to 0 to save memory if DBUG_scanf is not needed
#define AT91_DBGU_SCANF_ENABLE 1
#define DBGU_BUFFER_SIZE 128


//* ----------------------- External Function Prototype -----------------------
void AT91F_DBGU_Init(void);
void AT91F_DBGU_Printk(	char *buffer);
int AT91F_US_Get(char *val);
void AT91F_DBGU_scanf(char * type,unsigned int * val);
//int fputc(int out, FILE *f);

//debug routines used by Atmel USB source code

#define TR_INFO	1
#define TR_WARNING	1
#define TR_ERROR	1
#define TR_FATAL	1
//#define TR_DEBUG_H	1
//#define TR_DEBUG_M  1        // Class-level debug
//#define TR_DEBUG_L  1        // USB-level debug

#define TRACE_INIT()    AT91F_DBGU_Init()

#if defined(TR_DEBUG_H)
    #define TRACE_DEBUG_H(...)      printf(__VA_ARGS__)
#else
    #define TRACE_DEBUG_H(...)
#endif // TR_DEBUG_H

#if defined(TR_DEBUG_M)
    #define TRACE_DEBUG_M(...)      printf(__VA_ARGS__)
#else
    #define TRACE_DEBUG_M(...)
#endif // TR_DEBUG_M

#ifdef TR_DEBUG_L
    #define TRACE_DEBUG_L(...)      printf(__VA_ARGS__)
#else
    #define TRACE_DEBUG_L(...)
#endif // TR_DEBUG_L

#if defined(TR_DEBUG_L) || defined(TR_DEBUG_M) || defined(TR_DEBUG_H)
    #define TRACE_DEBUG_ALL(...)    printf(__VA_ARGS__)
#else
    #define TRACE_DEBUG_ALL(...)	
#endif

#ifdef TR_INFO
    #define TRACE_INFO(...)         printf(__VA_ARGS__)
#else
    #define TRACE_INFO(...)
#endif // TR_INFO

#ifdef TR_WARNING
    #define TRACE_WARNING(...)      printf(__VA_ARGS__)
#else
    #define TRACE_WARNING(...)
#endif // TR_WARNING

#ifdef TR_ERROR
    #define TRACE_ERROR(...)        printf(__VA_ARGS__)
#else
    #define TRACE_ERROR(...)
#endif // TR_ERROR

#ifdef TR_FATAL
    #define TRACE_FATAL(...)        printf(__VA_ARGS__)
#else
    #define TRACE_FATAL(...)
#endif // TR_FATAL




#endif /* dbgu_h_ */
