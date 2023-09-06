#include "cubicspline.h"

using namespace cv;
using namespace std;

int CubicSpline::FindEdgePoint(const cv::Mat& DstMat, const cv::Point& offset, AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION direction, AlgorithmOperatorCommonEnum::EDGEFINDER_MODE EdgeMode, int _step, int _min_grad, int _edge_value, double _accuracy, AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE _search_style, int _test_step, int m_shield_start, int m_shield_end)
{
	m_step = _step;
	m_edge_value = _edge_value;
	m_iteration_accuracy = _accuracy;
	bool edge_mode = EdgeMode == AlgorithmOperatorCommonEnum::EDGEFINDER_MODE::enumBLACK2WHITE;
	m_min_grad = edge_mode ? -_min_grad : _min_grad;
	try
	{
		switch (direction)
		{
		case AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION::enumLEFT2RIGHT:
			return left2rightEdge2(DstMat, offset, edge_mode, _search_style, _test_step, m_shield_start, m_shield_end);
		case AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION::enumRIGHT2LEFT:
			return right2leftEdge2(DstMat, offset, edge_mode, _search_style, _test_step, m_shield_start, m_shield_end);
		case AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION::enumTOP2BOTTOM:
			return top2bottomEdge2(DstMat, offset, edge_mode, _search_style, _test_step, m_shield_start, m_shield_end);
		case AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION::enumBOTTOM2TOP:
			return bottom2topEdge2(DstMat, offset, edge_mode, _search_style, _test_step, m_shield_start, m_shield_end);
		}
	}
	catch (const std::exception& ex)
	{
		//qDebug() << ex.what();
	}
	return -3;
}

cv::Mat CubicSpline::drawEdge(cv::Mat SrcMat)
{
	Mat ColorMat;
	if (SrcMat.type() == CV_8UC1)
		cvtColor(SrcMat, ColorMat, cv::ColorConversionCodes::COLOR_GRAY2BGR);
	else
		ColorMat = SrcMat;

	for (int i = 0; i < EdgePoint.size(); i++)
	{
		ColorMat.at<Vec3b>(round(EdgePoint.at(i).y), round(EdgePoint.at(i).x))[0] = 0;
		ColorMat.at<Vec3b>(round(EdgePoint.at(i).y), round(EdgePoint.at(i).x))[1] = 0;
		ColorMat.at<Vec3b>(round(EdgePoint.at(i).y), round(EdgePoint.at(i).x))[2] = 255;
	}
	return ColorMat;
}

int CubicSpline::left2rightEdge2(const cv::Mat& DstMat, const cv::Point& offset, bool is_black2white, AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE _search_style, int test_step, int m_shield_start, int m_shield_end)
{
	if (m_step <= 2) {
		//qDebug("m_step is too small");
		EdgePoint.clear();
		return -4;
	}

	if (DstMat.cols <= m_step)
	{
		//qDebug("image too narrow");
		return -1;
	}

	EdgePoint.reserve(DstMat.rows);
	EdgePoint.clear();
	// 		double fmap[m_step * 2];//样条数组，其中fmap[0]~fmap[m_step-1]为X值，fmap[m_step]~fmap[m_step*2]为Y值
	// 		double M2[m_step];//样条计算矩阵
	double *fmap = static_cast<double*>(malloc(sizeof(double) * (m_step * 2)));
	double *M2 = static_cast<double*>(malloc(sizeof(double) * m_step));

	vector<Point2d> fit_point(DstMat.cols);
	int grad, gray_1, gray_2;

	bool m_use_shield = (m_shield_start + m_shield_end > 0);
	int m_min_shield = (m_shield_start > m_shield_end) ? m_shield_end : m_shield_start;
	int m_max_shiled = (m_shield_start > m_shield_end) ? m_shield_start : m_shield_end;
	if (m_min_shield > DstMat.rows || m_max_shiled > DstMat.rows || (m_max_shiled - m_min_shield) == 0)
	{
		m_use_shield = false;
	}

	double* h = static_cast<double*>(malloc(sizeof(double) * (m_step - 1)));
	double* A = static_cast<double*>(malloc(sizeof(double) * (m_step - 2)));
	double* B = static_cast<double*>(malloc(sizeof(double) * (m_step - 2)));
	double* C = static_cast<double*>(malloc(sizeof(double) * (m_step - 2)));
	double* D = static_cast<double*>(malloc(sizeof(double) * (m_step - 2)));
	double* E = static_cast<double*>(malloc(sizeof(double) * (m_step - 2)));

	for (int i = 0; i < DstMat.rows; i += test_step)
	{
		fit_point.clear();
		if (m_use_shield)
		{
			if (i > m_min_shield&&i < m_max_shiled)
			{
				continue;
			}
		}
		const uchar* line = DstMat.ptr<uchar>(i);
		if (is_black2white) {
			for (int j = 0, k = 1; k < DstMat.cols; ++j, ++k) {
				gray_2 = line[k];
				if (gray_2 >= m_edge_value) {
					gray_1 = line[j];
					if (gray_1 <= m_edge_value) {
						grad = gray_1 - gray_2;
						if (grad < m_min_grad) {
							fit_point.push_back(Point2d(j, grad));
							if (_search_style == AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE::enumFRISTGRAD) { break; }
						}
					}
				}
			}
		}
		else {
			for (int j = 0, k = 1; k < DstMat.cols; ++j, ++k) {
				gray_2 = line[k];
				if (gray_2 <= m_edge_value) {
					gray_1 = line[j];
					if (gray_1 >= m_edge_value) {
						grad = gray_1 - gray_2;
						if (grad > m_min_grad) {
							fit_point.push_back(Point2d(j, grad));
							if (_search_style == AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE::enumFRISTGRAD) { break; }
						}
					}
				}
			}
		}

		m_start = getmaxgrad(fit_point);
		m_end = m_start + 1;

		if (fit_point.size() > 0)//填充fmap[]值，在最大索引附近找m_step长度的值
		{
			int halfStep = m_step / 2;
			if (m_start < halfStep)//最大索引靠左情况
			{
				for (int j = 0; j < m_step; j++)
				{
					fmap[j] = j;
					fmap[j + m_step] = line[j];
				}
			}
			else if (m_start > DstMat.cols - halfStep - 1)//最大索引靠右情况
			{
				for (int j = 0; j < m_step; j++)
				{
					fmap[j] = DstMat.cols - m_step + j;
					fmap[j + m_step] = line[DstMat.cols - m_step + j];
				}
			}
			else//最大索引在中间
			{
				for (int j = 0; j < m_step; j++)
				{
					fmap[j] = m_start - halfStep + j;
					fmap[j + m_step] = line[m_start - halfStep + j];
				}
			}

			int tmpRst = 0;
			int iii;
			//计算x的步长
			for (iii = 0; iii < m_step - 1; iii++)
			{
				h[iii] = fmap[iii + 1] - fmap[iii];
			}
			//指定系数
			for (iii = 0; iii < m_step - 2; iii++)
			{
				A[iii] = h[iii]; //忽略A[0]
				B[iii] = 2 * (h[iii] + h[iii + 1]);
				C[iii] = h[iii + 1]; //忽略C(m_step-1)
			}
			//指定常数D
			for (iii = 0; iii < m_step - 2; iii++)
			{
				D[iii] = 6 * ((fmap[m_step + iii + 2] - fmap[m_step + iii + 1]) / h[iii + 1] - (fmap[m_step + iii + 1] - fmap[m_step + iii]) / h[iii]);
			}
			//求解三对角矩阵，结果赋值给E
			if (triDiagRes(m_step - 2, A, B, C, D, E) != 0)
				tmpRst = -1;
			M2[0] = 0; //自然边界的首端M2为0
			M2[m_step - 1] = 0;  //自然边界的末端M2为0
			for (iii = 1; iii < m_step - 1; iii++)
			{
				M2[iii] = E[iii - 1]; //其它的M2值
			}

			if (tmpRst != 0) {
				return -2;
			}
			double xval = -1;
			// 				if (invPlot2(m_step, fmap, M2, m_edge_value, xval,is_black2white) != 0) {
			// 					continue;
			// 				}
			if (invPlot3(m_step, fmap, M2, m_edge_value, xval) != 0)
			{
				continue;
			}
			EdgePoint.push_back(Point2d(offset.x + xval, offset.y + i));
		}
	}

	free(h);
	free(A);
	free(B);
	free(C);
	free(D);
	free(E);

	free(fmap);
	free(M2);
	return 0;
}

int CubicSpline::right2leftEdge2(const cv::Mat& DstMat, const cv::Point& offset, bool is_black2white, AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE _search_style, int test_step, int m_shield_start, int m_shield_end)
{
	if (m_step <= 2) {
		//qDebug("m_step is too small");
		EdgePoint.clear();
		return -4;
	}

	if (DstMat.cols <= m_step)
	{
		//qDebug("image too narrow");
		return -1;
	}

	EdgePoint.reserve(DstMat.rows);
	EdgePoint.clear();
	// 		double fmap[m_step * 2];//样条数组，其中fmap[0]~fmap[m_step-1]为X值，fmap[m_step]~fmap[m_step*2]为Y值
	// 		double M2[m_step];//样条计算矩阵
	double *fmap = static_cast<double*>(malloc(sizeof(double) * (m_step * 2)));
	double *M2 = static_cast<double*>(malloc(sizeof(double) * m_step));

	vector<Point2d> fit_point(DstMat.cols);
	int grad, gray_1, gray_2;

	bool m_use_shield = (m_shield_start + m_shield_end > 0);
	int m_min_shield = (m_shield_start > m_shield_end) ? m_shield_end : m_shield_start;
	int m_max_shiled = (m_shield_start > m_shield_end) ? m_shield_start : m_shield_end;
	if (m_min_shield > DstMat.rows || m_max_shiled > DstMat.rows || (m_max_shiled - m_min_shield) == 0)
	{
		m_use_shield = false;
	}

	double* h = static_cast<double*>(malloc(sizeof(double) * (m_step - 1)));
	double* A = static_cast<double*>(malloc(sizeof(double) * (m_step - 2)));
	double* B = static_cast<double*>(malloc(sizeof(double) * (m_step - 2)));
	double* C = static_cast<double*>(malloc(sizeof(double) * (m_step - 2)));
	double* D = static_cast<double*>(malloc(sizeof(double) * (m_step - 2)));
	double* E = static_cast<double*>(malloc(sizeof(double) * (m_step - 2)));

	for (int i = 0; i < DstMat.rows; i += test_step)
	{
		fit_point.clear();
		if (m_use_shield)
		{
			if (i > m_min_shield&&i < m_max_shiled)
			{
				continue;
			}
		}
		const uchar* line = DstMat.ptr<uchar>(i);
		if (is_black2white) {
			for (int j = DstMat.cols - 1, k = j - 1; j > 0; --j, --k) {
				gray_2 = line[k];
				if (gray_2 >= m_edge_value) {
					gray_1 = line[j];
					if (gray_1 <= m_edge_value) {
						grad = gray_1 - gray_2;
						if (grad < m_min_grad) {
							fit_point.push_back(Point2d(j, grad));
							if (_search_style == AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE::enumFRISTGRAD) { break; }
						}
					}
				}
			}
		}
		else {
			for (int j = DstMat.cols - 1, k = j - 1; j > 0; --j, --k) {
				gray_2 = line[k];
				if (gray_2 <= m_edge_value) {
					gray_1 = line[j];
					if (gray_1 >= m_edge_value) {
						grad = gray_1 - gray_2;
						if (grad > m_min_grad) {
							fit_point.push_back(Point2d(j, grad));
							if (_search_style == AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE::enumFRISTGRAD) { break; }
						}
					}
				}
			}
		}

		m_start = getmaxgrad(fit_point);
		m_end = m_start - 1;

		if (fit_point.size() > 0)
		{
			int halfStep = m_step / 2;
			if (m_start < halfStep)
			{
				for (int j = 0; j < m_step; j++)
				{
					fmap[j] = j;
					fmap[j + m_step] = line[j];
				}
			}
			else if (m_start > DstMat.cols - halfStep - 1)
			{
				for (int j = 0; j < m_step; j++)
				{
					fmap[j] = DstMat.cols - m_step + j;
					fmap[j + m_step] = line[DstMat.cols - m_step + j];
				}
			}
			else
			{
				for (int j = 0; j < m_step; j++)
				{
					fmap[j] = m_start - halfStep + j;
					fmap[j + m_step] = line[m_start - halfStep + j];
				}
			}

			int tmpRst = 0;
			int iii;
			//计算x的步长
			for (iii = 0; iii < m_step - 1; iii++)
			{
				h[iii] = fmap[iii + 1] - fmap[iii];
			}
			//指定系数
			for (iii = 0; iii < m_step - 2; iii++)
			{
				A[iii] = h[iii]; //忽略A[0]
				B[iii] = 2 * (h[iii] + h[iii + 1]);
				C[iii] = h[iii + 1]; //忽略C(m_step-1)
			}
			//指定常数D
			for (iii = 0; iii < m_step - 2; iii++)
			{
				D[iii] = 6 * ((fmap[m_step + iii + 2] - fmap[m_step + iii + 1]) / h[iii + 1] - (fmap[m_step + iii + 1] - fmap[m_step + iii]) / h[iii]);
			}
			//求解三对角矩阵，结果赋值给E
			if (triDiagRes(m_step - 2, A, B, C, D, E) != 0)
				tmpRst = -1;
			M2[0] = 0; //自然边界的首端M2为0
			M2[m_step - 1] = 0;  //自然边界的末端M2为0
			for (iii = 1; iii < m_step - 1; iii++)
			{
				M2[iii] = E[iii - 1]; //其它的M2值
			}

			if (tmpRst != 0) {
				return -2;
			}
			double xval = -1;
			// 				if (invPlot2(m_step, fmap, M2, m_edge_value, xval, is_black2white) != 0) {
			// 					continue;
			// 				}
			if (invPlot3(m_step, fmap, M2, m_edge_value, xval, true) != 0)
			{
				continue;
			}

			EdgePoint.push_back(Point2d(offset.x + xval, offset.y + i));
		}
	}

	free(h);
	free(A);
	free(B);
	free(C);
	free(D);
	free(E);

	free(fmap);
	free(M2);
	return 0;
}

int CubicSpline::top2bottomEdge2(const cv::Mat& DstMat, const cv::Point& offset, bool is_black2white, AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE _search_style, int test_step, int m_shield_start, int m_shield_end)
{
	if (m_step <= 2) {
		qDebug("m_step is too small");
		EdgePoint.clear();
		return -4;
	}

	if (DstMat.rows <= m_step)
	{
		qDebug("image too narrow");
		return -1;
	}

	EdgePoint.reserve(DstMat.cols);
	EdgePoint.clear();
	// 		double fmap[m_step * 2];//样条数组，其中fmap[0]~fmap[m_step-1]为X值，fmap[m_step]~fmap[m_step*2]为Y值
	// 		double M2[m_step];//样条计算矩阵
	double *fmap = static_cast<double*>(malloc(sizeof(double) * (m_step * 2)));
	double *M2 = static_cast<double*>(malloc(sizeof(double) * m_step));

	vector<Point2d> fit_point(DstMat.rows);
	int grad, gray_1, gray_2;

	bool m_use_shield = (m_shield_start + m_shield_end > 0);
	int m_min_shield = (m_shield_start > m_shield_end) ? m_shield_end : m_shield_start;
	int m_max_shiled = (m_shield_start > m_shield_end) ? m_shield_start : m_shield_end;
	if (m_min_shield > DstMat.cols || m_max_shiled > DstMat.cols || (m_max_shiled - m_min_shield) == 0)
	{
		m_use_shield = false;
	}

	double* h = static_cast<double*>(malloc(sizeof(double) * (m_step - 1)));
	double* A = static_cast<double*>(malloc(sizeof(double) * (m_step - 2)));
	double* B = static_cast<double*>(malloc(sizeof(double) * (m_step - 2)));
	double* C = static_cast<double*>(malloc(sizeof(double) * (m_step - 2)));
	double* D = static_cast<double*>(malloc(sizeof(double) * (m_step - 2)));
	double* E = static_cast<double*>(malloc(sizeof(double) * (m_step - 2)));

	// 预存行首指针
	vector<const uchar*> lines(DstMat.rows);
	for (int i = 0; i < DstMat.rows; ++i) {
		lines[i] = DstMat.ptr<uchar>(i);
	}

	for (int i = 0; i < DstMat.cols; i += test_step)
	{
		fit_point.clear();
		if (m_use_shield)
		{
			if (i > m_min_shield&&i < m_max_shiled)
			{
				continue;
			}
		}
		if (is_black2white) {
			for (int j = 0, k = 1; k < DstMat.rows; ++j, ++k) {
				gray_2 = lines[k][i];
				if (gray_2 >= m_edge_value) {
					gray_1 = lines[j][i];
					if (gray_1 <= m_edge_value) {
						grad = gray_1 - gray_2;
						if (grad < m_min_grad) {
							fit_point.push_back(Point2d(j, grad));
							if (_search_style == AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE::enumFRISTGRAD) { break; }
						}
					}
				}
			}
		}
		else {
			for (int j = 0, k = 1; k < DstMat.rows; ++j, ++k) {
				gray_2 = lines[k][i];
				if (gray_2 <= m_edge_value) {
					gray_1 = lines[j][i];
					if (gray_1 >= m_edge_value) {
						grad = gray_1 - gray_2;
						if (grad > m_min_grad) {
							fit_point.push_back(Point2d(j, grad));
							if (_search_style == AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE::enumFRISTGRAD) { break; }
						}
					}
				}
			}
		}

		m_start = getmaxgrad(fit_point);
		m_end = m_start + 1;

		if (fit_point.size() > 0)
		{
			int halfStep = m_step / 2;
			if (m_start < halfStep)
			{
				for (int j = 0; j < m_step; j++)
				{
					fmap[j] = j;
					fmap[j + m_step] = lines[j][i];
				}
			}
			else if (m_start > DstMat.rows - halfStep - 1)
			{
				for (int j = 0; j < m_step; j++)
				{
					fmap[j] = DstMat.rows - m_step + j;
					fmap[j + m_step] = lines[DstMat.rows - m_step + j][i];
				}
			}
			else
			{
				for (int j = 0; j < m_step; j++)
				{
					fmap[j] = m_start - halfStep + j;
					fmap[j + m_step] = lines[m_start - halfStep + j][i];
				}
			}

			int tmpRst = 0;
			int iii;
			//计算x的步长
			for (iii = 0; iii < m_step - 1; iii++)
			{
				h[iii] = fmap[iii + 1] - fmap[iii];
			}
			//指定系数
			for (iii = 0; iii < m_step - 2; iii++)
			{
				A[iii] = h[iii]; //忽略A[0]
				B[iii] = 2 * (h[iii] + h[iii + 1]);
				C[iii] = h[iii + 1]; //忽略C(m_step-1)
			}
			//指定常数D
			for (iii = 0; iii < m_step - 2; iii++)
			{
				D[iii] = 6 * ((fmap[m_step + iii + 2] - fmap[m_step + iii + 1]) / h[iii + 1] - (fmap[m_step + iii + 1] - fmap[m_step + iii]) / h[iii]);
			}
			//求解三对角矩阵，结果赋值给E
			if (triDiagRes(m_step - 2, A, B, C, D, E) != 0)
				tmpRst = -1;
			M2[0] = 0; //自然边界的首端M2为0
			M2[m_step - 1] = 0;  //自然边界的末端M2为0
			for (iii = 1; iii < m_step - 1; iii++)
			{
				M2[iii] = E[iii - 1]; //其它的M2值
			}

			if (tmpRst != 0) {
				return -2;
			}
			double yval = -1;
			// 				if (invPlot2(m_step, fmap, M2, m_edge_value, yval,is_black2white) != 0) {
			// 					continue;
			// 				}
			if (invPlot3(m_step, fmap, M2, m_edge_value, yval) != 0)
			{
				continue;
			}
			EdgePoint.push_back(Point2d(offset.x + i, offset.y + yval));
		}
	}

	free(h);
	free(A);
	free(B);
	free(C);
	free(D);
	free(E);

	free(fmap);
	free(M2);
	return 0;
}

int CubicSpline::bottom2topEdge2(const cv::Mat& DstMat, const cv::Point& offset, bool is_black2white, AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE _search_style, int test_step, int m_shield_start, int m_shield_end)
{
	if (m_step <= 2) {
		qDebug("m_step is too small");
		EdgePoint.clear();
		return -4;
	}

	if (DstMat.rows <= m_step)
	{
		qDebug("image too narrow");
		return -1;
	}

	EdgePoint.reserve(DstMat.cols);
	EdgePoint.clear();
	// 		double fmap[m_step * 2];//样条数组，其中fmap[0]~fmap[m_step-1]为X值，fmap[m_step]~fmap[m_step*2]为Y值
	// 		double M2[m_step];//样条计算矩阵
	double *fmap = static_cast<double*>(malloc(sizeof(double) * (m_step * 2)));
	double *M2 = static_cast<double*>(malloc(sizeof(double) * m_step));

	vector<Point2d> fit_point(DstMat.rows);
	int grad, gray_1, gray_2;

	bool m_use_shield = (m_shield_start + m_shield_end > 0);
	int m_min_shield = (m_shield_start > m_shield_end) ? m_shield_end : m_shield_start;
	int m_max_shiled = (m_shield_start > m_shield_end) ? m_shield_start : m_shield_end;
	if (m_min_shield > DstMat.cols || m_max_shiled > DstMat.cols || (m_max_shiled - m_min_shield) == 0)
	{
		m_use_shield = false;
	}

	double* h = static_cast<double*>(malloc(sizeof(double) * (m_step - 1)));
	double* A = static_cast<double*>(malloc(sizeof(double) * (m_step - 2)));
	double* B = static_cast<double*>(malloc(sizeof(double) * (m_step - 2)));
	double* C = static_cast<double*>(malloc(sizeof(double) * (m_step - 2)));
	double* D = static_cast<double*>(malloc(sizeof(double) * (m_step - 2)));
	double* E = static_cast<double*>(malloc(sizeof(double) * (m_step - 2)));

	// 预存行首指针
	vector<const uchar*> lines(DstMat.rows);
	for (int i = 0; i < DstMat.rows; ++i) {
		lines[i] = DstMat.ptr<uchar>(i);
	}

	for (int i = 0; i < DstMat.cols; i += test_step)
	{
		fit_point.clear();
		if (m_use_shield)
		{
			if (i > m_min_shield&&i < m_max_shiled)
			{
				continue;
			}
		}
		if (is_black2white) {
			for (int j = DstMat.rows - 1, k = j - 1; j > 0; --j, --k) {
				gray_2 = lines[k][i];
				if (gray_2 >= m_edge_value) {
					gray_1 = lines[j][i];
					if (gray_1 <= m_edge_value) {
						grad = gray_1 - gray_2;
						if (grad < m_min_grad) {
							fit_point.push_back(Point2d(j, grad));
							if (_search_style == AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE::enumFRISTGRAD) { break; }
						}
					}
				}
			}
		}
		else {
			for (int j = DstMat.rows - 1, k = j - 1; j > 0; --j, --k) {
				gray_2 = lines[k][i];
				if (gray_2 <= m_edge_value) {
					gray_1 = lines[j][i];
					if (gray_1 >= m_edge_value) {
						grad = gray_1 - gray_2;
						if (grad > m_min_grad) {
							fit_point.push_back(Point2d(j, grad));
							if (_search_style == AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE::enumFRISTGRAD) { break; }
						}
					}
				}
			}
		}

		m_start = getmaxgrad(fit_point);
		m_end = m_start - 1;
		if (fit_point.size() > 0)
		{
			int halfStep = m_step / 2;
			if (m_start < halfStep)
			{
				for (int j = 0; j < m_step; j++)
				{
					fmap[j] = j;
					fmap[j + m_step] = lines[j][i];
				}
			}
			else if (m_start > DstMat.rows - halfStep - 1)
			{
				for (int j = 0; j < m_step; j++)
				{
					fmap[j] = DstMat.rows - m_step + j;
					fmap[j + m_step] = lines[DstMat.rows - m_step + j][i];
				}
			}
			else
			{
				for (int j = 0; j < m_step; j++)
				{
					fmap[j] = m_start - halfStep + j;
					fmap[j + m_step] = lines[m_start - halfStep + j][i];
				}
			}

			int tmpRst = 0;
			int iii;
			//计算x的步长
			for (iii = 0; iii < m_step - 1; iii++)
			{
				h[iii] = fmap[iii + 1] - fmap[iii];
			}
			//指定系数
			for (iii = 0; iii < m_step - 2; iii++)
			{
				A[iii] = h[iii]; //忽略A[0]
				B[iii] = 2 * (h[iii] + h[iii + 1]);
				C[iii] = h[iii + 1]; //忽略C(m_step-1)
			}
			//指定常数D
			for (iii = 0; iii < m_step - 2; iii++)
			{
				D[iii] = 6 * ((fmap[m_step + iii + 2] - fmap[m_step + iii + 1]) / h[iii + 1] - (fmap[m_step + iii + 1] - fmap[m_step + iii]) / h[iii]);
			}
			//求解三对角矩阵，结果赋值给E
			if (triDiagRes(m_step - 2, A, B, C, D, E) != 0)
				tmpRst = -1;
			M2[0] = 0; //自然边界的首端M2为0
			M2[m_step - 1] = 0;  //自然边界的末端M2为0
			for (iii = 1; iii < m_step - 1; iii++)
			{
				M2[iii] = E[iii - 1]; //其它的M2值
			}

			if (tmpRst != 0) {
				return -2;
			}
			double yval = -1;
			// 				if (invPlot2(m_step, fmap, M2, m_edge_value, yval,is_black2white) != 0) {
			// 					continue;
			// 				}
			if (invPlot3(m_step, fmap, M2, m_edge_value, yval, true) != 0)
			{
				continue;
			}
			EdgePoint.push_back(Point2d(offset.x + i, offset.y + yval));
		}
	}

	free(h);
	free(A);
	free(B);
	free(C);
	free(D);
	free(E);

	free(fmap);
	free(M2);
	return 0;
}

//自然边界的三次样条曲线函数
int CubicSpline::cubicSpline(const int n, const double * map, double * M, double * ai, double * bi, double * ci, double *di)
{
	double* h = static_cast<double*>(malloc(sizeof(double) * (n - 1)));  //x的??
															/* M矩阵的系数
															*[B0, C0, ...
															*[A1, B1, C1, ...
															*[0,  A2, B2, C2, ...
															*[0, ...             An-1, Bn-1]
															*/
	double* A = static_cast<double*>(malloc(sizeof(double) * (n - 2)));
	double* B = static_cast<double*>(malloc(sizeof(double) * (n - 2)));
	double* C = static_cast<double*>(malloc(sizeof(double) * (n - 2)));
	double* D = static_cast<double*>(malloc(sizeof(double) * (n - 2))); //等号右边的常数矩阵
	double* E = static_cast<double*>(malloc(sizeof(double) * (n - 2))); //M矩阵
	int i;
	//计算x的步长
	for (i = 0; i < n - 1; i++)
	{
		h[i] = map[i + 1] - map[i];
	}
	//指定系数
	for (i = 0; i < n - 2; i++)
	{
		A[i] = h[i]; //忽略A[0]
		B[i] = 2 * (h[i] + h[i + 1]);
		C[i] = h[i + 1]; //忽略C(n-1)
	}
	//指定常数D
	for (i = 0; i < n - 2; i++)
	{
		D[i] = 6 * ((map[n + i + 2] - map[n + i + 1]) / h[i + 1] - (map[n + i + 1] - map[n + i]) / h[i]);
	}
	//求解三对角矩阵，结果赋值给E
	if (triDiagRes(n - 2, A, B, C, D, E) != 0)
		return -1;
	M[0] = 0; //自然边界的首端M为0
	M[n - 1] = 0;  //自然边界的末端M为0
	for (i = 1; i < n - 1; i++)
	{
		M[i] = E[i - 1]; //其它的M值
	}
	//计算三次样条曲线的系数
	for (i = 0; i < n - 1; i++)
	{
		ai[i] = map[n + i];
		bi[i] = (map[n + i + 1] - map[n + i]) / h[i] - (2 * h[i] * M[i] + h[i] * M[i + 1]) / 6;
		ci[i] = M[i] / 2;
		di[i] = (M[i + 1] - M[i]) / (6 * h[i]);
	}
	free(h);
	free(A);
	free(B);
	free(C);
	free(D);
	free(E);
	return 0;
}

int CubicSpline::invPlot(int n, double * map, double * M, double y, double & x)
{
	int ilo = -1;
	int ihi = -1;
	bool flaglo = false;
	bool flaghi = false;
	for (int i = 0; i < n; i++) {
		if (map[n + i] < y) {
			ilo = i;
			if (flaghi)
				break;
			flaglo = true;
		}
		else {
			ihi = i;
			if (flaglo)
				break;
			flaghi = true;
		}
	}
	if (ilo < 0 || ihi < 0) {
		return -1;
	}
	double xlo = map[ilo];
	double ylo = map[n + ilo];
	double xhi = map[ihi];
	double yhi = map[n + ihi];
	while (true) {
		if (fabs(ylo - y) < m_iteration_accuracy) {
			x = xlo;
			break;
		}
		else if (fabs(yhi - y) < m_iteration_accuracy) {
			x = xhi;
			break;
		}
		else {
			double xtmp = (xlo + xhi)*0.5;
			double ytmp;
			if (splineGetVal(n, map, M, xtmp, ytmp) != 0) {
				qDebug("getval error in invPlot");
				break;
			}
			if (ytmp < y) {           
				xlo = xtmp;
				ylo = ytmp;
			}
			else {
				xhi = xtmp;
				yhi = ytmp;
			}
		}
	}
	return 0;
}

int CubicSpline::invPlot2(int n, double * map, double * M, double y, double & x, bool is_black2white)
{
	try
	{
		if ((n + m_start) > (sizeof(double) * (n * 2) - 1) || (n + m_end) > (sizeof(double) * (n * 2) - 1))
		{
			//qDebug() << "error";
			return -1;
		}
		if (m_start < 0 || m_end < 0)
		{
			//qDebug() << "error";
			return -1;
		}
		double xlo = m_start;
		double ylo = map[n + m_start];
		double xhi = m_end;
		double yhi = map[n + m_end];

		if (is_black2white)
		{
			xlo = m_end;
			ylo = map[n + m_end];
			xhi = m_start;
			yhi = map[n + m_start];
		}

		while (true) {
			if (fabs(ylo - y) < m_iteration_accuracy) {
				x = xlo;
				break;
			}
			else if (fabs(yhi - y) < m_iteration_accuracy) {
				x = xhi;
				break;
			}
			else {
				double xtmp = (xlo + xhi) * 0.5;
				double ytmp;
				if (splineGetVal(n, map, M, xtmp, ytmp) != 0) {
					qDebug("getval error in invPlot");
					break;
				}
				if (ytmp > y) {				// warning
					xlo = xtmp;
					ylo = ytmp;
				}
				else {
					xhi = xtmp;
					yhi = ytmp;
				}
			}
		}
	}
	catch (std::exception& ex)
	{
		// todo
	}
	catch (...)
	{
		// todo
	}
	return 0;
}

int CubicSpline::invPlot3(int n, double * map, double * M, double y, double & x, bool is_transpose)
{
	int ilo = -1;
	int ihi = -1;
	bool flaglo = false;
	bool flaghi = false;
	if (is_transpose)
	{
		for (int i = n - 1; i >= 0; i--) {
			if (map[n + i] < y) {
				ilo = i;
				if (flaghi)
					break;
				flaglo = true;
			}
			else {
				ihi = i;
				if (flaglo)
					break;
				flaghi = true;
			}
		}
	}
	else
	{
		for (int i = 0; i < n; i++) {
			if (map[n + i] < y) {
				ilo = i;
				if (flaghi)
					break;
				flaglo = true;
			}
			else {
				ihi = i;
				if (flaglo)
					break;
				flaghi = true;
			}
		}
	}
	if (ilo < 0 || ihi < 0) {
		return -1;
	}
	double xlo = map[ilo];
	double ylo = map[n + ilo];
	double xhi = map[ihi];
	double yhi = map[n + ihi];
	while (true) {
		if (fabs(ylo - y) < m_iteration_accuracy) {
			x = xlo;
			break;
		}
		else if (fabs(yhi - y) < m_iteration_accuracy) {
			x = xhi;
			break;
		}
		else {
			double xtmp = (xlo + xhi)*0.5;
			double ytmp;
			if (splineGetVal(n, map, M, xtmp, ytmp) != 0) {
				//qDebug("getval error in invPlot");
				break;
			}
			if (ytmp < y) {
				xlo = xtmp;
				ylo = ytmp;
			}
			else {
				xhi = xtmp;
				yhi = ytmp;
			}
		}
	}
	return 0;
}

int CubicSpline::splineGetVal(int n, double * map, double * M, const double x, double & y)
{
	try
	{
		int klo = 0;
		int khi = n - 1;
		while (khi - klo > 1) {
			int k = (khi + klo) >> 1;
			if (map[k] > x) khi = k;
			else klo = k;
		}
		double h = map[khi] - map[klo];
		if (h <= DBL_EPSILON && h >= -DBL_EPSILON) {
			qDebug("Bad xa input to routine splint");
			return -1;
		}
		double a = (map[khi] - x) / h;
		double b = (x - map[klo]) / h;
		y = a * map[n + klo] + b * map[n + khi] + ((a * a * a - a) * M[klo]
			+ (b * b * b - b) * M[khi]) * (h * h) / 6.0;
	}
	catch (std::exception& ex)
	{
		// todo
	}
	catch (...)
	{
		// todo
	}
	return 0;
}
// solve tridiagonal linear equation
int CubicSpline::triDiagRes(int sz, double * A, double * B, double * C, double * D, double * X)
{
	if (sz < 0)
		return -1;
	if (B[0] <= DBL_EPSILON && B[0] >= -DBL_EPSILON)
		return -1;
	C[0] = C[0] / B[0];
	D[0] = D[0] / B[0];
	double tmp;
	for (int i = 1; i < sz; i++)
	{
		tmp = (B[i] - A[i] * C[i - 1]);
		if (tmp <= DBL_EPSILON && tmp >= -DBL_EPSILON)
			return -1;
		C[i] = C[i] / tmp;
		D[i] = (D[i] - A[i] * D[i - 1]) / tmp;
	}
	//直接求出X的最后一个值
	X[sz - 1] = D[sz - 1];
	//逆向迭代， 求出X
	for (int i = sz - 2; i >= 0; i--)
	{
		X[i] = D[i] - C[i] * X[i + 1];
	}
	return 0;
}

int CubicSpline::getmaxgrad(const std::vector<cv::Point2d>& pts)
{
	int pos = 0;
	double max_grad=0;
	for (auto& t : pts)
	{
		auto y = abs(t.y);
		if (y > max_grad)
		{
			pos = t.x;
			max_grad = y;
		}
	}
	return pos;
}
