#include "FindTemplateByOpenCV.h"

#include "../../../common_tool/source/normal_tool/NormalTool.h"

FindTemplateByOpenCV::FindTemplateByOpenCV()
{

}

FindTemplateByOpenCV::~FindTemplateByOpenCV()
{

}

void FindTemplateByOpenCV::createTemplate(cv::Mat cvmTeachImage)
{
	try
	{
		m_stParam.bIsAvailable = false;

		if (true == cvmTeachImage.empty())
		{
			return;
		}

		m_stParam.cvmTemplImage = cvmTeachImage(m_stParam.cvrTemplRoi).clone();

		m_stParam.bIsAvailable = true;
	}
	catch (std::exception& ex)
	{
		m_stParam.bIsAvailable = false;
	}
}

void FindTemplateByOpenCV::findTemplate(cv::Mat cvmImage, FindTemplResult& stResult)
{
	try
	{
		stResult.bIsOK = false;
		stResult.lststFindTemplResult.clear();
		stResult.lststFindTemplResult.squeeze();

		if (false == m_stParam.bIsAvailable)
		{
			return;
		}

		cv::Mat cvmMatchResult;
		cv::matchTemplate(cvmImage, m_stParam.cvmTemplImage, cvmMatchResult, m_stParam.cvtmmMatchMode);
		cv::normalize(cvmMatchResult, cvmMatchResult, 0, 1, cv::NORM_MINMAX);

		if (true == m_stParam.bIsBestMatch)
		{
			double dMinValue = 0, dMaxValue = 0;
			cv::Point cvpiMinLoc, cvpiMaxLoc;
			cv::Point2d cvpMatchLoc;
			double dMatchScore = 0;
			cv::Rect cvrMatchRoi;
			cv::minMaxLoc(cvmMatchResult, &dMinValue, &dMaxValue, &cvpiMinLoc, &cvpiMaxLoc);
			if (m_stParam.cvtmmMatchMode == cv::TemplateMatchModes::TM_SQDIFF ||
				m_stParam.cvtmmMatchMode == cv::TemplateMatchModes::TM_SQDIFF_NORMED)
			{
				cvpMatchLoc = cvpiMinLoc;
				dMatchScore = 1 - dMinValue;
			}
			else
			{
				cvpMatchLoc = cvpiMaxLoc;
				dMatchScore = dMaxValue;
			}
			cvrMatchRoi = cv::Rect(
				cv::Point(cvpMatchLoc.x, cvpMatchLoc.y), \
				cv::Point(cvpMatchLoc.x + m_stParam.cvmTemplImage.cols, cvpMatchLoc.y + m_stParam.cvmTemplImage.rows)
			);
			cvpMatchLoc += cv::Point2d(m_stParam.cvmTemplImage.cols / 2.0, m_stParam.cvmTemplImage.rows / 2.0);
			
			if (dMatchScore >= m_stParam.dMatchScoreThres && 
				CommonTool_NormalTool::isMatchPointInSearchRegion(cvpMatchLoc, m_stParam.lstcvrSearchRoi))
			{
				FindTemplResult::FindTemplOneResult stOneResult;
				stOneResult.cvpMatchCenter = cvpMatchLoc;
				stOneResult.cvrMatchRoi = cvrMatchRoi;
				stOneResult.dMatchScore = dMatchScore;

				stResult.lststFindTemplResult.append(stOneResult);
			}
		}
		else
		{
			int iResultRows = cvmMatchResult.rows;
			int iResultCols = cvmMatchResult.cols;
			for (int iRow = 0; iRow < iResultRows; iRow += m_stParam.iStepY)
			{
				float* pdRowValues = cvmMatchResult.ptr<float>(iRow);
				for (int iCol = 0; iCol < iResultCols; iCol += m_stParam.iStepX)
				{
					double dMatchScore = 0;
					if (m_stParam.cvtmmMatchMode == cv::TemplateMatchModes::TM_SQDIFF ||
						m_stParam.cvtmmMatchMode == cv::TemplateMatchModes::TM_SQDIFF_NORMED)
					{
						dMatchScore = 1.0 - (double)pdRowValues[iCol];
					}
					else
					{
						dMatchScore = pdRowValues[iCol];
					}
					cv::Point2d cvpMatchLoc(iCol + m_stParam.cvmTemplImage.cols / 2.0, iRow + m_stParam.cvmTemplImage.rows / 2.0);

					if (dMatchScore < m_stParam.dMatchScoreThres ||
						false == CommonTool_NormalTool::isMatchPointInSearchRegion(cvpMatchLoc, m_stParam.lstcvrSearchRoi))
					{
						continue;
					}

					cv::Rect cvrMatchRoi(cv::Point(iCol, iRow), cv::Point(iCol+m_stParam.cvmTemplImage.cols, iRow+m_stParam.cvmTemplImage.rows));

					FindTemplResult::FindTemplOneResult stOneResult;
					stOneResult.cvpMatchCenter = cvpMatchLoc;
					stOneResult.cvrMatchRoi = cvrMatchRoi;
					stOneResult.dMatchScore = dMatchScore;

					stResult.lststFindTemplResult.append(stOneResult);
				}
			}
			std::sort(stResult.lststFindTemplResult.begin(), stResult.lststFindTemplResult.end(),
				[](FindTemplResult::FindTemplOneResult& stResult1, FindTemplResult::FindTemplOneResult& stResult2) {
					return (stResult1.dMatchScore > stResult2.dMatchScore);
				});
		}

		stResult.bIsOK = true;
	}
	catch (std::exception& ex)
	{
		stResult.bIsOK = false;
	}
}



	   