#include <unistd.h>
#include <QVector>
#include "inspcore.hpp"
#include "LEDHandler.hpp"

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

    QVector<uint16_t> vec = {0,1,2,3};
    QVector<RgbColor> veccolor = {COLOR_BLUE,COLOR_GREEN,COLOR_PURPLE,COLOR_LIGHT_GREEN};
    //test 3
    while(1){
        usleep(1000);
        pLEDHandler->setLed(vec,veccolor);
    }
#endif
    return 0;
}
