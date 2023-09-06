#include "bspline.h"

using namespace cv;
using namespace std;

BSpline::BSpline()
{
}

BSpline::~BSpline()
{

}

int BSpline::FindEdgePoint(Mat DstMat, Point offset, AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION direction, AlgorithmOperatorCommonEnum::EDGEFINDER_MODE EdgeMode, int _step, int _min_grad, int _edge_value, double _accuracy, AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE _search_style, int _test_step /*= 1*/, int m_shield_start /*= 0*/, int m_shield_end /*= 0*/)
{
	m_step = _step;
	m_min_grad = _min_grad;
	m_edge_value = _edge_value;
	m_iteration_accuracy = _accuracy;
	bool edge_mode;
	if (EdgeMode == AlgorithmOperatorCommonEnum::EDGEFINDER_MODE::enumBLACK2WHITE)
		edge_mode = true;
	else
		edge_mode = false;

	int rslt = -3;
	switch (direction)
	{
	case AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION::enumLEFT2RIGHT:
		try
		{
			rslt = left2rightEdge(DstMat, offset, edge_mode, _search_style, _test_step, m_shield_start, m_shield_end);
		}
		catch (const std::exception& ex)
		{
			//qDebug() << ex.what();
			break;
		}
		break;
	case AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION::enumRIGHT2LEFT:
		try
		{
			rslt = right2leftEdge(DstMat, offset, edge_mode, _search_style, _test_step, m_shield_start, m_shield_end);
		}
		catch (const std::exception& ex)
		{
			//qDebug() << ex.what();
			break;
		}
		break;
	case AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION::enumTOP2BOTTOM:
		try
		{
			rslt = top2bottomEdge(DstMat, offset, edge_mode, _search_style, _test_step, m_shield_start, m_shield_end);
		}
		catch (const std::exception& ex)
		{
			//qDebug() << ex.what();
			break;
		}
		break;
	case AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION::enumBOTTOM2TOP:
		try
		{
			rslt = bottom2topEdge(DstMat, offset, edge_mode, _search_style, _test_step, m_shield_start, m_shield_end);
		}
		catch (const std::exception& ex)
		{
			//qDebug() << ex.what();
			break;
		}
		break;
	default:
		rslt = -3;
		break;
	}
	return rslt;
}

int BSpline::left2rightEdge(Mat DstMat, Point offset, bool is_black2white, AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE _search_style, int test_step, int m_shield_start, int m_shield_end)
{
	if (DstMat.cols <= m_step)
	{
		qDebug("image too narrow");
		return -1;
	}
	else
	{
		EdgePoint.clear();

		QPolygonF inputArr;
// 		for (int i = 0; i < m_step; i++)
// 		{
// 			inputArr << QPointF(i, 0);
// 		}

		vector<Point2d> fit_point;
		int idx = -1;
		int max = 0;
		int grad = 0;
		bool findmaxflag = false;
		bool secGrad = false;

		bool m_use_shield = (m_shield_start + m_shield_end > 0);
		int m_min_shield = (m_shield_start > m_shield_end) ? m_shield_end : m_shield_start;
		int m_max_shiled = (m_shield_start > m_shield_end) ? m_shield_start : m_shield_end;
		if (m_min_shield > DstMat.rows || m_max_shiled > DstMat.rows || (m_max_shiled - m_min_shield) == 0)
		{
			m_use_shield = false;
		}

		for (int i = 0; i < DstMat.rows; i = i + test_step)
		{
			fit_point.clear();
			idx = -1;
			max = 0;
			grad = 0;
			findmaxflag = false;
			if (m_use_shield)
			{
				if (i > m_min_shield&&i < m_max_shiled)
				{
					continue;
				}
			}
			for (int j = 0; j < DstMat.cols - 1; j++)
			{
				if (is_black2white)
				{
					int gray_1 = DstMat.at<uchar>(i, j);
					int gray_2 = DstMat.at<uchar>(i, j + 1);
					grad = gray_1 - gray_2;
					if (grad < -m_min_grad && gray_1 <= m_edge_value && gray_2 >= m_edge_value)
					{
						fit_point.push_back(Point2d(j, grad));
						if (_search_style == AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE::enumFRISTGRAD)
							break;
					}
				}
				else
				{
					int gray_1 = DstMat.at<uchar>(i, j);
					int gray_2 = DstMat.at<uchar>(i, j + 1);
					grad = gray_1 - gray_2;
					if (grad > m_min_grad   &&  gray_1 >= m_edge_value && gray_2 <= m_edge_value)
					{
						fit_point.push_back(Point2d(j, grad));
						if (_search_style == AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE::enumFRISTGRAD)
							break;
					}
				}
			}
			m_start = getmaxgrad(fit_point, is_black2white);
			m_end = m_start + 1;

			if (fit_point.size() > 0)//填充fmap[]值，在最大索引附近找m_step长度的值
			{
				idx = m_start;
				int halfStep = m_step / 2;
				if (idx < halfStep)//最大索引靠左情况
				{
					for (int j = 0; j < m_step; j++)
					{
// 						fmap[j] = j;
// 						fmap[j + m_step] = DstMat.at<uchar>(i, j);
// 						inputArr.at(j).x = j;
// 						inputArr.at(j).y = DstMat.at<uchar>(i, j);
						inputArr << QPointF(j, DstMat.at<uchar>(i, j));
					}
				}
				else if (idx > DstMat.cols - halfStep - 1)//最大索引靠右情况
				{
					for (int j = 0; j < m_step; j++)
					{
// 						fmap[j] = DstMat.cols - m_step + j;
// 						fmap[j + m_step] = DstMat.at<uchar>(i, DstMat.cols - m_step + j);
// 						inputArr.at(j).x = DstMat.cols - m_step + j;
// 						inputArr.at(j).y = DstMat.at<uchar>(i, DstMat.cols - m_step + j);
						inputArr << QPointF(DstMat.cols - m_step + j, DstMat.at<uchar>(i, DstMat.cols - m_step + j));
					}
				}
				else//最大索引在中间
				{
					for (int j = 0; j < m_step; j++)
					{
// 						fmap[j] = idx - halfStep + j;
// 						fmap[j + m_step] = DstMat.at<uchar>(i, idx - halfStep + j);
// 						inputArr.at(j).x = idx - halfStep + j;
// 						inputArr.at(j).y = DstMat.at<uchar>(i, idx - halfStep + j);
						inputArr << QPointF(idx - halfStep + j, DstMat.at<uchar>(i, idx - halfStep + j));

					}
				}
				double xval = -1;
				if (bSpline(inputArr, m_edge_value, xval) != 0) {
					continue;
				}
				EdgePoint.push_back(Point2d(offset.x + xval, offset.y + i));
			}
		}
		return 0;
	}
}

int BSpline::right2leftEdge(Mat DstMat, Point offset, bool is_black2white, AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE _search_style, int test_step, int m_shield_start, int m_shield_end)
{
	if (DstMat.cols <= m_step)
	{
		qDebug("image too narrow");
		return -1;
	}
	else
	{
		EdgePoint.clear();

		QPolygonF inputArr;
// 		for (int i = 0; i < m_step; i++)
// 		{
// 			inputArr << QPointF(i, 0);
// 		}

		vector<Point2d> fit_point;
		int idx = -1;
		int max = 0;
		int grad = 0;
		bool findmaxflag = false;
		bool secGrad = false;
		bool m_use_shield = (m_shield_start + m_shield_end > 0);
		int m_min_shield = (m_shield_start > m_shield_end) ? m_shield_end : m_shield_start;
		int m_max_shiled = (m_shield_start > m_shield_end) ? m_shield_start : m_shield_end;
		if (m_min_shield > DstMat.rows || m_max_shiled > DstMat.rows || (m_max_shiled - m_min_shield) == 0)
		{
			m_use_shield = false;
		}
		for (int i = 0; i < DstMat.rows; i = i + test_step)
		{
			fit_point.clear();
			idx = -1;
			max = 0;
			grad = 0;
			findmaxflag = false;
			if (m_use_shield)
			{
				if (i > m_min_shield&&i < m_max_shiled)
				{
					continue;
				}
			}
			for (int j = DstMat.cols - 1; j > 0; j--)
			{
				if (is_black2white)
				{
					int gray_1 = DstMat.at<uchar>(i, j);
					int gray_2 = DstMat.at<uchar>(i, j - 1);
					grad = gray_1 - gray_2;
					if (grad < -m_min_grad && gray_1 <= m_edge_value && gray_2 >= m_edge_value)
					{
						fit_point.push_back(Point2d(j, grad));
						if (_search_style == AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE::enumFRISTGRAD)
							break;
					}
				}
				else
				{
					int gray_1 = DstMat.at<uchar>(i, j);
					int gray_2 = DstMat.at<uchar>(i, j - 1);
					grad = gray_1 - gray_2;
					if (grad > m_min_grad   &&  gray_1 >= m_edge_value && gray_2 <= m_edge_value)
					{
						fit_point.push_back(Point2d(j, grad));
						if (_search_style == AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE::enumFRISTGRAD)
						{
							break;
						}
					}
				}
			}
			m_start = getmaxgrad(fit_point, is_black2white);
			m_end = m_start - 1;
			if (fit_point.size() > 0)
			{
				idx = m_start;
				int halfStep = m_step / 2;
				if (idx < halfStep)
				{
					for (int j = 0; j < m_step; j++)
					{
// 						fmap[j] = j;
// 						fmap[j + m_step] = DstMat.at<uchar>(i, j);
// 						inputArr.at(j).x = j;
// 						inputArr.at(j).y = DstMat.at<uchar>(i, j);
						inputArr << QPointF(j, DstMat.at<uchar>(i, j));
					}
				}
				else if (idx > DstMat.cols - halfStep - 1)
				{
					for (int j = 0; j < m_step; j++)
					{
// 						fmap[j] = DstMat.cols - m_step + j;
// 						fmap[j + m_step] = DstMat.at<uchar>(i, DstMat.cols - m_step + j);
// 						inputArr.at(j).x = DstMat.cols - m_step + j;
// 						inputArr.at(j).y = DstMat.at<uchar>(i, DstMat.cols - m_step + j);
						inputArr << QPointF(DstMat.cols - m_step + j, DstMat.at<uchar>(i, DstMat.cols - m_step + j));
					}
				}
				else
				{
					for (int j = 0; j < m_step; j++)
					{
// 						fmap[j] = idx - halfStep + j;
// 						fmap[j + m_step] = DstMat.at<uchar>(i, idx - halfStep + j);
// 						inputArr.at(j).x = idx - halfStep + j;
// 						inputArr.at(j).y = DstMat.at<uchar>(i, idx - halfStep + j);
						inputArr << QPointF(idx - halfStep + j, DstMat.at<uchar>(i, idx - halfStep + j));
					}
				}
				double xval = -1;

				if (bSpline(inputArr, m_edge_value, xval, true) != 0) {
					continue;
				}

				EdgePoint.push_back(Point2d(offset.x + xval, offset.y + i));
			}
		}
		return 0;
	}
}

int BSpline::top2bottomEdge(Mat DstMat, Point offset, bool is_black2white, AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE _search_style, int test_step, int m_shield_start, int m_shield_end)
{
	if (DstMat.rows <= m_step)
	{
		qDebug("image too narrow");
		return -1;
	}
	else
	{
		EdgePoint.clear();

		QPolygonF inputArr;
// 		for (int i = 0; i < m_step; i++)
// 		{
// 			inputArr << QPointF(i, 0);
// 		}

		vector<Point2d> fit_point;
		int idx = -1;
		int max = 0;
		int grad = 0;
		bool findmaxflag = false;
		bool secGrad = false;
		bool m_use_shield = (m_shield_start + m_shield_end > 0);
		int m_min_shield = (m_shield_start > m_shield_end) ? m_shield_end : m_shield_start;
		int m_max_shiled = (m_shield_start > m_shield_end) ? m_shield_start : m_shield_end;
		if (m_min_shield > DstMat.cols || m_max_shiled > DstMat.cols || (m_max_shiled - m_min_shield) == 0)
		{
			m_use_shield = false;
		}
		for (int i = 0; i < DstMat.cols; i = i + test_step)
		{
			fit_point.clear();
			idx = -1;
			max = 0;
			grad = 0;
			findmaxflag = false;
			if (m_use_shield)
			{
				if (i > m_min_shield&&i < m_max_shiled)
				{
					continue;
				}
			}
			for (int j = 0; j < DstMat.rows - 1; j++)
			{
				int gray_1 = DstMat.at<uchar>(j, i);
				int gray_2 = DstMat.at<uchar>(j + 1, i);
				if (is_black2white)
				{
					grad = gray_1 - gray_2;
					if (grad < -m_min_grad && gray_1 <= m_edge_value && gray_2 >= m_edge_value)
					{
						fit_point.push_back(Point2d(j, grad));
						if (_search_style == AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE::enumFRISTGRAD)
							break;
					}
				}
				else
				{
					grad = gray_1 - gray_2;
					if (grad > m_min_grad   &&  gray_1 >= m_edge_value && gray_2 <= m_edge_value)
					{
						fit_point.push_back(Point2d(j, grad));
						if (_search_style == AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE::enumFRISTGRAD)
							break;
					}
				}
			}
			m_start = getmaxgrad(fit_point, is_black2white);
			m_end = m_start + 1;
			if (fit_point.size() > 0)
			{
				idx = m_start;
				int halfStep = m_step / 2;
				if (idx < halfStep)
				{
					for (int j = 0; j < m_step; j++)
					{
// 						fmap[j] = j;
// 						fmap[j + m_step] = DstMat.at<uchar>(j, i);
// 						inputArr.at(j).x = j;
// 						inputArr.at(j).y = DstMat.at<uchar>(j, i);
						inputArr << QPointF(j, DstMat.at<uchar>(j, i));
					}
				}
				else if (idx > DstMat.rows - halfStep - 1)
				{
					for (int j = 0; j < m_step; j++)
					{
// 						fmap[j] = DstMat.rows - m_step + j;
// 						fmap[j + m_step] = DstMat.at<uchar>(DstMat.rows - m_step + j, i);
// 						inputArr.at(j).x = DstMat.rows - m_step + j;
// 						inputArr.at(j).y = DstMat.at<uchar>(DstMat.rows - m_step + j, i);
						inputArr << QPointF(DstMat.rows - m_step + j, DstMat.at<uchar>(DstMat.rows - m_step + j, i));
					}
				}
				else
				{
					for (int j = 0; j < m_step; j++)
					{
// 						fmap[j] = idx - halfStep + j;
// 						fmap[j + m_step] = DstMat.at<uchar>(idx - halfStep + j, i);
// 						inputArr.at(j).x = idx - halfStep + j;
// 						inputArr.at(j).y = DstMat.at<uchar>(idx - halfStep + j, i);
						inputArr << QPointF(idx - halfStep + j, DstMat.at<uchar>(idx - halfStep + j, i));
					}
				}

				double yval = -1;
				if (bSpline(inputArr, m_edge_value, yval) != 0)
				{
					continue;
				}
				EdgePoint.push_back(Point2d(offset.x + i, offset.y + yval));
			}
		}
		return 0;
	}
}

int BSpline::bottom2topEdge(Mat DstMat, Point offset, bool is_black2white, AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE _search_style, int test_step, int m_shield_start, int m_shield_end)
{
	if (DstMat.rows <= m_step)
	{
		qDebug("image too narrow");
		return -1;
	}
	else
	{
		EdgePoint.clear();

		QPolygonF inputArr;
// 		for (int i = 0; i < m_step; i++)
// 		{
// 			inputArr << QPointF(i, 0);
// 		}

		vector<Point2d> fit_point;
		int idx = -1;
		int max = 0;
		int grad = 0;
		bool findmaxflag = false;
		bool secGrad = false;
		bool m_use_shield = (m_shield_start + m_shield_end > 0);
		int m_min_shield = (m_shield_start > m_shield_end) ? m_shield_end : m_shield_start;
		int m_max_shiled = (m_shield_start > m_shield_end) ? m_shield_start : m_shield_end;
		if (m_min_shield > DstMat.cols || m_max_shiled > DstMat.cols || (m_max_shiled - m_min_shield) == 0)
		{
			m_use_shield = false;
		}
		for (int i = 0; i < DstMat.cols; i = i + test_step)
		{
			fit_point.clear();
			idx = -1;
			max = 0;
			grad = 0;
			findmaxflag = false;
			if (m_use_shield)
			{
				if (i > m_min_shield&&i < m_max_shiled)
				{
					continue;
				}
			}
			for (int j = DstMat.rows - 1; j > 0; j--)
			{
				int gray_1 = DstMat.at<uchar>(j, i);
				int gray_2 = DstMat.at<uchar>(j - 1, i);
				if (is_black2white)
				{
					grad = gray_1 - gray_2;
					if (grad < -m_min_grad && gray_1 <= m_edge_value && gray_2 >= m_edge_value)
					{
						fit_point.push_back(Point2d(j, grad));
						if (_search_style == AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE::enumFRISTGRAD)
							break;
					}
				}
				else
				{
					grad = gray_1 - gray_2;
					if (grad > m_min_grad   &&  gray_1 >= m_edge_value && gray_2 <= m_edge_value)
					{
						fit_point.push_back(Point2d(j, grad));
						if (_search_style == AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE::enumFRISTGRAD)
							break;
					}
				}
			}
			m_start = getmaxgrad(fit_point, is_black2white);
			m_end = m_start - 1;
			if (fit_point.size() > 0)
			{
				idx = m_start;
				int halfStep = m_step / 2;
				if (idx < halfStep)
				{
					for (int j = 0; j < m_step; j++)
					{
// 						fmap[j] = j;
// 						fmap[j + m_step] = DstMat.at<uchar>(j, i);
// 						inputArr.at(j).x = j;
// 						inputArr.at(j).y = DstMat.at<uchar>(j, i);
						inputArr << QPointF(j, DstMat.at<uchar>(j, i));
					}
				}
				else if (idx > DstMat.rows - halfStep - 1)
				{
					for (int j = 0; j < m_step; j++)
					{
// 						fmap[j] = DstMat.rows - m_step + j;
// 						fmap[j + m_step] = DstMat.at<uchar>(DstMat.rows - m_step + j, i);
// 						inputArr.at(j).x = DstMat.rows - m_step + j;
// 						inputArr.at(j).y = DstMat.at<uchar>(DstMat.rows - m_step + j, i);
						inputArr << QPointF(DstMat.rows - m_step + j, DstMat.at<uchar>(DstMat.rows - m_step + j, i));
					}
				}
				else
				{
					for (int j = 0; j < m_step; j++)
					{
// 						fmap[j] = idx - halfStep + j;
// 						fmap[j + m_step] = DstMat.at<uchar>(idx - halfStep + j, i);
// 						inputArr.at(j).x = idx - halfStep + j;
// 						inputArr.at(j).y = DstMat.at<uchar>(idx - halfStep + j, i);
						inputArr << QPointF(idx - halfStep + j, DstMat.at<uchar>(idx - halfStep + j, i));
					}
				}

				double yval = -1;
				if (bSpline(inputArr, m_edge_value, yval, true) != 0)
				{
					continue;
				}
				EdgePoint.push_back(Point2d(offset.x + i, offset.y + yval));
			}
		}
		return 0;
	}
}

int BSpline::bSpline(QPolygonF &inputArr, qreal edgeValue, qreal & xValue, bool is_transpose)
{
	QPolygonF pts;
	QPointF bpt;
	int ptCnt = inputArr.count();
	qreal maxVal = -1;
	qreal minVal = -1;
	int idxMax = -1;
	int idxMin = -1;
	for (int i = 0; i < ptCnt; i++) {
		if (maxVal < 0 || inputArr.at(i).y() > maxVal) {
			idxMax = i;
			maxVal = inputArr.at(i).y();
		}
		if (minVal < 0 || inputArr.at(i).y() < minVal) {
			idxMin = i;
			minVal = inputArr.at(i).y();
		}
	}
	if (inputArr.at(idxMax).y() < edgeValue || inputArr.at(idxMin).y() > edgeValue) {
		return 2;
	}
	else if (inputArr.at(idxMax).y() > edgeValue) {
		if (inputArr.at(idxMin).y() < edgeValue) {
			;
		}
		else {
			xValue = inputArr.at(idxMin).x();
			return 0;
		}
	}
	else {
		xValue = inputArr.at(idxMax).x();
		return 0;
	}

	pts.clear();
	if (idxMax < idxMin) {
		int tmp = idxMin;
		idxMin = idxMax;
		idxMax = tmp;
	}
	for (int n = idxMin; n <= idxMax; n++)
		pts << inputArr.at(n);

	//class BSpline * spl = nullptr;
	int cnt = idxMax - idxMin;
	int ord = 1;
	if (cnt > 3)
		ord = 2;
	else if (cnt == 1) {
		pts.insert(1, QPointF(idxMin + 0.5, (inputArr.at(idxMin).y() + inputArr.at(idxMax).y())*0.5));
	}
	else if (cnt < 1) {
		return 6;
	}
	int ret;
	ret = quasiUniform(ord, pts);

	if (ret) {
		return 3;
	}

	qreal u;
	if (InvPolate(pts, edgeValue, u, is_transpose) != 0) {
		return 4;
	}
	if (interpolate(u, bpt) != 0) {
		return 5;
	}

	xValue = bpt.x();

	return 0;
}

int BSpline::InvPolate(QPolygonF & pts, qreal y, qreal &u, bool is_transpose)
{
	double ulo, ylo, uhi, yhi;
	int sz = knots.size();
	if (pts.front().y() <= y && pts.back().y() >= y) {
		ulo = knots.at(order);
		ylo = pts.front().y();
		uhi = knots.at(sz - 1 - order);
		yhi = pts.back().y();
	}
	else if (pts.front().y() >= y && pts.back().y() <= y) {
		uhi = knots.at(order);
		yhi = pts.front().y();
		ulo = knots.at(sz - 1 - order);
		ylo = pts.back().y();
	}
	else {
		return 1;
	}

	while (true) {
		if (fabs(ylo - y) < m_iteration_accuracy) {
			u = ulo;
			break;
		}
		else if (fabs(yhi - y) < m_iteration_accuracy) {
			u = uhi;
			break;
		}
		else {
			qreal utmp = (ulo + uhi)*0.5;
			QPointF btmp;
			if (interpolate(utmp, btmp) != 0) {
				break;
			}
			if (btmp.y() < y) {
				ulo = utmp;
				ylo = btmp.y();
			}
			else {
				uhi = utmp;
				yhi = btmp.y();
			}
		}
	}
	return 0;
}

int BSpline::scalarMultiply(const RealVec & vec, qreal scale, RealVec & res)
{
	res.clear();
	for (int i = 0; i < vec.size(); i++) {
		res << vec.at(i) * scale;
	}
	return 0;
}

int BSpline::addRealVec(const RealVec & vec1, const RealVec & vec2, RealVec & res)
{
	const RealVec & vecL = vec1.size() > vec2.size() ? vec1 : vec2;
	const RealVec & vecS = vec1.size() > vec2.size() ? vec2 : vec1;
	res = vecL;
	for (int i = 0; i < vecS.size(); i++) {
		res[i] += vecS.at(i);
	}
	return 0;
}

int BSpline::addFunc(const Bfunc & f1, Bfunc & f2, Bfunc & res)
{
	const Bfunc & fL = f1.size() > f2.size() ? f1 : f2;
	const Bfunc & fS = f1.size() > f2.size() ? f2 : f1;
	res = fL;
	for (int i = 0; i < fS.size(); i++) {
		addRealVec(fL.at(i), fS.at(i), res[i]);
	}
	return 0;
}

int BSpline::polynomial(const RealVec & coeff, qreal t, qreal & val)
{
	int sz = coeff.size();
	val = 0;
	for (int i = sz - 1; i >= 0; i--) {
		val = coeff.at(i) + val * t;
	}
	return 0;
}

int BSpline::inputKnots(RealVec & knots)
{
	//std::vector<qreal> tmp = knots.toStdVector();
	//std::sort(tmp.begin(), tmp.end());
	//this->knots = RealVec::fromStdVector(tmp);
	std::sort(knots.begin(), knots.end()); //这里在移植的时候进行了更改，之前转成标准库vector进行排序再重新赋值，觉得没必要直接排序原来的
	for (int i = 1; i < this->knots.size(); i++) {
		if (knots.at(i) - knots.at(i - 1) <= 2 * m_iteration_accuracy)
			knots[i] = knots[i - 1];
	}
	return 0;
}

int BSpline::calcCoeffs()
{
	if (order < 1)
		return 1;
	int kCnts = knots.size();
	if (kCnts < order + 1)
		return 2;
	RealVec vec, vec0, vec1, vec2;
	vec << 1;
	Bfunc bfunc;
	bfunc << vec;
	BfArray bfarr;
	for (int i = 0; i < kCnts - 1; i++) {
		bfarr << bfunc;
	}
	bfHierarchy << bfarr;
	BfArray & refBfArr = bfarr;
	for (int k = 1; k <= order; k++) {
		BfArray resArr;
		for (int i = 0; i < refBfArr.size() - 1; i++) {
			const Bfunc & func1 = refBfArr.at(i);
			const Bfunc & func2 = refBfArr.at(i + 1);
			Bfunc resFunc1, resFunc2, resFunc;
			for (int j = 0; j < func1.size(); j++) {
				vec0 = func1.at(j);
				qreal div = knots.at(i + k) - knots.at(i);
				if (div < m_iteration_accuracy)
					div = 1;
				scalarMultiply(vec0, -knots.at(i) / div, vec1);
				scalarMultiply(vec0, 1 / div, vec2);
				vec2.push_front(0);
				addRealVec(vec1, vec2, vec);
				resFunc1 << vec;
			}
			for (int j = 0; j < func2.size(); j++) {
				vec0 = func2.at(j);
				qreal div = knots.at(i + k + 1) - knots.at(i + 1);
				if (div < m_iteration_accuracy)
					div = 1;
				scalarMultiply(vec0, knots.at(i + k + 1) / div, vec1);
				scalarMultiply(vec0, -1 / div, vec2);
				vec2.push_front(0);
				addRealVec(vec1, vec2, vec);
				resFunc2 << vec;
			}
			RealVec rv = { 0 };
			resFunc2.push_front(rv);
			addFunc(resFunc1, resFunc2, resFunc);
			resArr << resFunc;
		}
		bfHierarchy << resArr;
		refBfArr = bfHierarchy.back();
	}
	return 0;
}

int BSpline::interpolate(qreal u, QPointF & pt)
{
	if (bfHierarchy.size() < 2)
		return 1;
	int sz = knots.size();
	if (controls.size() < sz - 1 - order)
		return 2;
	if (u < knots.front() || u > knots.back())
		return 3;
	int idx = sz - 2;
	for (int i = 0; i < sz; i++) {
		if (u >= knots.at(i))
			continue;
		else {
			idx = i - 1;
			break;
		}
	}
	BfArray & arr = bfHierarchy.back();
	int idxLo = qMax(idx - order, 0);
	int offset = idx;
	idx = qMin(idx, sz - order - 2);
	offset -= idx;
	pt = QPointF(0, 0);
	qreal val;
	qreal res = 0;
	for (int i = idx; i >= idxLo; i--) {
		polynomial(arr.at(i).at(offset), u, val);
		res += val;
		pt += val * controls.at(i);
		offset++;
	}
	return 0;
}

int BSpline::quasiUniform(const int ord, const QPolygonF &ctrls)
{
	order = ord;
	if (ctrls.size() < 1 + ord * 2) {
		return 12;
	}
	controls = ctrls;
	int segCnt = ctrls.size() - ord;
	RealVec rv;
	for (int i = 0; i < ord; i++) {
		rv << 0;
	}
	for (int i = 0; i <= segCnt; i++) {
		rv << i;
	}
	for (int i = 0; i < ord; i++) {
		rv << segCnt;
	}
	inputKnots(rv);
	return calcCoeffs();
}

int BSpline::getmaxgrad(vector<Point2d> pts, bool is_black2white)
{
	int pos = 0;
	double max_grad = 0;

	for (int i = 0; i < pts.size(); i++)
	{
		auto t = pts.at(i);
		auto y = abs(t.y);

		if (y > max_grad)
		{
			pos = t.x;
			max_grad = y;
		}

	}

	return pos;
}

qreal BSpline::m_iteration_accuracy = 0.05;

int BSpline::m_start;

int BSpline::m_end;

