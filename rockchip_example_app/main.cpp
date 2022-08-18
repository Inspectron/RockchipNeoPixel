#include "LEDHandler.hpp"
#include "inspcore.hpp"


using namespace LEDHANDLER;

LEDHandler* pLEDHandler;


int main()
{

    InspCore::setFormattedDebugOutput("Led App");
    qDebug()<<__LINE__;
#if 1
    pLEDHandler = new LEDHandler();

    // spi dev filename const is in ../src/internal/TwoWireSpiImple.h
    // spi linux handler is in ../src/internal/SPIHandler.h
    // ! spi has not been tested on hardware !
    pLEDHandler->startSpi();

    pLEDHandler->turnOffAllLEDs();
    pLEDHandler->testLights();

    // test states of some leds
    pLEDHandler->setLEDState(eLED_STATE_LASER_DOT_ON);
    pLEDHandler->setLEDState(eLED_STATE_LIVE_STREAM_ACTIVE);
    pLEDHandler->setLEDState(eLED_STATE_ZOOM_STEP_ONE);

    pLEDHandler->setLEDWifiStatusState(eLED_WIFI_HOTSPOT_CONNECTED);

    // test battery led blinking
    pLEDHandler->setLEDBatteryStatusState(eLED_BATTERY_CHARGING);
    pLEDHandler->testLights();
    
#endif

#if 0
    const uint16_t pixels = 22;
    const RgbColor COLOR_RED        (255,   0,   0);
    const RgbColor COLOR_BLUE       (  0,   0, 255);
    const RgbColor COLOR_GREEN      (  0, 255,   0);
    const RgbColor COLOR_LIGHT_GREEN ( 19, 255,  15);
    const RgbColor COLOR_WHITE      (255, 255, 255);
    const RgbColor COLOR_BLACK      (  0,   0,   0);
    const RgbColor COLOR_YELLOW     (255, 255,   0);
    const RgbColor COLOR_PURPLE     (255,   0, 255);
    const RgbColor COLOR_ORANGE     (255, 50,   0);

    NeoPixelBus<DotStarBgrFeature, DotStarSpi2MhzMethod> mTrial(pixels);
    mTrial.Begin();
    mTrial.ClearTo(COLOR_BLACK);
    mTrial.Show();

    mTrial.SetPixelColor(1, COLOR_RED);
    mTrial.SetPixelColor(2, COLOR_GREEN);
    mTrial.SetPixelColor(3, COLOR_ORANGE);
    mTrial.SetPixelColor(4, COLOR_BLUE);
    mTrial.SetPixelColor(5, COLOR_GREEN);
    mTrial.SetPixelColor(6, COLOR_ORANGE);
    mTrial.SetPixelColor(7, COLOR_RED);
    mTrial.SetPixelColor(8, COLOR_BLUE);
    mTrial.SetPixelColor(9, COLOR_GREEN);
    mTrial.SetPixelColor(10, COLOR_ORANGE);
    mTrial.SetPixelColor(11, COLOR_RED);
    mTrial.SetPixelColor(12, COLOR_PURPLE);
    mTrial.SetPixelColor(13, COLOR_GREEN);
    mTrial.SetPixelColor(14, COLOR_PURPLE);
    mTrial.SetPixelColor(15, COLOR_RED);
    mTrial.SetPixelColor(16, COLOR_PURPLE);
    mTrial.SetPixelColor(17, COLOR_GREEN);
    mTrial.SetPixelColor(18, COLOR_PURPLE);
    mTrial.SetPixelColor(19, COLOR_RED);
    mTrial.SetPixelColor(20, COLOR_PURPLE);
    //processBlendedAnimationForever(0, PIXEL_COUNT, ONE_SECOND_DURATION, 3, RGBTestColors);
    mTrial.Show();
#endif


    return 0;
}
