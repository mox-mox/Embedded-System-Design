#ifndef SPI_H_
#define SPI_H_

/*
 *
 * This is the driver for the spi interface of the SAM9XE512 microcontroller.
 * This file was written by mox for the lecture "Embedded System Design" in SS2014 at Heidelberg University
 *
 * Setting:
 * Using SPI1
 * SPI-Master
 * Uses Adress decoding
 *
 * Pins:
 * PB0/SPI1_MISO/TIOA3
 * PB1/SPI1_MOSI/TIOB3
 * PB2/SPI1_SPCK/TIOA4
 *
 * PB3/SPI1_NPCS0/TIOA5
 * PC13/D18/SPI1_NPCS1
 * PC20/D20/SPI1_NPCS3
 *
 */

/* Adress decoding table:
 * //NPCS2 is unused
 * NPCS3	NPCS1	NPCS0
 * 0		0		0			/LDAC
 * 0		0		1           Select_SPI_MP3_Control (VS1053)
 * 0		1		0           Select_SPI_MP3_Data (VS1053)
 * 0		1		1			--
 * 1		0		0			Select_SPI_FLASH_RAM
 * 1		0		1       	Select_SPI_WLAN
 * 1		1		0       	Select_SPI_DAC
 * 1		1		1       	Select_SPI_MP3_MMC
 */


/*
Registers

Offset	Register 								Name		Access		Reset
0x00	Control Register						SPI_CR		Write-only	---
0x04	Mode Register							SPI_MR		Read-write	0x0
0x08	Receive Data Register					SPI_RDR		Read-only	0x0
0x0C	Transmit Data Register					SPI_TDR		Write-only	---
0x10	Status Register							SPI_SR		Read-only	0x000000F0
0x14	Interrupt Enable Register				SPI_IER		Write-only	---
0x18	Interrupt Disable Register				SPI_IDR		Write-only	---
0x1C	Interrupt Mask Register					SPI_IMR		Read-only	0x0
0x30	Chip Select Register 0					SPI_CSR0	Read-write	0x0
0x34	Chip Select Register 1					SPI_CSR1	Read-write	0x0
0x38	Chip Select Register 2					SPI_CSR2	Read-write	0x0
0x3C	Chip Select Register 3					SPI_CSR3	Read-write	0x0
*/

//MP3-Dekoder VS1053 (CLKI = 36,864 MHz)
//Dateninterface:
//max. SCLK = CLKI/4 ≈ 9 MHz;
//Datenlänge: 8 Bit
//Kontrollinterface:
//max. SCLK = CLKI/7 ≈ 5 MHz;
//Datenlänge: 16 Bit
//
//SD-Card:
//max. SCLK ≈ 1 MHz; (Initialisierung)
//Datenlänge: 8 Bit
//max. SCLK ≈ 10 ... > 25 MHz;
//Datenlänge: 8 Bit
//
//Flash-ROM
//max. SCLK = 13 MHz; SCLK Polarität wählbar
//Datenlänge: > 16 Bit
//
//WLAN-Modul max. SCLK = 3 MHz?
//Datenlänge: 8 Bit?
//
//Digital-Analog-Wandler
//max. SCLK = 12 MHz
//Datenlänge: 16 Bit






// The necessary clock rates as taken from the Datasheets
	#define SPCK_MP3_DATA  9216000	//  9MHz
	#define SPCK_MP3_CNTL  5266300	//  5MHz
	#define SPCK_SD_INIT   1000000	//  1MHz
	#define SPCK_SD_NRML  20000000	// 20MHz
	#define SPCK_FLASH    13000000	// 13MHz
//  #define SPCK_WLAN      3000000	//  3MHz
	#define SPCK_DAC      12000000	// 12MHz

void spi_init();






#endif /*SPI_H_*/
