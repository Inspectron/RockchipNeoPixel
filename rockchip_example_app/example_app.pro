TARGET = exampleLEDapp

SOURCES += \
    $$PWD/main.cpp \
    $$PWD/LEDHandler.cpp \

HEADERS += \
    $$PWD/LEDHandler.hpp \
    $$PWD/LEDTypes.h

INCLUDEPATH  += ../src

INCLUDEPATH += $$PWD/include

unix {
    LIBS += -L$$PWD/lib -lNeoPixelBus
    LIBS += -L$$PWD/lib/ -linspCore
}
