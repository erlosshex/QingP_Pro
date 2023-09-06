#ifndef ALGORITHM_OPERATOR_BSPLINE_H
#define ALGORITHM_OPERATOR_BSPLINE_H

#include "../../algorithm_operator_global.h"

#include <QObject>
#include <QPolygon>
#include <QVector>

#include <opencv2/opencv.hpp>

#include "../algorithm_operator_common/algorithm_operator_common_enum.h"

class ALGORITHM_OPERATOR_EXPORT BSpline : public QObject
{
	Q_OBJECT
public:
	typedef QVector<qreal> RealVec;
	typedef QVector<RealVec> Bfunc;
	typedef QVector<Bfunc> BfArray;

public:
	BSpline();
	~BSpline();

	int FindEdgePoint(cv::Mat DstMat, cv::Point offset, AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION direction, AlgorithmOperatorCommonEnum::EDGEFINDER_MODE EdgeMode, int _step, int _min_grad, int _edge_value, double _accuracy, AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE _search_style, int _test_step = 1, int m_shield_start = 0, int m_shield_end = 0);

	std::vector<cv::Point2d> EdgePoint;//边缘点存放缓存，每次运行FindEdgePoint后清空上一次缓存

private:
	int left2rightEdge(cv::Mat DstMat, cv::Point offset, bool is_black2white, AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE _search_style, int test_step, int m_shield_start, int m_shield_end);//从左到右找边
	int right2leftEdge(cv::Mat DstMat, cv::Point offset, bool is_black2white, AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE _search_style, int test_step, int m_shield_start, int m_shield_end);//从右到左找边
	int top2bottomEdge(cv::Mat DstMat, cv::Point offset, bool is_black2white, AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE _search_style, int test_step, int m_shield_start, int m_shield_end);//从上到下找边
	int bottom2topEdge(cv::Mat DstMat, cv::Point offset, bool is_black2white, AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE _search_style, int test_step, int m_shield_start, int m_shield_end);//从下到上找边


	int bSpline(QPolygonF &inputArr, qreal edgeValue, qreal & xValue, bool is_transpose = false);
	int InvPolate(QPolygonF & pts, qreal y, qreal &u, bool is_transpose);

	int scalarMultiply(const RealVec & vec, qreal scale, RealVec & res);
	int addRealVec(const RealVec & vec1, const RealVec & vec2, RealVec & res);
	int addFunc(const Bfunc & f1, Bfunc & f2, Bfunc & res);
	int polynomial(const RealVec & coeff, qreal t, qreal & val);

	int inputKnots(RealVec & knots);
	int calcCoeffs();
	int interpolate(qreal u, QPointF & pt);
	int quasiUniform(const int order, const QPolygonF &ctrls);

	int getmaxgrad(std::vector<cv::Point2d> pts, bool is_black2white);

private:
	int order;
	RealVec knots;
	QPolygonF controls;
	QVector<BfArray> bfHierarchy;

	AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION m_direction = AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION::enumLEFT2RIGHT;
	AlgorithmOperatorCommonEnum::EDGEFINDER_MODE m_mode = AlgorithmOperatorCommonEnum::EDGEFINDER_MODE::enumWHITE2BLACK;
	int m_step = 15;
	int m_min_grad = 25;
	int m_grad_step = 3;
	int m_edge_value = 180;
	static double m_iteration_accuracy;
	static int m_start;
	static int m_end;

};

#endif 