#ifndef ALGORITHM_OPERATOR_CUBICSPLINE_H
#define ALGORITHM_OPERATOR_CUBICSPLINE_H

#include "../../algorithm_operator_global.h"

#include <opencv2/opencv.hpp>

#include <QObject>

#include "../algorithm_operator_common/algorithm_operator_common_enum.h"

class ALGORITHM_OPERATOR_EXPORT CubicSpline : public QObject
{
	Q_OBJECT
public:
	CubicSpline() {}
	~CubicSpline() {}
	//************************************
	// Method:    FindEdgePoint
	// FullName:  CubicSpline::FindEdgePoint
	// Access:    public static 
	// Returns:   int 0:success  
	//                -1:image too narrow
	//                -2:cubicSpline error
	//                -3:direction error
	//                -4:m_step too small
	// Qualifier: 对目标图像找边缘点，按扫描方向的边沿方式最先出现的边沿即为边缘点
	// Parameter: CV_Mat DstMat 目标图像
	// Parameter: CV_Point offset 目标图像在原图像中的偏移量，用于计算边缘点坐标
	// Parameter: int direction 扫描方向，包括LEFT2RIGHT（从左到右），RIGHT2LEFT（从右到左），TOP2BOTTOM（从上到下），BOTTOM2TOP（从下到上）四个
	// Parameter: bool EdgeMode 边沿方式，包括BLACK2WHITE（从黑到白），WHITE2BLACK（从白到黑）两个
	//************************************
	int FindEdgePoint(const cv::Mat& DstMat, const cv::Point& offset, AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION direction, AlgorithmOperatorCommonEnum::EDGEFINDER_MODE EdgeMode, int _step, int _min_grad, int _edge_value, double _accuracy, AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE _search_style, int _test_step = 1, int m_shield_start = 0, int m_shield_end = 0);
	cv::Mat drawEdge(cv::Mat SrcMat);//将边缘点绘制成红色显示
	std::vector<cv::Point2d> EdgePoint;//边缘点存放缓存，每次运行FindEdgePoint后清空上一次缓存

private:
	int left2rightEdge2(const cv::Mat& DstMat, const cv::Point& offset, bool is_black2white, AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE _search_style, int test_step, int m_shield_start, int m_shield_end);//从左到右找边
	int right2leftEdge2(const cv::Mat& DstMat, const cv::Point& offset, bool is_black2white, AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE _search_style, int test_step, int m_shield_start, int m_shield_end);//从右到左找边
	int top2bottomEdge2(const cv::Mat& DstMat, const cv::Point& offset, bool is_black2white, AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE _search_style, int test_step, int m_shield_start, int m_shield_end);//从上到下找边
	int bottom2topEdge2(const cv::Mat& DstMat, const cv::Point& offset, bool is_black2white, AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE _search_style, int test_step, int m_shield_start, int m_shield_end);//从下到上找边

	int cubicSpline(const int n, const double * map, double * M, double * ai, double * bi, double * ci, double *di);//三次样条插值算法
	int invPlot(int n, double * map, double * M, double y, double & x);//逆向迭代求X值算法
	int invPlot2(int n, double * map, double * M, double y, double & x, bool is_black2white);//逆向迭代求X值算法
	int invPlot3(int n, double * map, double * M, double y, double & x, bool is_transpose = false);//逆向迭代求X值算法
	int splineGetVal(int n, double * map, double * M, const double x, double & y);//逆向迭代求X值算法所用函数
	int triDiagRes(int sz, double * A, double * B, double * C, double * D, double * X);//三次样条插值算法所用函数
	int getmaxgrad(const std::vector<cv::Point2d>& pts);

private:
	AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION m_direction = AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION::enumLEFT2RIGHT;
	AlgorithmOperatorCommonEnum::EDGEFINDER_MODE m_mode = AlgorithmOperatorCommonEnum::EDGEFINDER_MODE::enumWHITE2BLACK;
	int m_step = 15;
	int m_min_grad = 25;
	int m_grad_step = 3;
	int m_edge_value = 180;
	double m_iteration_accuracy;

	int m_start;
	int m_end;
};

#endif 