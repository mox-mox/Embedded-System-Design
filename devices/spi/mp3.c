#include "mp3.h"
#include "spi.h"

#include <board.h>
//#include <dbgu.h>
#include <stdio.h>





void mp3_init()
{
	if(!spi_is_initialised)
		spi_init();

	AT91F_PIO_CfgInput(AT91C_BASE_PIOB, MP3_DREQ); // Make MP3_DREQ an input. The MP3 decoder raises this signal when it is ready to receive new data.
}


static inline uint8_t mp3_is_ready()
{
	return (!(AT91F_PIO_IsInputSet(AT91C_BASE_PIOB, MP3_DREQ)));
}




uint8_t mp3_read(uint8_t adress, uint16_t* data)
{
	while(mp3_is_ready()); // Wait until the decoder is ready to receive a new register transfer
	spi_put(SPI_MP3_CTRL, VS1053_READ, NOT_LAST_TRANSFER);	// Tell the decoder we want to read a register
	spi_put(SPI_MP3_CTRL, adress, NOT_LAST_TRANSFER);		// Tell the decoder which register we want to read
	spi_put(SPI_MP3_CTRL, VOID_DATA, LAST_TRANSFER);		// "Wait" to receive the data
	return spi_get_checked(data, SPI_MP3_DATA);				// Return received data with error checking
}

void mp3_write(uint8_t adress, uint16_t data)
{
	while(mp3_is_ready()); // Wait until the decoder is ready to receive a new register transfer
	spi_put(SPI_MP3_CTRL, VS1053_WRITE, NOT_LAST_TRANSFER);	// Tell the decoder we want to read a register
	spi_put(SPI_MP3_CTRL, adress, NOT_LAST_TRANSFER);		// Tell the decoder which register we want to read
	spi_put(SPI_MP3_CTRL, data, LAST_TRANSFER);				// "Wait" to receive the data
}


