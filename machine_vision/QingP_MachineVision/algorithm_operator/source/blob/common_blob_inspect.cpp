#include "common_blob_inspect.h"

CommonBlobInspect::CommonBlobInspect()
{

}

CommonBlobInspect::~CommonBlobInspect()
{

}

void CommonBlobInspect::findBlob(HalconCpp::HObject hoImage, FindBlobResult& stResult)
{
	try
	{
		stResult.bIsDetectOK = false;
		for (int iIdx = 0; iIdx < stResult.lststBlobResult.size(); ++iIdx)
		{
			stResult.lststBlobResult[iIdx].veccvpiContour.clear();
		}
		stResult.lststBlobResult.clear();
		stResult.lststBlobResult.squeeze();

		HalconCpp::HObject hoReduceImage;
		HalconCpp::ReduceDomain(hoImage, m_stParam.hoBlobRegion, &hoReduceImage);

		HalconCpp::HObject hoBinRegion;
		HalconCpp::Threshold(hoReduceImage, &hoBinRegion, m_stParam.iBlobGrayMin, m_stParam.iBlobGrayMax);

		HalconCpp::HObject hoConnectedRegions;
		HalconCpp::Connection(hoBinRegion, &hoConnectedRegions);

		if (true == m_stParam.bIsFilterSmallArea)
		{
			HalconCpp::SelectShape(hoConnectedRegions, &hoConnectedRegions, "area", "and", m_stParam.dFilterBlobArea, 999999999);
		}

		if (true == m_stParam.bIsEntireBlob)
		{
			HalconCpp::Union1(hoConnectedRegions, &hoConnectedRegions);
		}

		HalconCpp::HTuple hvBlobNum;
		HalconCpp::CountObj(hoConnectedRegions, &hvBlobNum);

		for (int iIdx = 0; iIdx < hvBlobNum.I(); ++iIdx)
		{
			try
			{
				HalconCpp::HObject hoSelectedObj;
				HalconCpp::SelectObj(hoConnectedRegions, &hoSelectedObj, iIdx + 1);

				HalconCpp::HTuple hvArea, hvRow, hvCol;
				HalconCpp::AreaCenter(hoSelectedObj, &hvArea, &hvRow, &hvCol);

				HalconCpp::HTuple hvPhi, hvLen1, hvLen2;
				HalconCpp::SmallestRectangle2(hoSelectedObj, &hvRow, &hvCol, &hvPhi, &hvLen1, &hvLen2);

				double dArea = hvArea.D();
				double dWidth = 2 * hvLen1.D();
				double dHeight = 2 * hvLen2.D();

				if (dWidth > dHeight)
				{
					qSwap(dWidth, dHeight);
				}

				bool bIsBlob = false;
				if (dArea >= m_stParam.dAreaMin)
				{
					bIsBlob = true;
				}
				else
				{
					if (true == m_stParam.bIsWidthHeightOr)
					{
						if (dWidth >= m_stParam.dWidthMin || dHeight >= m_stParam.dHeightMin)
						{
							bIsBlob = true;
						}
					}
					else
					{
						if (dWidth >= m_stParam.dWidthMin && dHeight >= m_stParam.dHeightMin)
						{
							bIsBlob = true;
						}
					}
				}

				if (true == bIsBlob)
				{
					FindBlobResult::OneBlobResult stOneBlob;
					stOneBlob.dBlobArea = dArea;
					stOneBlob.dBlobWidth = dWidth;
					stOneBlob.dBlobHeight = dHeight;

					stOneBlob.cvrrBlobRrect = CommonTool_CommonTransform::getRotatedRectOfOneRegion(hoSelectedObj, false, true, true);

					if (true == m_stParam.bNeedContour)
					{
						stOneBlob.veccvpiContour = CommonTool_CommonTransform::getRegionContourPointsInt(hoSelectedObj);
					}

					stResult.lststBlobResult.append(stOneBlob);
				}
				
			}
			catch (HalconCpp::HException& ex)
			{
				continue;
			}
			catch (std::exception& ex)
			{
				continue;
			}
		}

		stResult.bIsDetectOK = true;
	}
	catch (HalconCpp::HException& ex)
	{
		stResult.bIsDetectOK = false;
	}
	catch (std::exception& ex)
	{
		stResult.bIsDetectOK = false;
	}
}
