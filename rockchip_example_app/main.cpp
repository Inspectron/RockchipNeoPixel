#include "LEDHandler.hpp"
#include "inspcore.hpp"
#include <QVector>


using namespace LEDHANDLER;

LEDHandler* pLEDHandler;

#define DBG 0


int main()
{

    InspCore::setFormattedDebugOutput("Led App");
#if DBG

    pLEDHandler = new LEDHandler();

    // spi dev filename const is in ../src/internal/TwoWireSpiImple.h
    // spi linux handler is in ../src/internal/SPIHandler.h
    // ! spi has not been tested on hardware !
    pLEDHandler->startSpi();

    pLEDHandler->turnOffAllLEDs();

    // test states of some leds
    pLEDHandler->setLEDState(eLED_STATE_LASER_DOT_ON);
    pLEDHandler->setLEDState(eLED_STATE_LIVE_STREAM_ACTIVE);
    pLEDHandler->setLEDState(eLED_STATE_ZOOM_STEP_ONE);

    pLEDHandler->setLEDWifiStatusState(eLED_WIFI_HOTSPOT_CONNECTED);

    // test battery led blinking
    pLEDHandler->setLEDBatteryStatusState(eLED_BATTERY_CHARGING);

#else
    pLEDHandler = new LEDHandler();
    RgbColor CGN      (  0, 255,   0);
    RgbColor CGN1      (  0, 255,   255);
    RgbColor CGN2      (  255, 0,   0);
    RgbColor CGN3      (  255, 255,   0);
    uint16_t radio = 6;
    uint16_t radio1 = 7;
    uint16_t radio2 = 8;
    uint16_t radio3 = 9;
    uint16_t radio4 = 10;
    uint16_t radio5 = 11;
    uint16_t radio6 = 12;
    uint16_t radio7 = 13;
    uint16_t radio8 = 14;
    uint16_t radio9 = 15;
    uint16_t radio10 = 16;
    uint16_t radio11 = 17;
    uint16_t radio12 = 18;
    uint16_t radio13 = 19;
    uint16_t radio14 = 20;

    QVector<uint16_t> vec = {0,1,2,3};
    QVector<RgbColor> veccolor = {CGN,CGN1,CGN2,CGN3};
    //test 1
    pLEDHandler->setLed(radio,CGN);
    pLEDHandler->setLed(radio1,CGN1);
    pLEDHandler->setLed(radio2,CGN2);
    pLEDHandler->setLed(radio3,CGN);
    pLEDHandler->setLed(radio4,CGN1);
    pLEDHandler->setLed(radio5,CGN1);
    pLEDHandler->setLed(radio6,CGN2);
    pLEDHandler->setLed(radio7,CGN2);
    pLEDHandler->setLed(radio8,CGN);
    pLEDHandler->setLed(radio9,CGN);
    pLEDHandler->setLed(radio10,CGN1);
    pLEDHandler->setLed(radio11,CGN);
    pLEDHandler->setLed(radio12,CGN2);
    pLEDHandler->setLed(radio13,CGN1);
    pLEDHandler->setLed(radio14,CGN1);
    //test 2
    pLEDHandler->setLed(vec,CGN);
    //test 3
    while(1){
        pLEDHandler->setLed(vec,veccolor);
    }
#endif
    return 0;
}
