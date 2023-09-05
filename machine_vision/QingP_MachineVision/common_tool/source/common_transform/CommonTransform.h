#ifndef COMMONTOOL_COMMONTRANSFORM_H
#define COMMONTOOL_COMMONTRANSFORM_H

#include "../../common_tool_global.h"

#include <QObject>

#include <opencv2/opencv.hpp>
#include <halconcpp/HalconCpp.h>

#include "../common_judge/CommonJudge.h"

class COMMON_TOOL_EXPORT CommonTool_CommonTransform : public QObject
{
	Q_OBJECT
public:

	/// <summary>
	/// 将halcon中的轮廓区域转成OpenCV中的旋转矩形区域
	/// </summary>
	/// <param name="hoRegionXld">halcon中的轮廓区域</param>
	/// <param name="cvrrRect">OpenCV中的旋转矩形区域，返回值</param>
	static void hoRegionXld2CvRotatedRect(HalconCpp::HObject hoRegionXld, cv::RotatedRect& cvrrRect, bool bSwapLen1Len2 = false);

	/// <summary>
	/// Halcon正矩形区域生成OpenCV的Rect
	/// </summary>
	/// <param name="">Halcon正矩形区域</param>
	/// <returns>对应的OpenCV的Rect</returns>
	static cv::Rect getOpenCVRectFromRegion(HalconCpp::HObject hoRegion);

	/// <summary>
	/// 将halcon中的区域转成OpenCV中的旋转矩形区域
	/// </summary>
	/// <param name="hoRegion">halcon中的区域</param>
	/// <param name="qlistRotatedRect">获得的旋转矩形，返回值</param>
	static void getRotatedRectOfRegion(HalconCpp::HObject hoRegion, QList<cv::RotatedRect>* plistRotatedRect);

	/// <summary>
	/// 将halcon中分离后的区域转成OpenCV中的旋转矩形区域
	/// </summary>
	/// <param name="hoConnectioinRegion">halcon中分离后的区域</param>
	/// <param name="qlistRotatedRect">获得的旋转矩形，返回值</param>
	static void getRotatedRectOfConnectionRegion(HalconCpp::HObject hoConnectioinRegion, QList<cv::RotatedRect>* plistRotatedRect);

	/// <summary>
	/// 将halcon中的单个Region转换成OpenCV中的旋转矩形区域
	/// </summary>
	/// <param name="hoRegion">halcon中的单个区域</param>
	/// <param name="bIsSwapLen12">交换Len1和Len2</param>
	/// <param name="bIsFlipAngle>反向角度</param>
	/// <returns>获得的旋转矩形，返回值</returns>
	static cv::RotatedRect getRotatedRectOfOneRegion(HalconCpp::HObject hoRegion, bool bIsSwapLen12 = false, bool bIsUseDeg = false, bool bIsFlipAngle = false);

	/// <summary>
	/// 获取区域轮廓的点集，Halcon
	/// </summary>
	/// <param name="hoRegion">区域，Halcon</param>
	/// <returns>获取的轮廓点集，返回值</returns>
	static std::vector<cv::Point> getRegionContourPointsInt(HalconCpp::HObject hoRegion);
};

#endif 