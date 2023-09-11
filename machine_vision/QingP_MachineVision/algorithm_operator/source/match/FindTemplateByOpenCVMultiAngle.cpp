#include "FindTemplateByOpenCVMultiAngle.h"

#include "../../../common_tool/source/common_transform/CommonAffineOperator.h"

FindTemplateByOpenCVMultiAngle::FindTemplateByOpenCVMultiAngle()
{

}

FindTemplateByOpenCVMultiAngle::~FindTemplateByOpenCVMultiAngle()
{

}

void FindTemplateByOpenCVMultiAngle::createTemplate(cv::Mat cvmTeachImage)
{
	try
	{
		m_stParam.bIsAvailable = false;

		if (true == cvmTeachImage.empty())
		{
			return;
		}

		m_stParam.cvmTeachImage = cvmTeachImage;

		m_stParam.cvmTemplImage = cvmTeachImage(m_stParam.cvrTemplRoi).clone();

		m_stParam.bIsAvailable = generateMultiAngleTempl(m_stParam);
	}
	catch (std::exception& ex)
	{
		m_stParam.bIsAvailable = false;
	}
}

void FindTemplateByOpenCVMultiAngle::findTemplate(cv::Mat cvmImage, FindTemplResult& stResult)
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

		if (true == m_stParam.bIsFirstMatch)
		{
			for (int iIdx = 0; iIdx < m_stParam.lststAngleTempl.size(); ++iIdx)
			{
				double dCurAngle = m_stParam.lststAngleTempl[iIdx].dAngle;
				FindTemplateByOpenCV& stCurTempl = m_stParam.lststAngleTempl[iIdx].clsTempl;

				FindTemplateByOpenCV::FindTemplResult stInnerResult;
				stCurTempl.findTemplate(cvmImage, stInnerResult);

				if (false == stInnerResult.bIsOK ||
					stInnerResult.lststFindTemplResult.size() <= 0)
				{
					continue;
				}

				if (stInnerResult.lststFindTemplResult[0].dMatchScore >= m_stParam.dMatchScoreThres)
				{
					FindTemplResult::FindTemplOneResult stOneResult;
					stOneResult.cvpMatchCenter = stInnerResult.lststFindTemplResult[0].cvpMatchCenter;
					stOneResult.cvrMatchRoi = stInnerResult.lststFindTemplResult[0].cvrMatchRoi;
					stOneResult.dMatchScore = stInnerResult.lststFindTemplResult[0].dMatchScore;
					stOneResult.dMatchAngle = dCurAngle;
					stResult.lststFindTemplResult.append(stOneResult);
					break;
				}
			}
		}
		else
		{
			if (true == m_stParam.bIsBestMatch)
			{
				for (int iIdx = 0; iIdx < m_stParam.lststAngleTempl.size(); ++iIdx)
				{
					double dCurAngle = m_stParam.lststAngleTempl[iIdx].dAngle;
					FindTemplateByOpenCV& stCurTempl = m_stParam.lststAngleTempl[iIdx].clsTempl;

					FindTemplateByOpenCV::FindTemplResult stInnerResult;
					stCurTempl.findTemplate(cvmImage, stInnerResult);

					if (false == stInnerResult.bIsOK ||
						stInnerResult.lststFindTemplResult.size() <= 0)
					{
						continue;
					}

					if (stInnerResult.lststFindTemplResult[0].dMatchScore >= m_stParam.dMatchScoreThres)
					{
						if (stResult.lststFindTemplResult.size() <= 0 ||
							stInnerResult.lststFindTemplResult[0].dMatchScore > stResult.lststFindTemplResult[0].dMatchScore)
						{
							FindTemplResult::FindTemplOneResult stOneResult;
							stOneResult.cvpMatchCenter = stInnerResult.lststFindTemplResult[0].cvpMatchCenter;
							stOneResult.cvrMatchRoi = stInnerResult.lststFindTemplResult[0].cvrMatchRoi;
							stOneResult.dMatchScore = stInnerResult.lststFindTemplResult[0].dMatchScore;
							stOneResult.dMatchAngle = dCurAngle;
							stResult.lststFindTemplResult.append(stOneResult);
						}
					}
				}
			}
			else
			{
				for (int iIdx = 0; iIdx < m_stParam.lststAngleTempl.size(); ++iIdx)
				{
					double dCurAngle = m_stParam.lststAngleTempl[iIdx].dAngle;
					FindTemplateByOpenCV& stCurTempl = m_stParam.lststAngleTempl[iIdx].clsTempl;

					FindTemplateByOpenCV::FindTemplResult stInnerResult;
					stCurTempl.findTemplate(cvmImage, stInnerResult);

					if (false == stInnerResult.bIsOK ||
						stInnerResult.lststFindTemplResult.size() <= 0)
					{
						continue;
					}

					for (int jIdx = 0; jIdx < stInnerResult.lststFindTemplResult.size(); ++jIdx)
					{
						FindTemplResult::FindTemplOneResult stOneResult;
						stOneResult.cvpMatchCenter = stInnerResult.lststFindTemplResult[jIdx].cvpMatchCenter;
						stOneResult.cvrMatchRoi = stInnerResult.lststFindTemplResult[jIdx].cvrMatchRoi;
						stOneResult.dMatchScore = stInnerResult.lststFindTemplResult[jIdx].dMatchScore;
						stOneResult.dMatchAngle = dCurAngle;
						stResult.lststFindTemplResult.append(stOneResult);
					}
				}

				std::sort(stResult.lststFindTemplResult.begin(), stResult.lststFindTemplResult.end(),
					[](FindTemplResult::FindTemplOneResult& stResult1, FindTemplResult::FindTemplOneResult& stResult2) 
					{
						return (stResult1.dMatchScore > stResult2.dMatchScore);
					});
			}
		}

		rotateMatchRectInResult(stResult);

		stResult.bIsOK = true;
	}
	catch (std::exception& ex)
	{
		stResult.bIsOK = false;
	}
}

bool FindTemplateByOpenCVMultiAngle::generateMultiAngleTempl(FindTemplParam& stParam)
{
	try
	{
		stParam.lststAngleTempl.clear();
		stParam.lststAngleTempl.squeeze();

		if (stParam.lstdTemplAngle.size() <= 0)
		{
			return true;
		}

		cv::Rect cvrTemplRoi = stParam.cvrTemplRoi;
		cv::Point2d cvpTemplCenter(0, 0);
		cvpTemplCenter.x = (double)cvrTemplRoi.x + (double)cvrTemplRoi.width / 2.0;
		cvpTemplCenter.y = (double)cvrTemplRoi.y + (double)cvrTemplRoi.height / 2.0;
		cv::Mat cvmTeachImage = stParam.cvmTeachImage;

		stParam.lststAngleTempl.resize(stParam.lstdTemplAngle.size());
		for (int iIdx = 0; iIdx < stParam.lstdTemplAngle.size(); ++iIdx)
		{
			cv::Mat cvmRotateImage = CommonAffineOperator::rotateImage(cvmTeachImage, cvpTemplCenter, stParam.lstdTemplAngle[iIdx]);

			stParam.lststAngleTempl[iIdx].dAngle = stParam.lstdTemplAngle[iIdx];

			stParam.lststAngleTempl[iIdx].clsTempl.m_stParam.cvrTemplRoi = cvrTemplRoi;
			stParam.lststAngleTempl[iIdx].clsTempl.m_stParam.lstcvrSearchRoi = stParam.lstcvrSearchRoi;
			stParam.lststAngleTempl[iIdx].clsTempl.m_stParam.dMatchScoreThres = stParam.dMatchScoreThres;
			stParam.lststAngleTempl[iIdx].clsTempl.m_stParam.cvtmmMatchMode = stParam.cvtmmMatchMode;
			stParam.lststAngleTempl[iIdx].clsTempl.m_stParam.bIsBestMatch = (stParam.bIsFirstMatch ? true : stParam.bIsBestMatch);
			stParam.lststAngleTempl[iIdx].clsTempl.m_stParam.iStepX = stParam.iStepX;
			stParam.lststAngleTempl[iIdx].clsTempl.m_stParam.iStepY = stParam.iStepY;

			stParam.lststAngleTempl[iIdx].clsTempl.createTemplate(cvmRotateImage);
		}

		return true;
	}
	catch (std::exception& ex)
	{
		return false;
	}
	catch (HalconCpp::HException& ex)
	{
		return false;
	}
}

void FindTemplateByOpenCVMultiAngle::rotateMatchRectInResult(FindTemplResult& stResult)
{
	if (false == stResult.bIsOK)
	{
		return;
	}

	for (int iIdx = 0; iIdx < stResult.lststFindTemplResult.size(); ++iIdx)
	{
		cv::Point2d cvpRectCenter(0, 0);
		cvpRectCenter.x = (double)stResult.lststFindTemplResult[iIdx].cvrMatchRoi.x + (double)stResult.lststFindTemplResult[iIdx].cvrMatchRoi.width / 2.0;
		cvpRectCenter.y = (double)stResult.lststFindTemplResult[iIdx].cvrMatchRoi.y + (double)stResult.lststFindTemplResult[iIdx].cvrMatchRoi.height / 2.0;

		stResult.lststFindTemplResult[iIdx].cvrrMatchRoi = \
			CommonAffineOperator::rotateRect(stResult.lststFindTemplResult[iIdx].cvrMatchRoi, \
				cvpRectCenter, stResult.lststFindTemplResult[iIdx].dMatchAngle);
	}
}


	   