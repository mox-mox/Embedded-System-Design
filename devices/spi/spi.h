#ifndef SPI_H
#define SPI_H
#include <stdint.h>
#include <AT91SAM9XE512.h>

/*
 *
 * This is the driver for the spi interface of the SAM9XE512 microcontroller.
 * This file was written by mox for the lecture "Embedded System Design" in SS2014 at Heidelberg University
 *
 * {{{
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
 * }}}
 */

//{{{
// SPI-adress-to-Slave translation

#define SPI_LDAC          0
#define SPI_MP3_CTRL      1
#define SPI_MP3_DATA      6
#define SPI_MP3_MMC      11
#define SPI_FLASH_RAM    12
#define SPI_WLAN         13
#define SPI_DAC          14

//}}}


// Configure the SPI port. Call this function before using the SPI port the first time!
void spi_init();


#define LAST_TRANSFER 1
#define CONTINUE 0
#define VOID_DATA 0 // Data to transmit, when when only receiving is required
uint16_t spi_transmit(uint8_t slave_number, uint32_t out_data, uint8_t is_last_transfer);



uint8_t spi_is_ready_to_send();


void set_en_spi();

void clear_en_spi();

// flag, that indicats whether the spi port has already been initialised
uint8_t spi_is_initialised;













//{{{

//		
//		// Send 8..16 bit of data trough the SPI port.
//		// Always check that the SPI port is ready to send before sending, or data loss may occur!
//		inline void spi_put(uint8_t slave_number, uint32_t data, uint8_t is_last_transfer);
//		
//		//{{{
//		//
//		// This function will return the data, that was received by the last transfer
//		// Args:   uint16_t* destination. This is where the received data will be saved to
//		// Retval: 0 -> Everything ok,
//		//         1-> No new data available,
//		//         2..255 -> reserved
//		//}}}
//		inline uint8_t spi_get(uint16_t* destination);
//		
//		// This function will return the data, that was received by the last transfer
//		// Args:   uint16_t* destination. This is where the received data will be saved to
//		// Retval: 0 -> Everything ok,
//		//         1-> No new data available,
//		//         2 -> Data came from wrong peripheral,
//		//         3 -> No new data available and data is from wrong source,
//		//         4..255 -> reserved
//		inline uint8_t spi_get_checked(uint16_t* destination, uint8_t peripheral);
//		
//		inline uint8_t spi_is_ready_to_send();
//		inline uint8_t spi_receive_data_is_waiting();
//		inline uint8_t spi_transmit_buffer_is_empty();
//		
//		
//		// Send 8..16 bit of data trough the SPI port.
//		// Always check that the SPI port is ready to send before sending, or data loss may occur!
//		// The data length is 8..16 bits, but the variable "data" is 32 bit to avoid a cast (and because it would need to be at least 16 bit anyways...)
//		inline void spi_put(uint8_t slave_number, uint32_t data, uint8_t is_last_transfer)
//		{
//			// data                : 00000000 00000000 XXXXXXXX YYYYYYYY
//			// slave_number<<16    : 00000000 0000XXXX 00000000 00000000
//			// is_last_transfer<<24: 0000000x 00000000 00000000 00000000
//		#define SPI_LASTXFER 24
//		#define SPI_PCS      16
//			AT91C_BASE_SPI1->SPI_TDR = (data | (((uint32_t) slave_number) << SPI_PCS) | (is_last_transfer << SPI_LASTXFER));
//		}
//		
//		inline uint8_t spi_receive_data_is_waiting()
//		{
//			return AT91C_BASE_SPI->SPI_SR & AT91C_SPI_RDRF;
//		}
//		
//		
//		// This function will return the data, that was received by the last transfer
//		// Args:   uint16_t* destination. This is where the received data will be saved to
//		// Retval: 0 -> Everything ok,
//		//         1-> No new data available,
//		//         2..255 -> reserved
//		inline uint8_t spi_get(uint16_t* destination)
//		{
//			uint8_t retval=spi_receive_data_is_waiting();
//			*destination=(uint16_t) (AT91C_BASE_SPI1->SPI_RDR && 0xFFFF);
//			return retval;
//		}
//		
//		
//		// This function will return the data, that was received by the last transfer
//		// Args:   uint16_t* destination. This is where the received data will be saved to
//		// Retval: 0 -> Everything ok,
//		//         1-> No new data available,
//		//         2 -> Data came from wrong peripheral,
//		//         3 -> No new data available and data is from wrong source,
//		//         4..255 -> reserved
//		inline uint8_t spi_get_checked(uint16_t* destination, uint8_t peripheral)
//		{
//			uint8_t retval= spi_receive_data_is_waiting(); // check if the data is stale
//			// generate the the peripheral mismatch bit: compare peripherals and shift the resulting 0 or 1 to the left
//			uint8_t peripheral_error = ((((uint8_t) (AT91C_BASE_SPI1->SPI_RDR >> 16)) == peripheral)<<1);
//			retval |= peripheral_error; // "add" both errors
//			*destination=(uint16_t) (AT91C_BASE_SPI1->SPI_RDR && 0xFFFF);
//			return retval;
//		}

//}}}




#endif /*SPI_H_*/
