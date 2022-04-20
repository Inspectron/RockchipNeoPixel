/*-------------------------------------------------------------------------
NeoPixel library helper functions for WS2801

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

// must also check for arm due to Teensy incorrectly having ARDUINO_ARCH_AVR set
#if defined(ARDUINO_ARCH_AVR) && !defined(__arm__)
#include "TwoWireBitBangImpleAvr.h"
#else
#include "TwoWireBitBangImple.h"
#endif


template<typename T_TWOWIRE> class Ws2801MethodBase
{
public:
	Ws2801MethodBase(uint8_t pinClock, uint8_t pinData, uint16_t pixelCount, size_t elementSize, size_t settingsSize) :
        _sizeData(pixelCount * elementSize + settingsSize),
		_wire(pinClock, pinData)
    {
        _data = static_cast<uint8_t*>(malloc(_sizeData));
        memset(_data, 0, _sizeData);
    }

    ~Ws2801MethodBase()
    {
        free(_data);
    }

    void Initialize()
    {
		_wire.begin();
    }

    void Update(bool)
    {

		_wire.beginTransaction();
        
        // data
		_wire.transmitBytes(_data, _sizeData);
        
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
    const size_t  _sizeData;   // Size of '_data' buffer below

    T_TWOWIRE _wire;
    uint8_t* _data;       // Holds LED color values
};

typedef Ws2801MethodBase<TwoWireBitBangImple> NeoWs2801Method;

#if !defined(__AVR_ATtiny85__) && !defined(ARDUINO_attiny)
#include "TwoWireSpiImple.h"
typedef Ws2801MethodBase<TwoWireSpiImple<SpiSpeed20Mhz>> NeoWs2801Spi20MhzMethod;
typedef Ws2801MethodBase<TwoWireSpiImple<SpiSpeed10Mhz>> NeoWs2801Spi10MhzMethod;
typedef Ws2801MethodBase<TwoWireSpiImple<SpiSpeed2Mhz>> NeoWs2801Spi2MhzMethod;
typedef NeoWs2801Spi10MhzMethod NeoWs2801SpiMethod;
#endif



