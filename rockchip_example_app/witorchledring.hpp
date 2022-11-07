#ifndef WITORCHLEDRING_HPP
#define WITORCHLEDRING_HPP


#include <QObject>
#include <QThread>
#include "LEDHandler.hpp"

namespace WITORCH_LED_RING
{
    // LED Modes
    enum eLED_MODE
    {
        eMODE_OFF,                           // ring is off
        eMODE_WIFI_HOTSPOT_CONNECTED,        // ring is solid green
        eMODE_WIFI_INFRASTRUCTURE_CONNECTED, // ring is solid blue
    };
}

/**
 * @brief The WiTorchLEDRing class - object to control the LED Ring to customer specs
 *
 * The LED ring indexes are laid out like this:
 *
 *
 *     <front of board>
 *
 *       (14)  (15)  (0)   (1)
 *        O     O    O     O
 *
 * (13)  O                    O  (2)
 *
 * (12)  O                    O  (3)
 *
 * (11)  O                    O  (4)
 *
 * (10)  O                    O  (5)
 *
 *        (9)   (8)   (7) (6)
 *        O     O    O     O
 *        X     O    O     O
 *             (16) (17) (18)
 *
 */
class WiTorchLEDRing
{
public:
    WiTorchLEDRing();
    ~WiTorchLEDRing();
    void init();
    void setMode(WITORCH_LED_RING::eLED_MODE mode);
    void blinkMediaCapture();
    void blinkMicrophone();
    // TODO add functions to individually change the status LEDs (wifi/etc)

private:



private:
    WITORCH_LED_RING::eLED_MODE mCurrentLEDMode;
    QThread *mpLEDHandlerThread;
    LEDHandler *mpLEDHandler;


};

#endif // WITORCHLEDRING_HPP
