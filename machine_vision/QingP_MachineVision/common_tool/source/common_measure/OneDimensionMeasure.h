#ifndef COMMONTOOL_COMMONMEASURE_ONEDIMENSION_MEASURE_H
#define COMMONTOOL_COMMONMEASURE_ONEDIMENSION_MEASURE_H

#include "../../common_tool_global.h"
#include "../../source/tool_common/tool_common_define.h"
#include "../../source/tool_common/tool_common_enum.h"

#include <QObject>

#include <opencv2/opencv.hpp>
#include <halconcpp/HalconCpp.h>

class COMMON_TOOL_EXPORT OneDimensionMeasure : public QObject
{
	Q_OBJECT
public:
	OneDimensionMeasure();

	static void ensureValueInRange(double dValue, double dMinValue, double dMaxValue, double& dRsltValue, bool bIstruc = false);

	template<typename T>
	static void cvDistTwoPoints(cv::Point_<T> cvPoint1, cv::Point_<T> cvPoint2, double& dDist)
	{
		dDist = std::sqrt(std::pow(cvPoint2.x - cvPoint1.x, 2) + pow(cvPoint2.y - cvPoint1.y, 2));
	}

	template<typename T, typename T1>
	static void cvDistTwoPoints(T p1x, T p1y, T1 p2x, T1 p2y, double& dDist)
	{
		dDist = std::sqrt(std::pow(p1x - p2x, 2) + std::pow(p1y - p2y, 2));
	}

	template<typename T>
	static void cvDistTwoPointsOnAxis(cv::Point_<T>& cvPoint1, cv::Point_<T>& cvPoint2, bool bIsXAxis, double& dDist)
	{
		dDist = bIsXAxis ? (std::abs(cvPoint1.x - cvPoint2.x)) : (std::abs(cvPoint1.y - cvPoint2.y));
	}

	static void cvFilterContoursWithArea(const std::vector<std::vector<cv::Point> >& vecContours, double dAreaThres,
		std::vector<std::vector<cv::Point> > &vecResultContours, bool bIsGreater = true);

	static void cvFilterContoursWithArea(const std::vector<std::vector<cv::Point> >& vecContours, double dMinAreaThres, 
		double dMaxAreaThres, std::vector<std::vector<cv::Point> >& vecResultContours);

	static uint cvGetTwoDiagonalLineFromContours(std::vector<std::vector<cv::Point> >& vecContours, cv::Vec4f& vec4fLine1, cv::Vec4f& vec4fLine2);

	template<typename T>
	static uint cvConvertLinePoint2Vec(cv::Point_<T> cvPoint1, cv::Point_<T> cvPoint2, cv::Vec4f& vec4fLine)
	{
		if (std::abs(cvPoint1.x - cvPoint2.x) <= 1e-3 && std::abs(cvPoint1.y - cvPoint2.y) <= 1e-3)
		{
			return TOOL_COMMON_PARAERROR;
		}

		double dPoint1x = cvPoint1.x;
		double dPoint1y = cvPoint1.y;
		double dPoint2x = cvPoint2.x;
		double dPoint2y = cvPoint2.y;

		double vx = dPoint2x - dPoint1x;
		double vy = dPoint2y - dPoint1y;

		vec4fLine[2] = dPoint1x;
		vec4fLine[3] = dPoint1y;

		vec4fLine[0] = vx / sqrt(vx * vx + vy * vy);
		vec4fLine[1] = vy / sqrt(vx * vx + vy * vy);

		return TOOL_COMMON_OK;
	}

	static void cvMoveLine(cv::Vec4f &vec4fLine, double dDistX, double dDistY);

	template<typename T>
	static void cvGetCrossPoint(const cv::Vec4f& vec4fLine1, const cv::Vec4f& vec4fLine2, cv::Point_<T>& cvPoint)
	{
		double vx1 = vec4fLine1[0];
		double vy1 = vec4fLine1[1];
		double x1 = vec4fLine1[2];
		double y1 = vec4fLine1[3];

		double vx2 = vec4fLine2[0];
		double vy2 = vec4fLine2[1];
		double x2 = vec4fLine2[2];
		double y2 = vec4fLine2[3];

		double operation = vx1 * vy2 - vx2 * vy1;

		if (operation != 0.0) {
			double x = (x2 * vx1 * vy2 - y2 * vx1 * vx2 - x1 * vx2 * vy1 + y1 * vx1 * vx2) / operation;
			double y = (x2 * vy1 * vy2 - y2 * vx2 * vy1 - x1 * vy1 * vy2 + y1 * vx1 * vy2) / operation;

			cvPoint.x = x;
			cvPoint.y = y;
		}
		else {
			cvPoint.x = -1;
			cvPoint.y = -1;
		}
	}

	template<typename T>
	static void cvGetCrossPointFrom3Lines(const cv::Vec4f& vec4fCrossLine, const cv::Vec4f& vec4fLine1, const cv::Vec4f& vec4fLine2, cv::Point_<T>& cvCrossPoint1, cv::Point_<T>& cvCrossPoint2, cv::Point_<T>& cvMidPoint)
	{
		getCrossPoint(vec4fCrossLine, vec4fLine1, cvCrossPoint1);
		getCrossPoint(vec4fCrossLine, vec4fLine2, cvCrossPoint2);
		cvMidPoint.x = (cvCrossPoint1.x + cvCrossPoint2.x) / 2.0;
		cvMidPoint.y = (cvCrossPoint1.y + cvCrossPoint2.y) / 2.0;
		return;
	}

	static uint cvGetLineSegmentPoint(cv::Point2d cvPoint1, cv::Point2d cvPoint2, int step, std::vector<cv::Point2f>& veccvPoints);

	template<typename T, typename T1>
	static void cvGetFourPointsFromRect(const cv::Rect_<T>& rect, std::vector<cv::Point_<T1> >& veccvPoints)
	{
		cv::Point_<T1> cvpBottomLeft(rect.x, rect.y);
		cv::Point_<T1> cvpTopLeft(rect.x, rect.y);
		cv::Point_<T1> cvpTopRight(rect.x, rect.y);
		cv::Point_<T1> cvpBottomRight(rect.x, rect.y);
		veccvPoints.clear();
		veccvPoints.push_back(cvpBottomLeft);
		veccvPoints.push_back(cvpTopLeft);
		veccvPoints.push_back(cvpTopRight);
		veccvPoints.push_back(cvpBottomRight);
		return;
	}

	template<typename T>
	static void cvCombineAllContours(const std::vector<std::vector<cv::Point_<T>>>& vecContours, std::vector<cv::Point_<T>>& vecCombinePoints)
	{
		vecCombinePoints.clear();
		for (int i = 0; i < vecContours.size(); ++i) {
			auto con = vecContours[i];
			for (int j = 0; j < con.size(); ++j) {
				vecCombinePoints.push_back(con[j]);
			}
		}
		return;
	}
	
	template<typename T>
	static void cvJudgePointPositionWithLine(cv::Point_<T> cvPoint, cv::Vec4f vec4fLine, int& iPointPos, int iFlag = 0, double dCompensate = 0.0)
	{
		if (iFlag == 0) {
			double vx = vec4fLine[0];
			double vy = vec4fLine[1];
			double x0 = vec4fLine[2];
			double y0 = vec4fLine[3];

			double y1 = 0;

			if (std::abs(vx) < 0.0001) {
				y1 = y0 + dCompensate;
			}
			else {
				double k = vy / vx;
				double b = y0 - k * x0;

				y1 = k * cvPoint.x + b + dCompensate;
			}

			if (cvPoint.y > y1) {
				iPointPos = 1;
				return;
			}
			else if (cvPoint.y < y1) {
				iPointPos= -1;
				return;
			}
			else {
				iPointPos = 0;
				return;
			}
		}
		else {
			double vx = vec4fLine[0];
			double vy = vec4fLine[1];
			double x0 = vec4fLine[2];
			double y0 = vec4fLine[3];

			double x1 = 0;

			if (std::abs(vy) < 0.0001) {
				x1 = x0 + dCompensate;
			}
			else {
				double k = vx / vy;
				double b = x0 - k * y0;

				x1 = k * cvPoint.y + b + dCompensate;
			}

			if (cvPoint.x > x1) {
				iPointPos = 1;
				return;
			}
			else if (cvPoint.x < x1) {
				iPointPos = -1;
				return;
			}
			else {
				iPointPos = 0;
				return;
			}
		}
	}

	// get vertical line of a certain line
	template<typename T>
	static uint cvGetVerticalLineOfLine(cv::Vec4f vec4fLine, cv::Point_<T> cvPoint, cv::Vec4f& vec4fVertLine)
	{
		if (cvPoint.x == -1 && cvPoint.y == -1)
		{
			return TOOL_COMMON_PARAERROR;
		}

		vec4fVertLine[0] = vec4fLine[1];
		vec4fVertLine[1] = -vec4fLine[0];

		if (vec4fVertLine[0] < 0) {
			vec4fVertLine[0] = -vec4fVertLine[0];
			vec4fVertLine[1] = -vec4fVertLine[1];
		}

		vec4fVertLine[2] = cvPoint.x;
		vec4fVertLine[3] = cvPoint.y;
	
		return TOOL_COMMON_OK;
	}

	// get the cross point of line and vertical line
	template<typename T>
	static void cvGetVerticalCrossPointOfLinePoint(cv::Vec4f vec4fLine, cv::Point_<T> cvPoint, cv::Point_<T>& cvCrossPoint)
	{
		//cv::Vec4f vec4fVerticalline = cvGetVerticalLineOfLine(vec4fLine, cvPoint);
		cv::Vec4f vec4fVerticalline = cv::Vec4f(-1, -1, -1, -1); 
		cvGetVerticalLineOfLine(vec4fLine, cvPoint, vec4fVerticalline);
		cvGetCrossPoint(vec4fLine, vec4fVerticalline, cvCrossPoint);
	}

	//get two cross distance on the std line;
	template<typename T>
	static void cvGetDistTwoPointsOnStdLine(cv::Vec4f vec4fStdLine, cv::Point_<T> cvPoint0, cv::Point_<T> cvPoint1, double& dDist)
	{
		cv::Point2d cvpVerCrossPoint0 = cvPoint0;
		cv::Point2d cvpVerCrossPoint1 = cvPoint1;

		cvGetVerticalCrossPointOfLinePoint(vec4fStdLine, cvPoint0, cvpVerCrossPoint0);
		cvGetVerticalCrossPointOfLinePoint(vec4fStdLine, cvPoint1, cvpVerCrossPoint1);

		cvDistTwoPoints(cvpVerCrossPoint0, cvpVerCrossPoint1, dDist);
	}

	// calculate the theta of two lines
	static void cvGetCosThetaOfTwoLines(cv::Vec4f vec4fLine1, cv::Vec4f vec4fLine2, double& dTheta, bool bIsAngle = true);
	static void cvGetRotateThetaOfLine2CorrespondLine(cv::Vec4f vec4fLine, cv::Vec4f vec4fCorrespondLine, double& dTheta, bool bIsAngle = true);
	static void cvGetRotateThetaOfCorrespondLine2Line(cv::Vec4f vec4fLine, cv::Vec4f vec4fCorrespondLine, double& dTheta, bool bIsAngle = true);

	static void cvGetRotateThetaOfLine2CorrespondLineByLineDir(double vx, double vy, double cor_vx, double cor_vy, double& dTheta, bool isAngle = true, bool vxy_need_normal = false, bool cor_vxy_need_normal = false);
	static void cvGetRotateThetaOfCorrespondLine2LineByLineDir(double vx, double vy, double cor_vx, double cor_vy, double& dTheta, bool isAngle = true, bool vxy_need_normal = false, bool cor_vxy_need_normal = false);
 	static uint cvCrossAngleOfLines(cv::Vec4f vec4fLine1, cv::Vec4f vec4fLine2, double& dTheta);

	template<typename T>
	static uint cvLinesCrossPointDir(cv::Vec4f vec4fLine1, cv::Vec4f vec4fLine2, cv::Point_<T>& cvCrossPoint)
	{
		double A = vec4fLine1[1];
		double B = -vec4fLine1[0];
		double C = vec4fLine1[0] * vec4fLine1[3] - vec4fLine1[1] * vec4fLine1[2];//Ax1+By1+C=0
		double D = vec4fLine2[1];
		double E = -vec4fLine2[0];
		double F = vec4fLine2[0] * vec4fLine2[3] - vec4fLine2[1] * vec4fLine2[2];//Dx2+Ey2+F=0
		if (A * E != B * D)
		{
			cvCrossPoint.x = (B * F - C * E) / (A * E - B * D);
			cvCrossPoint.y = (C * D - A * F) / (A * E - B * D);
			return TOOL_COMMON_OK;
		}
		else
		{
			return TOOL_COMMON_PARAERROR;
		}
	}

	static void cvDisOfPointToLine(cv::Vec4f linexy, cv::Point2d pointxy, double& dDist);
	static void cvDisOfPointToLine_Dir(cv::Vec4f linedir, cv::Point2d pointxy, double& dDist);
	static void cvDisOfPointToLineNoAbs_Dir(cv::Vec4f linedir, cv::Point2d pointxy, double& dDist);
	static void cvDisOfPointToLine_Dir(cv::Vec4f linedir, cv::Point2d pointxy, cv::Point2d& corssPoint, double& dDist);
	static void cvDisOfPointToLine_DirNoAbs(cv::Vec4f linedir, cv::Point2d pointxy, double& dDist);
	static void cvDisOfPointToLine_DirXY(cv::Point2d p, cv::Vec4f l, ToolCommonEnum::EDGEFINDER_DIRECTION positive_direct, double& dDist, int flag = 0, double compensate = 0);

	static void cvDisOfLineToLine(cv::Vec4f lineOne, cv::Vec4f lineTwo, std::vector<double> &vecDist);
	static void cvDisOfLineSegment(cv::Vec4f linexy, double& dDist);

	static void cvSelectMaxLines(const std::vector<cv::Vec4f> &allLines, std::vector<cv::Vec4f> &vecRsltLines, int counter = 1);
	static void cvLinesCrossPoint(cv::Vec4f lineOne, cv::Vec4f lineTwo, cv::Point2d& cvCrossPoint);
	static void cvLinesCrossPoint_Dir(cv::Vec4f lineOne, cv::Vec4f lineTwo, cv::Point2d& cvCrossPoint);
	static void cvLinesIsParallel(cv::Vec4f lineOne, cv::Vec4f lineTwo, double precision, bool& bIsParallel);
	static void cvLineGradientCos(cv::Vec4f linexy, double& dGradientCos);
	static void cvDirLineInRectConvert(cv::Vec4f dirLine, cv::Rect rect, cv::Vec4f& vec4fConvertLine);

	// calculate the distance of two point sets, with point to line
 	static void cvGetDistTwoPointSetsByPoint2Line(const std::vector<cv::Point2d>& ps1, const std::vector<cv::Point2d>& ps2, double& dDist);
 	static void cvGetDistTwoPointSetsByPoint2Line(const std::vector<cv::Point2d>& ps1, cv::Vec4f& line1, const std::vector<cv::Point2d>& ps2, cv::Vec4f& line2, double& dDist);

	// calculate the distance of point set to line
	static void cvGetDistPointSets2Line(std::vector<cv::Point2d>& ps, cv::Vec4f& line, double& dDist);
	static void cvGetDistPointSets2LineWithFarPoint(std::vector<cv::Point2d>& ps, cv::Vec4f& line, double& dDist);
	static void cvGetDistPointSets2LineWithFarPointNoAbs(std::vector<cv::Point2d>& ps, cv::Vec4f& line, double& dDist);
	static void cvGetDistPointSets2LineWithMidPoint(std::vector<cv::Point2d>& ps, cv::Vec4f& line, double& dDist);

	static void cvGetDistPointSets2Line(std::vector<cv::Point2d>& ps, cv::Vec4f& line, cv::Point2d& findP, double& dDist);
	static void cvGetDistPointSets2LineWithFarPoint(std::vector<cv::Point2d>& ps, cv::Vec4f& line, cv::Point2d& findP, double& dDist);
	static void cvGetDistPointSets2LineWithNearPoint(std::vector<cv::Point2d>& ps, cv::Vec4f& line, cv::Point2d& findP, double& dDist);
	static void cvGetDistPointSets2LineWithMidPoint(std::vector<cv::Point2d>& ps, cv::Vec4f& line, cv::Point2d& findP, double& dDist);

	// calculate the distance of two point sets
	static void cvGetDistOfTwoPointSets(std::vector<cv::Point2d>& ps1, std::vector<cv::Point2d>& ps2, double& minDist, double& maxDist, double &dDist);
	static void cvGetDistOfTwoPointSetsLine(std::vector<cv::Point2d>& ps1, std::vector<cv::Point2d>& ps2, double& minDist, double& maxDist, double& dDist);
	static void cvGetDistOfTwoPointSetsLine1(std::vector<cv::Point2d>& ps1, std::vector<cv::Point2d>& ps2, double& minDist, double& maxDist, cv::Point2d& min_p1, cv::Point2d& max_p1, cv::Point2d& min_p2, cv::Point2d& max_p2, double& dDist);
	static void cvGetAvgDistOfTwoPointSetsLine(std::vector<cv::Point2d>& ps1, std::vector<cv::Point2d>& ps2, double& avgDist, cv::Vec4f& line1, cv::Vec4f& line2, double& dDist);

};

#endif // !IMGPROC_ONEDIMENSION_MEASURE_H
