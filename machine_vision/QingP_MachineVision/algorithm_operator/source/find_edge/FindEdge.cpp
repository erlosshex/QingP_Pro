#include "FindEdge.h"

FindEdge::FindEdge()
{
}

void FindEdge::setSplineType(AlgorithmOperatorCommonEnum::SplineType eSplineType)
{
	if (eSplineType == AlgorithmOperatorCommonEnum::SplineType::CubicSpl)  eps.m_type = AlgorithmOperatorCommonEnum::SPLINE_TYPE::enumCubicSpline;
	else if (eSplineType == AlgorithmOperatorCommonEnum::SplineType::BSpl) eps.m_type = AlgorithmOperatorCommonEnum::SPLINE_TYPE::enumBSpline;
}

uint FindEdge::findEdgePoints(QVector<cv::Point2d> & qveccvpEdgePoints, cv::Mat mImg, cv::Rect cvrRoi, AlgorithmOperatorCommonEnum::FindEdgeDirectionType eDirection, AlgorithmOperatorCommonEnum::FindEdgeModeType eEdgeMode, int iStep, int iMinGrad, int iEdgeValue, double dAccuracy, AlgorithmOperatorCommonEnum::FindEdgeSearchStyleType eSearchStyle)
{
	try
	{
		qveccvpEdgePoints.clear();

		if (mImg.empty()) return ALGORITHM_OPERATOR_FINDEDGE_PARAERROR;
		bool isRoiValid = false;
		CommonTool_NormalTool::isRoiValid(isRoiValid, mImg, cvrRoi);
		if (!isRoiValid) return ALGORITHM_OPERATOR_FINDEDGE_PARAERROR;

		AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION eDirectionChanged = AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION::enumLEFT2RIGHT;
		AlgorithmOperatorCommonEnum::EDGEFINDER_MODE eEdgeModeChanged = AlgorithmOperatorCommonEnum::EDGEFINDER_MODE::enumBLACK2WHITE;
		AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE eSearchStyleChanged = AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE::enumFRISTGRAD;

		switch (eDirection)
		{
		case AlgorithmOperatorCommonEnum::FindEdgeDirectionType::Left2Right: eDirectionChanged = AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION::enumLEFT2RIGHT; break;
		case AlgorithmOperatorCommonEnum::FindEdgeDirectionType::Right2Left: eDirectionChanged = AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION::enumRIGHT2LEFT; break;
		case AlgorithmOperatorCommonEnum::FindEdgeDirectionType::Top2Bottom: eDirectionChanged = AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION::enumTOP2BOTTOM; break;
		case AlgorithmOperatorCommonEnum::FindEdgeDirectionType::Bottom2Top: eDirectionChanged = AlgorithmOperatorCommonEnum::EDGEFINDER_DIRECTION::enumBOTTOM2TOP; break;
		default: break;
		}

		switch (eEdgeMode)
		{
		case AlgorithmOperatorCommonEnum::FindEdgeModeType::Black2White: eEdgeModeChanged = AlgorithmOperatorCommonEnum::EDGEFINDER_MODE::enumBLACK2WHITE; break;
		case AlgorithmOperatorCommonEnum::FindEdgeModeType::White2Black: eEdgeModeChanged = AlgorithmOperatorCommonEnum::EDGEFINDER_MODE::enumWHITE2BLACK; break;
		default: break;
		}

		switch (eSearchStyle)
		{
		case AlgorithmOperatorCommonEnum::FindEdgeSearchStyleType::MaxGrad: eSearchStyleChanged = AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE::enumMAXGRAD; break;
		case AlgorithmOperatorCommonEnum::FindEdgeSearchStyleType::FirstGrad: eSearchStyleChanged = AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE::enumFRISTGRAD; break;
		case AlgorithmOperatorCommonEnum::FindEdgeSearchStyleType::AutoGrad: eSearchStyleChanged = AlgorithmOperatorCommonEnum::EDGEFINDER_SEARCHSTYLE::enumAUTOGRAD; break;
		default: break;
		}

		eps.FindEdgePoint(mImg, cvrRoi, 0, 0, 0, 0, eDirectionChanged, eEdgeModeChanged, iStep, iMinGrad, iEdgeValue, dAccuracy, eSearchStyleChanged);

		for (auto& p : eps.EdgePoint) qveccvpEdgePoints.push_back(p);

		return ALGORITHM_OPERATOR_FINDEDGE_OK;
	}
	catch (std::exception& ex)
	{
		return ALGORITHM_OPERATOR_FINDEDGE_UNKNOWERROR;
	}
}

uint FindEdge::findDualEdgePoints(QVector<cv::Point2d>& qveccvpEdgePoints1, QVector<cv::Point2d> qveccvpEdgePoints2, cv::Mat mImg, cv::Rect cvrRoi, AlgorithmOperatorCommonEnum::FindEdgeDualDirectionType eDirection, AlgorithmOperatorCommonEnum::FindEdgeModeType eEdgeMode, int iStep, int iMinGrad, int iEdgeValue, double dAccuracy, AlgorithmOperatorCommonEnum::FindEdgeSearchStyleType eSearchStyle)
{
	try
	{
		qveccvpEdgePoints1.clear();
		qveccvpEdgePoints2.clear();

		AlgorithmOperatorCommonEnum::FindEdgeDirectionType eDirectionChanged1 = AlgorithmOperatorCommonEnum::FindEdgeDirectionType::Left2Right;
		AlgorithmOperatorCommonEnum::FindEdgeDirectionType eDirectionChanged2 = AlgorithmOperatorCommonEnum::FindEdgeDirectionType::Right2Left;

		switch (eDirection)
		{
		case AlgorithmOperatorCommonEnum::FindEdgeDualDirectionType::HoriIn2Out: eDirectionChanged1 = AlgorithmOperatorCommonEnum::FindEdgeDirectionType::Right2Left; eDirectionChanged2 = AlgorithmOperatorCommonEnum::FindEdgeDirectionType::Left2Right; break;
		case AlgorithmOperatorCommonEnum::FindEdgeDualDirectionType::HoriOut2In: eDirectionChanged1 = AlgorithmOperatorCommonEnum::FindEdgeDirectionType::Left2Right; eDirectionChanged2 = AlgorithmOperatorCommonEnum::FindEdgeDirectionType::Right2Left; break;
		case AlgorithmOperatorCommonEnum::FindEdgeDualDirectionType::VertIn2Out: eDirectionChanged1 = AlgorithmOperatorCommonEnum::FindEdgeDirectionType::Bottom2Top; eDirectionChanged2 = AlgorithmOperatorCommonEnum::FindEdgeDirectionType::Top2Bottom; break;
		case AlgorithmOperatorCommonEnum::FindEdgeDualDirectionType::VertOut2In: eDirectionChanged1 = AlgorithmOperatorCommonEnum::FindEdgeDirectionType::Top2Bottom; eDirectionChanged2 = AlgorithmOperatorCommonEnum::FindEdgeDirectionType::Bottom2Top; break;
		default: break;
		}

		if (ALGORITHM_OPERATOR_FINDEDGE_OK != findEdgePoints(qveccvpEdgePoints1, mImg, cvrRoi, eDirectionChanged1, eEdgeMode, iStep, iMinGrad, iEdgeValue, dAccuracy, eSearchStyle) ||
			ALGORITHM_OPERATOR_FINDEDGE_OK != findEdgePoints(qveccvpEdgePoints2, mImg, cvrRoi, eDirectionChanged2, eEdgeMode, iStep, iMinGrad, iEdgeValue, dAccuracy, eSearchStyle))
		{
			return ALGORITHM_OPERATOR_FINDEDGEDUAL_FAIL;
		}

		return ALGORITHM_OPERATOR_FINDEDGEDUAL_OK;
	}
	catch (std::exception& ex)
	{
		return ALGORITHM_OPERATOR_FINDEDGEDUAL_UNKNOWERROR;
	}
}

void FindEdge::ransacFilterHorVer(QVector<cv::Point2d>& qveccvpRetPoints, QVector<cv::Point2d> qveccvpPoints, int iThreshold, int iStep, bool bIsHor, int iAngleScope)
{
	try
	{
		qveccvpRetPoints.clear();
		std::vector<cv::Point2d> veccvpPoints;
		for (auto p : qveccvpPoints) veccvpPoints.push_back(p);
		std::vector<cv::Point2d> veccvpRansacFilterPoints = RansacAlgorithm::RansacFilterHorVer(veccvpPoints, iThreshold, iStep, bIsHor, iAngleScope);
		for (auto p : veccvpRansacFilterPoints) qveccvpRetPoints.push_back(p);
	}
	catch (std::exception& ex)
	{
		qveccvpRetPoints.clear();
	}
}


