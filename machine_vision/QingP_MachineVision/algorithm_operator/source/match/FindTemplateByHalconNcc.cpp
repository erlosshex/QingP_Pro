#include "FindTemplateByHalconNcc.h"

FindTemplateByHalconNcc::FindTemplateByHalconNcc()
{

}

FindTemplateByHalconNcc::~FindTemplateByHalconNcc()
{

}

void FindTemplateByHalconNcc::createTemplate(HalconCpp::HObject hoTeachImage)
{
	try
	{
		m_stParam.bIsAvailable = false;

		m_stParam.hoTemplImage = hoTeachImage;
		HalconCpp::CreateNccModel(hoTeachImage, \
			"auto", \
			m_stParam.hvAngleStart, m_stParam.hvAngleExtent, \
			"auto", "use_polarity", \
			& m_stParam.hvModelID
		);

		m_stParam.bIsAvailable = true;
	}
	catch (HalconCpp::HException& ex)
	{
		m_stParam.bIsAvailable = false;
	}
}

void FindTemplateByHalconNcc::findTemplate(HalconCpp::HObject hoImage, FindTemplResult& stResult)
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

		HalconCpp::HTuple hvMatchRows, hvMatchCols, hvMatchAngles, hvMatchScores;
		HalconCpp::FindNccModel(hoImage, m_stParam.hvModelID, \
			m_stParam.hvAngleStart, m_stParam.hvAngleExtent, m_stParam.hvMinScore, \
			m_stParam.hvNumMatches, m_stParam.hvMaxOverlap, "true", m_stParam.hvNumLevels, \
			& hvMatchRows, &hvMatchCols, &hvMatchAngles, &hvMatchScores);

		if (hvMatchRows.Length() > 0)
		{
			for (int iIdx = 0; iIdx < hvMatchRows.Length(); ++iIdx)
			{
				if (hvMatchScores[iIdx].D() >= m_stParam.hvScoreThres.D())
				{
					FindTemplResult::FindTemplOneResult stOneResult;
					stOneResult.hvMatchRow = hvMatchRows[iIdx].D();
					stOneResult.hvMatchCol = hvMatchCols[iIdx].D();
					stOneResult.hvMatchAngle = hvMatchAngles[iIdx].D();
					stOneResult.hvMatchScore = hvMatchScores[iIdx].D();

					stResult.lststFindTemplResult.append(stOneResult);
				}
			}
		}

		stResult.bIsOK = true;
	}
	catch (HalconCpp::HException& ex)
	{
		stResult.bIsOK = false;
	}
}



	   