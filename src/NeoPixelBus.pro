TARGET = NeoPixelBus
TEMPLATE = lib

INCLUDEPATH  += $$PWD

HEADERS += $$PWD/NeoPixelAnimator.h \
        $$PWD/NeoPixelBrightnessBus.h \
        $$PWD/NeoPixelBus.h \
        $$PWD/NeoPixelSegmentBus.h \
        $$PWD/internal/SPIHandler.h \
        $$PWD/internal/DotStarColorFeatures.h \
        $$PWD/internal/DotStarGenericMethod.h \
        $$PWD/internal/Esp32_i2s.h \
        $$PWD/internal/HsbColor.h \
        $$PWD/internal/HslColor.h \
        $$PWD/internal/HtmlColor.h \
        $$PWD/internal/HtmlColorNameStrings.h \
        $$PWD/internal/Layouts.h \
        $$PWD/internal/NeoBitmapFile.h \
        $$PWD/internal/NeoBuffer.h \
        $$PWD/internal/NeoBufferContext.h \
        $$PWD/internal/NeoBufferMethods.h \
        $$PWD/internal/NeoColorFeatures.h \
        $$PWD/internal/NeoDib.h \
        $$PWD/internal/NeoEase.h \
        $$PWD/internal/NeoGamma.h \
        $$PWD/internal/NeoHueBlend.h \
        $$PWD/internal/NeoMosaic.h \
        $$PWD/internal/NeoRingTopology.h \
        $$PWD/internal/NeoSpriteSheet.h \
        $$PWD/internal/NeoTiles.h \
        $$PWD/internal/NeoTopology.h \
        $$PWD/internal/RgbColor.h \
        $$PWD/internal/RgbwColor.h \


SOURCES += $$PWD/internal/NeoPixelAnimator.cpp \
        $$PWD/internal/Esp32_i2s.c \
        $$PWD/internal/SPIHandler.cpp \
        $$PWD/internal/HsbColor.cpp \
        $$PWD/internal/HslColor.cpp \
        $$PWD/internal/HtmlColor.cpp \
        $$PWD/internal/HtmlColorNames.cpp \
        $$PWD/internal/HtmlColorNameStrings.cpp \
        $$PWD/internal/HtmlColorShortNames.cpp \
        $$PWD/internal/NeoGamma.cpp \   
        $$PWD/internal/NeoPixelAvr.c \
        $$PWD/internal/RgbColor.cpp \
        $$PWD/internal/RgbwColor.cpp \
