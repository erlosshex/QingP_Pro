#include "EdgePointSpline.h"

using namespace cv;
using namespace std;

EdgePointSpline::EdgePointSpline()
{
}

EdgePointSpline::~EdgePointSpline()
{

}

int EdgePointSpline::FindEdgePoint(cv::Mat DstMat, cv::Point offset, AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION direction, AlgorithmOperatorCommonEnum::EDGEFINDER_MODE EdgeMode, int _step, int _min_grad, int _edge_value, double _accuracy, AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE _search_style, int _test_step, int m_shield_start, int m_shield_end)
{
	if (abs(_accuracy) < 0.01) _accuracy = 0.01;
	int rslt = 0;
	EdgePoint.clear();
	switch (m_type)
	{
	case AlgorithmOperatorCommonEnum::SPLINE_TYPE::enumCubicSpline:
		rslt = cs.FindEdgePoint(DstMat, offset, direction, EdgeMode, _step, _min_grad, _edge_value, _accuracy, _search_style, _test_step, m_shield_start, m_shield_end);
		EdgePoint.assign(cs.EdgePoint.begin(), cs.EdgePoint.end());
		break;
	case AlgorithmOperatorCommonEnum::SPLINE_TYPE::enumBSpline:
		rslt = bs.FindEdgePoint(DstMat, offset, direction, EdgeMode, _step, _min_grad, _edge_value, _accuracy, _search_style, _test_step, m_shield_start, m_shield_end);
		EdgePoint.assign(bs.EdgePoint.begin(), bs.EdgePoint.end());
		break;
	default:
		break;
	}
	return rslt;
}

int EdgePointSpline::FindEdgePointColor(cv::Mat DstMat, cv::Point offset, AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION direction, AlgorithmOperatorCommonEnum::EDGEFINDER_MODE EdgeMode, int _step, int _min_grad, int _edge_value_red_min, int _edge_value_red_max, int _edge_value_green_min, int _edge_value_green_max, int _edge_value_blue_min, int _edge_value_blue_max, bool _is_reverse, double _accurancy, AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE _search_style, int _test_step, int m_shield_start, int m_shield_end)
{
	Mat BinDstMat = regionThresholdColor(DstMat, _edge_value_red_min, _edge_value_red_max, _edge_value_green_min, _edge_value_green_max, _edge_value_blue_min, _edge_value_blue_max, _is_reverse);
	cv::cvtColor(BinDstMat, BinDstMat, cv::ColorConversionCodes::COLOR_BGR2GRAY);
	return FindEdgePoint(BinDstMat, offset, direction, EdgeMode, _step, _min_grad, 128, _accurancy, _search_style, _test_step, m_shield_start, m_shield_end);
}

int EdgePointSpline::FindEdgePoint(cv::Mat DstMat, cv::Rect roi, int left_extand, int right_extand, int top_extand, int bottom_extand, AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION direction, AlgorithmOperatorCommonEnum::EDGEFINDER_MODE EdgeMode, int _step, int _min_grad, int _edge_value, double _accuracy, AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE _search_style, int _test_step, int m_shield_start, int m_shield_end)
{
	if (abs(_accuracy) < 0.01) _accuracy = 0.01;
	int rslt = 0;
	EdgePoint.clear();

	Point offset;

	roi.x = roi.x - left_extand;
	roi.width = roi.width + left_extand + right_extand;
	roi.y = roi.y - top_extand;
	roi.height = roi.height + top_extand + bottom_extand;

	offset.x = roi.x;
	offset.y = roi.y;

	Mat DstMatRoi = DstMat(roi).clone();

	switch (m_type)
	{
	case AlgorithmOperatorCommonEnum::SPLINE_TYPE::enumCubicSpline:
		rslt = cs.FindEdgePoint(DstMatRoi, offset, direction, EdgeMode, _step, _min_grad, _edge_value, _accuracy, _search_style, _test_step, m_shield_start, m_shield_end);
		EdgePoint.assign(cs.EdgePoint.begin(), cs.EdgePoint.end());
		break;
	case AlgorithmOperatorCommonEnum::SPLINE_TYPE::enumBSpline:
		rslt = bs.FindEdgePoint(DstMatRoi, offset, direction, EdgeMode, _step, _min_grad, _edge_value, _accuracy, _search_style, _test_step, m_shield_start, m_shield_end);
		EdgePoint.assign(bs.EdgePoint.begin(), bs.EdgePoint.end());
		break;
	default:
		break;
	}
	return rslt;
}

int EdgePointSpline::FindEdgePointColor(cv::Mat DstMat, cv::Rect roi, int left_extand, int right_extand, int top_extand, int bottom_extand, AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION direction, AlgorithmOperatorCommonEnum::EDGEFINDER_MODE EdgeMode, int _step, int _min_grad, int _edge_value_red_min, int _edge_value_red_max, int _edge_value_green_min, int _edge_value_green_max, int _edge_value_blue_min, int _edge_value_blue_max, bool _is_reverse, double _accurancy, AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE _search_style, int _test_step, int m_shield_start, int m_shield_end)
{
	Mat BinDstMat = regionThresholdColor(DstMat, _edge_value_red_min, _edge_value_red_max, _edge_value_green_min, _edge_value_green_max, _edge_value_blue_min, _edge_value_blue_max, _is_reverse);
	cv::cvtColor(BinDstMat, BinDstMat, cv::ColorConversionCodes::COLOR_BGR2GRAY);
	return FindEdgePoint(BinDstMat, roi, left_extand, right_extand, top_extand, bottom_extand, direction, EdgeMode, _step, _min_grad, 128, _accurancy, _search_style, _test_step, m_shield_start, m_shield_end);
}

cv::Mat EdgePointSpline::drawEdge(cv::Mat SrcMat)
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

int EdgePointSpline::FindEdgePointTwoSides(cv::Mat DstMat, cv::Point offset, AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION_NEW direction, AlgorithmOperatorCommonEnum::EDGEFINDER_MODE EdgeMode, int _step, int _min_grad, int _edge_value, double _accuracy, AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE _search_style, int _test_step /*= 1*/, int m_shield_start /*= 0*/, int m_shield_end /*= 0*/)
{
	int rslt = 0;

	try {
		AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION direction0;
		AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION direction1;

		int direction_num = convertEdgeFinderDirection(direction, direction0, direction1);

		if (direction_num == 1) {
			FindEdgePoint(DstMat, offset, direction0, EdgeMode, _step, _min_grad, _edge_value, _accuracy, _search_style, _test_step, m_shield_start, m_shield_end);
			EdgePoint0 = EdgePoint1 = EdgePoint;

			rslt = 1;
		}
		else if (direction_num == 2) {
			Rect rect0, rect1;
			Mat DstMat0, DstMat1;
			int rows = DstMat.rows;
			int cols = DstMat.cols;

			if (direction == AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION_NEW::enumHOROUT2IN_NEW || direction == AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION_NEW::enumHORIN2OUT_NEW) {
				rect0.x = 0;
				rect0.y = 0;
				rect0.width = cols / 2;
				rect0.height = rows;
				DstMat0 = DstMat(rect0);
				FindEdgePoint(DstMat0, offset + rect0.tl(), direction0, EdgeMode, _step, _min_grad, _edge_value, _accuracy, _search_style, _test_step, m_shield_start, m_shield_end);
				EdgePoint0 = EdgePoint;

				rect1.x = cols / 2;
				rect1.y = 0;
				rect1.width = cols / 2;
				rect1.height = rows;
				DstMat1 = DstMat(rect1);
				FindEdgePoint(DstMat1, offset + rect1.tl(), direction1, EdgeMode, _step, _min_grad, _edge_value, _accuracy, _search_style, _test_step, m_shield_start, m_shield_end);
				EdgePoint1 = EdgePoint;
			}
			if (direction == AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION_NEW::enumVEROUT2IN_NEW || direction == AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION_NEW::enumVERIN2OUT_NEW) {
				rect0.x = 0;
				rect0.y = 0;
				rect0.width = cols;
				rect0.height = rows / 2;
				DstMat0 = DstMat(rect0);
				FindEdgePoint(DstMat0, offset + rect0.tl(), direction0, EdgeMode, _step, _min_grad, _edge_value, _accuracy, _search_style, _test_step, m_shield_start, m_shield_end);
				EdgePoint0 = EdgePoint;

				rect1.x = 0;
				rect1.y = rows / 2;
				rect1.width = cols;
				rect1.height = rows / 2;
				DstMat1 = DstMat(rect1);
				FindEdgePoint(DstMat1, offset + rect1.tl(), direction1, EdgeMode, _step, _min_grad, _edge_value, _accuracy, _search_style, _test_step, m_shield_start, m_shield_end);
				EdgePoint1 = EdgePoint;
			}

			rslt = 2;
		}
		else {
			rslt = 0;
		}
	}
	catch (exception &e) {
		rslt = 0;
	}

	return rslt;
}

int EdgePointSpline::FindEdgePointTwoSides(cv::Mat DstMat, cv::Rect roi, int left_extand, int right_extand, int top_extand, int bottom_extand, AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION_NEW direction, AlgorithmOperatorCommonEnum::EDGEFINDER_MODE EdgeMode, int _step, int _min_grad, int _edge_value, double _accuracy, AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE _search_style, int _test_step /*= 1*/, int m_shield_start /*= 0*/, int m_shield_end /*= 0*/)
{
	int rslt = 0;

	try {
		AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION direction0;
		AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION direction1;

		int direction_num = convertEdgeFinderDirection(direction, direction0, direction1);

		if (direction_num == 1) {
			FindEdgePoint(DstMat, roi, left_extand, right_extand, top_extand, bottom_extand, direction0, EdgeMode, _step, _min_grad, _edge_value, _accuracy, _search_style, _test_step, m_shield_start, m_shield_end);
			EdgePoint0 = EdgePoint1 = EdgePoint;

			rslt = 1;
		}
		else if (direction_num == 2) {
			FindEdgePoint(DstMat, roi, left_extand, right_extand, top_extand, bottom_extand, direction0, EdgeMode, _step, _min_grad, _edge_value, _accuracy, _search_style, _test_step, m_shield_start, m_shield_end);
			EdgePoint0 = EdgePoint;

			FindEdgePoint(DstMat, roi, left_extand, right_extand, top_extand, bottom_extand, direction1, EdgeMode, _step, _min_grad, _edge_value, _accuracy, _search_style, _test_step, m_shield_start, m_shield_end);
			EdgePoint1 = EdgePoint;

			rslt = 2;
		}
		else {
			rslt = 0;
		}
	}
	catch (exception &e) {
		rslt = 0;
	}

	return rslt;
}

int EdgePointSpline::convertEdgeFinderDirection(AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION_NEW edn, AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION &direction0, AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION &direction1)
{
	int res = 0;

	if (isSingleDirection(edn)) {
		if (edn == AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION_NEW::enumLEFT2RIGHTNEW) {
			direction0 = AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION::enumLEFT2RIGHT;
		}
		else if (edn == AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION_NEW::enumRIGHT2LEFTNEW) {
			direction0 = AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION::enumRIGHT2LEFT;
		}
		else if (edn == AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION_NEW::enumTOP2BOTTOMNEW) {
			direction0 = AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION::enumTOP2BOTTOM;
		}
		else if (edn == AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION_NEW::enumBOTTOM2TOPNEW) {
			direction0 = AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION::enumBOTTOM2TOP;
		}
		direction1 = direction0;

		res = 1;
	}
	else {
		if (edn == AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION_NEW::enumHOROUT2IN_NEW) {
			direction0 = AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION::enumLEFT2RIGHT;
			direction1 = AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION::enumRIGHT2LEFT;
		}
		else if (edn == AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION_NEW::enumHORIN2OUT_NEW) {
			direction0 = AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION::enumRIGHT2LEFT;
			direction1 = AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION::enumLEFT2RIGHT;
		}
		else if (edn == AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION_NEW::enumVEROUT2IN_NEW) {
			direction0 = AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION::enumTOP2BOTTOM;
			direction1 = AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION::enumBOTTOM2TOP;
		}
		else if (edn == AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION_NEW::enumVERIN2OUT_NEW) {
			direction0 = AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION::enumBOTTOM2TOP;
			direction1 = AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION::enumTOP2BOTTOM;
		}

		res = 2;
	}

	return res;
}

bool EdgePointSpline::isSingleDirection(AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION_NEW edn)
{
	bool res = false;

	if (edn == AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION_NEW::enumLEFT2RIGHTNEW ||
		edn == AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION_NEW::enumRIGHT2LEFTNEW ||
		edn == AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION_NEW::enumTOP2BOTTOMNEW ||
		edn == AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION_NEW::enumBOTTOM2TOPNEW) {
		res = true;
	}

	return res;
}

AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION EdgePointSpline::convertEdgeFinderDirectionNew2Old(AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION_NEW edn)
{
	AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION res;

	if (isSingleDirection(edn)) {
		if (edn == AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION_NEW::enumLEFT2RIGHTNEW) {
			res = AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION::enumLEFT2RIGHT;
		}
		else if (edn == AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION_NEW::enumRIGHT2LEFTNEW) {
			res = AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION::enumRIGHT2LEFT;
		}
		else if (edn == AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION_NEW::enumTOP2BOTTOMNEW) {
			res = AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION::enumTOP2BOTTOM;
		}
		else if (edn == AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION_NEW::enumBOTTOM2TOPNEW) {
			res = AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION::enumBOTTOM2TOP;
		}
	}
	else {
		res = AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION::enumNODIRECTION;
	}

	return res;
}

cv::Mat EdgePointSpline::drawEdgeTwoSides(cv::Mat SrcMat)
{
	Mat ColorMat;
	if (SrcMat.type() == CV_8UC1)
		cvtColor(SrcMat, ColorMat, cv::ColorConversionCodes::COLOR_GRAY2BGR);
	else
		ColorMat = SrcMat;

	for (int i = 0; i < EdgePoint0.size(); i++)
	{
		ColorMat.at<Vec3b>(round(EdgePoint.at(i).y), round(EdgePoint.at(i).x))[0] = 0;
		ColorMat.at<Vec3b>(round(EdgePoint.at(i).y), round(EdgePoint.at(i).x))[1] = 0;
		ColorMat.at<Vec3b>(round(EdgePoint.at(i).y), round(EdgePoint.at(i).x))[2] = 255;
	}

	for (int i = 0; i < EdgePoint1.size(); i++)
	{
		ColorMat.at<Vec3b>(round(EdgePoint.at(i).y), round(EdgePoint.at(i).x))[0] = 0;
		ColorMat.at<Vec3b>(round(EdgePoint.at(i).y), round(EdgePoint.at(i).x))[1] = 0;
		ColorMat.at<Vec3b>(round(EdgePoint.at(i).y), round(EdgePoint.at(i).x))[2] = 255;
	}

	return ColorMat;
}

cv::Mat EdgePointSpline::regionThresholdColor(cv::Mat& img, int redLowPixel, int redUpPixel, int greenLowPixel, int greenUpPixel, int blueLowPixel, int blueUpPixel, bool isReverse)
{
	return regionThresholdColorBGR(img, blueLowPixel, blueUpPixel, greenLowPixel, greenUpPixel, redLowPixel, redUpPixel, isReverse);
}

cv::Mat EdgePointSpline::regionThresholdColorBGR(cv::Mat& img, int blueLowPixel, int blueUpPixel, int greenLowPixel, int greenUpPixel, int redLowPixel, int redUpPixel, bool isReverse)
{
	Mat res = img.clone();
	Vec3b* p = nullptr;
	for (int r = 0; r < res.rows; ++r) {
		p = res.ptr<Vec3b>(r);
		for (int c = 0; c < res.cols; ++c) {
			if (p[c][0] >= blueLowPixel && p[c][0] <= blueUpPixel &&
				p[c][1] >= greenLowPixel && p[c][1] <= greenUpPixel &&
				p[c][2] >= redLowPixel && p[c][2] <= redUpPixel) {
				if (!isReverse) {
					p[c][0] = 255;
					p[c][1] = 255;
					p[c][2] = 255;
				}
				else {
					p[c][0] = 0;
					p[c][1] = 0;
					p[c][2] = 0;
				}
			}
			else {
				if (!isReverse) {
					p[c][0] = 0;
					p[c][1] = 0;
					p[c][2] = 0;
				}
				else {
					p[c][0] = 255;
					p[c][1] = 255;
					p[c][2] = 255;
				}
			}
		}
	}

	return res;
}
