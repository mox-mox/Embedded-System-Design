/*
** board.c
*/

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <board.h>
#include <dbgu.h>
#include <sdram.h>

void delayus(uint32_t us)
{
    register uint32_t i,j;

    // one loop takes 2 cpu cycles = 10ns -> 100 nops
    for(i=0; i < us; i++) {
    	 for(j=0; j < 24; j++) {
	 	asm volatile ("nop");
	 	asm volatile ("nop");
	 	asm volatile ("nop");
	 	asm volatile ("nop");
    	 }
    }
}

void delayms(uint32_t ms) 
{
    uint32_t i;
    for(i=0; i<ms; i++) {
 		delayus(1000);
    }
}



