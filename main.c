/*
** main.c
*/


#include <board.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <dbgu.h>


#define LED_A          AT91C_PIO_PB3
#define LED_B          AT91C_PIO_PC18
#define LED_C          AT91C_PIO_PC20
#define LED_APIO       AT91C_BASE_PIOB
#define LED_BPIO       AT91C_BASE_PIOC
#define LED_CPIO       AT91C_BASE_PIOC
#define LED_EN		   AT91C_PIO_PA26
#define LED_ENPIO	   AT91C_BASE_PIOA

#define LED_0          AT91C_PIO_PB8      //low active
#define LED_0PIO       AT91C_BASE_PIOB
#define LED_1          AT91C_PIO_PC8      //low active
#define LED_1PIO       AT91C_BASE_PIOC

#define LED_VS1053_CONTROL  1
#define LED_VS1053_DATA     2
#define LED_ADC             3   //gibts leider nicht!
#define LED_FLASH           4
#define LED_WLAN            5
#define LED_DAC             6
#define LED_MMC             7
#define LED_TEST0           8
#define LED_TEST1           9


void led_init_io (void){
//spi LEDs
	AT91F_PIO_CfgOutput(LED_ENPIO,LED_EN);
	AT91F_PIO_CfgOutput(LED_APIO,LED_A);
	AT91F_PIO_CfgOutput(LED_BPIO,LED_B);
	AT91F_PIO_CfgOutput(LED_CPIO,LED_C);
//Test LEDs
	AT91F_PIO_CfgOutput(LED_0PIO,LED_0);
	AT91F_PIO_CfgOutput(LED_1PIO,LED_1);

}


void led_on (unsigned char led){

	AT91F_PIO_SetOutput(LED_ENPIO,LED_EN);
	AT91F_PIO_SetOutput(LED_0PIO,LED_0);
	AT91F_PIO_SetOutput(LED_0PIO,LED_1);
	switch(led)
		{
		case LED_VS1053_CONTROL:
			AT91F_PIO_SetOutput(LED_APIO,LED_A);
			AT91F_PIO_ClearOutput(LED_BPIO,LED_B);
			AT91F_PIO_ClearOutput(LED_CPIO,LED_C);
			AT91F_PIO_ClearOutput(LED_ENPIO,LED_EN);
			break;
		case LED_VS1053_DATA:
			AT91F_PIO_ClearOutput(LED_APIO,LED_A);
			AT91F_PIO_SetOutput(LED_BPIO,LED_B);
			AT91F_PIO_ClearOutput(LED_CPIO,LED_C);
			AT91F_PIO_ClearOutput(LED_ENPIO,LED_EN);
			break;
		case LED_FLASH:
			AT91F_PIO_ClearOutput(LED_APIO,LED_A);
			AT91F_PIO_ClearOutput(LED_BPIO,LED_B);
			AT91F_PIO_SetOutput(LED_CPIO,LED_C);
			AT91F_PIO_ClearOutput(LED_ENPIO,LED_EN);
			break;
		case LED_WLAN:
			AT91F_PIO_SetOutput(LED_APIO,LED_A);
			AT91F_PIO_ClearOutput(LED_BPIO,LED_B);
			AT91F_PIO_SetOutput(LED_CPIO,LED_C);
			AT91F_PIO_ClearOutput(LED_ENPIO,LED_EN);
			break;
		case LED_DAC:
			AT91F_PIO_ClearOutput(LED_APIO,LED_A);
			AT91F_PIO_SetOutput(LED_BPIO,LED_B);
			AT91F_PIO_SetOutput(LED_CPIO,LED_C);
			AT91F_PIO_ClearOutput(LED_ENPIO,LED_EN);
			break;
		case LED_MMC:
			AT91F_PIO_SetOutput(LED_APIO,LED_A);
			AT91F_PIO_SetOutput(LED_BPIO,LED_B);
			AT91F_PIO_SetOutput(LED_CPIO,LED_C);
			AT91F_PIO_ClearOutput(LED_ENPIO,LED_EN);
			break;
		case LED_TEST0:
			AT91F_PIO_ClearOutput(LED_0PIO,LED_0);
			break;
		case LED_TEST1:
			AT91F_PIO_ClearOutput(LED_1PIO,LED_1);
			break;

			default:
				AT91F_PIO_ClearOutput(LED_APIO,LED_A);
				AT91F_PIO_ClearOutput(LED_BPIO,LED_B);
				AT91F_PIO_ClearOutput(LED_CPIO,LED_C);
				AT91F_PIO_SetOutput(LED_ENPIO,LED_EN);
				break;
		}

}

void leds_off (void){
	AT91F_PIO_ClearOutput(LED_ENPIO,LED_EN);
	AT91F_PIO_SetOutput(LED_0PIO,LED_0);
	AT91F_PIO_SetOutput(LED_1PIO,LED_1);
}

void io_init(void)
{
	AT91F_PMC_EnablePeriphClock(AT91C_BASE_PMC, 1 << AT91C_ID_PIOA);
	AT91F_PMC_EnablePeriphClock(AT91C_BASE_PMC, 1 << AT91C_ID_PIOB);
	AT91F_PMC_EnablePeriphClock(AT91C_BASE_PMC, 1 << AT91C_ID_PIOC);
}



int main(void)
{
	
	/*
	** initialization
	*/
	io_init();
	dbgu_init();
	dbgu_clear();
  	dbgu_puts("Hello World! (" __DATE__ " " __TIME__ ")\n");
  	led_init_io();

	//sdram_init();
	
    while(1){
     	printf("Hello World\n");
       	led_on(LED_VS1053_CONTROL);
         	delayms(100);
        	led_on(LED_VS1053_DATA);
         	delayms(200);
           	led_on(LED_FLASH);
            delayms(300);
            led_on(LED_WLAN);
            delayms(400);
           	led_on(LED_DAC);
            delayms(500);
            led_on(LED_MMC);
            delayms(300);
            led_on(LED_TEST0);
            delayms(200);
            led_on(LED_TEST1);
            delayms(400);
            leds_off();
    }
	
	return 0;
}

