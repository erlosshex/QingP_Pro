#ifndef ALGORITHM_OPERATOR_EDGEPOINTSPLINE_H
#define ALGORITHM_OPERATOR_EDGEPOINTSPLINE_H

#include "../../algorithm_operator_global.h"

#include <QObject>

#include "bspline.h"
#include "cubicspline.h"

class ALGORITHM_OPERATOR_EXPORT EdgePointSpline : public QObject
{
	Q_OBJECT
public:
	EdgePointSpline();
	~EdgePointSpline();

	int FindEdgePoint(cv::Mat DstMat, cv::Point offset, AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION direction, AlgorithmOperatorCommonEnum::EDGEFINDER_MODE EdgeMode, int _step, int _min_grad, int _edge_value, double _accuracy, AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE _search_style, int _test_step = 1, int m_shield_start = 0, int m_shield_end = 0);
	int FindEdgePointColor(cv::Mat DstMat, cv::Point offset, AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION direction, AlgorithmOperatorCommonEnum::EDGEFINDER_MODE EdgeMode, int _step, int _min_grad, int _edge_value_red_min, int _edge_value_red_max, int _edge_value_green_min, int _edge_value_green_max, int _edge_value_blue_min, int _edge_value_blue_max, bool _is_reverse, double _accurancy, AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE _search_style, int _test_step = 1, int m_shield_start = 0, int m_shield_end = 0);
	int FindEdgePoint(cv::Mat DstMat, cv::Rect roi, int left_extand, int right_extand, int top_extand, int bottom_extand, AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION direction, AlgorithmOperatorCommonEnum::EDGEFINDER_MODE EdgeMode, int _step, int _min_grad, int _edge_value, double _accuracy, AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE _search_style, int _test_step = 1, int m_shield_start = 0, int m_shield_end = 0);
	int FindEdgePointColor(cv::Mat DstMat, cv::Rect roi, int left_extand, int right_extand, int top_extand, int bottom_extand, AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION direction, AlgorithmOperatorCommonEnum::EDGEFINDER_MODE EdgeMode, int _step, int _min_grad, int _edge_value_red_min, int _edge_value_red_max, int _edge_value_green_min, int _edge_value_green_max, int _edge_value_blue_min, int _edge_value_blue_max, bool _is_reverse, double _accurancy, AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE _search_style, int _test_step = 1, int m_shield_start = 0, int m_shield_end = 0);

	std::vector<cv::Point2d> EdgePoint;//边缘点存放缓存，每次运行FindEdgePoint后清空上一次缓存
	
	cv::Mat drawEdge(cv::Mat SrcMat);//可以的话，将边缘点绘制成红色点显示

	int FindEdgePointTwoSides(cv::Mat DstMat, cv::Point offset, AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION_NEW direction, AlgorithmOperatorCommonEnum::EDGEFINDER_MODE EdgeMode, int _step, int _min_grad, int _edge_value, double _accuracy, AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE _search_style, int _test_step = 1, int m_shield_start = 0, int m_shield_end = 0);
	int FindEdgePointTwoSides(cv::Mat DstMat, cv::Rect roi, int left_extand, int right_extand, int top_extand, int bottom_extand, AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION_NEW direction, AlgorithmOperatorCommonEnum::EDGEFINDER_MODE EdgeMode, int _step, int _min_grad, int _edge_value, double _accuracy, AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE _search_style, int _test_step = 1, int m_shield_start = 0, int m_shield_end = 0);
	int convertEdgeFinderDirection(AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION_NEW edn, AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION &direction0, AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION &direction1);
	bool isSingleDirection(AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION_NEW edn);
	AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION convertEdgeFinderDirectionNew2Old(AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION_NEW edn);

	std::vector<cv::Point2d> EdgePoint0;
	std::vector<cv::Point2d> EdgePoint1;

	cv::Mat drawEdgeTwoSides(cv::Mat SrcMat);//可以的话，将两边边缘点绘制成红色点显示

	AlgorithmOperatorCommonEnum::SPLINE_TYPE m_type = AlgorithmOperatorCommonEnum::SPLINE_TYPE::enumCubicSpline;

private:
	CubicSpline cs;
	BSpline bs;

	cv::Mat regionThresholdColor(cv::Mat& img, int redLowPixel, int redUpPixel, int greenLowPixel, int greenUpPixel, int blueLowPixel, int blueUpPixel, bool isReverse = false);
	cv::Mat regionThresholdColorBGR(cv::Mat& img, int blueLowPixel, int blueUpPixel, int greenLowPixel, int greenUpPixel, int redLowPixel, int redUpPixel, bool isReverse = false);

};

#endif 