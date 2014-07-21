/*
 * Parts of this source code is adapted from Martin Thomas
 * and the Atmel Team with adaptions made by Maximilian Th�rmer
 * Disclaimer below
 */
/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support  -  ROUSSET  -
 * ----------------------------------------------------------------------------
 * Copyright (c) 2006, Atmel Corporation

 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaiimer below.
 *
 * - Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the disclaimer below in the documentation and/or
 * other materials provided with the distribution.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */
 
#include <stdio.h>
#include <board.h>
#include <dbgu.h>
#include "system.h"
#include "../spi/spi.h"
#include "../../swi/swi.h"
#include "dclass_driver.h"



//this device doesnt require initiation on detection of USB Attatchment
void DEV_Init(void){}

//though it requires initiation on bootup, call init() functions for all devices from here
void System_Init(void)
{
	AT91F_DBGU_Init();
	AT91F_PIO_Init();
	AT91F_SPI_Init(AT91C_BASE_SPI);
//	Dclass_USB_Init();
	  
}


/* suspend and resume of the device would be inserted here.
 * Since this is a standalone device, they are not needed */
void DEV_Resume(void){}
void DEV_Suspend() {}


void AT91F_PIO_Init(void)
{
	AT91F_PMC_EnablePeriphClock ( AT91C_BASE_PMC, 1 << AT91C_ID_PIOA ) ;
	AT91F_PMC_EnablePeriphClock ( AT91C_BASE_PMC, 1 << AT91C_ID_PIOB ) ;
}


 void AT91F_PITC_Init(void* Periodic_Interval_Timer_handler)
{
	AT91F_AIC_ConfigureIt ( AT91C_BASE_AIC, AT91C_ID_SYS, RTTC_INTERRUPT_LEVEL,AT91C_AIC_SRCTYPE_INT_POSITIVE_EDGE, Periodic_Interval_Timer_handler);
	AT91C_BASE_PITC->PITC_PIMR = AT91C_PITC_PITEN | AT91C_PITC_PITIEN | PIV_200_MS;  //  IRQ enable CPC
	AT91F_AIC_EnableIt (AT91C_BASE_AIC, AT91C_ID_SYS);
}


/* functions to present system information or for debugging purposes */

void showsp(void)
{
	unsigned long mysp;
	
	asm volatile ("mov %0, r13" : "=r" (mysp) );
	iprintf("SP = 0x%08lx\n", mysp);
}

void dump_interrupt_state(void)
{
	unsigned long cpsr;
	const unsigned long I_Bit = 0x80;
	const unsigned long F_Bit	= 0x40;
	
	cpsr = IntGetCPSR();
	
	iprintf("State : stat-reg 0x%08x -> ", cpsr);
	
	if ( cpsr & I_Bit ) {
		iprintf("IRQ disabled, ");
	}
	else {
		iprintf("IRQ enabled, ");
	}
	
	if ( cpsr & F_Bit ) {
		iprintf("FIQ disabled\n");
	}
	else {
		iprintf("FIQ enabled\n");
	}
}

void dump_mem(unsigned long startaddress, int n)
{
	volatile unsigned long *p;
	unsigned long val;
	
	p = (unsigned long*)startaddress;
	int i;
	
	for (i=0; i<n; i++) {
		val = *p;
		iprintf("Addr:0x%08lx = 0x%08lx (0x%04x:0x%04x)\n", 
				p, val, val>>16, val&0xffff);
		p++;
	}
}
