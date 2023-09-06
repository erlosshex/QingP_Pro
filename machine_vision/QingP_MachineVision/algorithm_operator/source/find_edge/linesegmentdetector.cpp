#include "linesegmentdetector.h"

using namespace cv;
using namespace std;

CLineSegmentDetector::CLineSegmentDetector()
{
	//detector = createLineSegmentDetector(LSD_REFINE_NONE);
}

CLineSegmentDetector::~CLineSegmentDetector()
{

}


int CLineSegmentDetector::FindLines(Mat DstMat, Point offset)
{
	lines.clear();
	//detector->detect(DstMat, lines);
	for (int i = 0; i < lines.size(); i++)
	{
		lines.at(i)[0] += offset.x;
		lines.at(i)[1] += offset.y;
		lines.at(i)[2] += offset.x;
		lines.at(i)[3] += offset.y;
	}
	return lines.size();
}

void CLineSegmentDetector::DrawLines(Mat SrcMat)
{
	//detector->drawSegments(SrcMat, lines);
}

double CLineSegmentDetector::DisOfPointToLine(const Vec4f& linexy, const Point2d& pointxy)
{
	double A = linexy[1] - linexy[3];
	double B = -(linexy[0] - linexy[2]);
	double C = linexy[0] * linexy[3] - linexy[1] * linexy[2];//Ax+By+C=0
	return abs(A*pointxy.x + B*pointxy.y + C) / sqrt(A*A + B*B);
}

double CLineSegmentDetector::DisOfPointToLine_Dir(Vec4f linedir, Point2d pointxy)
{
	double A = linedir[1];
	double B = -linedir[0];
	double C = linedir[0] * linedir[3] - linedir[1] * linedir[2];//Ax1+By1+C=0
	return abs(A*pointxy.x + B*pointxy.y + C) / sqrt(A*A + B*B);
}

double CLineSegmentDetector::DisOfPointToLine_Dir(Vec4f linedir, Point2d pointxy, Point2d &corssPoint)
{
	double A = linedir[1];
	double B = -linedir[0];
	double C = linedir[0] * linedir[3] - linedir[1] * linedir[2];//Ax1+By1+C=0

	Vec4f c_line;
	c_line[0] = -linedir[1];
	c_line[1] = linedir[0];
	c_line[2] = pointxy.x;
	c_line[3] = pointxy.y;

	corssPoint = linesCrossPoint_Dir(c_line, linedir);

	return abs(A*pointxy.x + B*pointxy.y + C) / sqrt(A*A + B*B);
}

double CLineSegmentDetector::DisOfPointToLine_DirNoAbs(Vec4f linedir, Point2d pointxy)
{
	double A = linedir[1];
	double B = -linedir[0];
	double C = linedir[0] * linedir[3] - linedir[1] * linedir[2];//Ax1+By1+C=0
	
	if (A != 0)
	{
		double	x_0 = (-B*pointxy.y - C) / A;


		if (pointxy.x > x_0)
		{
			
			return abs(A*pointxy.x + B*pointxy.y + C) / sqrt(A*A + B*B);
		}
		else
		{
			return -abs(A*pointxy.x + B*pointxy.y + C) / sqrt(A*A + B*B);
		}


	}
	else
	{
		double x_0 = -C / A;

		if (x_0>pointxy.x)
		{
			return -abs(A*pointxy.x + B*pointxy.y + C) / sqrt(A*A + B*B);
		}
		else
		{
			return abs(A*pointxy.x + B*pointxy.y + C) / sqrt(A*A + B*B);
		}
	}
	 

}

vector<double> CLineSegmentDetector::DisOfLineToLine(Vec4f lineOne, Vec4f lineTwo)
{
	vector<double> tempDis;
	tempDis.push_back(DisOfPointToLine(lineOne, Point2d(lineTwo[0], lineTwo[1])));
	tempDis.push_back(DisOfPointToLine(lineOne, Point2d(lineTwo[2], lineTwo[3])));
	tempDis.push_back(DisOfPointToLine(lineTwo, Point2d(lineOne[0], lineOne[1])));
	tempDis.push_back(DisOfPointToLine(lineTwo, Point2d(lineOne[2], lineOne[3])));
	return tempDis;
}

double CLineSegmentDetector::DisOfLineSegment(Vec4f linexy)
{
	return sqrt((linexy[0] - linexy[2])*(linexy[0] - linexy[2]) + (linexy[1] - linexy[3])*(linexy[1] - linexy[3]));
}

vector<Vec4f> CLineSegmentDetector::SelectMaxLines(vector<Vec4f> allLines, int counter)
{	
	if (counter < allLines.size())
	{
		vector<Vec4f> tempLine;
		vector<double> distanceOfLines;
		vector<int> sortArray;
		int tempsort;
		double tempdis;
		for (int i = 0; i < allLines.size(); i++)
		{
			distanceOfLines.push_back(DisOfLineSegment(allLines.at(i)));
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
		return tempLine;
	}
	else 
	{
		return allLines;
	}
	
}

Point2d CLineSegmentDetector::linesCrossPoint(Vec4f lineOne, Vec4f lineTwo)
{
	Point2d crossPoint(-1, -1);
	double A = lineOne[1] - lineOne[3];
	double B = -(lineOne[0] - lineOne[2]);
	double C = lineOne[0] * lineOne[3] - lineOne[1] * lineOne[2];//Ax1+By1+C=0
	double D = lineTwo[1] - lineTwo[3];
	double E = -(lineTwo[0] - lineTwo[2]);
	double F = lineTwo[0] * lineTwo[3] - lineTwo[1] * lineTwo[2];//Dx2+Ey2+F=0
	if (A*E != B*D)
	{
		crossPoint.x = (B*F - C*E) / (A*E - B*D);
		crossPoint.y = (C*D - A*F) / (A*E - B*D);
	}
	return crossPoint;
}

Point2d CLineSegmentDetector::linesCrossPoint_Dir(Vec4f lineOne, Vec4f lineTwo)
{
	Point2d crossPoint(-1, -1);
	double A = lineOne[1];
	double B = -lineOne[0];
	double C = lineOne[0] * lineOne[3] - lineOne[1] * lineOne[2];//Ax1+By1+C=0
	double D = lineTwo[1];
	double E = -lineTwo[0];
	double F = lineTwo[0] * lineTwo[3] - lineTwo[1] * lineTwo[2];//Dx2+Ey2+F=0
	if (A*E != B*D)
	{
		crossPoint.x = (B*F - C*E) / (A*E - B*D);
		crossPoint.y = (C*D - A*F) / (A*E - B*D);
	}
	return crossPoint;
}

bool CLineSegmentDetector::linesIsParallel(Vec4f lineOne, Vec4f lineTwo, double precision)
{
	double A = lineOne[1] - lineOne[3];
	double B = -(lineOne[0] - lineOne[2]);
	double D = lineTwo[1] - lineTwo[3];
	double E = -(lineTwo[0] - lineTwo[2]);
	if (abs(A*E - B*D) <= precision)
	{
		return true;
	}
	else
	{
		return false;
	}
}

double CLineSegmentDetector::lineGradientCos(Vec4f linexy)
{
	if (linexy[0] == linexy[2])
	{
		return 0;
	}
	else if (linexy[1] == linexy[3])
	{
		return 1;
	}
	else
	{
		return abs(linexy[0] - linexy[2]) / DisOfLineSegment(linexy);
	}
}

Vec4f CLineSegmentDetector::DirLineInRectConvert(Vec4f dirLine, QRect rect)
{
	if (dirLine[0] == 0)
	{
		return Vec4f(dirLine[2], rect.y(), dirLine[2], rect.y()+rect.height());
	}
	else
	{
		QPointF p1 = QPointF(rect.x(), (dirLine[1] * rect.x() + dirLine[0] * dirLine[3] - dirLine[1] * dirLine[2]) / dirLine[0]);
		QPointF p2 = QPointF(rect.x() + rect.width(), (dirLine[1] * (rect.x() + rect.width()) + dirLine[0] * dirLine[3] - dirLine[1] * dirLine[2]) / dirLine[0]);
		QPointF p3 = QPointF((dirLine[0] * rect.y() - dirLine[0] * dirLine[3] + dirLine[1] * dirLine[2]) / dirLine[1], rect.y());
		QPointF p4 = QPointF((dirLine[0] * (rect.y() + rect.height()) - dirLine[0] * dirLine[3] + dirLine[1] * dirLine[2]) / dirLine[1], rect.y() + rect.height());

		// *** origin code ***
		//vector<QPointF> ptsList;
		//if ((p1.y() >= rect.y() - 1) && (p1.y() <= rect.y() + rect.height() + 1))
		//{
		//	ptsList.push_back(p1);
		//}
		//if ((p2.y() >= rect.y() - 1) && (p2.y() <= rect.y() + rect.height()) + 1)
		//{
		//	ptsList.push_back(p2);
		//}
		//if ((p3.x() >= rect.x() - 1) && (p3.x() <= rect.x() + rect.width()) + 1)
		//{
		//	ptsList.push_back(p3);
		//}
		//if ((p4.x() >= rect.x() - 1) && (p4.x() <= rect.x() + rect.width()) + 1)
		//{
		//	ptsList.push_back(p4);
		//}
		// *** origin code end ***

		// *** new code ***
		// 增加顶点判断
		bool push_p1_flag = false;
		bool push_p2_flag = false;
		bool push_p3_flag = false;
		bool push_p4_flag = false;

		vector<QPointF> ptsList;
		double dist_threshold = 2;
		if ((p1.y() >= rect.y()) && (p1.y() <= rect.y() + rect.height()))
		{
			//ptsList.push_back(p1);
			//push_p1_flag = true;

			if (!isInPointSet(ptsList, p1, dist_threshold)) {
				ptsList.push_back(p1);
				push_p1_flag = true;
			}
		}
		if ((p2.y() >= rect.y()) && (p2.y() <= rect.y() + rect.height()))
		{
			//ptsList.push_back(p2);
			//push_p2_flag = true;

			if (!isInPointSet(ptsList, p2, dist_threshold)) {
				ptsList.push_back(p2);
				push_p2_flag = true;
			}
		}
		if ((p3.x() >= rect.x()) && (p3.x() <= rect.x() + rect.width()))
		{
			//ptsList.push_back(p3);
			//push_p3_flag = true;

			if (!isInPointSet(ptsList, p3, dist_threshold)) {
				ptsList.push_back(p3);
				push_p3_flag = true;
			}
		}
		if ((p4.x() >= rect.x()) && (p4.x() <= rect.x() + rect.width()))
		{
			//ptsList.push_back(p4);
			//push_p4_flag = true;

			if (!isInPointSet(ptsList, p4, dist_threshold)) {
				ptsList.push_back(p4);
				push_p4_flag = true;
			}
		}

		if (ptsList.size() < 2) {
			int x_start = rect.x();
			int y_start = rect.y();
			int x_end = x_start + rect.width();
			int y_end = y_start + rect.height();

			float dist_thres = 2;

			if (!push_p1_flag) {
				if (abs(p1.y() - y_start) < dist_thres || abs(p1.y() - y_end) < dist_thres) {
					//ptsList.push_back(p1);
					//push_p1_flag = true;

					if (!isInPointSet(ptsList, p1, dist_thres)) {
						ptsList.push_back(p1);
						push_p1_flag = true;
					}
				}
			}
			if (!push_p2_flag) {
				if (abs(p2.y() - y_start) < dist_thres || abs(p2.y() - y_end) < dist_thres) {
					//ptsList.push_back(p2);
					//push_p2_flag = true;

					if (!isInPointSet(ptsList, p2, dist_thres)) {
						ptsList.push_back(p2);
						push_p2_flag = true;
					}
				}
			}
			if (!push_p3_flag) {
				if (abs(p3.x() - x_start) < dist_thres || abs(p3.y() - x_end) < dist_thres) {
					//ptsList.push_back(p3);
					//push_p3_flag = true;

					if (!isInPointSet(ptsList, p3, dist_thres)) {
						ptsList.push_back(p3);
						push_p3_flag = true;
					}
				}
			}
			if (!push_p4_flag) {
				if (abs(p4.x() - x_start) < dist_thres || abs(p4.y() - x_end) < dist_thres) {
					//ptsList.push_back(p4);
					//push_p4_flag = true;

					if (!isInPointSet(ptsList, p4, dist_thres)) {
						ptsList.push_back(p4);
						push_p4_flag = true;
					}
				}
			}
		}
		// *** new code end ***

		if (ptsList.size() < 2)
		{
			return Vec4f(-1, -1, -1, -1);
		}
		else if (ptsList.size() == 2)
		{
			return Vec4f(ptsList.at(0).x(), ptsList.at(0).y(), ptsList.at(1).x(), ptsList.at(1).y());
		}
		else
		{
			vector<double> m_distance;
			vector<Vec4f> m_lineList;
			for (int i = 0; i < ptsList.size() - 1; i++)
			{
				for (int j = i + 1; j < ptsList.size(); j++)
				{
					m_lineList.push_back(Vec4f(ptsList.at(i).x(), ptsList.at(i).y(), ptsList.at(j).x(), ptsList.at(j).y()));
					m_distance.push_back(sqrt(pow((ptsList.at(i).x() - ptsList.at(j).x()), 2) + pow((ptsList.at(i).y() - ptsList.at(j).y()), 2)));
				}
			}
			int max_idx = 0;
			double max_val = 0;
			for (int i = 0; i < m_distance.size() - 1; i++)
			{
				if (m_distance.at(i) > max_val)
				{
					max_val = m_distance.at(i);
					max_idx = i;
				}
			}
			return m_lineList.at(max_idx);

		}
	}
}

Vec4f CLineSegmentDetector::DirLineInRectConvert(Vec4f dirLine, Rect rect)
{
	if (dirLine[0] == 0)
	{
		return Vec4f(dirLine[2], rect.y, dirLine[2], rect.y + rect.height);
	}
	else
	{
		QPointF p1 = QPointF(rect.x, (dirLine[1] * rect.x + dirLine[0] * dirLine[3] - dirLine[1] * dirLine[2]) / dirLine[0]);
		QPointF p2 = QPointF(rect.x + rect.width, (dirLine[1] * (rect.x + rect.width) + dirLine[0] * dirLine[3] - dirLine[1] * dirLine[2]) / dirLine[0]);
		QPointF p3 = QPointF((dirLine[0] * rect.y - dirLine[0] * dirLine[3] + dirLine[1] * dirLine[2]) / dirLine[1], rect.y);
		QPointF p4 = QPointF((dirLine[0] * (rect.y + rect.height) - dirLine[0] * dirLine[3] + dirLine[1] * dirLine[2]) / dirLine[1], rect.y + rect.height);

		// *** origin code ***
		//vector<QPointF> ptsList;
		//if ((p1.y() >= rect.y) && (p1.y() <= rect.y + rect.height))
		//{
		//	ptsList.push_back(p1);
		//}
		//if ((p2.y() >= rect.y) && (p2.y() <= rect.y + rect.height))
		//{
		//	ptsList.push_back(p2);
		//}
		//if ((p3.x() >= rect.x) && (p3.x() <= rect.x + rect.width))
		//{
		//	ptsList.push_back(p3);
		//}
		//if ((p4.x() >= rect.x) && (p4.x() <= rect.x + rect.width))
		//{
		//	ptsList.push_back(p4);
		//}
		// *** origin code end ***

		// *** new code ***
		// 增加顶点检测
		bool push_p1_flag = false;
		bool push_p2_flag = false;
		bool push_p3_flag = false;
		bool push_p4_flag = false;

		vector<QPointF> ptsList;
		double dist_threshold = 2;
		if ((p1.y() >= rect.y) && (p1.y() <= rect.y + rect.height))
		{
			//ptsList.push_back(p1);
			//push_p1_flag = true;

			if (!isInPointSet(ptsList, p1, dist_threshold)) {
				ptsList.push_back(p1);
				push_p1_flag = true;
			}
		}
		if ((p2.y() >= rect.y) && (p2.y() <= rect.y + rect.height))
		{
			//ptsList.push_back(p2);
			//push_p2_flag = true;

			if (!isInPointSet(ptsList, p2, dist_threshold)) {
				ptsList.push_back(p2);
				push_p2_flag = true;
			}
		}
		if ((p3.x() >= rect.x) && (p3.x() <= rect.x + rect.width))
		{
			//ptsList.push_back(p3);
			//push_p3_flag = true;

			if (!isInPointSet(ptsList, p3, dist_threshold)) {
				ptsList.push_back(p3);
				push_p3_flag = true;
			}
		}
		if ((p4.x() >= rect.x) && (p4.x() <= rect.x + rect.width))
		{
			//ptsList.push_back(p4);
			//push_p4_flag = true;

			if (!isInPointSet(ptsList, p4, dist_threshold)) {
				ptsList.push_back(p4);
				push_p4_flag = true;
			}
		}

		if (ptsList.size() < 2) {
			int x_start = rect.x;
			int y_start = rect.y;
			int x_end = x_start + rect.width;
			int y_end = y_start + rect.height;

			float dist_thres = 2;

			if (!push_p1_flag) {
				if (abs(p1.y() - y_start) < dist_thres || abs(p1.y() - y_end) < dist_thres) {
					//ptsList.push_back(p1);
					//push_p1_flag = true;

					if (!isInPointSet(ptsList, p1, dist_thres)) {
						ptsList.push_back(p1);
						push_p1_flag = true;
					}
				}
			}
			if (!push_p2_flag) {
				if (abs(p2.y() - y_start) < dist_thres || abs(p2.y() - y_end) < dist_thres) {
					//ptsList.push_back(p2);
					//push_p2_flag = true;

					if (!isInPointSet(ptsList, p2, dist_thres)) {
						ptsList.push_back(p2);
						push_p2_flag = true;
					}
				}
			}
			if (!push_p3_flag) {
				if (abs(p3.x() - x_start) < dist_thres || abs(p3.y() - x_end) < dist_thres) {
					//ptsList.push_back(p3);
					//push_p3_flag = true;

					if (!isInPointSet(ptsList, p3, dist_thres)) {
						ptsList.push_back(p3);
						push_p3_flag = true;
					}
				}
			}
			if (!push_p4_flag) {
				if (abs(p4.x() - x_start) < dist_thres || abs(p4.y() - x_end) < dist_thres) {
					//ptsList.push_back(p4);
					//push_p4_flag = true;

					if (!isInPointSet(ptsList, p4, dist_thres)) {
						ptsList.push_back(p4);
						push_p4_flag = true;
					}
				}
			}
		}
		// *** new code end ***

		if (ptsList.size() < 2)
		{
			return Vec4f(-1, -1, -1, -1);
		}
		else if (ptsList.size() == 2)
		{
			return Vec4f(ptsList.at(0).x(), ptsList.at(0).y(), ptsList.at(1).x(), ptsList.at(1).y());
		}
		else
		{
			vector<double> m_distance;
			vector<Vec4f> m_lineList;
			for (int i = 0; i < ptsList.size() - 1; i++)
			{
				for (int j = i + 1; j < ptsList.size(); j++)
				{
					m_lineList.push_back(Vec4f(ptsList.at(i).x(), ptsList.at(i).y(), ptsList.at(j).x(), ptsList.at(j).y()));
					m_distance.push_back(sqrt(pow((ptsList.at(i).x() - ptsList.at(j).x()), 2) + pow((ptsList.at(i).y() - ptsList.at(j).y()), 2)));
				}
			}
			int max_idx = 0;
			double max_val = 0;
			for (int i = 0; i < m_distance.size() - 1; i++)
			{
				if (m_distance.at(i) > max_val)
				{
					max_val = m_distance.at(i);
					max_idx = i;
				}
			}
			return m_lineList.at(max_idx);

		}
	}
}

Point2d CLineSegmentDetector::shortestVectorPointDistance2Line(vector<Point2d> points, Vec4f line)
{
	//计算点集中的点到直线的距离最小的点
	Point2d res = Point2d(-1, -1);
	if (line != Vec4f(0, 0, 0, 0))
	{
		double m_minDis = 9999999;
		for (auto &t : points)
		{
			double m_dis = DisOfPointToLine_Dir(line, t);
			if (m_dis < m_minDis)
			{
				m_minDis = m_dis;
				res = t;
			}
			else
			{
				break;
			}
		}
	}
	return res;
}

bool CLineSegmentDetector::isInPointSet(vector<Point2d>& points, Point2d & point, double dist_thres)
{
	bool res = false;

	try {
		for (auto &p : points) {
			if (isSamePoint(point, p, dist_thres)) {
				res = true;
				break;
			}
		}
	}
	catch (exception &e) {
		res = false;
	}

	return res;
}

bool CLineSegmentDetector::isInPointSet(vector<QPointF>& points, QPointF & point, double dist_thres)
{
	bool res = false;

	try {
		for (auto &p : points) {
			if (isSamePoint(point, p, dist_thres)) {
				res = true;
				break;
			}
		}
	}
	catch (exception &e) {
		res = false;
	}

	return res;
}

bool CLineSegmentDetector::isSamePoint(Point2d & point1, Point2d & point2, double dist_thres)
{
	bool res = false;

	try {
		double dist = sqrt(pow(point1.x - point2.x, 2) + pow(point1.y - point2.y, 2));
		if (dist <= dist_thres) {
			res = true;
		}
		else {
			res = false;
		}
	}
	catch (exception &e) {
		res = false;
	}

	return res;
}

bool CLineSegmentDetector::isSamePoint(QPointF & point1, QPointF & point2, double dist_thres)
{
	bool res = false;

	try {
		double dist = sqrt(pow(point1.x() - point2.x(), 2) + pow(point1.y() - point2.y(), 2));
		if (dist < dist_thres) {
			res = true;
		}
		else {
			res = false;
		}
	}
	catch (exception &e) {
		res = false;
	}

	return res;
}

