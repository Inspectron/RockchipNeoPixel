/*-------------------------------------------------------------------------
Lpd8806ColorFeatures provides feature classes to describe color order and
color depth for NeoPixelBus template class when used with DotStar like chips

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

class Lpd88063Elements
{
public:
    static const size_t PixelSize = 3; 

    static uint8_t* getPixelAddress(uint8_t* pPixels, uint16_t indexPixel)
    {
        return pPixels + indexPixel * PixelSize;
    }
    static const uint8_t* getPixelAddress(const uint8_t* pPixels, uint16_t indexPixel)
    {
        return pPixels + indexPixel * PixelSize;
    }

    static void replicatePixel(uint8_t* pPixelDest, const uint8_t* pPixelSrc, uint16_t count)
    {
        uint8_t* pEnd = pPixelDest + (count * PixelSize);
        while (pPixelDest < pEnd)
        {
            *pPixelDest++ = pPixelSrc[0];
            *pPixelDest++ = pPixelSrc[1];
            *pPixelDest++ = pPixelSrc[2];
        }
    }

    static void movePixelsInc(uint8_t* pPixelDest, const uint8_t* pPixelSrc, uint16_t count)
    {
        uint8_t* pEnd = pPixelDest + (count * PixelSize);
        while (pPixelDest < pEnd)
        {
            *pPixelDest++ = *pPixelSrc++;
            *pPixelDest++ = *pPixelSrc++;
            *pPixelDest++ = *pPixelSrc++;
        }
    }

    static void movePixelsDec(uint8_t* pPixelDest, const uint8_t* pPixelSrc, uint16_t count)
    {
        uint8_t* pDestBack = pPixelDest + (count * PixelSize);
        const uint8_t* pSrcBack = pPixelSrc + (count * PixelSize);
        while (pDestBack > pPixelDest)
        {
            *--pDestBack = *--pSrcBack;
            *--pDestBack = *--pSrcBack;
            *--pDestBack = *--pSrcBack;
        }
    }

    typedef RgbColor ColorObject;
};

class  Lpd8806BrgFeature : public Lpd88063Elements
{
public:
	static void applyPixelColor(uint8_t* pPixels, uint16_t indexPixel, ColorObject color)
	{
		uint8_t* p = getPixelAddress(pPixels, indexPixel);

		*p++ = (color.B >> 1) | 0x80;
		*p++ = (color.R >> 1) | 0x80;
		*p = (color.G >> 1) | 0x80;
	}

	static ColorObject retrievePixelColor(const uint8_t* pPixels, uint16_t indexPixel)
	{
		ColorObject color;
		const uint8_t* p = getPixelAddress(pPixels, indexPixel);

		color.B = (*p++) << 1;
		color.R = (*p++) << 1;
		color.G = (*p) << 1;

		return color;
	}
};

class  Lpd8806GrbFeature : public Lpd88063Elements
{
public:
    static void applyPixelColor(uint8_t* pPixels, uint16_t indexPixel, ColorObject color)
    {
        uint8_t* p = getPixelAddress(pPixels, indexPixel);

        *p++ = (color.G >> 1) | 0x80;
        *p++ = (color.R >> 1) | 0x80;
        *p = (color.B >> 1) | 0x80;
    }

    static ColorObject retrievePixelColor(const uint8_t* pPixels, uint16_t indexPixel)
    {
        ColorObject color;
        const uint8_t* p = getPixelAddress(pPixels, indexPixel);

        color.G = (*p++) << 1;
        color.R = (*p++) << 1;
        color.B = (*p) << 1;

        return color;
    }
};

