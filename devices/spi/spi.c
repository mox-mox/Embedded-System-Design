#include "spi.h"
#include <board.h>
#include <stdio.h>

//{{{Selection of SPI control register

//#define SELECT_SPI_CSR_MP3_CTRL     0	// SPI_CSR0
//
//#define SELECT_SPI_CSR_MP3_DATA     1	// SPI_CSR1
//
//#define SELECT_SPI_CSR_MP3_MMC      2	// SPI_CSR2
//
//#define SELECT_SPI_CSR_WLAN         3	// SPI_CSR3
//#define SELECT_SPI_CSR_FLASH_RAM    3	// SPI_CSR3
//#define SELECT_SPI_CSR_DAC          3	// SPI_CSR3
//}}}

//{{{Connection settings for the SPI devices

#define MP3_CTRL_DLYBCT     0	//TODO
#define MP3_CTRL_DLYBS      0	//TODO
#define MP3_CTRL_SPCK       5266300		//  5MHz
#define MP3_CTRL_BITS       (16-8)
#define MP3_CTRL_CSAAT      0
#define MP3_CTRL_NCPHA      0
#define MP3_CTRL_CPOL       0

#define MP3_DATA_DLYBCT     0	//TODO
#define MP3_DATA_DLYBS      0	//TODO
#define MP3_DATA_SPCK       9216000		//  9MHz
#define MP3_DATA_BITS       (8-8)
#define MP3_DATA_CSAAT      0	//TODO
#define MP3_DATA_NCPHA      0	//TODO
#define MP3_DATA_CPOL       0	//TODO

#define MMC_DLYBCT          0	//TODO
#define MMC_DLYBS           0	//TODO
#define MMC_SPCK_INIT       1000000		//  1MHz
#define MMC_SPCK_NRML       20000000	// 20MHz
#define MMC_BITS            (8-8)
#define MMC_CSAAT           0	//TODO
#define MMC_NCPHA           0	//TODO
#define MMC_CPOL            0	//TODO

#define DAC_DLYBCT          0	//TODO
#define DAC_DLYBS           0	//TODO
#define DAC_SPCK            12000000	// 12MHz
#define DAC_BITS            (8-8)
#define DAC_CSAAT           0	//TODO
#define DAC_NCPHA           0	//TODO
#define DAC_CPOL            0	//TODO

#define FLASH_RAM_DLYBCT    0	//TODO
#define FLASH_RAM_DLYBS     0	//TODO
#define FLASH_RAM_SPCK      13000000	// 12MHz
#define FLASH_RAM_BITS      (8-8)
#define FLASH_RAM_CSAAT     0	//TODO
#define FLASH_RAM_NCPHA     0	//TODO
#define FLASH_RAM_CPOL      0	//User defined

//{{{
//{{{Unused...

//#define WLAN_DLYBCT         0	//TODO
//#define WLAN_DLYBS          0	//TODO
//#define WLAN_SPCK           3000000		//  3MHz
//#define WLAN_BITS           (8-8)
//#define WLAN_CSAAT          0	//TODO
//#define WLAN_NCPHA          0	//TODO
//#define WLAN_CPOL           0	//TODO
//}}}

//{{{Defines necessary because the lib is rather stupid here...

#define SPI_DLYBCT 24
#define SPI_DLYBS 16
#define SPI_SCBR  8
#define SPI_BITS  4
#define SPI_CSAAT 3
#define SPI_NCPHA 1
#define SPI_CPOL  0

//}}}

//{{{Some devices share a configuration register. Check, if those devices use the same settings.

//If equal, set once and forget. If not equal, condens configuration into define and and write into the register upon usage

#if ((MP3_CTRL_DLYBCT == DAC_DLYBCT) && (MP3_CTRL_DLYBS == DAC_DLYBS) && (MP3_CTRL_BITS == DAC_BITS) && (MP3_CTRL_CSAAT == DAC_CSAAT) && (MP3_CTRL_NCPHA == DAC_NCPHA) && (MP3_CTRL_CPOL == DAC_CPOL))
	#define SPI_CSR0_IS_EQUAL 1
#else
	#define SPI_CSR0_IS_EQUAL 0
	#define SPI_CSR0_MP3_CTRL (((MP3_CTRL_DLYBCT<<SPI_DLYBCT) | (MP3_CTRL_DLYBS<<SPI_DLYBS) | (MP3_CTRL_SPCK<<SPI_SCBR) | (MP3_CTRL_BITS<<SPI_BITS) | (MP3_CTRL_CSAAT<<SPI_CSAAT) | (MP3_CTRL_NCPHA<<SPI_NCPHA) | (MP3_CTRL_CPOL<<SPI_CPOL)))
	#define SPI_CSR0_DAC (((DAC_DLYBCT<<SPI_DLYBCT) | (DAC_DLYBS<<SPI_DLYBS) | (DAC_SPCK<<SPI_SCBR) | (DAC_BITS<<SPI_BITS) | (DAC_CSAAT<<SPI_CSAAT) | (DAC_NCPHA<<SPI_NCPHA) | (DAC_CPOL<<SPI_CPOL)))
#endif


#if ((DAC_DLYBCT == FLASH_RAM_DLYBCT) && (DAC_DLYBS == FLASH_RAM_DLYBS) && (DAC_SPCK == FLASH_RAM_SPCK) && (DAC_BITS == FLASH_RAM_BITS) && (DAC_CSAAT == FLASH_RAM_CSAAT) && (DAC_NCPHA == FLASH_RAM_NCPHA) && (DAC_CPOL == FLASH_RAM_CPOL))
	#define SPI_CSR3_IS_EQUAL 1
#else
	#define SPI_CSR3_IS_EQUAL 0
	#define SPI_CSR0_DAC (((DAC_DLYBCT<<SPI_DLYBCT) | (DAC_DLYBS<<SPI_DLYBS) | (DAC_SPCK<<SPI_SCBR) | (DAC_BITS<<SPI_BITS) | (DAC_CSAAT<<SPI_CSAAT) | (DAC_NCPHA<<SPI_NCPHA) | (DAC_CPOL<<SPI_CPOL)))

	#define SPI_CSR0_FLASH_RAM (((FLASH_RAM_DLYBCT<<SPI_DLYBCT) | (FLASH_RAM_DLYBS<<SPI_DLYBS) | (FLASH_RAM_SPCK<<SPI_SCBR) | (FLASH_RAM_BITS<<SPI_BITS) | (FLASH_RAM_CSAAT<<SPI_CSAAT) | (FLASH_RAM_NCPHA<<SPI_NCPHA) | (FLASH_RAM_CPOL<<SPI_CPOL)))
#endif
//}}}

//{{{Clock calculations

#ifndef MCK
	#error "Please check the Master Clock frequency and define MCK with an appropriate value"
#else
	#define MIN(a, b) ((a) < (b) ? (a) : (b))
//{{{
// calculate the prescaler values
	#define SPI_CSR0_SCBR__ ((MCK) / (MIN((MP3_CTRL_SPCK), (DAC_SPCK))))
	#define SPI_CSR1_SCBR__ ((MCK) / (MP3_DATA_SPCK))
	#define SPI_CSR2_SCBR__ ((MCK) / (MMC_SPCK_INIT))	// TODO: Decrease scaling after init, to use full speed
//#define SPI_CSR3_SCBR__ ( (MCK) / (MIN( (MIN((DAC_SPCK), (FLASH_RAM_SPCK))), (WLAN_SPCK)) ) )
	#define SPI_CSR3_SCBR__ ((MCK) / (MIN((DAC_SPCK), (FLASH_RAM_SPCK))))
	#define SPI_CSR2_SCBR_FAST__ ((MCK) / (MMC_SPCK_NRML))	// The register setting to use after configuring the sd card
//}}}

//{{{
// check if the values fit in the range 1..255
	#if ((SPI_CSR0_SCBR__ < 1) || (SPI_CSR0_SCBR__ > 255) || (SPI_CSR1_SCBR__ < 1) || (SPI_CSR1_SCBR__ > 255) || (SPI_CSR2_SCBR__ < 1) || (SPI_CSR2_SCBR__ > 255) || (SPI_CSR3_SCBR__ < 1) || (SPI_CSR3_SCBR__ > 255) || (SPI_CSR2_SCBR_FAST__ < 1) || (SPI_CSR2_SCBR_FAST__ > 255))
		#error "One or more SPI clock prescaler values exceed the allowed range of 1..255."
	#else	// if everything fits, create the actual _integer_ values to use in the program code
		#define SPI_CSR0_SCBR ((uint8_t) SPI_CSR0_SCBR__)
		#define SPI_CSR1_SCBR ((uint8_t) SPI_CSR1_SCBR__)
		#define SPI_CSR2_SCBR ((uint8_t) SPI_CSR2_SCBR__)
		#define SPI_CSR3_SCBR ((uint8_t) SPI_CSR3_SCBR__)
		#define SPI_CSR2_SCBR_FAST ((uint8_t) SPI_CSR2_SCBR_FAST__)
	#endif
//}}}
#endif
//}}}
//}}}

//}}}



uint8_t spi_is_initialised=0;

uint8_t spi_transmit_buffer_is_empty();



// Configure the SPI port. Call this function before using the SPI port the first time!
#define EN_SPI   AT91C_PIO_PA26 // This pin gates the power to the adress decoder
void spi_init()
{


	AT91F_SPI1_CfgPMC();	// Enable Peripheral clock in PMC for SPI1
	AT91F_SPI1_CfgPIO();	// Configure PIO controllers to drive SPI1 signals

	AT91F_PIO_CfgOutput(AT91C_BASE_PIOA, EN_SPI);   // Make EN_SPI an output...
	set_en_spi();									// ...and enable the adress decoder

	// enable the SPI
	AT91C_BASE_SPI1->SPI_CR =  (AT91C_SPI_SPIEN);// | !AT91C_SPI_LASTXFER | !AT91C_SPI_SWRST | !AT91C_SPI_SPIDIS;

	// Master Mode, Variable Peripheral Select, using a 4- to 16-bit decoder for Chip Select, use (0!!) Mode Fault detection, no Local Loop Back
	AT91C_BASE_SPI1->SPI_MR = (AT91C_SPI_PCSDEC | AT91C_SPI_PS | AT91C_SPI_MSTR);// | !AT91C_SPI_DLYBCS | !AT91C_SPI_PCS | !AT91C_SPI_LLB | !AT91C_SPI_MODFDIS;

#if SPI_CSR0_IS_EQUAL == 1
	// Settings for DAC (LDAC) and MP3 decoder control line:
	AT91C_BASE_SPI1->SPI_CSR[0] = ((MP3_CTRL_DLYBCT<<SPI_DLYBCT) | (MP3_CTRL_DLYBS<<SPI_DLYBS) | (SPI_CSR0_SCBR<<SPI_SCBR) | (MP3_CTRL_BITS<<SPI_BITS) | (MP3_CTRL_CSAAT<<SPI_CSAAT) | (MP3_CTRL_NCPHA<<SPI_NCPHA) | (MP3_CTRL_CPOL<<SPI_CPOL));
#endif

	// Settings for the MP3 decoder data line
	AT91C_BASE_SPI1->SPI_CSR[1] = ((MP3_DATA_DLYBCT<<SPI_DLYBCT) | (MP3_DATA_DLYBS<<SPI_DLYBS) | (SPI_CSR1_SCBR<<SPI_SCBR) | (MP3_DATA_BITS<<SPI_BITS) | (MP3_DATA_CSAAT<<SPI_CSAAT) | (MP3_DATA_NCPHA<<SPI_NCPHA) | (MP3_DATA_CPOL<<SPI_CPOL));

	// Settings for the SD card interface
	AT91C_BASE_SPI1->SPI_CSR[2] = ((MMC_DLYBCT<<SPI_DLYBCT) | (MMC_DLYBS<<SPI_DLYBS) | (SPI_CSR2_SCBR<<SPI_SCBR) | (MMC_BITS<<SPI_BITS) | (MMC_CSAAT<<SPI_CSAAT) | (MMC_NCPHA<<SPI_NCPHA) | (MMC_CPOL<<SPI_CPOL));

#if SPI_CSR3_IS_EQUAL == 1
	// Settings for the DAC, (WLAN) and FLASH RAM
	AT91C_BASE_SPI1->SPI_CSR[3] = ((DAC_DLYBCT<<SPI_DLYBCT) | (DAC_DLYBS<<SPI_DLYBS) | (SPI_CSR3_SCBR<<SPI_SCBR) | (DAC_BITS<<SPI_BITS) | (DAC_CSAAT<<SPI_CSAAT) | (DAC_NCPHA<<SPI_NCPHA) | (DAC_CPOL<<SPI_CPOL));
#endif

	// indicate that the init was done
	spi_is_initialised=1;
}












//uint8_t spi_is_ready_to_send()
//{   //      check if no received data is waiting	     and if the transmit register is free (last data has been sent completely)
//	return !(AT91C_BASE_SPI->SPI_SR & AT91C_SPI_RDRF) && (AT91C_BASE_SPI->SPI_SR & AT91C_SPI_TDRE);
//}

uint8_t spi_transmit_buffer_is_empty()
{
	printf("spi_transmit_buffer_is_empty() = %i\n", (AT91C_BASE_SPI->SPI_SR & AT91C_SPI_TDRE));
	return AT91C_BASE_SPI->SPI_SR & AT91C_SPI_TDRE;
}

uint16_t spi_transmit(uint8_t slave_number, uint32_t out_data, uint8_t is_last_transfer)
{
	// data                : 00000000 00000000 XXXXXXXX YYYYYYYY
	// slave_number<<16    : 00000000 0000XXXX 00000000 00000000
	// is_last_transfer<<24: 0000000x 00000000 00000000 00000000
#define SPI_LASTXFER 24
#define SPI_PCS      16
	AT91C_BASE_SPI1->SPI_TDR = (out_data | (((uint32_t) slave_number) << SPI_PCS) | (is_last_transfer << SPI_LASTXFER));
	printf("spi_transmit_1\n");
	while(!spi_transmit_buffer_is_empty()); // TODO: Das will irgendwie nicht enden...
	printf("spi_transmit_2\n");
	return (uint16_t) (AT91C_BASE_SPI1->SPI_RDR && 0xFFFF);
}

void set_en_spi()
{
	AT91F_PIO_SetOutput(AT91C_BASE_PIOA, EN_SPI);
}
void clear_en_spi()
{
	AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, EN_SPI);
}

