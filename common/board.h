/*
** board.h
*/

#ifndef _BOARD_H_
#define _BOARD_H_

#define __inline static inline

#include <stdint.h>
#include <stddef.h>
#include <AT91SAM9XE512.h>
#include <lib_AT91SAM9XE512.h>
#include <common.h>


#define _WLAN_ARM  /* version 2012 wlan interface */

// Master Clock
#define EXT_OC          18432000   // External oscillator MAINCK
#define MCK             96109714   // MCK (PLLRC div by 2)
#define MCKKHz          (MCK/1000) // clocks per ms
#define SLOWCLOCK       32768


// Main oscillator startup time (in number of slow clock ticks).
#define BOARD_OSCOUNT           (AT91C_CKGR_OSCOUNT & (64 << 8))
// PLLA frequency range.
#define BOARD_CKGR_PLLA         (AT91C_CKGR_SRCA | AT91C_CKGR_OUTA_2)
// PLLA startup time (in number of slow clock ticks).
#define BOARD_PLLACOUNT         (63 << 8)
// PLLA MUL value.
#define BOARD_MULA              (AT91C_CKGR_MULA & (96 << 16))
// PLLA DIV value.
//#define BOARD_DIVA              (AT91C_CKGR_DIVA & 9)
#define BOARD_DIVA              (AT91C_CKGR_DIVA & 9)
// PLLB frequency range
#define BOARD_CKGR_PLLB         AT91C_CKGR_OUTB_1
// PLLB startup time (in number of slow clock ticks).
#define BOARD_PLLBCOUNT         BOARD_PLLACOUNT
// PLLB MUL value.
#define BOARD_MULB              (124 << 16)
// PLLB DIV value.
#define BOARD_DIVB              12
// USB PLL divisor value to obtain a 48MHz clock.
#define BOARD_USBDIV            AT91C_CKGR_USBDIV_2
// Master clock prescaler value.
#define BOARD_PRESCALER         AT91C_PMC_MDIV_2

// USART Definition
#define DBGU_RXD		AT91C_PA9_DRXD	#AT91C_PIO_PA9
#define DBGU_TXD		AT91C_PA10_DTXD	#AT91C_PIO_PA10

#ifndef AT91C_DBGU_BAUD
#define AT91C_DBGU_BAUD 115200		// DBGU Baud Rate
#endif

#define HEAP_SIZE		(SDRAM_SIZE / 2)
size_t heap_usage(void); /* from syscalls.c */

void delayus(uint32_t us);
void delayms(uint32_t ms);

#define IRQ_MASK 		0x00000080
#define FIQ_MASK 		0x00000040
#define IRQ_FIQ_MASK	(IRQ_MASK | FIQ_MASK)


static inline uint32_t get_cpsr(void)
{
	uint32_t retval;
	asm volatile (".align                \n\t"
            	  ".arm                  \n\t"
			      "mrs %0, cpsr" : "=r" (retval) :  ); // no inputs
	return retval;
}

static inline void set_cpsr(uint32_t val)
{
	asm volatile (".align                \n\t"
                  ".arm                  \n\t"
			      "msr cpsr, %0" :  : "r" (val) );// no outputs
}


/*
       asm volatile
          (
            ".align                \n\t"
            ".arm                  \n\t"
            "mrs     r0,     cpsr  \n\t"
            "msr     cpsr_c, r0    \n\t"
            "pop     { r0 }"
          );
   */

char __aeabi_unwind_cpp_pr0[0];

static inline void enable_irq(void)				{	set_cpsr(get_cpsr() & ~IRQ_MASK);		}
static inline void disable_irq(void)			{	set_cpsr(get_cpsr() |  IRQ_MASK);		}
static inline void enable_fiq(void)				{	set_cpsr(get_cpsr() & ~FIQ_MASK);		}
static inline void disable_fiq(void)			{	set_cpsr(get_cpsr() |  FIQ_MASK);		}
static inline void enable_irq_and_fiq(void)		{	set_cpsr(get_cpsr() & ~IRQ_FIQ_MASK);	}
static inline void disable_irq_and_fiq(void) 	{	set_cpsr(get_cpsr() |  IRQ_FIQ_MASK);	}

static inline void reset(void) {
	AT91C_BASE_RSTC->RSTC_RCR = (0xA5 << 24) | AT91C_RSTC_PROCRST | AT91C_RSTC_PERRST;
}

#endif

