/*-------------------------------------------------------------------------
NeoPixel library helper functions for DotStars using general Pins (APA102).

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

#include "TwoWireBitBangImple.h"

//TODO remove these once Linux SPI tested and works OK
#define MOSI  9
#define SCK   11


template<typename T_TWOWIRE> class DotStarMethodBase
{
public:
	DotStarMethodBase(uint8_t pinClock, uint8_t pinData, uint16_t pixelCount, size_t elementSize, size_t settingsSize) :
        _sizeData(pixelCount * elementSize + settingsSize),
		_sizeEndFrame((pixelCount + 15) / 16), // 16 = div 2 (bit for every two pixels) div 8 (bits to bytes)
		_wire(pinClock, pinData)
    {
        _data = static_cast<uint8_t*>(malloc(_sizeData));
        memset(_data, 0, _sizeData);
    }

	DotStarMethodBase(uint16_t pixelCount, size_t elementSize, size_t settingsSize) :
		DotStarMethodBase(SCK, MOSI, pixelCount, elementSize, settingsSize)
	{
	}

    ~DotStarMethodBase()
    {
        free(_data);
    }

    bool IsReadyToUpdate() const
    {
        return true; // dot stars don't have a required delay
    }

    void Initialize()
    {
		_wire.begin();
    }

    void Update(bool)
    {
		const uint8_t startFrame[4] = { 0x00 };
		uint8_t resetFrame[4] = { 0xff };
		memset(resetFrame, 0xff, sizeof(resetFrame));
		_wire.beginTransaction();

        // start frame
		_wire.transmitBytes(startFrame, sizeof(startFrame));
        
        // data
		_wire.transmitBytes(_data, _sizeData);

       // reset frame
		_wire.transmitBytes(resetFrame, sizeof(resetFrame));
        
        // end frame 
        
		// one bit for every two pixels with no less than 1 byte
// 		for (size_t endFrameByte = 0; endFrameByte < _sizeEndFrame; endFrameByte++)
// 		{
// 			_wire.transmitByte(0x00);
// 		}
		
		_wire.endTransaction();
    }

    uint8_t* getData() const
    {
        return _data;
    };

    size_t getDataSize() const
    {
        return _sizeData;
    };

private:
	const size_t   _sizeData;   // Size of '_data' buffer below
	const size_t   _sizeEndFrame;

	T_TWOWIRE _wire;
    uint8_t* _data;       // Holds LED color values
};

typedef DotStarMethodBase<TwoWireBitBangImple> DotStarMethod;

#include "TwoWireSpiImple.h"
typedef DotStarMethodBase<TwoWireSpiImple<SpiSpeed40Mhz>> DotStarSpi40MhzMethod;
typedef DotStarMethodBase<TwoWireSpiImple<SpiSpeed20Mhz>> DotStarSpi20MhzMethod;
typedef DotStarMethodBase<TwoWireSpiImple<SpiSpeed10Mhz>> DotStarSpi10MhzMethod;
typedef DotStarMethodBase<TwoWireSpiImple<SpiSpeed2Mhz>> DotStarSpi2MhzMethod;
typedef DotStarSpi10MhzMethod DotStarSpiMethod;




