#ifndef COMMONTOOL_COMMONAFFINEOPERATOR_H
#define COMMONTOOL_COMMONAFFINEOPERATOR_H

#include "../../common_tool_global.h"

#include <QObject>
#include <vector>

#include <opencv2/opencv.hpp>
#include <halconcpp/HalconCpp.h>

#include "../tool_common/tool_common_struct.h"
#include "../tool_common/tool_common_enum.h"

class COMMON_TOOL_EXPORT CommonAffineOperator : public QObject
{
	Q_OBJECT
public:
	
	/// <summary>
	/// 通过旋转中心与指定角度，把已有的Rect生成1个方向的Rect，指定角度为0度，90度，180度，270度之一，逆时针旋转
	/// </summary>
	/// <param name="cvrRect">矩形</param>
	/// <param name="cvpRotateCenter">旋转中心点</param>
	/// <param name="dRotateAngle">旋转角度</param>
	/// <param name="cvr1OrienRect">旋转过后的矩形</param>
	static void rotateRect1Orien(const cv::Rect& cvrRect, const cv::Point2d& cvpRotateCenter, double dRotateAngle, cv::Rect& cvr1OrienRect);
	
	/// <summary>
	/// 通过旋转中心与指定角度，把已有的Rect生成1个方向的Rect，指定角度为0度，90度，180度，270度之一，逆时针旋转
	/// </summary>
	/// <param name="cvrRect">矩形</param>
	/// <param name="cvpRotateCenter">旋转中心点</param>
	/// <param name="dRotateAngle">旋转角度</param>
	/// <returns>旋转过后的矩形</returns>
	static cv::Rect rotateRect1Orien(const cv::Rect& cvrRect, const cv::Point2d& cvpRotateCenter, double dRotateAngle);

	/// <summary>
	/// 通过旋转中心，把已有的Rect生成4个方向的Rect，排列顺序为0度，90度，180度，270度，逆时针旋转
	/// </summary>
	/// <param name="cvrRect">参照的Rect</param>
	/// <param name="cvpRotateCenter">旋转中心</param>
	/// <param name="lstcvr4OrienRect">生成的4方向Rect</param>
	static void rotateRect4Orien(const cv::Rect& cvrRect, const cv::Point2d& cvpRotateCenter, QList<cv::Rect>& lstcvr4OrienRect);

	/// <summary>
	/// 通过旋转中心，把已有的Rect生成4个方向的Rect，排列顺序为0度，90度，180度，270度，逆时针旋转
	/// </summary>
	/// <param name="cvrRect">参照的Rect</param>
	/// <param name="cvpRotateCenter">旋转中心</param>
	/// <returns>生成的4方向Rect</returns>
	static QList<cv::Rect>& rotateRect4Orien(const cv::Rect& cvrRect, const cv::Point2d& cvpRotateCenter);

	/// <summary>
	/// 通过旋转中心与指定角度，把已有的Point生成1个方向的Point，指定角度为0度，90度，180度，270度之一，逆时针旋转
	/// </summary>
	/// <param name="cvpPoint">参照的Point</param>
	/// <param name="cvpRotateCenter">旋转中心</param>
	/// <param name="dRotateAngle">旋转角度</param>
	/// <param name="cvp1OrienPoint">生成的1方向Point</param>
	static void rotatePoint1Orien(const cv::Point2d& cvpPoint, const cv::Point2d& cvpRotateCenter, double dRotateAngle, cv::Point2d& cvp1OrienPoint);

	/// <summary>
	/// 通过旋转中心与指定角度，把已有的Point生成1个方向的Point，指定角度为0度，90度，180度，270度之一，逆时针旋转
	/// </summary>
	/// <param name="cvpPoint">参照的Point</param>
	/// <param name="cvpRotateCenter">旋转中心</param>
	/// <param name="dRotateAngle">旋转角度</param>
	/// <returns>生成的1方向Point</returns>
	static cv::Point2d rotatePoint1Orien(const cv::Point2d& cvpPoint, const cv::Point2d& cvpRotateCenter, double dRotateAngle);

	/// <summary>
	/// 获取真实的Pin1旋转后的矩形框角点坐标
	/// </summary>
	/// <param name="cvpTopLeftPoint">检测到的左上角坐标</param>
	/// <param name="cvpTopRightPoint">检测到的右上角坐标</param>
	/// <param name="cvpBottomLeftPoint">检测到的左下角坐标</param>
	/// <param name="cvpBottomRightPoint">检测到的右下角坐标</param>
	/// <param name="ePin1Orien">Pin1检测的方向</param>
	/// <param name="ePin1Direction">真实需要的角点位置</param>
	/// <returns>真实需要的点的坐标</returns>
	static cv::Point2d getPin1RotatedRectCorner(const cv::Point2d& cvpTopLeftPoint, const cv::Point2d& cvpTopRightPoint, const cv::Point2d& cvpBottomLeftPoint, const cv::Point2d& cvpBottomRightPoint, ToolCommonEnum::EnumPin1Orientation ePin1Orien, ToolCommonEnum::EnumPin1Direction ePin1Direction);

	/// <summary>
	/// 获取真实的Pin1旋转后的矩形框角点坐标
	/// </summary>
	/// <param name="stRectFourCornerPoints">原始角点</param>
	/// <param name="pstRectFourCornerPoints">旋转后的角点，输出</param>
	/// <param name="ePin1Orien">Pin1检测的方向</param>
	static void getPin1RotatedRectCorner(ToolCommonStruct::RectFourCornerPoints stRectFourCornerPoints, ToolCommonStruct::RectFourCornerPoints* pstRectFourCornerPoints, ToolCommonEnum::EnumPin1Orientation ePin1Orien);

	/// <summary>
	/// 旋转图像
	/// </summary>
	/// <param name="cvmImage">输入图像</param>
	/// <param name="cvpRotateCenter">旋转中心</param>
	/// <param name="dRotateAngle">旋转角度</param>
	/// <returns>返回旋转之后的图像</returns>
	static cv::Mat rotateImage(cv::Mat cvmImage, cv::Point2d cvpRotateCenter, double dRotateAngle);

	/// <summary>
	/// 旋转图像，Halcon，逆时针是正方向
	/// </summary>
	/// <param name="hoImage">输入图像</param>
	/// <param name="hvRotateCenterRow">旋转中心行坐标</param>
	/// <param name="hvRotateCenterCol">旋转中心列坐标</param>
	/// <param name="hvRotateAngle">旋转角度</param>
	/// <returns>返回旋转之后的图像</returns>
	static HalconCpp::HObject rotateImage(HalconCpp::HObject hoImage, HalconCpp::HTuple hvRotateCenterRow, HalconCpp::HTuple hvRotateCenterCol, HalconCpp::HTuple hvRotateAngle);

	/// <summary>
	/// 移动图像
	/// </summary>
	/// <param name="cvmImage">输入图像</param>
	/// <param name="cvpMove">移动的向量</param>
	/// <returns>返回移动之后的图像</returns>
	static cv::Mat moveImage(cv::Mat cvmImage, cv::Point2d cvpMove);

	/// <summary>
	/// 移动图像，Halcon
	/// </summary>
	/// <param name="hoImage">输入图像</param>
	/// <param name="hvMoveRow">移动的行距离</param>
	/// <param name="hvMoveCol">移动的列距离</param>
	/// <returns>返回移动之后的图像</returns>
	static HalconCpp::HObject moveImage(HalconCpp::HObject hoImage, HalconCpp::HTuple hvMoveRow, HalconCpp::HTuple hvMoveCol);

	/// <summary>
	/// 旋转点，顺时针是正方向
	/// </summary>
	/// <param name="cvpPoint">输入点</param>
	/// <param name="cvpRotateCenter">旋转中心</param>
	/// <param name="dRotateAngle">旋转角度</param>
	/// <returns>返回旋转之后的点</returns>
	static cv::Point2d rotatePoint(cv::Point2d cvpPoint, cv::Point2d cvpRotateCenter, double dRotateAngle);

	/// <summary>
	/// 旋转点，逆时针是正方向
	/// </summary>
	/// <param name="cvpPoint">输入点</param>
	/// <param name="cvpRotateCenter">旋转中心</param>
	/// <param name="dRotateAngle">旋转角度</param>
	/// <returns>返回旋转之后的点</returns>
	static cv::Point2d rotatePoint1(cv::Point2d cvpPoint, cv::Point2d cvpRotateCenter, double dRotateAngle);

	/// <summary>
	/// 旋转点，Halcon，逆时针是正方向
	/// </summary>
	/// <param name="hvPointRow">待旋转点的行坐标</param>
	/// <param name="hvPointCol">待旋转点的列坐标</param>
	/// <param name="hvRotateCenterRow">旋转中心的行坐标</param>
	/// <param name="hvRotateCenterCol">旋转中心的列坐标</param>
	/// <param name="hvRotateAngle">旋转角度</param>
	/// <param name="hvRotatePointRow">旋转之后的行坐标</param>
	/// <param name="hvRotatePointCol">旋转之后的列坐标</param>
	static void rotatePoint(HalconCpp::HTuple hvPointRow, HalconCpp::HTuple hvPointCol, HalconCpp::HTuple hvRotateCenterRow, HalconCpp::HTuple hvRotateCenterCol, HalconCpp::HTuple hvRotateAngle, HalconCpp::HTuple& hvRotatePointRow, HalconCpp::HTuple& hvRotatePointCol);

	/// <summary>
	/// 移动点
	/// </summary>
	/// <param name="cvpPoint">欲移动的点</param>
	/// <param name="cvpMove">移动的向量</param>
	/// <returns>返回移动之后的点</returns>
	static cv::Point2d movePoint(cv::Point2d cvpPoint, cv::Point2d cvpMove);

	/// <summary>
	/// 移动点，Halcon
	/// </summary>
	/// <param name="hvPointRow">欲移动的点的行坐标</param>
	/// <param name="hvPointCol">欲移动的点的列坐标</param>
	/// <param name="hvMoveRow">移动的行向量</param>
	/// <param name="hvMoveCol">移动的列向量</param>
	/// <param name="hvMovePointRow">返回值，移动之后的点的行坐标</param>
	/// <param name="hvMovePointCol">返回值，移动之后的点的列坐标</param>
	static void movePoint(HalconCpp::HTuple hvPointRow, HalconCpp::HTuple hvPointCol, HalconCpp::HTuple hvMoveRow, HalconCpp::HTuple hvMoveCol, HalconCpp::HTuple& hvMovePointRow, HalconCpp::HTuple& hvMovePointCol);

	/// <summary>
	/// 旋转矩形，逆时针是正方向
	/// </summary>
	/// <param name="cvrRect">输入的正矩形</param>
	/// <param name="cvpRotateCenter">旋转中心</param>
	/// <param name="dRotateAngle">旋转角度</param>
	/// <returns>返回旋转之后的旋转矩形</returns>
	static cv::RotatedRect rotateRect(cv::Rect cvrRect, cv::Point2d cvpRotateCenter, double dRotateAngle);

	/// <summary>
	/// 用Halcon的旋转矩阵旋转OpenCV的Rect，得到旋转之后的正矩形
	/// </summary>
	/// <param name="cvrRect">待旋转的OpenCV的Rect</param>
	/// <param name="hvAffineMat">Halcon旋转矩阵</param>
	/// <returns>返回旋转之后的正矩形</returns>
	static cv::Rect getBoundingRectFromRotateRect(cv::Rect cvrRect, HalconCpp::HTuple hvAffineMat);
};


#endif 

