#include "FindTemplateByTwoLines.h"

#include "../../../common_tool/source/common_transform/CvAffineTransform.h"
#include "../../../common_tool/source/common_measure/OneDimensionMeasure.h"

FindTemplateByTwoLines::FindTemplateByTwoLines()
{

}

FindTemplateByTwoLines::~FindTemplateByTwoLines()
{

}

void FindTemplateByTwoLines::createTemplate(cv::Vec4f cvlHoriLine, cv::Vec4f cvlVertLine, ToolCommonEnum::PosType ePosType)
{
	try
	{
		m_stParam.cvlTeachHoriLine = cvlHoriLine;
		m_stParam.cvlTeachVertLine = cvlVertLine;
		m_stParam.eTeachPosType = ePosType;

		if (ePosType == ToolCommonEnum::PosType::TopLeftPos)
		{
			CvAffineTransForm::correctLineDirByX(m_stParam.cvlTeachHoriLine, true);
			CvAffineTransForm::correctLineDirByY(m_stParam.cvlTeachVertLine, true);
		}
		else if (ePosType == ToolCommonEnum::PosType::TopRightPos)
		{
			CvAffineTransForm::correctLineDirByX(m_stParam.cvlTeachHoriLine, false);
			CvAffineTransForm::correctLineDirByY(m_stParam.cvlTeachVertLine, true);
		}
		else if (ePosType == ToolCommonEnum::PosType::BottomLeftPos)
		{
			CvAffineTransForm::correctLineDirByX(m_stParam.cvlTeachHoriLine, true);
			CvAffineTransForm::correctLineDirByY(m_stParam.cvlTeachVertLine, false);
		}
		else if (ePosType == ToolCommonEnum::PosType::BottomRightPos)
		{
			CvAffineTransForm::correctLineDirByX(m_stParam.cvlTeachHoriLine, false);
			CvAffineTransForm::correctLineDirByY(m_stParam.cvlTeachVertLine, false);
		}

		CvAffineTransForm::getCrossPoint(&m_stParam.cvlTeachHoriLine, &m_stParam.cvlTeachVertLine, &m_stParam.cvpTeachCrossPoint);
	
		CvAffineTransForm::getLineAngle1(m_stParam.dTeachAngle, m_stParam.cvlTeachHoriLine);

	}
	catch (std::exception& ex)
	{
		// todo
	}
}

void FindTemplateByTwoLines::findTemplate(FindTemplResult& stResult, cv::Vec4f cvlHoriLine, cv::Vec4f cvlVertLine, ToolCommonEnum::PosType eCurPosType)
{
	try
	{
		stResult.cvlInspectHoriLine = cvlHoriLine;
		stResult.cvlInspectVertLine = cvlVertLine;
		stResult.eInspectPosType = eCurPosType;

		if (eCurPosType == ToolCommonEnum::PosType::TopLeftPos)
		{
			CvAffineTransForm::correctLineDirByX(stResult.cvlInspectHoriLine, true);
			CvAffineTransForm::correctLineDirByY(stResult.cvlInspectVertLine, true);
		}
		else if (eCurPosType == ToolCommonEnum::PosType::TopRightPos)
		{
			CvAffineTransForm::correctLineDirByX(stResult.cvlInspectHoriLine, false);
			CvAffineTransForm::correctLineDirByY(stResult.cvlInspectVertLine, true);
		}
		else if (eCurPosType == ToolCommonEnum::PosType::BottomLeftPos)
		{
			CvAffineTransForm::correctLineDirByX(stResult.cvlInspectHoriLine, true);
			CvAffineTransForm::correctLineDirByY(stResult.cvlInspectVertLine, false);
		}
		else if (eCurPosType == ToolCommonEnum::PosType::BottomRightPos)
		{
			CvAffineTransForm::correctLineDirByX(stResult.cvlInspectHoriLine, false);
			CvAffineTransForm::correctLineDirByY(stResult.cvlInspectVertLine, false);
		}

		CvAffineTransForm::getCrossPoint(&stResult.cvlInspectHoriLine, &stResult.cvlInspectVertLine, &stResult.cvpInspectCrossPoint);

		CvAffineTransForm::getLineAngle1(stResult.dInspectAngle, stResult.cvlInspectHoriLine);

		HalconCpp::HTuple hvTeachRow, hvTeachCol, hvTeachAngle;
		HalconCpp::HTuple hvMatchRow, hvMatchCol, hvMatchAngle;

		hvTeachRow = m_stParam.cvpTeachCrossPoint.y;
		hvTeachCol = m_stParam.cvpTeachCrossPoint.x;
		hvTeachAngle = 0;

		hvMatchRow = stResult.cvpInspectCrossPoint.y;
		hvMatchCol = stResult.cvpInspectCrossPoint.x;

		cv::Vec4f cvlTeachLine;
		cv::Vec4f cvlInspectLine;

		switch (m_stParam.eTeachPosType)
		{
		case ToolCommonEnum::TopLeftPos: cvlTeachLine = m_stParam.cvlTeachHoriLine; break;
		case ToolCommonEnum::TopRightPos: cvlTeachLine = m_stParam.cvlTeachVertLine; break;
		case ToolCommonEnum::BottomLeftPos: cvlTeachLine = m_stParam.cvlTeachVertLine; break;
		case ToolCommonEnum::BottomRightPos: cvlTeachLine = m_stParam.cvlTeachHoriLine; break;
		default: cvlTeachLine = m_stParam.cvlTeachHoriLine; break;
		}

		switch (stResult.eInspectPosType)
		{
		case ToolCommonEnum::TopLeftPos: cvlInspectLine = stResult.cvlInspectHoriLine; break;
		case ToolCommonEnum::TopRightPos: cvlInspectLine = stResult.cvlInspectVertLine; break;
		case ToolCommonEnum::BottomLeftPos: cvlInspectLine = stResult.cvlInspectVertLine; break;
		case ToolCommonEnum::BottomRightPos: cvlInspectLine = stResult.cvlInspectHoriLine; break;
		default: cvlInspectLine = stResult.cvlInspectHoriLine; break;
		}

		double dAngle = 0;
		OneDimensionMeasure::cvGetRotateThetaOfCorrespondLine2Line(cvlInspectLine, cvlTeachLine, dAngle, false);

		hvMatchAngle = dAngle;

		HalconCpp::VectorAngleToRigid(hvMatchRow, hvMatchCol, hvMatchAngle, hvTeachRow, hvTeachCol, 0, &stResult.hvAffineMat);
		HalconCpp::HomMat2dInvert(stResult.hvAffineMat, &stResult.hvAffineMatInv);

		stResult.cvpTeachCenter = m_stParam.cvpTeachCrossPoint;
		stResult.cvpMatchCenter = stResult.cvpInspectCrossPoint;
		stResult.dMatchAngle = dAngle;

	}
	catch (std::exception& ex)
	{
		// todo
	}
}


	   