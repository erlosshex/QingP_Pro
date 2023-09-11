#ifndef TOOL_COMMON_STRUCT
#define TOOL_COMMON_STRUCT

#include "tool_common_enum.h"

#include <opencv2/opencv.hpp>

namespace ToolCommonStruct
{
	//正矩形的四点
	struct RectFourCornerPoints
	{
		cv::Point2d cvpTopLeftPoint;
		cv::Point2d cvpTopRightPoint;
		cv::Point2d cvpBottomLeftPoint;
		cv::Point2d cvpBottomRightPoint;
	};
}


#endif
