
#ifndef LEDHANDLER_HPP_
#define LEDHANDLER_HPP_

#include <QObject>

#include "NeoPixelAnimator.h"
#include "LEDTypes.h"

namespace LEDHANDLER
{
    const uint8_t SATURATION = 255;

    //color definitions
    //TO DO:moved from LEDHandler.cpp
    const RgbColor COLOR_RED        (SATURATION,   0,   0);
    const RgbColor COLOR_BLUE       (  0,   0, SATURATION);
    const RgbColor COLOR_GREEN      (  0, SATURATION,   0);
    const RgbColor COLOR_LIGHT_GREEN ( 19, SATURATION,  15);
    const RgbColor COLOR_WHITE      (SATURATION, SATURATION, SATURATION);
    const RgbColor COLOR_BLACK      (  0,   0,   0);
    const RgbColor COLOR_YELLOW     (SATURATION, SATURATION,   0);
    const RgbColor COLOR_PURPLE     (SATURATION,   0, SATURATION);
    const RgbColor COLOR_ORANGE     (SATURATION, 50,   0);


    enum eLEDStates
    {
        eLED_STATE_INACTIVE = 0x00,
        eLED_STATE_ZOOM_INACTIVE,
        eLED_STATE_ZOOM_STEP_ONE,
        eLED_STATE_ZOOM_STEP_TWO,
        eLED_STATE_ZOOM_STEP_THREE,
        eLED_STATE_LED_STEP_INACTIVE = 0x05,
        eLED_STATE_LED_STEP_ONE,
        eLED_STATE_LED_STEP_TWO,
        eLED_STATE_LED_STEP_THREE,
        eLED_STATE_TAKE_PICTURE,
        eLED_STATE_VIDEO_CAPTURE_START = 0x0A,
        eLED_STATE_VIDEO_CAPTURE_END,
        eLED_STATE_LIVE_STREAM_ACTIVE,
        eLED_STATE_LIVE_STREAM_INACTIVE,
        eLED_STATE_UPLOAD_TO_CLOUD_START,
        eLED_STATE_UPLOAD_TO_CLOUD_END = 0x0F,
        eLED_STATE_LASER_DOT_ON,
    };

    enum eScanStates
    {
        eSCAN_INACTIVE = 0x00,
        eSCAN_ACTIVE = 0x01, //scanning is active
        eSCAN_SCANNED = 0x02, //Successful scan
        eSCAN_ERROR = 0x03 //error scanning
    };

    enum eLEDWifiStatus
    {
        eLED_WIFI_BOOTING = 0x00,
        eLED_WIFI_HOTSPOT_NO_CONNECTION,
        eLED_WIFI_HOTSPOT_CONNECTED,
        eLED_WIFI_INFRASTRUCTURE_NO_CONNECTION,
        eLED_WIFI_INFRASTRUCTURE_CONNECTED,
        eLED_WIFI_ERROR
    };

    enum eLEDMicStatus
    {
        eLED_MIC_OFF = 0x00,
        eLED_MIC_ON = 0x01,
        eLED_MIC_IDLE = 0x02
    };

    enum eLEDBatteryStatus
    {
        eLED_BATTERY_INACTIVE = 0x00,
        eLED_BATTERY_CHARGING,
        eLED_BATTERY_CHARGE_COMPLETE,
        eLED_BATTERY_HALF_FULL,
        eLED_BATTERY_QUARTER_FULL,
        eLED_BATTERY_ERROR
    };
}


class LEDHandler
: public QObject
{
    Q_OBJECT
public:
	LEDHandler();
	virtual ~LEDHandler();

    void setUpLights();
	void determineLightState();

	//main button LED states
	void setLEDState(const uint8_t ledState);

	void setLEDScanState(uint8_t state);

	void setPowerLEDOn(bool isOn);
	
	void turnOffAllLEDs(bool isChargeLightOn=false);

    //Set leds to required color
    void setLed(uint16_t &index, const RgbColor &color);
    void setLed(QVector<uint16_t> &index,  const RgbColor &color);
    void setLed(QVector<uint16_t> &index, QVector<RgbColor> &color);
	
	//status indicators
	void setLEDBatteryStatusState(uint8_t state);
	void setLEDWifiStatusState(uint8_t state);
	void setLEDBluetoothStatusState(uint8_t state);
	void setLEDMicStatusState(uint8_t state);
	
	inline void setPowerLightIsBooting(bool isBooting)
	{
		mIsPowerLightBlinking = isBooting;
	}

public slots:
    void start();
    void stop();

private:
	void determineWifiStatusState();
	void determineMicStatusState();
	void determineScanStateLights();
	void determinePowerLightState();
	void determineBatteryLightState();
	
	static void fadeAnimUpdate(const AnimationParam& param);
	static void loopAnimUpdate(const AnimationParam& param);
    static void loopAnimUpdate1(const AnimationParam& param);
    static void loopAnimUpdate2(const AnimationParam& param);

	void fadeInFadeOutRinseRepeat(uint8_t animIdx);
	void blendInBlendOutRinseRepeat();

	void processAnimationForNmillseconds(uint16_t nMilliseconds, uint8_t startLED,
			uint8_t nLEDs, RgbColor LEDcolor, uint16_t duration_ms, uint8_t animIdx);

	void processAnimationForever(uint8_t startLED, uint8_t nLEDs,
			RgbColor LEDcolor, uint16_t duration_ms, uint8_t animIdx);

	void processBlendedAnimationForever(uint8_t startLED, uint8_t nLEDs,
			uint16_t duration_ms, uint8_t nColors, const RgbColor* colors);

	void drawTailPixels();

	void setFlashlightLED(LEDTypes::eLEDPercentStep step);
	void setZoomLED(const uint8_t zoomState);
	void setLaserLED();
	
	void clearLEDRing();
	void stopAnimations();
	void resetRing();

	void resumeAnimations(uint8_t animIdx);
	void checkPreviousState();

	bool setupAnimationTimer(const uint8_t index, const uint32_t duration);
	
	void processAnimationForeverWRTBarcode(uint8_t startIdx, uint8_t numberOfLEDs,
			RgbColor color, uint16_t duration, uint8_t animIdx);
	void processBlendedAnimationForeverWRTBarcode(uint8_t startIdx, uint8_t numberOfLEDs,
			uint16_t duration, uint8_t nColors, const RgbColor* colors);

//	const char* getColorString(const RgbColor &color);
//	const char* getLightStateString(unsigned int state);

	static LEDHandler* msLEDHandler;
	
	uint8_t mLEDState;
	uint8_t mPreviousLEDState;
	uint8_t mMicStatus;
	uint8_t mWifiStatus;
	uint8_t mBluetoothStatus;
	uint8_t mBarcodeScanState;
	uint8_t mBatteryChargeState;
	uint8_t mInactiveClearCounter;
	bool mIsVideoCaptureInProgress;
	bool mIsLiveStreamActive;
	bool mIsPowerLightBlinking;
	
	// one entry per pixel to match the animation timing manager
	LEDTypes::InspectronAnimationState* mAnimationStates;

	NeoPixelBus<DotStarBgrFeature, DotStarSpi2MhzMethod> mPixelStrip;
	NeoPixelAnimator mPixelAnimator;
};

#endif /* LEDHANDLER_HPP_ */
