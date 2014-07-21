#include "spi.h"
#include <board.h>

#include <lib_AT91SAM9XE512.h>
#include <AT91SAM9XE512.h>

//#include <dbgu.h>

// SPI-adress-to-Slave translation
#define SELECT_LDAC				 0
#define SELECT_SPI_MP3_CTRL		 1
#define SELECT_SPI_MP3_DATA		 6
#define SELECT_SPI_MP3_MMC		11
#define SELECT_SPI_FLASH_RAM	12
#define SELECT_SPI_WLAN			13
#define SELECT_SPI_DAC			14

// Selection of SPI control register
#define SELECT_SPI_CSR_MP3_CTRL		0	// SPI_CSR0

#define SELECT_SPI_CSR_MP3_DATA		1	// SPI_CSR1

#define SELECT_SPI_CSR_MP3_MMC		2	// SPI_CSR2

#define SELECT_SPI_CSR_WLAN			3	// SPI_CSR3
#define SELECT_SPI_CSR_FLASH_RAM	3	// SPI_CSR3
#define SELECT_SPI_CSR_DAC			3	// SPI_CSR3

// Clock calculations
#ifndef MCK
	#error "Please check the Master Clock frequency and define MCK with an appropriate value"
#else
	#define MIN(a, b) ((a) < (b) ? (a) : (b))


// calculate the prescaler values
	#define SPI_CSR0_SCBR__ ((MCK) / (MIN((SPCK_MP3_CNTL), (SPCK_DAC))))
	#define SPI_CSR1_SCBR__ ((MCK) / (SPCK_MP3_DATA))
	#define SPI_CSR2_SCBR__ ((MCK) / (SPCK_SD_INIT))// TODO: Decrease scaling after init, to use full speed
//#define SPI_CSR3_SCBR__ ( (MCK) / (MIN( (MIN((SPCK_DAC), (SPCK_FLASH))), (SPCK_WLAN)) ) )
	#define SPI_CSR3_SCBR__ ((MCK) / (MIN((SPCK_DAC), (SPCK_FLASH))))
	#define SPI_CSR2_SCBR_FAST__ ((MCK) / (SPCK_SD_NRML))// The register setting to use after configuring the sd card

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
#endif

void spi_init()
{
	AT91F_SPI1_CfgPMC();	// Enable Peripheral clock in PMC for SPI1
	AT91F_SPI1_CfgPIO();	// Configure PIO controllers to drive SPI1 signals

	// enable the SPI
	SPI_CR   = (0<<LASTXFER) | (0<<SWRST) | (0<<SPIDIS) | (1<<SPIEN);
	// Master Mode, Variable Peripheral Select, using a 4- to 16-bit decoder for Chip Select, use (0!!) Mode Fault detection, no Local Loop Back
	SPI_MR   = (0<<DLYBCS)   | (0<<PCS)   | (0<<LLB)    | (0<<MODFDIS) | (1<<PCSDEC) | (1<<PS)    | (1<<MSTR);
	//SPI_TDR  = (0<<LASTXFER) | (0<<PCS)   | (0<<TD)     | (0<<TD);
	// Enable no interrupts so far
	//SPI_IER  = (0<<TXEMPTY)  | (0<<NSSR)  | (0<<TXBUFE) | (0<<RXBUFF)  | (0<<ENDTX)  | (0<<ENDRX) | (0<<OVRES) | (0<<MODF) | (0<<TDRE) | (0<<RDRF);
	//SPI_IDR  = (0<<TXEMPTY)  | (0<<NSSR)  | (0<<TXBUFE) | (0<<RXBUFF)  | (0<<ENDTX)  | (0<<ENDRX) | (0<<OVRES) | (0<<MODF) | (0<<TDRE) | (0<<RDRF);
	//SPI_IMR  = (0<<TXEMPTY)  | (0<<NSSR)  | (0<<MODF)   | (0<<TDRE)    | (0<<RDRF);


	// Settings for DAC (LDAC) and MP3 decoder control line
	SPI_CSR0 = (0<<DLYBCT)   | (0<<DLYBS) | (SPI_CSR0_SCBR<<SCBR)   | (0<<BITS)    | (0<<CSAAT)  | (0<<NCPHA) | (0<<CPOL);
	// Settings for the MP3 decoder data line
	SPI_CSR1 = (0<<DLYBCT)   | (0<<DLYBS) | (SPI_CSR1_SCBR<<SCBR)   | (0<<BITS)    | (0<<CSAAT)  | (0<<NCPHA) | (0<<CPOL);
	// Settings for the SD card interface
	SPI_CSR2 = (0<<DLYBCT)   | (0<<DLYBS) | (SPI_CSR2_SCBR<<SCBR)   | (0<<BITS)    | (0<<CSAAT)  | (0<<NCPHA) | (0<<CPOL);
	// Settings for the DAC, WLAN and FLASH RAM
	SPI_CSR3 = (0<<DLYBCT)   | (0<<DLYBS) | (SPI_CSR3_SCBR<<SCBR)   | (0<<BITS)    | (0<<CSAAT)  | (0<<NCPHA) | (0<<CPOL);




	return;
}
