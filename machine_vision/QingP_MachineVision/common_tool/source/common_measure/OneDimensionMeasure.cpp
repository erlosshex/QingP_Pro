#include "onedimensionmeasure.h"
#include <limits>

OneDimensionMeasure::OneDimensionMeasure()
{

}

void OneDimensionMeasure::ensureValueInRange(double dValue, double dMinValue, double dMaxValue, double& dRsltValue, bool bIstruc /* = false */)
{
	if (dValue < dMinValue) {
		if (bIstruc) {
			dRsltValue = dMinValue;
			return;
		}
		else {
			dRsltValue = (dMaxValue + (dValue - dMinValue));
			return;
		}
	}
	else if (dValue <= dMaxValue) {
		dRsltValue = dValue;
		return;
	}
	else {
		if (bIstruc) {
			dRsltValue = dMaxValue;
			return;
		}
		else {
			dRsltValue = (dMinValue + (dValue - dMaxValue));
			return;
		}
	}
}

void OneDimensionMeasure::cvFilterContoursWithArea(const std::vector<std::vector<cv::Point>>& vecContours, double dAreaThres, std::vector<std::vector<cv::Point>>& vecResultContours, bool bIsGreater /* = true */)
{
	vecResultContours.clear();
	for (auto iter = vecContours.begin(); iter != vecContours.end(); ++iter) {
		double area = cv::contourArea(*iter);
		bool compareFlag = area > dAreaThres ? true : false;
		if (compareFlag == bIsGreater) {
			vecResultContours.push_back(*iter);
		}
	}
}

void OneDimensionMeasure::cvFilterContoursWithArea(const std::vector<std::vector<cv::Point>>& vecContours, double dMinAreaThres, double dMaxAreaThres, std::vector<std::vector<cv::Point>>& vecResultContours)
{
	vecResultContours.clear();
	for (auto iter = vecContours.begin(); iter != vecContours.end(); ++iter)
	{
		double area = cv::contourArea(*iter);
		if (area > dMinAreaThres && area < dMaxAreaThres) {
			vecResultContours.push_back(*iter);
		}
	}
}

uint OneDimensionMeasure::cvGetTwoDiagonalLineFromContours(std::vector<std::vector<cv::Point>>& vecContours, cv::Vec4f& vec4fLine1, cv::Vec4f& vec4fLine2)
{
	if (vecContours.empty())
	{
		return TOOL_COMMON_PARAERROR;
	}

	std::vector<cv::Point> vecAllPoints;
	vecAllPoints.resize(vecContours.size() * vecContours[0].size());

	int iCnt = 0;
	for (int i = 0; i < vecContours.size(); ++i)
	{
		auto contour = vecContours[i];
		for (int j = 0; j < contour.size(); ++j)
		{
			vecAllPoints[iCnt++] = contour[j];
		}
	}

	auto cvRect = cv::boundingRect(vecAllPoints);

	cv::Point2f cvp2fLeftUp(cvRect.x, cvRect.y);
	cv::Point2f cvp2fLeftDown(cvRect.x, cvRect.y + cvRect.height - 1);
	cv::Point2f cvp2fRightUp(cvRect.x + cvRect.width - 1, cvRect.y);
	cv::Point2f cvp2fRightDown(cvRect.x + cvRect.width - 1, cvRect.y + cvRect.height - 1);

	if (cvConvertLinePoint2Vec(cvp2fLeftUp, cvp2fRightDown, vec4fLine1) == TOOL_COMMON_PARAERROR ||
		cvConvertLinePoint2Vec(cvp2fRightUp, cvp2fLeftDown, vec4fLine2) == TOOL_COMMON_PARAERROR)
	{
		return TOOL_COMMON_UNKNOWERROR;
	}

	return TOOL_COMMON_OK;
}

void OneDimensionMeasure::cvMoveLine(cv::Vec4f& vec4fLine, double dDistX, double dDistY)
{
	if (std::abs(dDistX) < 0.01 && std::abs(dDistY) < 0.01)
	{
		return;
	}

	vec4fLine[2] += dDistX;
	vec4fLine[3] += dDistY;
}

uint OneDimensionMeasure::cvGetLineSegmentPoint(cv::Point2d cvPoint1, cv::Point2d cvPoint2, int step, std::vector<cv::Point2f>& veccvPoints)
{
	if (std::abs(cvPoint1.x - cvPoint2.x) <= 1e-3 && std::abs(cvPoint1.y - cvPoint2.y) <= 1e-3)
	{
		return TOOL_COMMON_PARAERROR;
	}

	double dXMin, dXMax, dYMin, dYMax;
	if (cvPoint1.x < cvPoint2.x) {
		dXMin = int(cvPoint1.x) + 1.0;
		dXMax = int(cvPoint2.x);
	}
	else {
		dXMin = int(cvPoint2.x) + 1.0;
		dXMax = int(cvPoint1.x);
	}

	if (cvPoint1.y < cvPoint2.y) {
		dYMin = int(cvPoint1.y) + 1.0;
		dYMax = int(cvPoint2.y);
	}
	else {
		dYMin = int(cvPoint2.y) + 1.0;
		dYMax = int(cvPoint1.y);
	}

	double p1x = cvPoint1.x;
	double p1y = cvPoint1.y;
	double p2x = cvPoint2.x;
	double p2y = cvPoint2.y;

	double vx = p2x - p1x;
	double vy = p2y - p1y;

	double distP12 = sqrt(vx * vx + vy * vy);

	double pvx = vx / distP12;
	double pvy = vy / distP12;

	int count = 0;

	double px = p1x + count * pvx;
	double py = p1y + count * pvy;

	while (std::sqrt(std::pow(px - p1x, 2) + std::pow(py - p1y, 2)) < distP12) {
		if (px < dXMin) px = dXMin;
		if (px > dXMax) px = dXMax;
		if (py < dYMin) py = dYMin;
		if (py > dYMax) py = dYMax;

		cv::Point2f cvp(px, py);
		veccvPoints.push_back(cvp);
		count++;
		px = p1x + 1.0 * count * step * pvx;
		py = p1y + 1.0 * count * step * pvy;
	}

	return TOOL_COMMON_OK;
}

void OneDimensionMeasure::cvGetCosThetaOfTwoLines(cv::Vec4f vec4fLine1, cv::Vec4f vec4fLine2, double& dTheta, bool bIsAngle /* = true */)
{
	float vx1 = vec4fLine1[0];
	float vy1 = vec4fLine1[1];
	float vx2 = vec4fLine2[0];
	float vy2 = vec4fLine2[1];

	double cosTheta = abs(vx1 * vx2 + vy1 * vy2);

	ensureValueInRange(cosTheta, -1, 1, cosTheta, true);
	if (bIsAngle)
		dTheta = std::acos(cosTheta) * 180 / CV_PI;
	else
		dTheta = std::acos(cosTheta);

	return;
}

void OneDimensionMeasure::cvGetRotateThetaOfLine2CorrespondLine(cv::Vec4f vec4fLine, cv::Vec4f vec4fCorrespondLine, double& dTheta, bool bIsAngle /* = true */)
{
	double vx1 = vec4fLine[0];
	double vy1 = vec4fLine[1];
	double vx2 = vec4fCorrespondLine[0];
	double vy2 = vec4fCorrespondLine[1];
	double cosTheta = std::abs(vx1 * vx2 + vy1 * vy2);

	ensureValueInRange(cosTheta, -1, 1, cosTheta, true);

	if (bIsAngle)
		dTheta = std::acos(cosTheta) * 180 / CV_PI;
	else
		dTheta = std::acos(cosTheta);

	return;
}

void OneDimensionMeasure::cvGetRotateThetaOfCorrespondLine2Line(cv::Vec4f vec4fLine, cv::Vec4f vec4fCorrespondLine, double& dTheta, bool bIsAngle /* = true */)
{
	double vx1 = vec4fLine[0];
	double vy1 = vec4fLine[1];
	double vx2 = vec4fCorrespondLine[0];
	double vy2 = vec4fCorrespondLine[1];

	double cosTheta = vx1 * vx2 + vy1 * vy2;
	ensureValueInRange(cosTheta, -1, 1, cosTheta, true);
	double dCalcTheta = 0;
	if (bIsAngle)
		dCalcTheta = std::acos(cosTheta) * 180 / CV_PI;
	else
		dCalcTheta = std::acos(cosTheta);

	double sinTheta = vx2 * vy1 - vy2 * vx1;

	if (sinTheta < 0) {
		dCalcTheta = -dCalcTheta;
	}

	dTheta = dCalcTheta;

	return;
}

void OneDimensionMeasure::cvGetRotateThetaOfLine2CorrespondLineByLineDir(double vx, double vy, double cor_vx, double cor_vy, double& dTheta, bool isAngle /* = true */, bool vxy_need_normal /* = false */, bool cor_vxy_need_normal /* = false */)
{
	double vx1 = vx;
	double vy1 = vy;
	double vx2 = cor_vx;
	double vy2 = cor_vy;

	if (vxy_need_normal) {
		double square_root = sqrt(vx1 * vx1 + vy1 * vy1);
		vx1 = vx1 / square_root;
		vy1 = vy1 / square_root;
	}

	if (cor_vxy_need_normal) {
		double square_root = sqrt(vx2 * vx2 + vy2 * vy2);
		vx2 = vx2 / square_root;
		vy2 = vy2 / square_root;
	}

	double cosTheta = vx1 * vx2 + vy1 * vy2;
	ensureValueInRange(cosTheta, -1, 1, cosTheta, true);
	double calc_theta = 0;
	if (isAngle)
		calc_theta = acos(cosTheta) * 180 / CV_PI;
	else
		calc_theta = acos(cosTheta);

	double sinTheta = vx1 * vy2 - vy1 * vx2;

	if (sinTheta < 0) {
		calc_theta = -calc_theta;
	}

	dTheta = calc_theta;
}

void OneDimensionMeasure::cvGetRotateThetaOfCorrespondLine2LineByLineDir(double vx, double vy, double cor_vx, double cor_vy, double& dTheta, bool isAngle /* = true */, bool vxy_need_normal /* = false */, bool cor_vxy_need_normal /* = false */)
{
	double vx1 = vx;
	double vy1 = vy;
	double vx2 = cor_vx;
	double vy2 = cor_vy;

	if (vxy_need_normal) {
		double square_root = sqrt(vx1 * vx1 + vy1 * vy1);
		vx1 = vx1 / square_root;
		vy1 = vy1 / square_root;
	}

	if (cor_vxy_need_normal) {
		double square_root = sqrt(vx2 * vx2 + vy2 * vy2);
		vx2 = vx2 / square_root;
		vy2 = vy2 / square_root;
	}

	double cosTheta = vx1 * vx2 + vy1 * vy2;
	ensureValueInRange(cosTheta, -1, 1, cosTheta, true);
	double calc_theta = 0;
	if (isAngle)
		calc_theta = acos(cosTheta) * 180 / CV_PI;
	else
		calc_theta = acos(cosTheta);

	double sinTheta = vx2 * vy1 - vy2 * vx1;

	if (sinTheta < 0) {
		calc_theta = -calc_theta;
	}

	dTheta = calc_theta;
}

uint OneDimensionMeasure::cvCrossAngleOfLines(cv::Vec4f vec4fLine1, cv::Vec4f vec4fLine2, double& dTheta)
{
	cv::Point2d cvCrossPoint;
	if (cvLinesCrossPointDir(vec4fLine1, vec4fLine2, cvCrossPoint) == TOOL_COMMON_PARAERROR)
	{
		return TOOL_COMMON_PARAERROR;
	}
	double cos0 = (vec4fLine1[2] - cvCrossPoint.x > 0) ? abs(vec4fLine1[0]) : -abs(vec4fLine1[0]);
	double sin0 = (vec4fLine1[3] - cvCrossPoint.y < 0) ? abs(vec4fLine1[1]) : -abs(vec4fLine1[1]);
	double cos1 = (vec4fLine2[2] - cvCrossPoint.x > 0) ? abs(vec4fLine2[0]) : -abs(vec4fLine2[0]);
	double sin1 = (vec4fLine2[3] - cvCrossPoint.y < 0) ? abs(vec4fLine2[1]) : -abs(vec4fLine2[1]);
	dTheta = std::acos(cos0 * cos1 + sin0 * sin1) * 180 / CV_PI;

	return TOOL_COMMON_OK;
}

void OneDimensionMeasure::cvDisOfPointToLine(cv::Vec4f linexy, cv::Point2d pointxy, double& dDist)
{
	double A = linexy[1] - linexy[3];
	double B = -(linexy[0] - linexy[2]);
	double C = linexy[0] * linexy[3] - linexy[1] * linexy[2];
	dDist = std::abs(A * pointxy.x + B * pointxy.y + C) / std::sqrt(A * A + B * B);
}

void OneDimensionMeasure::cvDisOfPointToLine_Dir(cv::Vec4f linedir, cv::Point2d pointxy, double& dDist)
{
	double A = linedir[1];
	double B = -linedir[0];
	double C = linedir[0] * linedir[3] - linedir[1] * linedir[2];//Ax1+By1+C=0
	dDist = std::abs(A * pointxy.x + B * pointxy.y + C) / std::sqrt(A * A + B * B);
}

void OneDimensionMeasure::cvDisOfPointToLineNoAbs_Dir(cv::Vec4f linedir, cv::Point2d pointxy, double& dDist)
{
	double A = linedir[1];
	double B = -linedir[0];
	double C = linedir[0] * linedir[3] - linedir[1] * linedir[2];//Ax1+By1+C=0
	dDist = (A * pointxy.x + B * pointxy.y + C) / std::sqrt(A * A + B * B);
}

void OneDimensionMeasure::cvDisOfPointToLine_Dir(cv::Vec4f linedir, cv::Point2d pointxy, cv::Point2d& corssPoint, double& dDist)
{
	double A = linedir[1];
	double B = -linedir[0];
	double C = linedir[0] * linedir[3] - linedir[1] * linedir[2];//Ax1+By1+C=0

	cv::Vec4f c_line;
	c_line[0] = -linedir[1];
	c_line[1] = linedir[0];
	c_line[2] = pointxy.x;
	c_line[3] = pointxy.y;

	corssPoint;
	cvLinesCrossPoint_Dir(c_line, linedir, corssPoint);

	dDist = std::abs(A * pointxy.x + B * pointxy.y + C) / std::sqrt(A * A + B * B);
}

void OneDimensionMeasure::cvDisOfPointToLine_DirNoAbs(cv::Vec4f linedir, cv::Point2d pointxy, double& dDist)
{
	double A = linedir[1];
	double B = -linedir[0];
	double C = linedir[0] * linedir[3] - linedir[1] * linedir[2];//Ax1+By1+C=0

	if (A != 0)
	{
		double	x_0 = (-B * pointxy.y - C) / A;
		if (pointxy.x > x_0)
		{
			dDist = std::abs(A * pointxy.x + B * pointxy.y + C) / std::sqrt(A * A + B * B);
			return;
		}
		else
		{
			dDist = -std::abs(A * pointxy.x + B * pointxy.y + C) / std::sqrt(A * A + B * B);
			return;
		}
	}
	else
	{
		double x_0 = -C / A;
		if (x_0 > pointxy.x)
		{
			dDist = -std::abs(A * pointxy.x + B * pointxy.y + C) / std::sqrt(A * A + B * B);
			return;
		}
		else
		{
			dDist = std::abs(A * pointxy.x + B * pointxy.y + C) / std::sqrt(A * A + B * B);
			return;
		}
	}
}

void OneDimensionMeasure::cvDisOfPointToLine_DirXY(cv::Point2d p, cv::Vec4f l, ToolCommonEnum::EDGEFINDER_DIRECTION positive_direct, double& dDist, int flag /* = 0 */, double compensate /* = 0 */)
{
	//flag: 0 is Y, 1 is X
	double A = l[1];
	double B = -l[0];
	double C = l[0] * l[3] - l[1] * l[2];//Ax1+By1+C=0
	double dis = abs(A * p.x + B * p.y + C) / sqrt(A * A + B * B);

	int direction;
	cvJudgePointPositionWithLine(p, l, direction, flag, compensate);

	switch (positive_direct)
	{
	case ToolCommonEnum::enumLEFT2RIGHT:
		direction = -direction;
		break;
	case ToolCommonEnum::enumRIGHT2LEFT:
		direction = direction;
		break;
	case ToolCommonEnum::enumTOP2BOTTOM:
		direction = -direction;
		break;
	case ToolCommonEnum::enumBOTTOM2TOP:
		direction = direction;
		break;
	case ToolCommonEnum::enumNODIRECTION:
		direction = direction;
		break;
	default:
		break;
	}

	dis = dis * direction;
	dDist = dis;
}

void OneDimensionMeasure::cvDisOfLineToLine(cv::Vec4f lineOne, cv::Vec4f lineTwo, std::vector<double>& vecDist)
{
	vecDist.clear();
	double dDist = 0.0;
	cvDisOfPointToLine(lineOne, cv::Point2d(lineTwo[0], lineTwo[1]), dDist);
	vecDist.push_back(dDist);
	cvDisOfPointToLine(lineOne, cv::Point2d(lineTwo[2], lineTwo[3]), dDist);
	vecDist.push_back(dDist);
	cvDisOfPointToLine(lineTwo, cv::Point2d(lineOne[0], lineOne[1]), dDist);
	vecDist.push_back(dDist);
	cvDisOfPointToLine(lineTwo, cv::Point2d(lineOne[2], lineOne[3]), dDist);
	vecDist.push_back(dDist);
}

void OneDimensionMeasure::cvDisOfLineSegment(cv::Vec4f linexy, double& dDist)
{
	dDist = std::sqrt((linexy[0] - linexy[2]) * (linexy[0] - linexy[2]) + (linexy[1] - linexy[3]) * (linexy[1] - linexy[3]));
}

void OneDimensionMeasure::cvSelectMaxLines(const std::vector<cv::Vec4f> &allLines, std::vector<cv::Vec4f> &vecRsltLines, int counter /* = 1 */)
{
	if (counter < allLines.size())
	{
		std::vector<cv::Vec4f> tempLine;
		std::vector<double> distanceOfLines;
		std::vector<int> sortArray;
		int tempsort;
		double tempdis;
		for (int i = 0; i < allLines.size(); i++)
		{
			double dDist = 0.0;
			cvDisOfLineSegment(allLines.at(i), dDist);
			distanceOfLines.push_back(dDist);
			sortArray.push_back(i);
		}
		for (int i = 0; i < counter; i++)
		{
			for (int j = i + 1; j < distanceOfLines.size(); j++)
			{
				if (distanceOfLines.at(i) < distanceOfLines.at(j))
				{
					tempdis = distanceOfLines.at(i);
					distanceOfLines.at(i) = distanceOfLines.at(j);
					distanceOfLines.at(j) = tempdis;
					tempsort = sortArray.at(i);
					sortArray.at(i) = sortArray.at(j);
					sortArray.at(j) = tempsort;
				}
			}
			tempLine.push_back(allLines.at(sortArray.at(i)));
		}
		vecRsltLines = tempLine;
	}
	else
	{
		vecRsltLines = allLines;
	}
}

void OneDimensionMeasure::cvLinesCrossPoint(cv::Vec4f lineOne, cv::Vec4f lineTwo, cv::Point2d& cvCrossPoint)
{
	cvCrossPoint = cv::Point2d(-1, -1);
	double A = lineOne[1] - lineOne[3];
	double B = -(lineOne[0] - lineOne[2]);
	double C = lineOne[0] * lineOne[3] - lineOne[1] * lineOne[2];//Ax1+By1+C=0
	double D = lineTwo[1] - lineTwo[3];
	double E = -(lineTwo[0] - lineTwo[2]);
	double F = lineTwo[0] * lineTwo[3] - lineTwo[1] * lineTwo[2];//Dx2+Ey2+F=0
	if (A * E != B * D)
	{
		cvCrossPoint.x = (B * F - C * E) / (A * E - B * D);
		cvCrossPoint.y = (C * D - A * F) / (A * E - B * D);
	}
}

void OneDimensionMeasure::cvLinesCrossPoint_Dir(cv::Vec4f lineOne, cv::Vec4f lineTwo, cv::Point2d& cvCrossPoint)
{
	cvCrossPoint = cv::Point2d(-1, -1);
	double A = lineOne[1];
	double B = -lineOne[0];
	double C = lineOne[0] * lineOne[3] - lineOne[1] * lineOne[2];//Ax1+By1+C=0
	double D = lineTwo[1];
	double E = -lineTwo[0];
	double F = lineTwo[0] * lineTwo[3] - lineTwo[1] * lineTwo[2];//Dx2+Ey2+F=0
	if (A * E != B * D)
	{
		cvCrossPoint.x = (B * F - C * E) / (A * E - B * D);
		cvCrossPoint.y = (C * D - A * F) / (A * E - B * D);
	}
}

void OneDimensionMeasure::cvLinesIsParallel(cv::Vec4f lineOne, cv::Vec4f lineTwo, double precision, bool& bIsParallel)
{
	double A = lineOne[1] - lineOne[3];
	double B = -(lineOne[0] - lineOne[2]);
	double D = lineTwo[1] - lineTwo[3];
	double E = -(lineTwo[0] - lineTwo[2]);
	if (abs(A * E - B * D) <= precision)
	{
		bIsParallel = true;
	}
	else
	{
		bIsParallel = false;
	}
	return;
}

void OneDimensionMeasure::cvLineGradientCos(cv::Vec4f linexy, double& dGradientCos)
{
	if (linexy[0] == linexy[2])
	{
		dGradientCos = 0;
	}
	else if (linexy[1] == linexy[3])
	{
		dGradientCos = 1;
	}
	else
	{
		double dDist = 0.0;
		cvDisOfLineSegment(linexy, dDist);
		dGradientCos = std::abs(linexy[0] - linexy[2]) / dDist;
	}
	return;
}

void OneDimensionMeasure::cvDirLineInRectConvert(cv::Vec4f dirLine, cv::Rect rect, cv::Vec4f& vec4fConvertLine)
{
	if (dirLine[0] == 0)
	{
		vec4fConvertLine = cv::Vec4f(dirLine[2], rect.y, dirLine[2], rect.y + rect.height);
	}
	else
	{
		cv::Point2d p1 = cv::Point2d(rect.x, (dirLine[1] * rect.x + dirLine[0] * dirLine[3] - dirLine[1] * dirLine[2]) / dirLine[0]);
		cv::Point2d p2 = cv::Point2d(rect.x + rect.width, (dirLine[1] * (rect.x + rect.width) + dirLine[0] * dirLine[3] - dirLine[1] * dirLine[2]) / dirLine[0]);
		cv::Point2d p3 = cv::Point2d((dirLine[0] * rect.y - dirLine[0] * dirLine[3] + dirLine[1] * dirLine[2]) / dirLine[1], rect.y);
		cv::Point2d p4 = cv::Point2d((dirLine[0] * (rect.y + rect.height) - dirLine[0] * dirLine[3] + dirLine[1] * dirLine[2]) / dirLine[1], rect.y + rect.height);
		if ((p1.y >= rect.y) && (p1.y <= rect.y + rect.height))
		{
			if ((p2.y >= rect.y) && (p2.y <= rect.y + rect.height))
			{
				vec4fConvertLine = cv::Vec4f(p1.x, p1.y, p2.x, p2.y);
			}
			else if ((p3.x >= rect.x) && (p3.x <= rect.x + rect.width))
			{
				vec4fConvertLine = cv::Vec4f(p1.x, p1.y, p3.x, p3.y);
			}
			else if ((p4.x >= rect.x) && (p4.x <= rect.x + rect.width))
			{
				vec4fConvertLine = cv::Vec4f(p1.x, p1.y, p4.x, p4.y);
			}
		}
		else if ((p2.y >= rect.y) && (p2.y <= rect.y + rect.height))
		{
			if ((p3.x >= rect.x) && (p3.x <= rect.x + rect.width))
			{
				vec4fConvertLine = cv::Vec4f(p2.x, p2.y, p3.x, p3.y);
			}
			else if ((p4.x >= rect.x) && (p4.x <= rect.x + rect.width))
			{
				vec4fConvertLine = cv::Vec4f(p2.x, p2.y, p4.x, p4.y);
			}
		}
		else if ((p3.x >= rect.x) && (p3.x <= rect.x + rect.width))
		{
			if ((p4.x >= rect.x) && (p4.x <= rect.x + rect.width))
			{
				vec4fConvertLine = cv::Vec4f(p3.x, p3.y, p4.x, p4.y);
			}
		}
		vec4fConvertLine = cv::Vec4f(-1, -1, -1, -1);
	}
}

void OneDimensionMeasure::cvGetDistTwoPointSetsByPoint2Line(const std::vector<cv::Point2d>& ps1, const std::vector<cv::Point2d>& ps2, double& dDist)
{
	double res = -1;

	if (ps1.size() > 1 && ps2.size() > 1) {
		cv::Vec4f line1;
		cv::Vec4f line2;

		cv::fitLine(ps1, line1, cv::DIST_L2, 0, 0.001, 0.01);
		cv::fitLine(ps2, line2, cv::DIST_L2, 0, 0.01, 0.01);

		double d1 = 0;
		for (auto& p : ps1) {
			double _dDist = 0.0;
			cvDisOfPointToLine_Dir(line2, p, _dDist);
			d1 += _dDist;
		}
		d1 /= ps1.size();

		double d2 = 0;
		for (auto& p : ps2) {
			double _dDist = 0.0;
			cvDisOfPointToLine_Dir(line1, p, _dDist);
			d2 += _dDist;
		}
		d2 /= ps2.size();

		res = (d1 + d2) / 2;
	}

	dDist = res;
}

void OneDimensionMeasure::cvGetDistTwoPointSetsByPoint2Line(const std::vector<cv::Point2d>& ps1, cv::Vec4f& line1, const std::vector<cv::Point2d>& ps2, cv::Vec4f& line2, double& dDist)
{
	double res = -1;

	if (ps1.size() > 1 && ps2.size() > 2) {
		double d1 = 0;
		for (auto& p : ps1) {
			double _dDist = 0.0;
			cvDisOfPointToLine_Dir(line2, p, _dDist);
			d1 += _dDist;
		}
		d1 /= ps1.size();

		double d2 = 0;
		for (auto& p : ps2) {
			double _dDist = 0.0;
			cvDisOfPointToLine_Dir(line1, p, _dDist);
			d2 += _dDist;
		}
		d2 /= ps2.size();

		res = (d1 + d2) / 2;
	}

	dDist = res;
}

void OneDimensionMeasure::cvGetDistPointSets2Line(std::vector<cv::Point2d>& ps, cv::Vec4f& line, double& dDist)
{
	double res = -1;

	if (ps.size() > 0) {
		for (auto& p : ps) {
			double _dDist = 0.0;
			cvDisOfPointToLine_Dir(line, p, _dDist);
			res += _dDist;
		}
		res /= ps.size();
	}

	dDist = res;
}

void OneDimensionMeasure::cvGetDistPointSets2LineWithFarPoint(std::vector<cv::Point2d>& ps, cv::Vec4f& line, double& dDist)
{
	double res = -1;

	if (ps.size() > 0) {
		for (auto& p : ps) {
			double _dDist = 0.0;
			cvDisOfPointToLine_Dir(line, p, _dDist);
			if (_dDist > res)
			{
				res = _dDist;
			}
		}
	}

	dDist = res;
}

void OneDimensionMeasure::cvGetDistPointSets2LineWithFarPointNoAbs(std::vector<cv::Point2d>& ps, cv::Vec4f& line, double& dDist)
{
	double res = -9999;

	if (ps.size() > 0) {
		for (auto& p : ps) {
			double _dDist = 0.0;
			cvDisOfPointToLine_DirNoAbs(line, p, _dDist);
			if (_dDist > res)
			{
				res = _dDist;
			}
		}
	}

	dDist = res;
}

void OneDimensionMeasure::cvGetDistPointSets2LineWithMidPoint(std::vector<cv::Point2d>& ps, cv::Vec4f& line, double& dDist)
{
	double res = -1;

	if (ps.size() > 0) {
		double _dDist = 0.0;
		cvDisOfPointToLine_Dir(line, ps[ps.size() / 2], _dDist);
		res = _dDist;
	}

	dDist = res;
}

void OneDimensionMeasure::cvGetDistPointSets2Line(std::vector<cv::Point2d>& ps, cv::Vec4f& line, cv::Point2d& findP, double& dDist)
{
	double res = 0;

	if (ps.size() > 0) {
		for (auto& p : ps) {
			double _dDist = 0.0;
			cvDisOfPointToLine_Dir(line, p, _dDist);
			res += _dDist;
		}
		res /= ps.size();
	}

	findP = ps[ps.size() / 2];

	dDist = res;
}

void OneDimensionMeasure::cvGetDistPointSets2LineWithFarPoint(std::vector<cv::Point2d>& ps, cv::Vec4f& line, cv::Point2d& findP, double& dDist)
{
	double res = 0;

	if (ps.size() > 0) {
		for (auto& p : ps) {
			double _dDist = 0.0;
			cvDisOfPointToLine_Dir(line, p, _dDist);
			if (_dDist > res) {
				res = _dDist;
				findP = p;
			}
		}
	}

	dDist = res;
}

void OneDimensionMeasure::cvGetDistPointSets2LineWithNearPoint(std::vector<cv::Point2d>& ps, cv::Vec4f& line, cv::Point2d& findP, double& dDist)
{
	double res = 9999;

	if (ps.size() > 0) {
		for (auto& p : ps) {
			double _dDist = 0.0;
			cvDisOfPointToLine_Dir(line, p, _dDist);
			if (_dDist < res) {
				res = _dDist;
				findP = p;
			}
		}
	}

	dDist = res;
}

void OneDimensionMeasure::cvGetDistPointSets2LineWithMidPoint(std::vector<cv::Point2d>& ps, cv::Vec4f& line, cv::Point2d& findP, double& dDist)
{
	double res = 0;

	if (ps.size() > 0) {
		findP = ps[ps.size() / 2];
		double _dDist = 0.0;
		cvDisOfPointToLine_Dir(line, findP, _dDist);
		res = _dDist;
	}

	dDist = res;
}

void OneDimensionMeasure::cvGetDistOfTwoPointSets(std::vector<cv::Point2d>& ps1, std::vector<cv::Point2d>& ps2, double& minDist, double& maxDist, double& dDist)
{
	double res = 0;

	if (ps1.size() > 0 && ps2.size() > 0) {
		minDist = (std::numeric_limits<double>::max)();
		maxDist = -1;
		for (auto& p1 : ps1) {
			for (auto& p2 : ps2) {
				double _dDist = 0.0;
				cvDistTwoPoints(p1, p2, _dDist);
				res += _dDist;
				if (_dDist < minDist) minDist = _dDist;
				if (_dDist > maxDist) maxDist = _dDist;
			}
		}

		res /= (int(ps1.size()) * int(ps2.size()));
	}

	dDist = res;
}

void OneDimensionMeasure::cvGetDistOfTwoPointSetsLine(std::vector<cv::Point2d>& ps1, std::vector<cv::Point2d>& ps2, double& minDist, double& maxDist, double& dDist)
{
	double res = 0;

	if (ps1.size() > 1 && ps2.size() > 1) {
		minDist = (std::numeric_limits<double>::max)();
		maxDist = -1;
		double d = -1;

		cv::Vec4f line1;
		cv::Vec4f line2;

		cv::fitLine(ps1, line1, cv::DIST_L2, 0, 0.01, 0.01);
		cv::fitLine(ps2, line2, cv::DIST_L2, 0, 0.01, 0.01);

		for (auto& p1 : ps1) {
			cvDisOfPointToLine_Dir(line2, p1, d);
			res += d;
			if (d < minDist) minDist = d;
			if (d > maxDist) maxDist = d;
		}

		for (auto& p2 : ps2) {
			cvDisOfPointToLine_Dir(line1, p2, d);
			res += d;
			if (d < minDist) minDist = d;
			if (d > maxDist) maxDist = d;
		}

		res /= (int(ps1.size()) * int(ps2.size()));
	}

	dDist = res;
}

void OneDimensionMeasure::cvGetDistOfTwoPointSetsLine1(std::vector<cv::Point2d>& ps1, std::vector<cv::Point2d>& ps2, double& minDist, double& maxDist, cv::Point2d& min_p1, cv::Point2d& max_p1, cv::Point2d& min_p2, cv::Point2d& max_p2, double& dDist)
{
	double res = 0;

	if (ps1.size() > 1 && ps2.size() > 1) {
		minDist = (std::numeric_limits<double>::max)();
		maxDist = -1;
		double d = -1;

		cv::Vec4f line1;
		cv::Vec4f line2;

		cv::fitLine(ps1, line1, cv::DIST_L2, 0, 0.01, 0.01);
		cv::fitLine(ps2, line2, cv::DIST_L2, 0, 0.01, 0.01);

		for (auto& p1 : ps1) {
			cvDisOfPointToLine_Dir(line2, p1, d);
			res += d;
			if (d < minDist) {
				minDist = d;
				min_p1 = p1;
			}
			if (d > maxDist) {
				maxDist = d;
				max_p1 = p1;
			}
		}

		bool ps2_min_dist_enter = false;
		bool ps2_max_dist_enter = false;

		for (auto& p2 : ps2) {
			cvDisOfPointToLine_Dir(line1, p2, d);
			res += d;
			if (d < minDist) {
				ps2_min_dist_enter = true;
				minDist = d;
				min_p2 = p2;
			}
			if (d > maxDist) {
				ps2_max_dist_enter = true;
				maxDist = d;
				max_p2 = p2;
			}
		}

		if (!ps2_min_dist_enter) {
			min_p2 = ps2[ps2.size() / 2];
		}
		if (!ps2_max_dist_enter) {
			max_p2 = ps2[ps2.size() / 2];
		}

		res /= (int(ps1.size()) * int(ps2.size()));
	}

	dDist = res;
}

void OneDimensionMeasure::cvGetAvgDistOfTwoPointSetsLine(std::vector<cv::Point2d>& ps1, std::vector<cv::Point2d>& ps2, double& avgDist, cv::Vec4f& line1, cv::Vec4f& line2, double& dDist)
{
	double res = -1;

	if (ps1.size() > 1 && ps2.size() > 1) {
		avgDist = 0;

		double d = 0;

		cv::fitLine(ps1, line1, cv::DIST_L2, 0, 0.01, 0.01);
		cv::fitLine(ps2, line2, cv::DIST_L2, 0, 0.01, 0.01);

		for (auto& p1 : ps1) {
			cvDisOfPointToLine_Dir(line2, p1, d);
			avgDist += d;
		}

		for (auto& p2 : ps2) {
			cvDisOfPointToLine_Dir(line1, p2, d);
			avgDist += d;
		}

		avgDist /= (int(ps1.size()) + int(ps2.size()));

		res = 0;
	}
	else {
		res = -1;
	}

	dDist = res;
}