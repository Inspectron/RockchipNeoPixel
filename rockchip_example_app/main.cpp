#include <unistd.h>
#include <QVector>
#include <QThread>
#include "inspcore.hpp"
#include "LEDHandler.hpp"

using namespace LEDHANDLER;

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
    LEDHandler *pLEDHandler = new LEDHandler();
    QThread *pThread = new QThread;
    pLEDHandler->moveToThread(pThread);
    QObject::connect( pThread, SIGNAL(started()), pLEDHandler, SLOT(start()) );
    QObject::connect( pThread, SIGNAL(finished()), pLEDHandler, SLOT(stop()) );

    // start the thread
    pThread->start();

    while(1){
        pLEDHandler->setUpLights();
    }
#endif
    return 0;
}
