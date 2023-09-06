#ifndef ALGORITHM_OPERATOR_LINESEGMENTDETECTOR_H
#define ALGORITHM_OPERATOR_LINESEGMENTDETECTOR_H

#include "../../algorithm_operator_global.h"

#include <QObject>
#include <QRect>
#include <QPointF>

#include <opencv2/opencv.hpp>

#include "../algorithm_operator_common/algorithm_operator_common_enum.h"

class ALGORITHM_OPERATOR_EXPORT CLineSegmentDetector : public QObject
{
	Q_OBJECT
public:
	CLineSegmentDetector();
	~CLineSegmentDetector();

	std::vector<cv::Vec4f> lines;
	//Ptr<LineSegmentDetector> detector;

	int FindLines(cv::Mat DstMat, cv::Point offset);
	void DrawLines(cv::Mat SrcMat);

	static double DisOfPointToLine(const cv::Vec4f& linexy, const cv::Point2d& pointxy);//点到直线的距离
	static double DisOfPointToLine_Dir(cv::Vec4f linedir, cv::Point2d pointxy);//点到直线的距离


	static double DisOfPointToLine_Dir(cv::Vec4f linedir, cv::Point2d pointxy,cv::Point2d &corssPoint);//点到直线的距离

	static double DisOfPointToLine_DirNoAbs(cv::Vec4f linedir, cv::Point2d pointxy);//点到直线的距离

	static std::vector<double> DisOfLineToLine(cv::Vec4f lineOne, cv::Vec4f lineTwo);//两条直线间的距离
	static double DisOfLineSegment(cv::Vec4f linexy);//线段的长度
	static std::vector<cv::Vec4f> SelectMaxLines(std::vector<cv::Vec4f> allLines, int counter = 1);
	static cv::Point2d linesCrossPoint(cv::Vec4f lineOne, cv::Vec4f lineTwo);//两相交直线的交点坐标
	static cv::Point2d linesCrossPoint_Dir(cv::Vec4f lineOne, cv::Vec4f lineTwo);//两相交直线的交点坐标(方向向量)
	static bool linesIsParallel(cv::Vec4f lineOne, cv::Vec4f lineTwo, double precision);//两条直线是否平行
	static double lineGradientCos(cv::Vec4f linexy);
	static cv::Vec4f DirLineInRectConvert(cv::Vec4f dirLine, QRect rect);
	static cv::Vec4f DirLineInRectConvert(cv::Vec4f dirLine, cv::Rect rect);

	static cv::Point2d shortestVectorPointDistance2Line(std::vector<cv::Point2d> points, cv::Vec4f line);//计算点集中的点到直线的距离最小的点

	static bool isInPointSet(std::vector<cv::Point2d> &points, cv::Point2d &point, double dist_thres = 1.0);//判断点是否在点集中
	static bool isInPointSet(std::vector<QPointF> &points, QPointF &point, double dist_thres = 1.0);//判断点是否在点集中
	static bool isSamePoint(cv::Point2d &point1, cv::Point2d &point2, double dist_thres = 1.0);//判断两个点是否是同一个点
	static bool isSamePoint(QPointF &point1, QPointF &point2, double dist_thres = 1.0);//判断两个点是否是同一个点
};

#endif 