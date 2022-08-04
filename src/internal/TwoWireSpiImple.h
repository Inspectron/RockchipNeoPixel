/*-------------------------------------------------------------------------
NeoPixel library helper functions for DotStars using general Pins (APA102/LPD8806).

Written by Michael C. Miller.

I invest time and resources providing this open source code,
please support me by dontating (see https://github.com/Makuna/NeoPixelBus)

-------------------------------------------------------------------------
This file is part of the Makuna/NeoPixelBus library.

NeoPixelBus is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation, either version 3 of
the License, or (at your option) any later version.

NeoPixelBus is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with NeoPixel.  If not, see
<http://www.gnu.org/licenses/>.
-------------------------------------------------------------------------*/

#pragma once

#include "SPIHandler.h"

// anonymous namespace
namespace
{
    const char* SPI_DEV_FILENAME = "/dev/spidev0.0";  //TODO need to think how this can be done as arg
}

class SpiSpeed40Mhz
{
public:
    static const uint32_t Clock = 40000000L;
};

class SpiSpeed20Mhz
{
public:
	static const uint32_t Clock = 20000000L;
};

class SpiSpeed10Mhz
{
public:
	static const uint32_t Clock = 10000000L;
};

class SpiSpeed2Mhz
{
public:
	static const uint32_t Clock = 2000000L;
};

template<typename T_SPISPEED> class TwoWireSpiImple
{
public:
	TwoWireSpiImple(uint8_t, uint8_t): // clock and data pins ignored for hardware SPI
    mSPIHandler(*new SPIHandler())
	{
	}

	~TwoWireSpiImple()
	{
        mSPIHandler.end();
        delete &mSPIHandler;
	}

	void begin()
	{
        mSPIHandler.begin(SPI_DEV_FILENAME, T_SPISPEED::Clock);
	}

	void beginTransaction()
	{
	}

	void endTransaction()
	{
	}

	void transmitByte(uint8_t data)
	{
        transmitBytes(&data, 1);
	}

	void transmitBytes(uint8_t* data, size_t dataSize)
	{
        mSPIHandler.transfer(data, dataSize);
	}

private:
    SPIHandler mSPIHandler;
};
