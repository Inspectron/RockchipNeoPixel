#include <time.h>
#include <QDebug>
#include <QTimer>
#include "LEDHandler.hpp"

using namespace LEDTypes;
using namespace LEDHANDLER;

namespace
{
    const uint16_t PIXEL_RING_COUNT			= 16;
    const uint16_t PIXEL_COUNT				= 19; // number of pixels on the LED ring

    //test vectors
    QVector<uint16_t> vec = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    QVector<uint16_t> vec1 = {6,7,8,9};
    QVector<uint16_t> vec2 = {15,16,17,18};
    QVector<uint16_t> vec3 = {0,1,2,3};
    QVector<RgbColor> veccolor = {COLOR_RED,COLOR_BLUE,COLOR_GREEN,COLOR_LIGHT_GREEN,COLOR_YELLOW,COLOR_PURPLE,COLOR_ORANGE,
                                  COLOR_RED,COLOR_BLUE,COLOR_GREEN,COLOR_LIGHT_GREEN,COLOR_YELLOW,COLOR_PURPLE,COLOR_ORANGE,
                                  COLOR_RED,COLOR_BLUE,COLOR_GREEN,COLOR_LIGHT_GREEN,COLOR_BLUE,COLOR_ORANGE};
    QVector<RgbColor> veccolor1 = {COLOR_RED,COLOR_BLUE,COLOR_GREEN,COLOR_PURPLE};
    QVector<RgbColor> veccolor2 = {COLOR_BLUE,COLOR_PURPLE,COLOR_LIGHT_GREEN,COLOR_RED};

	const uint8_t POWER_LED					=  7;   // PA7
	const uint8_t LED_COB_LED_START_INDEX = 0;
	const uint8_t BARCODE_LED_START_INDEX = 4;
	//  const uint8_t CAMERA_LED_START_INDEX = 9;
	const uint8_t ZOOM_LED_START_INDEX = 12;
	const uint8_t WIFI_STATUS_LIGHT_IDX = PIXEL_RING_COUNT;
	const uint8_t BLUETOOTH_STATUS_LIGHT_IDX = WIFI_STATUS_LIGHT_IDX + 1;
	const uint8_t MIC_STATUS_LIGHT_IDX 	= WIFI_STATUS_LIGHT_IDX + 2;
	const uint8_t BATTERY_STATUS_LIGHT_IDX = WIFI_STATUS_LIGHT_IDX + 3;
	


	//animation indexes
	const uint8_t FLASHLIGHT_ANIMATION_IDX = 0;
	const uint8_t BLEND_ANIMATION_IDX = 1;
	const uint8_t CLOUD_ANIMATION_IDX = 2;
	const uint8_t MIC_ANIMATION_IDX	= 3;
	const uint8_t WIFI_ANIMATION_IDX = 4;
	const uint8_t SCAN_ANIMATION_IDX = 5;
	const uint8_t VID_CAP_ANIMATION_IDX = 6;
	const uint8_t LIVE_STREAM_ANIMATION_IDX = 7;
	const uint8_t TAKE_PIC_ANIMATION_IDX = 8;
	const uint8_t ZOOM_ANIMATION_IDX = 9;
	const uint8_t LASER_ANIMATION_IDX = 10;
	const uint8_t POWER_ANIMATION_IDX = 11;
	const uint8_t BATTERY_ANIMATION_IDX = 12;
	const uint8_t NUM_ANIM_CHANNELS = 13; // number of different animations we have
	
	const uint32_t ONE_SECOND_DURATION = 1000;
	const uint32_t ONE_HALF_SECOND_DURATION = 1500;
	const uint32_t TWO_SECOND_DURATION = 2000;
	const uint32_t ONE_HUNDRED_MILLISECOND_DURATION = 100;
	const uint32_t FIVE_HUNDRED_MILLISECOND_DURATION = 500;
	const uint32_t QUARTER_SECOND_DURATION = 250;

	const uint8_t MAX_INACTIVE_CLEARS = 10;
	const uint8_t LED_SECTION_SIZE 	= 4; //number of LED's per button section
	const uint16_t LED_TAIL_LENGTH 	= 6; // length of the animation tail, must be shorter than PixelCount
	const float MAX_LIGHTNESS 		= 0.4f; // max lightness at the head of the tail (0.5f is full bright)

	const RgbColor WIFI_COLOR_BOOTING		= COLOR_RED;
	const RgbColor WIFI_HOTSPOT_COLOR 		= COLOR_GREEN;//OP Task 3839 change from grn. to blue
	const RgbColor WIFI_INFRA_COLOR 		= COLOR_YELLOW;//OP Task 3839 change from yel. to blue
	const RgbColor FLASHLIGHT_COLOR 		= COLOR_WHITE;
	const RgbColor ZOOM_COLOR 				= COLOR_WHITE;
	const RgbColor VID_CAP_COLOR 			= COLOR_BLUE; //OP Task 3839 change from blue to red.
	const RgbColor PICTURE_COLOR			= COLOR_WHITE; //OP Task 3839 change from yel. to wht.
	const RgbColor LIVE_STREAM_COLOR 		= COLOR_LIGHT_GREEN; //OP Task 3839 change from grn. to red
	const RgbColor SCANNING_COLOR 			= COLOR_WHITE;
	const RgbColor SCANNED_COLOR 			= COLOR_GREEN;
	const RgbColor UPLOAD_TO_CLOUD_COLOR 	= COLOR_WHITE;
	const RgbColor BLUETOOTH_COLOR			= COLOR_BLUE;
	const RgbColor MIC_COLOR				= COLOR_GREEN; //OP Task 3839 change from red to grn.
	const RgbColor LASER_COLOR				= COLOR_WHITE;
	const RgbColor BATTERY_IDLE_COLOR		= COLOR_GREEN;
	const RgbColor BATTERY_CHARGING_COLOR	= COLOR_GREEN;
	const RgbColor BATTERY_HALF_COLOR		= COLOR_ORANGE;//orange
	const RgbColor BATTERY_QUARTER_COLOR	= COLOR_RED;

	const RgbColor LiveStreamToVidCapColors[2] = {VID_CAP_COLOR, LIVE_STREAM_COLOR};
	const RgbColor RGBTestColors[3] = {COLOR_RED, COLOR_GREEN, COLOR_BLUE};


}

LEDHandler* LEDHandler::msLEDHandler;

LEDHandler::LEDHandler()
: mLEDState(eNoOp)
, mPreviousLEDState(eInactive)
, mBatteryChargeState(eBatteryChargerIdle)
, mInactiveClearCounter(0)
, mIsVideoCaptureInProgress(false)
, mIsLiveStreamActive(false)
, mIsPowerLightBlinking(false)
, mAnimationStates(new InspectronAnimationState[NUM_ANIM_CHANNELS])
, mPixelStrip(PIXEL_COUNT)
, mPixelAnimator(NUM_ANIM_CHANNELS)
{
	if(msLEDHandler == NULL)
	{
		msLEDHandler = this;
	}
	
	//empty out all the structures
	for (int i = 0; i < NUM_ANIM_CHANNELS; i++)
	{
		mAnimationStates[i].clear();
	}

	mAnimationStates[WIFI_ANIMATION_IDX].nLEDs = 1;
	mAnimationStates[WIFI_ANIMATION_IDX].startLEDindex = WIFI_STATUS_LIGHT_IDX;
	mAnimationStates[WIFI_ANIMATION_IDX].run_time = ONE_SECOND_DURATION;
	mAnimationStates[WIFI_ANIMATION_IDX].nColors = 1;
	mAnimationStates[WIFI_ANIMATION_IDX].currentColorIdx = 0;

    //Initialize the Pixel strip and use SPI[mPixelStrip(PIXEL_COUNT) indicates we are using SPI method]
	mPixelStrip.Begin();

    //reset all the neopixels to an off state
	mPixelStrip.ClearTo(COLOR_BLACK);
	mPixelStrip.Show();

}

LEDHandler::~LEDHandler()
{
}
/*
 * Loop function which needs to run through the time of exection
*/
void LEDHandler::setUpLights()
{
    mPixelStrip.ClearTo(COLOR_BLACK);
    //mPixelAnimator.UpdateAnimations(); //Can include this once we start animations according to req

    //Library function  which sends the data to actual physical pixels
    mPixelStrip.Show();
}

//Thread Function
void LEDHandler::start()
{
    mIsVideoCaptureInProgress = true;

    while(mIsVideoCaptureInProgress)
    {
      setLed(vec1,veccolor1);
    }

}

void LEDHandler::stop()
{
    mIsVideoCaptureInProgress = false;
}

/*
 * index- list of all pixels you want to modify
 * color- ledcolor to set for provided set of indexes.
*/
void LEDHandler::setLed(uint16_t &index,const RgbColor &color)
{
    mPixelStrip.SetPixelColor(index,color);
}

/*
 * index- list of all pixels you want to modify
 * color- ledcolor to set for provided set of indexes.
*/
void LEDHandler::setLed(QVector<uint16_t> &index, const RgbColor &color)
{
    uint16_t colorindex =0;
    for(int i =0;i < index.size();i++)
   {
    colorindex = index.at(i);
    mPixelStrip.SetPixelColor(colorindex,color);
   }

}

/*
 * index- list of all pixels you want to modify
 * color- list of all ledcolor you want to set for a respective led.
 * size of input params should be same
 * Note-for now this function sets index[i] to color[i]
*/
void LEDHandler::setLed(QVector<uint16_t> &index, QVector<RgbColor> &color)
{
    uint16_t colorindex = 0;
    RgbColor ledcolor = COLOR_BLACK;
    if(index.size()==color.size())
    {
        for(int i =0;i < index.size();i++)
       {
        colorindex = index.at(i);
        ledcolor = color.at(i);
        setLed(colorindex,ledcolor);
       }
    }
    else
    {
        qWarning()<<"pixel and color vector should be of same size";
    }


}

void LEDHandler::turnOffAllLEDs(bool isChargeLightOn)
{
	mPixelStrip.ClearTo(COLOR_BLACK, 0, PIXEL_COUNT);
	
	setLEDState(eLED_STATE_INACTIVE);
	setLEDScanState(eSCAN_INACTIVE);
	setLEDWifiStatusState(eLED_WIFI_BOOTING);
	setLEDMicStatusState(eLED_MIC_OFF);
	
	mIsPowerLightBlinking = false;
	if(!isChargeLightOn)
	{
		setLEDBatteryStatusState(eLED_BATTERY_ERROR);
	}
	determineLightState();
}

void LEDHandler::determineLightState()
{
	switch(mLEDState)
	{
	case eInactive:
		mIsLiveStreamActive = false;
		mIsVideoCaptureInProgress = false;
		resetRing();
		if(mInactiveClearCounter++ > MAX_INACTIVE_CLEARS)
		{
			mLEDState = eNoOp;
			mInactiveClearCounter = 0;
			//Serial.println("setting LED state to NoOp");
		}
		break;
	case eZoomInactive:
		clearLEDRing();
		checkPreviousState();
		break;
	case eZoomStepOne:
		setZoomLED(eZoomStepOne);
		break;
	case eZoomStepTwo:
		setZoomLED(eZoomStepTwo);
		break;
	case eZoomStepThree:
		setZoomLED(eZoomStepThree);
		break;
	case eLEDStepInactive:
		clearLEDRing();
		checkPreviousState();
		break;
	case eLEDStepOne:
		setFlashlightLED(eLEDStep15);
		break;
	case eLEDStepTwo:
		setFlashlightLED(eLEDStep50);
		break;
	case eLEDStepThree:
		setFlashlightLED(eLEDStep100);
		break;
	case eTakePicture:
			processAnimationForNmillseconds(FIVE_HUNDRED_MILLISECOND_DURATION, 0, PIXEL_RING_COUNT, PICTURE_COLOR,
					FIVE_HUNDRED_MILLISECOND_DURATION, TAKE_PIC_ANIMATION_IDX);
		break;
	case eVideoCaptureStart:
		if(mIsLiveStreamActive)
		{
			processBlendedAnimationForeverWRTBarcode(0,PIXEL_RING_COUNT,ONE_SECOND_DURATION, 2, LiveStreamToVidCapColors);
		}
		else
		{
			processAnimationForeverWRTBarcode(0, PIXEL_RING_COUNT, VID_CAP_COLOR, ONE_SECOND_DURATION, VID_CAP_ANIMATION_IDX);
		}
		break;
	case eVideoCaptureEnd:
		resetRing();
		if(mIsLiveStreamActive)
		{
			processAnimationForeverWRTBarcode(0, PIXEL_RING_COUNT, LIVE_STREAM_COLOR, ONE_SECOND_DURATION, VID_CAP_ANIMATION_IDX);
			mLEDState = eLiveStreamingActive;
		}
		else
		{
			checkPreviousState();
		}
		break;
	case eLiveStreamingActive:
		if(mIsVideoCaptureInProgress)
		{
			processBlendedAnimationForeverWRTBarcode(0, PIXEL_RING_COUNT,ONE_SECOND_DURATION, 2, LiveStreamToVidCapColors);
		}
		else
		{
			processAnimationForeverWRTBarcode(0, PIXEL_RING_COUNT, LIVE_STREAM_COLOR, ONE_SECOND_DURATION, LIVE_STREAM_ANIMATION_IDX);
		}
		break;
	case eLiveStreamingInactive:
		resetRing();
		checkPreviousState();
		break;
	case eBarcodeScanStart:
		break;
	case eBarcodeScanEnd:
		break;
	case eUploadToCloudStart:
		resetRing();
		drawTailPixels();
		mPixelAnimator.StartAnimation(CLOUD_ANIMATION_IDX, 33, loopAnimUpdate);
		mLEDState = eUploadingToCloud;
		break;
	case eUploadingToCloud:
		mPixelAnimator.UpdateAnimations();
		mPixelStrip.Show();
		break;
	case eUploadToCloudEnd:
		mPixelAnimator.StopAll();
		mLEDState = eInactive;
		break;
	case eLaserDotOn:
		setLaserLED();
		break;
	case eNoOp:
	default:
		//do nothing
		break;
	}

	determineBatteryLightState();
	determineWifiStatusState();
	determineMicStatusState();
	determineScanStateLights();
	
	if(mPixelAnimator.IsAnimating())
	{
		mPixelAnimator.UpdateAnimations();
		mPixelStrip.Show();
	}
}

void LEDHandler::setLEDState(const uint8_t ledState)
{
	switch (ledState)
	{
	case eLED_STATE_ZOOM_INACTIVE:
        mLEDState = eZoomInactive;
		break;
	case eLED_STATE_ZOOM_STEP_ONE:
		mLEDState = eZoomStepOne;
		//reset the animation start time so they all get a fair share of time in rapid presses
		mAnimationStates[ZOOM_ANIMATION_IDX].start_time  = 0;
		break;
	case eLED_STATE_ZOOM_STEP_TWO:
		mLEDState = eZoomStepTwo;
		mAnimationStates[ZOOM_ANIMATION_IDX].start_time  = 0;
		break;
	case eLED_STATE_ZOOM_STEP_THREE:
		mLEDState = eZoomStepThree;
		mAnimationStates[ZOOM_ANIMATION_IDX].start_time  = 0;
		break;
	case eLED_STATE_LED_STEP_INACTIVE:
		mLEDState = eLEDStepInactive;
		break;
	case eLED_STATE_LED_STEP_ONE:
		mLEDState = eLEDStepOne;
		//reset the animation start time so they all get a fair share of time in rapid presses
		mAnimationStates[FLASHLIGHT_ANIMATION_IDX].start_time  = 0;
		break;
	case eLED_STATE_LED_STEP_TWO:
		mLEDState = eLEDStepTwo;
		mAnimationStates[FLASHLIGHT_ANIMATION_IDX].start_time  = 0;
		break;
	case eLED_STATE_LED_STEP_THREE:
		mLEDState = eLEDStepThree;
		mAnimationStates[FLASHLIGHT_ANIMATION_IDX].start_time  = 0;
		break;
	case eLED_STATE_TAKE_PICTURE:
		mLEDState = eTakePicture;
		break;
	case eLED_STATE_VIDEO_CAPTURE_START:
		mLEDState = eVideoCaptureStart;
		mIsVideoCaptureInProgress = true;
		break;
	case eLED_STATE_VIDEO_CAPTURE_END:
		mLEDState = eVideoCaptureEnd;
		mIsVideoCaptureInProgress = false;
		break;
	case eLED_STATE_LIVE_STREAM_ACTIVE:
		mLEDState = eLiveStreamingActive;
		mIsLiveStreamActive = true;
		break;
	case eLED_STATE_LIVE_STREAM_INACTIVE:
		mLEDState = eLiveStreamingInactive;
		mIsLiveStreamActive = false;
		break;
	case eLED_STATE_UPLOAD_TO_CLOUD_START:
		mLEDState = eUploadToCloudStart;
		break;
	case eLED_STATE_UPLOAD_TO_CLOUD_END:
		mLEDState = eUploadToCloudEnd;
		break;
	case eLED_STATE_LASER_DOT_ON:
		mLEDState = eLaserDotOn;
		break;
	case eLED_STATE_INACTIVE:
		//intentional fall through
	default:
		mLEDState = eInactive;
		break;
	}
}

void LEDHandler::setLEDScanState(uint8_t state)
{
	switch(state)
	{
	case eSCAN_INACTIVE:
		mBarcodeScanState = eScanInactive;
		break;
	case eSCAN_ACTIVE:
		mBarcodeScanState = eScanActive;
		break;
	case eSCAN_SCANNED:
		mBarcodeScanState = eScanScanned;
		break;
	case eSCAN_ERROR:
		mBarcodeScanState = eScanError;
		break;
	default:
		mBarcodeScanState = eScanNoOp;
		break;
	}
}

void LEDHandler::setLEDWifiStatusState(uint8_t state)
{
	mWifiStatus = state;	
}

void LEDHandler::setLEDBluetoothStatusState(uint8_t state)
{
	mBluetoothStatus = state;
}

void LEDHandler::setLEDMicStatusState(uint8_t state)
{
	mMicStatus = state;
}

void LEDHandler::determineScanStateLights()
{
	switch(mBarcodeScanState)
	{
	case eScanScanned:
		//processAnimationForNmillseconds(ONE_SECOND_DURATION, BARCODE_LED_START_INDEX, 4, SCANNED_COLOR, 100, SCAN_ANIMATION_IDX);
		{
			if(setupAnimationTimer(SCAN_ANIMATION_IDX, ONE_HALF_SECOND_DURATION))
			{
				uint8_t startLED = BARCODE_LED_START_INDEX, numLeds = LED_SECTION_SIZE;
				
				for(int i = 0; i < numLeds; i++)
				{
					mPixelStrip.SetPixelColor(startLED++, SCANNED_COLOR);
				}
				mPixelStrip.Show();
			}
			else
			{
				resumeAnimations(SCAN_ANIMATION_IDX);
			}
		}
		break;
	case eScanActive:
		//processAnimationForever(BARCODE_LED_START_INDEX, 4, SCANNING_COLOR, ONE_HUNDRED_MILLISECOND_DURATION, SCAN_ANIMATION_IDX);
		{
			uint8_t startLED = BARCODE_LED_START_INDEX;
				
			for(int i = 0; i < LED_SECTION_SIZE; i++)
			{
				mPixelStrip.SetPixelColor(startLED++, SCANNING_COLOR);
			}
			mPixelStrip.Show();
		}
		break;
	case eScanError:
		//processAnimationForNmillseconds(TWO_SECOND_DURATION, BARCODE_LED_START_INDEX, 4, COLOR_RED, 100, SCAN_ANIMATION_IDX);
		//intentional fall through
	case eScanInactive:
		mBarcodeScanState = eScanNoOp;
		resetRing();
		break;
	case eScanNoOp:
		//intentional fall through
	default:
		//turn the lights off
		break;
	}
}

void LEDHandler::setLEDBatteryStatusState(uint8_t state)
{
	switch(state)
	{
	case eLED_BATTERY_CHARGING:
        mBatteryChargeState = eZoomInactive;
		break;
	case eLED_BATTERY_HALF_FULL:
		mBatteryChargeState = eBatteryHalf;
		break;
	case eLED_BATTERY_QUARTER_FULL:
		mBatteryChargeState = eBatteryQuarter;
		break;		
	case eLED_BATTERY_CHARGE_COMPLETE:
		//intentional fall through
	case eLED_BATTERY_INACTIVE:
		mBatteryChargeState = eBatteryChargerIdle;
		break;
	case eLED_BATTERY_ERROR:
		//intentional fall through
	default:
		mBatteryChargeState = eBatteryChargerError;
		break;
	}
}

void LEDHandler::determineBatteryLightState()
{
	bool isAnimationNeeded = false;
	
	//the battery indicator is a solid color unless its charging
	switch(mBatteryChargeState)
	{
	case eBatteryChargerIdle:
 			mPixelStrip.SetPixelColor(BATTERY_STATUS_LIGHT_IDX, BATTERY_IDLE_COLOR);
		break;
	case eBatteryHalf:
			mPixelStrip.SetPixelColor(BATTERY_STATUS_LIGHT_IDX, BATTERY_HALF_COLOR);
		break;
	case eBatteryQuarter:
 			mPixelStrip.SetPixelColor(BATTERY_STATUS_LIGHT_IDX, BATTERY_QUARTER_COLOR);
		break;
	case eBatteryChargerError:
		//no defined error conditions
		mPixelStrip.SetPixelColor(BATTERY_STATUS_LIGHT_IDX, COLOR_BLACK);
		break;
	default:
		isAnimationNeeded = true;
		break;
	}

	if(isAnimationNeeded)
	{
		//we are charging the battery and need it to blink red 
		//(or whatever color they change their mind to in the future, a salmon color maybe?)
		if(setupAnimationTimer(BATTERY_ANIMATION_IDX, FIVE_HUNDRED_MILLISECOND_DURATION))
		{
			//do nothing here for now
		}
		else
		{
			if(mAnimationStates[BATTERY_ANIMATION_IDX].currentColorIdx == 0)
			{
				//use the current color idx as a way to figure out if the led should be turning
				//on or off
				mAnimationStates[BATTERY_ANIMATION_IDX].currentColorIdx = 1;
				mPixelStrip.SetPixelColor(BATTERY_STATUS_LIGHT_IDX, BATTERY_CHARGING_COLOR);
			}
			else
			{
				mAnimationStates[BATTERY_ANIMATION_IDX].currentColorIdx = 0;
				mPixelStrip.SetPixelColor(BATTERY_STATUS_LIGHT_IDX, COLOR_BLACK);
			}
			//reset the timer
			mAnimationStates[BATTERY_ANIMATION_IDX].start_time = 0;
			mAnimationStates[BATTERY_ANIMATION_IDX].current_duration = 0;
			mPixelStrip.Show();
		}
	}
}

void LEDHandler::determineWifiStatusState()
{
	if (mPixelAnimator.IsAnimationActive(WIFI_ANIMATION_IDX))
	{
		mPixelAnimator.UpdateAnimations();
		mPixelStrip.Show();
	}
	else
	{
		switch(mWifiStatus)
		{
		case eLED_WIFI_BOOTING:
			//turn the LED off
			mPixelStrip.SetPixelColor(WIFI_STATUS_LIGHT_IDX, COLOR_BLACK);
			//this no longer acts like the wiscope
			//processAnimationForever(WIFI_STATUS_LIGHT_IDX, 1, WIFI_COLOR_BOOTING, ONE_HUNDRED_MILLISECOND_DURATION, WIFI_ANIMATION_IDX);
			break;
		case eLED_WIFI_HOTSPOT_NO_CONNECTION:
			mAnimationStates[WIFI_ANIMATION_IDX].colors = &WIFI_HOTSPOT_COLOR;
			fadeInFadeOutRinseRepeat(WIFI_ANIMATION_IDX);
			break;
		case eLED_WIFI_HOTSPOT_CONNECTED:
			mPixelStrip.SetPixelColor(WIFI_STATUS_LIGHT_IDX, WIFI_HOTSPOT_COLOR);
			break;
		case eLED_WIFI_INFRASTRUCTURE_NO_CONNECTION:
			mAnimationStates[WIFI_ANIMATION_IDX].colors = &WIFI_INFRA_COLOR;
			fadeInFadeOutRinseRepeat(WIFI_ANIMATION_IDX);
			break;
		case eLED_WIFI_INFRASTRUCTURE_CONNECTED:
			mPixelStrip.SetPixelColor(WIFI_STATUS_LIGHT_IDX, WIFI_INFRA_COLOR);
			break;
		case eLED_WIFI_ERROR:
			mPixelStrip.SetPixelColor(WIFI_STATUS_LIGHT_IDX, COLOR_BLACK);
			break;
		default:
			break;
		}
	}
}

void LEDHandler::determineMicStatusState()
{
	switch(mMicStatus)
	{
	case eLED_MIC_ON:
		mPixelStrip.SetPixelColor(MIC_STATUS_LIGHT_IDX, MIC_COLOR);
		
		if(setupAnimationTimer(MIC_ANIMATION_IDX, ONE_HALF_SECOND_DURATION))
		{
			//the microphone button is a long press of the zoom button
			uint8_t startLED = ZOOM_LED_START_INDEX, numLeds = LED_SECTION_SIZE;
				
			for(int i = 0; i < numLeds; i++)
			{
				mPixelStrip.SetPixelColor(startLED++, ZOOM_COLOR);
			}
		}
		else
		{
			mMicStatus = eLED_MIC_IDLE;
			resumeAnimations(MIC_ANIMATION_IDX);
		}
		break;
	case eLED_MIC_OFF:
		mPixelStrip.SetPixelColor(MIC_STATUS_LIGHT_IDX, COLOR_BLACK);
		break;
	case eLED_MIC_IDLE:
		//just keep the status pixel enabled
		mPixelStrip.SetPixelColor(MIC_STATUS_LIGHT_IDX, MIC_COLOR);
		break;
	default:
		//take no action
		break;
	}
	mPixelStrip.Show();
}

bool LEDHandler::setupAnimationTimer(const uint8_t index, const uint32_t duration)
{
	bool isAnimationActive = false;
	
	if(mAnimationStates[index].current_duration <= duration)
	{
		//if the animation hasnt started yet, start its timer now
		if(mAnimationStates[index].start_time  == 0)
		{
			mAnimationStates[index].start_time = (uint32_t) 1000*(clock()/CLOCKS_PER_SEC);
		}

		//determine how long the animation has been going on
		mAnimationStates[index].current_duration =
				(uint32_t) 1000*(clock()/CLOCKS_PER_SEC) - mAnimationStates[index].start_time;
		
		isAnimationActive = true;
	}
	
	return isAnimationActive;
}

/***************************
 * Light State Helpers
 ***************************/
void LEDHandler::setFlashlightLED(eLEDPercentStep step)
{
	stopAnimations();

	if(setupAnimationTimer(FLASHLIGHT_ANIMATION_IDX, ONE_HALF_SECOND_DURATION))
	{				
		uint8_t startLED = 0, numLeds = 0;
	
		switch(step)
		{
		case eLEDStep15:
			//LED lights are backwards, start at the last index in its quadrant
			//which is index 3
			startLED = 3;
			numLeds = 1;
			break;
		case eLEDStep50:
			startLED = 1;
			numLeds = 3;
			break;
		case eLEDStep100:
			startLED = 0;
			numLeds = 4;
			break;
		}

		for(int i = 0; i < numLeds; i++)
		{
			mPixelStrip.SetPixelColor(startLED++, FLASHLIGHT_COLOR);
		}
		mPixelStrip.Show();
	}
	else
	{
		resumeAnimations(FLASHLIGHT_ANIMATION_IDX);
	}
}

void LEDHandler::setZoomLED(const uint8_t zoomState)
{
	stopAnimations();

	if(setupAnimationTimer(ZOOM_ANIMATION_IDX, ONE_HALF_SECOND_DURATION))
	{
		uint8_t startLED = 0, numLeds = 0;
		startLED = ZOOM_LED_START_INDEX;
		
		switch(zoomState)
		{
		case eZoomStepOne:
			numLeds = 1;
			break;
		case eZoomStepTwo:
			numLeds = 3;
			break;
		case eZoomStepThree:
			numLeds = 4;
			break;
		default:
			break;
		}
		
		for(int i = 0; i < numLeds; i++)
		{
			mPixelStrip.SetPixelColor(startLED++, ZOOM_COLOR);
		}
		mPixelStrip.Show();
	}
	else
	{
		resumeAnimations(ZOOM_ANIMATION_IDX);
	}
}

void LEDHandler::setLaserLED()
{
	stopAnimations();

	if(setupAnimationTimer(LASER_ANIMATION_IDX, FIVE_HUNDRED_MILLISECOND_DURATION))
	{
		uint8_t startLED = BARCODE_LED_START_INDEX, numLeds = LED_SECTION_SIZE;
		
		for(int i = 0; i < numLeds; i++)
		{
			mPixelStrip.SetPixelColor(startLED++, LASER_COLOR);
		}
		mPixelStrip.Show();		
	}
	else
	{
		//the animation should be done now
		resumeAnimations(LASER_ANIMATION_IDX);	
	}
}

void LEDHandler::resumeAnimations(uint8_t animIdx)
{
	//reset the timer and the state
	mAnimationStates[animIdx].clear();
	checkPreviousState();
	if(mBarcodeScanState != eScanActive)
	{
		mBarcodeScanState = eScanInactive;
	}
}

void LEDHandler::checkPreviousState()
{
	if(mIsVideoCaptureInProgress && mIsLiveStreamActive)
	{
		mLEDState = eVideoCaptureStart;
		mPixelAnimator.RestartAnimation(BLEND_ANIMATION_IDX);
	}
	else if(mIsVideoCaptureInProgress)
	{
		//if persistent states are active, then we need to return to THOSE states, not the previous state
		//because the previous state might be step 2
		mLEDState = eVideoCaptureStart;
		mPixelAnimator.RestartAnimation(VID_CAP_ANIMATION_IDX);
	}
	else if(mIsLiveStreamActive)
	{
		mLEDState = eLiveStreamingActive;
		mPixelAnimator.RestartAnimation(LIVE_STREAM_ANIMATION_IDX);
	}
	else
	{
		//set it to inactive if no persistent states are active
		mLEDState = eInactive;
	}
}

void LEDHandler::resetRing()
{
	mPixelAnimator.StopAll();
	clearLEDRing();
	mPixelStrip.Show();
}

//clear the LED ring with respect to the barcode being active or not
void LEDHandler::clearLEDRing()
{
	if(mBarcodeScanState == eScanNoOp)
	{
		//clearto with length arguments seems to be screwed up and produces
		//weird results with the first light lighting up incorrectly
//		mPixelStrip.ClearTo(COLOR_BLACK, 0, PIXEL_RING_COUNT);
		mPixelStrip.ClearTo(COLOR_BLACK);
	}
	else
	{
//		mPixelStrip.ClearTo(COLOR_BLACK, 0, (PIXEL_RING_COUNT - (LED_SECTION_SIZE +1)));
		mPixelStrip.ClearTo(COLOR_BLACK);
	}
}

void LEDHandler::stopAnimations()
{
	if(mPixelAnimator.IsAnimationActive(BLEND_ANIMATION_IDX))
	{
		clearLEDRing();
		mPixelAnimator.StopAnimation(BLEND_ANIMATION_IDX);
	}
	else if(mPixelAnimator.IsAnimationActive(VID_CAP_ANIMATION_IDX))
	{
		clearLEDRing();
		mPixelAnimator.StopAnimation(VID_CAP_ANIMATION_IDX);
	}
	else if(mPixelAnimator.IsAnimationActive(LIVE_STREAM_ANIMATION_IDX))
	{
		clearLEDRing();
		mPixelAnimator.StopAnimation(LIVE_STREAM_ANIMATION_IDX);
	}
}

//process the animation forever but With Respect To the current barcode scan state
void LEDHandler::processAnimationForeverWRTBarcode(uint8_t startIdx, uint8_t numberOfLEDs, RgbColor color, uint16_t duration, uint8_t animIdx)
{
//	if(mBarcodeScanState == eScanNoOp)
//	{
		processAnimationForever(startIdx, numberOfLEDs, color, duration, animIdx);
//	}
//	else
//	{
//		//if, and only if, the number of LEDs for the animation is the ENTIRE length, then we need to extinguish the last few for barcode
//		processAnimationForever(startIdx,
//				((numberOfLEDs == PIXEL_RING_COUNT) ? (numberOfLEDs - LED_SECTION_SIZE) : numberOfLEDs),
//				color,
//				duration,
//				animIdx);
//	}
}

void LEDHandler::processBlendedAnimationForeverWRTBarcode(uint8_t startIdx, uint8_t numberOfLEDs, uint16_t duration, uint8_t nColors, const RgbColor* colors)
{
//	if(mBarcodeScanState == eScanNoOp)
//	{
		processBlendedAnimationForever(startIdx, numberOfLEDs, duration, nColors, colors);
//	}
//	else
//	{
//		//if, and only if, the number of LEDs for the animation is the ENTIRE length, then we need to extinguish the last few for barcode
//		processBlendedAnimationForever(startIdx,
//				((numberOfLEDs == PIXEL_RING_COUNT) ? (numberOfLEDs - LED_SECTION_SIZE) : numberOfLEDs),
//				duration,
//				nColors,
//				colors);
//	}
}

/**************************
 * ANIMATION HANDLERS
 *************************/
void LEDHandler::fadeAnimUpdate(const AnimationParam& param)
{
	// this gets called for each animation on every time step
	// progress will start at 0.0 and end at 1.0
	// we use the blend function on the RgbColor to mix
	// color based on the progress given to us in the animation
	RgbColor updatedColor = RgbColor::LinearBlend(
		msLEDHandler->mAnimationStates[param.index].StartingColor,
		msLEDHandler->mAnimationStates[param.index].EndingColor, param.progress);

	uint8_t size = msLEDHandler->mAnimationStates[param.index].startLEDindex +
					msLEDHandler->mAnimationStates[param.index].nLEDs;

	// apply the color to the mPixelStrip
	for (uint8_t pixel = msLEDHandler->mAnimationStates[param.index].startLEDindex; pixel < size; pixel++)
	{
		msLEDHandler->mPixelStrip.SetPixelColor(pixel, updatedColor);
	}
}

void LEDHandler::fadeInFadeOutRinseRepeat(uint8_t animIdx)
{
	if (mAnimationStates[animIdx].fadeToColor)
	{
		mAnimationStates[animIdx].StartingColor =
				mPixelStrip.GetPixelColor(mAnimationStates[animIdx].startLEDindex);
		mAnimationStates[animIdx].EndingColor =
				mAnimationStates[animIdx].colors[mAnimationStates[animIdx].currentColorIdx];

		mPixelAnimator.StartAnimation(animIdx, mAnimationStates[animIdx].run_time, fadeAnimUpdate);
	}
	else
	{
		// fade to black
		mAnimationStates[animIdx].StartingColor =
				mAnimationStates[animIdx].colors[mAnimationStates[animIdx].currentColorIdx];

		mAnimationStates[animIdx].EndingColor = COLOR_BLACK;

		mPixelAnimator.StartAnimation(animIdx, mAnimationStates[animIdx].run_time, fadeAnimUpdate);
	}

	// toggle to the next effect state
	mAnimationStates[animIdx].fadeToColor = !mAnimationStates[animIdx].fadeToColor;
}

void LEDHandler::blendInBlendOutRinseRepeat()
{
	int currentColor = mAnimationStates[BLEND_ANIMATION_IDX].currentColorIdx;

	//if were on the last color, start over at the first color
	if(currentColor == mAnimationStates[BLEND_ANIMATION_IDX].nColors)
	{
		currentColor = 0;
	}

	//get the current color and make the next color it blends to the next color in the array
	mAnimationStates[BLEND_ANIMATION_IDX].StartingColor =
			mPixelStrip.GetPixelColor(mAnimationStates[BLEND_ANIMATION_IDX].startLEDindex);

	mAnimationStates[BLEND_ANIMATION_IDX].EndingColor =
			mAnimationStates[BLEND_ANIMATION_IDX].colors[currentColor];

	mPixelAnimator.StartAnimation(BLEND_ANIMATION_IDX,
			mAnimationStates[BLEND_ANIMATION_IDX].run_time, fadeAnimUpdate);

	mAnimationStates[BLEND_ANIMATION_IDX].currentColorIdx = ++currentColor;
}

void LEDHandler::processAnimationForNmillseconds(uint16_t nMilliseconds, uint8_t startLED, uint8_t nLEDs,
												RgbColor LEDcolor, uint16_t duration_ms, uint8_t animIdx)
{
	if (mPixelAnimator.IsAnimationActive(animIdx))
	{
		mPixelAnimator.UpdateAnimations();
		mPixelStrip.Show();
	}
	else
	{
		if(mAnimationStates[animIdx].current_duration <= nMilliseconds)
		{
			//if the animation hasnt started yet, start its timer now
			if(mAnimationStates[animIdx].start_time  == 0)
			{
				mAnimationStates[animIdx].start_time = (uint32_t) 1000*(clock()/CLOCKS_PER_SEC);
			}

			//determine how long the animation has been going on
			mAnimationStates[animIdx].current_duration = (uint32_t) 1000*(clock()/CLOCKS_PER_SEC) - mAnimationStates[animIdx].start_time ;
			mAnimationStates[animIdx].nLEDs = nLEDs;
			mAnimationStates[animIdx].startLEDindex = startLED;
			mAnimationStates[animIdx].run_time = duration_ms;
			mAnimationStates[animIdx].colors = &LEDcolor;
			mAnimationStates[animIdx].nColors = 1;
			mAnimationStates[animIdx].currentColorIdx = 0;

			fadeInFadeOutRinseRepeat(animIdx);
		}
		else
		{
			resumeAnimations(animIdx);
		}
	}
}

void LEDHandler::processAnimationForever(uint8_t startLED, uint8_t nLEDs, RgbColor LEDcolor, uint16_t duration_ms, uint8_t animIdx)
{
	if (mPixelAnimator.IsAnimationActive(animIdx))
	{
		mPixelAnimator.UpdateAnimations();
		mPixelStrip.Show();
	}
	else
	{
		mAnimationStates[animIdx].nLEDs = nLEDs;
		mAnimationStates[animIdx].startLEDindex = startLED;
		mAnimationStates[animIdx].run_time = duration_ms;
		mAnimationStates[animIdx].colors = &LEDcolor;
		mAnimationStates[animIdx].nColors = 1;
		mAnimationStates[animIdx].currentColorIdx = 0;

		fadeInFadeOutRinseRepeat(animIdx);
	}
}

void LEDHandler::processBlendedAnimationForever(uint8_t startLED, uint8_t nLEDs, uint16_t duration_ms, uint8_t nColors, const RgbColor* colors)
{
	if (mPixelAnimator.IsAnimationActive(BLEND_ANIMATION_IDX))
	{
		mPixelAnimator.UpdateAnimations();
		mPixelStrip.Show();
	}
	else
	{
		//setup the animation
		mAnimationStates[BLEND_ANIMATION_IDX].colors = colors;
		mAnimationStates[BLEND_ANIMATION_IDX].nColors = nColors;
		mAnimationStates[BLEND_ANIMATION_IDX].nLEDs = nLEDs;
		mAnimationStates[BLEND_ANIMATION_IDX].startLEDindex = startLED;
		mAnimationStates[BLEND_ANIMATION_IDX].run_time = duration_ms;

		blendInBlendOutRinseRepeat();
	}
}

void LEDHandler::drawTailPixels()
{
	// using Hsl as it makes it easy to pick from similar saturated colors
	float hue = 0; //185 / 360.0f;
	for (uint16_t index = 0; index < PIXEL_RING_COUNT && index <= LED_TAIL_LENGTH; index++)
	{
		float lightness = index * MAX_LIGHTNESS / LED_TAIL_LENGTH;
		RgbColor color = HslColor(hue, 0.0f, lightness);  // white is (0, 0%, xx%)

		mPixelStrip.SetPixelColor(index, NeoGamma<NeoGammaTableMethod>().Correct(color));
	}
}

void LEDHandler::loopAnimUpdate(const AnimationParam& param)
{
	// wait for this animation to complete,
	// we are using it as a timer of sorts
	if (param.state == AnimationState_Completed)
	{
		// done, time to restart this position tracking animation/timer
        msLEDHandler->mPixelAnimator.RestartAnimation(param.index);
        msLEDHandler->setLed(vec1,COLOR_BLUE);

	}
}

void LEDHandler::loopAnimUpdate1(const AnimationParam& param)
{
    // wait for this animation to complete,
    // we are using it as a timer of sorts
    if (param.state == AnimationState_Completed)
    {
        // done, time to restart this position tracking animation/timer
        msLEDHandler->mPixelAnimator.RestartAnimation(param.index);
        msLEDHandler->setLed(vec2,COLOR_PURPLE);
    }
}

void LEDHandler::loopAnimUpdate2(const AnimationParam& param)
{
    // wait for this animation to complete,
    // we are using it as a timer of sorts
    if (param.state == AnimationState_Completed)
    {
        // done, time to restart this position tracking animation/timer
        msLEDHandler->mPixelAnimator.RestartAnimation(param.index);
        msLEDHandler->setLed(vec3,COLOR_ORANGE);
    }
}

void LEDHandler::setLedForTime(const u_int16_t &time)
{
    msLEDHandler->setLed(vec1,COLOR_PURPLE);

    QTimer::singleShot(time, [&]()
    {
       qDebug() << "set the LED inside the timer";
       msLEDHandler->setLed(vec2,COLOR_PURPLE);
    });


}



