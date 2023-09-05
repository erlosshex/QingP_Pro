#ifndef COMMONTOOL_CONVERTIMAGE_H
#define COMMONTOOL_CONVERTIMAGE_H

#include "../../common_tool_global.h"

#include <QObject>

#include <opencv2/opencv.hpp>
#include <halconcpp/HalconCpp.h>

class COMMON_TOOL_EXPORT CommonTool_ConvertImage : public QObject
{
	Q_OBJECT
public:

	/// <summary>
	/// OpenCV图像转Halcon图像，内存拷贝的转换，速度快
	/// </summary>
	/// <param name="cvmImage">OpenCV图像</param>
	/// <returns>返回值，Halcon图像</returns>
	static HalconCpp::HObject Mat2HObject(cv::Mat cvmImage);

	/// <summary>
	/// OpenCV图像转Halcon图像，内存拷贝的转换，速度快
	/// </summary>
	/// <param name="cvmImage">OpenCV图像</param>
	/// <param name="hoImage">返回值，Halcon图像</param>
	static void Mat2HObject(const cv::Mat& cvmImage, HalconCpp::HObject& hoImage);

	/// <summary>
	/// Halcon图像转OpenCV图像，内存拷贝的转换，速度快
	/// </summary>
	/// <param name="hoImage">Halcon图像</param>
	/// <returns>返回值，OpenCV图像</returns>
	static cv::Mat HObject2Mat(const HalconCpp::HObject hoImage);

	/// <summary>
	/// Halcon图像转OpenCV图像，内存拷贝的转换，速度快
	/// </summary>
	/// <param name="hoImage">Halcon图像</param>
	/// <param name="cvmImage">返回值，OpenCV图像</param>
	static void HObject2Mat(const HalconCpp::HObject hoImage, cv::Mat& cvmImage);
};

#endif 