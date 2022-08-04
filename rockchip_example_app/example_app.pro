TARGET = exampleLEDapp

SOURCES += \
    $$PWD/main.cpp \
    $$PWD/LEDHandler.cpp \

HEADERS += \
    $$PWD/LEDHandler.hpp \
    $$PWD/LEDTypes.h

INCLUDEPATH  += ../src

unix {
    LIBS += -L$$PWD/lib -lNeoPixelBus
}
