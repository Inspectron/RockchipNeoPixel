TARGET = exampleLEDapp

SOURCES += \
    $$PWD/main.cpp \
    $$PWD/LEDHandler.cpp \

HEADERS += \
    $$PWD/LEDHandler.hpp \
    $$PWD/LEDTypes.h \

INCLUDEPATH  += $$PWD \
                ../src/

HEADERS += ../src/NeoPixelAnimator.h \
        ../src/NeoPixelBrightnessBus.h \
        ../src/NeoPixelBus.h \
        ../src/NeoPixelSegmentBus.h \
        ../src/internal/SPIHandler.h \
        ../src/internal/DotStarColorFeatures.h \
        ../src/internal/DotStarGenericMethod.h \
        ../src/internal/Esp32_i2s.h \
        ../src/internal/HsbColor.h \
        ../src/internal/HslColor.h \
        ../src/internal/HtmlColor.h \
        ../src/internal/HtmlColorNameStrings.h \
        ../src/internal/Layouts.h \
        ../src/internal/NeoBitmapFile.h \
        ../src/internal/NeoBuffer.h \
        ../src/internal/NeoBufferContext.h \
        ../src/internal/NeoBufferMethods.h \
        ../src/internal/NeoColorFeatures.h \
        ../src/internal/NeoDib.h \
        ../src/internal/NeoEase.h \
        ../src/internal/NeoGamma.h \
        ../src/internal/NeoHueBlend.h \
        ../src/internal/NeoMosaic.h \
        ../src/internal/NeoRingTopology.h \
        ../src/internal/NeoSpriteSheet.h \
        ../src/internal/NeoTiles.h \
        ../src/internal/NeoTopology.h \
        ../src/internal/RgbColor.h \
        ../src/internal/RgbwColor.h \


SOURCES +=  ../src/internal/NeoPixelAnimator.cpp \
        ../src/internal/Esp32_i2s.c \
        ../src/internal/SPIHandler.cpp \
        ../src/internal/HsbColor.cpp \
        ../src/internal/HslColor.cpp \
        ../src/internal/HtmlColor.cpp \
        ../src/internal/HtmlColorNames.cpp \
        ../src/internal/HtmlColorNameStrings.cpp \
        ../src/internal/HtmlColorShortNames.cpp \
        ../src/internal/NeoGamma.cpp \   
        ../src/internal/NeoPixelAvr.c \
        ../src/internal/RgbColor.cpp \
        ../src/internal/RgbwColor.cpp \

