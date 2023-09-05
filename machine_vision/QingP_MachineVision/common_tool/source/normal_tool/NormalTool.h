#ifndef COMMONTOOL_NORMALTOOL_H
#define COMMONTOOL_NORMALTOOL_H

#include "../../common_tool_global.h"

#include <QObject>

#include <opencv2/opencv.hpp>
#include <halconcpp/HalconCpp.h>

class COMMON_TOOL_EXPORT CommonTool_NormalTool : public QObject
{
	Q_OBJECT
public:

	/// <summary>
	/// 由OpenCV的Rect生成Halcon正矩形区域
	/// </summary>
	/// <param name="cvrRoi">OpenCV的Rect</param>
	/// <returns>生成的Halcon正矩形区域</returns>
	static HalconCpp::HObject genRectRegion(cv::Rect cvrRoi);

	/// <summary>
	/// 由OpenCV的Rect列表生成Halcon正矩形区域集合
	/// </summary>
	/// <param name="lstcvrRoi">OpenCV的Rect列表</param>
	/// <returns>生成的Halcon正矩形区域集合</returns>
	static HalconCpp::HObject genRectRegions(QList<cv::Rect> lstcvrRoi);

	/// <summary>
	/// 由OpenCV的Rect和屏蔽Rect列表生成Halcon区域集合
	/// </summary>
	/// <param name="cvrRoi">OpenCV的Rect</param>
	/// <param name="lstcvrShieldRoi">OpenCV的屏蔽Rect列表</param>
	/// <returns>生成的Halcon区域集合</returns>
	static HalconCpp::HObject genRectShieldRegion(cv::Rect cvrRoi, QList<cv::Rect> lstcvrShieldRoi);

	/// <summary>
	/// 由OpenCV的Rect列表和屏蔽Rect列表生成Halcon区域集合
	/// </summary>
	/// <param name="lstcvrRoi">OpenCV的Rect的列表</param>
	/// <param name="lstcvrShieldRoi">OpenCV的屏蔽Rect列表</param>
	/// <returns>生成的Halcon区域集合</returns>
	static HalconCpp::HObject genRectShieldRegion(QList<cv::Rect> lstcvrRoi, QList<cv::Rect> lstcvrShieldRoi);

	/// <summary>
	/// 由OpenCV的Rect和屏蔽Rect列表对列表生成Halcon区域集合
	/// </summary>
	/// <param name="lstpairRectShield">OpenCV的Rect和屏蔽Rect列表对列表</param>
	/// <returns>生成的Halcon区域集合</returns>
	static HalconCpp::HObject genRectShieldRegion(QList<QPair<cv::Rect, QList<cv::Rect>>> lstpairRectShield);

	/// <summary>
	/// Halcon正矩形区域生成OpenCV的Rect
	/// </summary>
	/// <param name="">Halcon正矩形区域</param>
	/// <returns>对应的OpenCV的Rect</returns>
	static cv::Rect getOpenCVRectFromRegion(HalconCpp::HObject);

	/// <summary>
	/// 计算两个矩形框之间的覆盖率
	/// </summary>
	/// <param name="box1">输入的矩形框1</param>
	/// <param name="box2">输入的矩形框2</param>
	/// <returns>覆盖率</returns>
	static double overlapOfTwoRect(const cv::Rect& box1, const cv::Rect& box2);

	/// <summary>
	/// 计算两组矩形框列表之间的覆盖率
	/// </summary>
	/// <param name="lstcvrRect1">输入的矩形框列表1</param>
	/// <param name="lstcvrRect2">输入的矩形框列表2</param>
	/// <returns>覆盖率</returns>
	static double overlapOfTwoRectList(const QList<cv::Rect>& lstcvrRect1, const QList<cv::Rect>& lstcvrRect2);
};

#endif 