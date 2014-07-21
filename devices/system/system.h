/*
 * Parts of this source code is adapted from Martin Thomas
 * and the Atmel Team with adaptions made by Maximilian Thürmer
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
 
#ifndef SYSTEM_H_
#define SYSTEM_H_

//#include <rtt.h>
//------------------------------------------------------------------------------
//      USB Controller
//------------------------------------------------------------------------------
    #define UDP										//udp is present...
    #define FULLSPEED								//... and has full speed capabilities
    #define USB_ENDPOINT0_MAXPACKETSIZE       8

//------------------------------------------------------------------------------
//      PITC
//------------------------------------------------------------------------------

#define RTTC_INTERRUPT_LEVEL   0
#define PIV_200_MS             600000  //* 200 ms for 48 MHz


//------------------------------------------------------------------------------
//      Prototypes needed to implement USB Init, Suspend and Resume
//------------------------------------------------------------------------------

extern void System_Init(void);
extern void DEV_Init(void);
extern void DEV_Resume(void);
extern void DEV_Suspend(void);

/* initiation routines for devices w/o subdir */
void AT91F_PIO_Init(void);
void AT91F_PITC_Init(void* Periodic_Interval_Timer_handler);


/* functions to present system information or for debugging purposes */
void showsp(void);
void dump_interrupt_state(void);
void dump_mem(unsigned long startaddress, int n);

#endif /*SYSTEM_H_*/
