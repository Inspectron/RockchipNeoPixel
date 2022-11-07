TARGET = exampleLEDapp

SOURCES += \
    $$PWD/main.cpp \
    $$PWD/LEDHandler.cpp \
    witorchledring.cpp

HEADERS += \
    $$PWD/LEDHandler.hpp \
    $$PWD/LEDTypes.h \
    witorchledring.hpp

INCLUDEPATH  += ../src

INCLUDEPATH += $$PWD/include

unix {
    LIBS += -L$$PWD/lib -lNeoPixelBus
    LIBS += -L$$PWD/lib/ -linspCore
}
