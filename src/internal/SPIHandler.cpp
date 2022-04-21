// This class is based on this Linux SPI example
// https://github.com/Johannes4Linux/Linux_Embedded_Interfaces/blob/main/SPI/mcp23s08.c



#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#include "SPIHandler.h"

// anonymous namespace
namespace
{
    const uint8_t SPI_MODE         = 0;  //TODO need to think if this can be done as arg, right now not sure exactly what this number means
    const int SPI_BITS_PER_WORD    = 8;
}


SPIHandler::SPIHandler()
: mSPIfd(-1)
, mSpeed(-1)
{
     
}

SPIHandler::~SPIHandler()
{
    delete &mSPIfd;
    delete &mSpeed;
}


/**
 * @brief function initializes SPI bus
 *
 * @param spiFilename SPIdev filename (e.g "/sys/class/spidev")
 * @param speed		Speed for spi transfer in Hz (e.g. 500000)
 *					will be overwritten with the received input data
 * @return           1  if success
 *                  -1  if error
 */
int SPIHandler::begin(char* spiFilename, uint32_t speed)
{
    mSpeed = speed;

	/* Open the SPI bus file descriptor */
	mSPIfd = open(spiFilename, O_RDWR);
	if(mSPIfd < 0) {
		perror("Error opening SPI Bus");
		return -1;
	}

	/* Setup of the SPI Bus */
	if(ioctl(mSPIfd, SPI_IOC_WR_MODE, &SPI_MODE) < 0) {
		perror("Error setting the SPI mode");
		close(mSPIfd);
		return -1;
	}
	if(ioctl(mSPIfd, SPI_IOC_WR_MAX_SPEED_HZ, &mSpeed) < 0) {
		perror("Error setting the SPI speed");
		close(mSPIfd);
		return -1;
	}
    
	if(ioctl(mSPIfd, SPI_IOC_WR_BITS_PER_WORD, &SPI_BITS_PER_WORD) < 0) {
		perror("Error setting the SPI wordlength");
		close(mSPIfd);
		return -1;
	}

    return 1;
}

void SPIHandler::end()
{
    close(mSPIfd);
}


/**
 * @brief function makes a SPI transfer
 *
 * @param data		Data array with output (write) data
 *					will be overwritten with the received input data
 * @param length	Length of the data array
 */
void SPIHandler::transfer(uint8_t *data, size_t length) {
	struct spi_ioc_transfer spi[length];

	/* Setup transfer struct */
	for (size_t i=0; i<length; i++) {
		memset(&spi[i], 0, sizeof(struct spi_ioc_transfer));
		spi[i].tx_buf = (unsigned long) (data+i);
		spi[i].rx_buf = (unsigned long) (data+i);
		spi[i].len = 1;
		spi[i].speed_hz = mSpeed;
		spi[i].bits_per_word = SPI_BITS_PER_WORD;
	}

	/* Let's do the transfer */
	if(ioctl(mSPIfd, SPI_IOC_MESSAGE(length), spi) < 0) {
		perror("Error transfering data over SPI bus");
		close(mSPIfd);
		exit(-1);
	}
}

