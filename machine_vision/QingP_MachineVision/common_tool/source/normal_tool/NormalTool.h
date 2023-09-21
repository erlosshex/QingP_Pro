#ifndef COMMONTOOL_NORMALTOOL_H
#define COMMONTOOL_NORMALTOOL_H

#include "../../common_tool_global.h"

#include <QObject>

#include <opencv2/opencv.hpp>
#include <halconcpp/HalconCpp.h>

#include "../tool_common/tool_common_define.h"

class COMMON_TOOL_EXPORT CommonTool_NormalTool : public QObject
{
	Q_OBJECT
public:

	/// <summary>
	/// 将指针变量所指向的内存释放
	/// </summary>
	/// <typeparam name="T">泛型类别</typeparam>
	/// <param name="ptrData">指针变量</param>
	template<typename T>
	static void deleteDataPtr(T*& pData);

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

	/// <summary>
	/// 判断ROI是否在图像中有效
	/// </summary>
	/// <param name="bIsValid">指示此ROI是否在图像中有效，返回值</param>
	/// <param name="iImageWidth">图像的宽度</param>
	/// <param name="iImageHeight">图像的高度</param>
	/// <param name="rRoi">待判断的ROI</param>
	/// <returns>错误代码</returns>
	static int isRoiValid(bool& bIsValid, int iImageWidth, int iImageHeight, cv::Rect rRoi);

	/// <summary>
	/// 判断ROI是否在图像中有效
	/// </summary>
	/// <param name="bIsValid">返回值，指示此ROI是否在图像中有效</param>
	/// <param name="mImage">图像</param>
	/// <param name="rRoi">待判断的ROI</param>
	/// <returns>错误代码</returns>
	static int isRoiValid(bool& bIsValid, cv::Mat mImage, cv::Rect rRoi);

	/// <summary>
	/// 判断传入的ROI区域是否在图片内，并做出修正
	/// </summary>
	/// <param name="_crRoi">传入的ROI区域</param>
	/// <param name="_iImageWidth">传入的图片宽度</param>
	/// <param name="_iImageHeight">传入的图片高度</param>
	/// <param name="_crEnsureRoi">传出的ROI区域</param>
	static void ensureRectROIOfImage(const cv::Rect _crRoi, const int _iImageWidth, const int _iImageHeight, cv::Rect& _crEnsureRoi);

	/// <summary>
	/// 判断传入的ROI区域是否在图片内，并做出修正，Halcon
	/// </summary>
	/// <param name="hoImage">传入的Image</param>
	/// <param name="hoRegion">传入的ROI区域</param>
	/// <returns>返回修正之后的区域</returns>
	static HalconCpp::HObject ensureRegionOfImage(HalconCpp::HObject hoImage, HalconCpp::HObject hoRegion);

	/// <summary>
	/// 判断halcon图像是否为空
	/// </summary>
	/// <param name="hoImg">halcon图像</param>
	/// <param name="isEmpty">true表示空图，false表示不是空图，返回值</param>
	static void isHoImageEmpty(HalconCpp::HObject hoImg, bool& isEmpty);

	/// <summary>
	/// 判断Halcon的图标变量是否已经初始化
	/// </summary>
	/// <param name="hoObj">Halcon的图标变量</param>
	/// <returns>返回是否已经初始化，true表示初始化，false表示未初始化</returns>
	static bool isHObjectInit(HalconCpp::HObject hoObj);

	/// <summary>
	/// 判断Halcon的图标变量是否为空
	/// </summary>
	/// <param name="hoObj">Halcon的图标变量</param>
	/// <returns>返回是否为空，true表示为空，false表示不为空</returns>
	static bool isHObjectEmpty(HalconCpp::HObject hoObj);

	/// <summary>
	/// 判断Halcon的图标变量是否有效
	/// </summary>
	/// <param name="hoObj">Halcon的图标变量</param>
	/// <returns>返回是否有效，true表示有效，false表示无效</returns>
	static bool isHObjectValid(HalconCpp::HObject hoObj);

	/// <summary>
	/// 返回整张图的ROI区域
	/// </summary>
	/// <param name="cvmImage">OpenCV的图片</param>
	/// <returns>返回的整张图片ROI区域</returns>
	static HalconCpp::HObject getEntireImageRoi(cv::Mat cvmImage);

	/// <summary>
	/// 返回整张图的ROI区域
	/// </summary>
	/// <param name="hoImage">Halcon的图片</param>
	/// <returns>返回的整张图片ROI区域</returns>
	static HalconCpp::HObject getEntireImageRoi(HalconCpp::HObject hoImage);

	/// <summary>
	/// 返回整张图的OpenCV的ROI
	/// </summary>
	/// <param name="hoImage">Halcon的图片</param>
	/// <param name="cvrEntireImageRoi">返回的整张图片ROI</param>
	static void getEntireImageRoi(HalconCpp::HObject hoImage, cv::Rect& cvrEntireImageRoi);

	/// <summary>
	/// 返回整张图的OpenCV的ROI
	/// </summary>
	/// <param name="cvmImage">OpenCV的图片</param>
	/// <returns>返回的整张图片ROI</returns>
	static cv::Rect getEntireImageCvRoi(cv::Mat cvmImage);

	/// <summary>
	/// 返回整张图的OpenCV的ROI
	/// </summary>
	/// <param name="hoImage">Halcon的图片</param>
	/// <returns>返回的整张图片ROI</returns>
	static cv::Rect getEntireImageCvRoi(HalconCpp::HObject hoImage);

	/// <summary>
	/// RGB通道转分离的HSV通道
	/// </summary>
	/// <param name="cvmRgbImg">RGB图像</param>
	/// <param name="veccvmHsvImg">分离的HSV通道，返回值</param>
	static void RGB2HSVFull(cv::Mat cvmRgbImg, std::vector<cv::Mat>& veccvmHsvImg);

	/// <summary>
	/// RGB通道转分离的HSV通道, QVector
	/// </summary>
	/// <param name="cvmRgbImg">RGB图像</param>
	/// <param name="veccvmHsvImg">分离的HSV通道，QVector，返回值</param>
	static void RGB2HSVFull(cv::Mat cvmRgbImg, QVector<cv::Mat>& veccvmHsvImg);

	/// <summary>
	/// BGR通道转分离的HSV通道
	/// </summary>
	/// <param name="cvmBgrImg">BGR图像</param>
	/// <param name="veccvmHsvImg">分离的HSV通道，返回值</param>
	static void BGR2HSVFull(cv::Mat cvmBgrImg, std::vector<cv::Mat>& veccvmHsvImg);

	/// <summary>
	/// BGR通道转分离的HSV通道, QVector
	/// </summary>
	/// <param name="cvmRgbImg">BGR图像</param>
	/// <param name="veccvmHsvImg">分离的HSV通道，QVector，返回值</param>
	static void BGR2HSVFull(cv::Mat cvmBgrImg, QVector<cv::Mat>& veccvmHsvImg);

	/// <summary>
	/// 判断匹配的位置是否在查找范围内
	/// </summary>
	/// <param name="cvpMatchLoc">匹配的位置坐标</param>
	/// <param name="lstcvrSearchRoi">搜索的范围列表</param>
	/// <returns>返回值，true表示在搜索范围内，false表示不在搜索范围内</returns>
	static bool isMatchPointInSearchRegion(cv::Point2d cvpMatchLoc, QList<cv::Rect> lstcvrSearchRoi);

	/// <summary>
	/// 计算曲线的曲率
	/// </summary>
	/// <param name="veccvpiContourPoints">曲线的点集</param>
	/// <param name="iStep">步长</param>
	/// <returns>返回的曲率的集合</returns>
	static std::vector<double> getCurvature(const std::vector<cv::Point>& veccvpiContourPoints, int iStep);

};

template<typename T>
inline void deleteDataPtr(T*& pData)
{
	try
	{
		if (pData == nullptr) return;
		delete pData;
		pData = nullptr;
	}
	catch (std::exception& ex)
	{
		// todo
	}
}

#endif 