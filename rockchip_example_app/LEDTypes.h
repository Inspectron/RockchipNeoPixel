/*
 * LEDTypes.h
 *
 *  Created on: Mar 27, 2019
 *      Author: root
 */

#ifndef LEDTYPES_H_
#define LEDTYPES_H_

#include "NeoPixelBus.h"

namespace LEDTypes
{
	// what is stored for state is specific to the need, in this case, the colors.
	// basically what ever you need inside the animation update function
	struct InspectronAnimationState
	{
		uint32_t run_time;
		uint32_t start_time;
		uint32_t current_duration;
		uint8_t  startLEDindex;
		RgbColor StartingColor;
		RgbColor EndingColor;
		uint8_t  nLEDs;
		const RgbColor* colors;
		uint8_t nColors;
		uint8_t currentColorIdx;
		bool fadeToColor;

		InspectronAnimationState()
		{
			clear();
		}

		void clear()
		{
			run_time = 0;
			start_time = 0;
			current_duration = 0;
			startLEDindex = 0;
			StartingColor = {0,0,0};//black
			EndingColor = {0,0,0};//black
			nLEDs = 0;
			colors = NULL;
			nColors = 1;
			currentColorIdx = 0;//index, not color
			fadeToColor = true;
		}
	};

	enum enumLEDRingStates
	{
		eInactive,
		eZoomInactive,
		eZoomStepOne,
		eZoomStepTwo,
		eZoomStepThree,
		eLEDStepInactive,
		eLEDStepOne,
		eLEDStepTwo,
		eLEDStepThree,
		eTakePicture,
		eVideoCaptureStart,
		eVideoCaptureEnd,
		eLiveStreamingActive,
		eLiveStreamingInactive,
		eBarcodeScanStart,
		eBarcodeScanEnd,
		eUploadToCloudStart,
		eUploadingToCloud,
		eUploadToCloudEnd,
		eBatteryChargerIdle,
		eBatteryChargerCharging,
		eBatteryHalf,
		eBatteryQuarter,
		eBatteryChargerError,
		eLaserDotOn,
		eNoOp
	};

	enum eLEDPercentStep
	{
		eLEDStep15,
		eLEDStep50,
		eLEDStep100
	};

	enum eScanStates
	{
		eScanInactive,
		eScanActive,
		eScanScanned,
		eScanError,
		eScanNoOp
	};

}

#endif /* LEDTYPES_H_ */
