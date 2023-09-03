#ifndef CAMERA_COMMON_STRUCT
#define CAMERA_COMMON_STRUCT

#include "camera_common_enum.h"

#include <opencv2/opencv.hpp>
#include <halconcpp/HalconCpp.h>

namespace CameraCommonStruct
{
	struct CameraCaptureFrame
	{
		bool bIsAvailable = false;
		CameraCommonEnum::CameraFrameType eCameraFrameType = CameraCommonEnum::CameraFrameType::DefaultMode;
		int iImageWidth = 0;
		int iImageHeight = 0;
		uint uiChannelType = CV_8UC1; //复用OpenCV里面的类型
		cv::Mat cvmImage;
		HalconCpp::HObject hoImage;
	};
}


#endif
