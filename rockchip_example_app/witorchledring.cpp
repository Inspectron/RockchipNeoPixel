#include "witorchledring.hpp"
#include <QTimer>
#include <QDebug>
#include <QtConcurrent/QtConcurrent>

// anonymous namespace
namespace
{
    const int RING_BLINK_DURATION = 200; // 200ms
    const int MIC_LED_START_IDX   = 12;
    const int MIC_LED_END_IDX     = 15;
}

/**
 * @brief WiTorchLEDRing::WiTorchLEDRing - ctor
 */
WiTorchLEDRing::WiTorchLEDRing()
: mCurrentLEDMode(WITORCH_LED_RING::eMODE_OFF)
, mpLEDHandlerThread(new QThread())
, mpLEDHandler(new LEDHandler())
{

}

/**
 * @brief WiTorchLEDRing::~WiTorchLEDRing - dtor
 */
WiTorchLEDRing::~WiTorchLEDRing()
{
    delete mpLEDHandlerThread;
}

/**
 * @brief WiTorchLEDRing::init - init the LED ring
 */
void WiTorchLEDRing::init()
{
    // move the LED handler to a new thread
    mpLEDHandler->moveToThread(mpLEDHandlerThread);
    QObject::connect(mpLEDHandlerThread, &QThread::started, mpLEDHandler, &LEDHandler::start);
    QObject::connect(mpLEDHandlerThread, &QThread::finished, mpLEDHandler, &LEDHandler::deleteLater);
    // start it
    mpLEDHandlerThread->start();
    // set the mode
    setMode(mCurrentLEDMode);
}

/**
 * @brief WiTorchLEDRing::setMode - set the mode of the ring
 */
void WiTorchLEDRing::setMode(WITORCH_LED_RING::eLED_MODE mode)
{
    mCurrentLEDMode = mode;
    RgbColor color = LEDHANDLER::COLOR_BLACK;

    switch (mCurrentLEDMode)
    {
    case WITORCH_LED_RING::eMODE_WIFI_HOTSPOT_CONNECTED:
        color = LEDHANDLER::COLOR_GREEN;
        break;
    case WITORCH_LED_RING::eMODE_WIFI_INFRASTRUCTURE_CONNECTED:
        color = LEDHANDLER::COLOR_BLUE;
        break;
    case WITORCH_LED_RING::eMODE_OFF:
        // intentional fallthrough
    default:
        // default off
        break;
    }

    // set all the pixels
    mpLEDHandler->setAllPixels(color);
}

/**
 * @brief WiTorchLEDRing::blinkMediaCapture - blink the ring momentarily when theres a photo capture
 */
void WiTorchLEDRing::blinkMediaCapture()
{
    if (mCurrentLEDMode != WITORCH_LED_RING::eMODE_OFF)
    {
        QtConcurrent::run([&]()
        {
            // blink the ring off
            mpLEDHandler->setAllPixels(LEDHANDLER::COLOR_BLACK);

            // short sleep
            QThread::msleep(RING_BLINK_DURATION);

            // revert the mode
            setMode(mCurrentLEDMode);

        });
    }
}

/**
 * @brief WiTorchLEDRing::blinkMicrophone - blink the mic quadrant 2x when the mic button is pressed
 * The Mic quadrant is index 12->15
 */
void WiTorchLEDRing::blinkMicrophone()
{
    if (mCurrentLEDMode != WITORCH_LED_RING::eMODE_OFF)
    {
        QtConcurrent::run([&]()
        {
            // blink the quadrant 2x
            mpLEDHandler->setPixelRange(MIC_LED_START_IDX, MIC_LED_END_IDX, LEDHANDLER::COLOR_WHITE);
            QThread::msleep(RING_BLINK_DURATION);
            mpLEDHandler->setPixelRange(MIC_LED_START_IDX, MIC_LED_END_IDX, LEDHANDLER::COLOR_BLACK);
            QThread::msleep(RING_BLINK_DURATION);
            mpLEDHandler->setPixelRange(MIC_LED_START_IDX, MIC_LED_END_IDX, LEDHANDLER::COLOR_WHITE);
            QThread::msleep(RING_BLINK_DURATION);
            mpLEDHandler->setPixelRange(MIC_LED_START_IDX, MIC_LED_END_IDX, LEDHANDLER::COLOR_BLACK);
            QThread::msleep(RING_BLINK_DURATION);
            // revert the mode
            setMode(mCurrentLEDMode);
        });
    }
}
