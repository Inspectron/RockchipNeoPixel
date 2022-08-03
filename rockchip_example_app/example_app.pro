TARGET = exampleLEDapp

SOURCES += \
    $$PWD/main.cpp \
    $$PWD/LEDHandler.cpp \

HEADERS += \
    $$PWD/LEDHandler.hpp \
    $$PWD/LEDTypes.h \
    include/NeoPixelAnimator.h \
    include/NeoPixelBrightnessBus.h \
    include/NeoPixelBus.h \
    include/NeoPixelSegmentBus.h

INCLUDEPATH  += include

unix {
    LIBS += -L$$PWD/lib -lNeoPixelBus
}
