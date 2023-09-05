#include "CommonTransform.h"

void CommonTool_CommonTransform::hoRegionXld2CvRotatedRect(HalconCpp::HObject hoRegionXld, cv::RotatedRect& cvrrRect, bool bSwapLen1Len2)
{
	try
	{
		bool bIsRegionXldEmpty = false;
		CommonTool_CommonJudge::isHoImageEmpty(hoRegionXld, bIsRegionXldEmpty);
		if (bIsRegionXldEmpty) return;

		HalconCpp::HTuple hvRow, hvCol, hvPhi, hvLen1, hvLen2;
		HalconCpp::SmallestRectangle2Xld(hoRegionXld, &hvRow, &hvCol, &hvPhi, &hvLen1, &hvLen2);

		if (hvRow.TupleLength().I() <= 0) return;

		cv::Point2f cvpfCenter;
		cvpfCenter.x = hvCol.D();
		cvpfCenter.y = hvRow.D();

		cv::Size2f cvsfSize;
		if (bSwapLen1Len2)
		{
			cvsfSize.width = 2 * hvLen2.D();
			cvsfSize.height = 2 * hvLen1.D();
		}
		else
		{
			cvsfSize.width = 2 * hvLen1.D();
			cvsfSize.height = 2 * hvLen2.D();
		}

		float fAngle = hvPhi.D();

		cvrrRect = cv::RotatedRect(cvpfCenter, cvsfSize, fAngle);

	}
	catch (HalconCpp::HException& ex)
	{
		// toto
	}

}

cv::Rect CommonTool_CommonTransform::getOpenCVRectFromRegion(HalconCpp::HObject hoRegion)
{
	try
	{
		if (!hoRegion.IsInitialized())
		{
			return cv::Rect();
		}

		HalconCpp::HTuple hvRow1, hvCol1, hvRow2, hvCol2;

		HalconCpp::SmallestRectangle1(hoRegion, &hvRow1, &hvCol1, &hvRow2, &hvCol2);

		cv::Rect cvrRect;
		cvrRect.x = hvCol1.D();
		cvrRect.y = hvRow1.D();
		cvrRect.width = hvCol2.D() - hvCol1.D();
		cvrRect.height = hvRow2.D() - hvRow1.D();

		return cvrRect;
	}
	catch (HalconCpp::HException& ex)
	{
		return cv::Rect();
	}
	catch (std::exception& ex)
	{
		return cv::Rect();
	}
}

void CommonTool_CommonTransform::getRotatedRectOfRegion(HalconCpp::HObject hoRegion, QList<cv::RotatedRect>* plistRotatedRect)
{
	try
	{
		bool bIsRegionXldEmpty = false;
		CommonTool_CommonJudge::isHoImageEmpty(hoRegion, bIsRegionXldEmpty);
		if (bIsRegionXldEmpty) return;

		HalconCpp::HObject hoConnectionRegions;
		HalconCpp::Connection(hoRegion, &hoConnectionRegions);
		getRotatedRectOfConnectionRegion(hoConnectionRegions, plistRotatedRect);
	}
	catch (HalconCpp::HException& ex)
	{
		// toto
	}
}

void CommonTool_CommonTransform::getRotatedRectOfConnectionRegion(HalconCpp::HObject hoConnectioinRegion, QList<cv::RotatedRect>* plistRotatedRect)
{
	try
	{
		bool bIsRegionXldEmpty = false;
		CommonTool_CommonJudge::isHoImageEmpty(hoConnectioinRegion, bIsRegionXldEmpty);
		if (bIsRegionXldEmpty) return;

		HalconCpp::HTuple hvRow, hvCol, hvPhi, hvLen1, hvLen2;
		HalconCpp::SmallestRectangle2(hoConnectioinRegion, &hvRow, &hvCol, &hvPhi, &hvLen1, &hvLen2);

		if (hvRow.TupleLength().I() <= 0) return;

		for (int iIdx = 0; iIdx < hvRow.Length(); iIdx++)
		{
			cv::Point2f cvpfCenter;
			cvpfCenter.x = hvCol[iIdx].D();
			cvpfCenter.y = hvRow[iIdx].D();

			cv::Size2f cvsfSize;
			cvsfSize.width = 2 * hvLen1[iIdx].D();
			cvsfSize.height = 2 * hvLen2[iIdx].D();

			float fAngle = hvPhi[iIdx].D();

			cv::RotatedRect cvrrRect = cv::RotatedRect(cvpfCenter, cvsfSize, fAngle);
			plistRotatedRect->append(cvrrRect);
		}
	}
	catch (HalconCpp::HException& ex)
	{
		// toto
	}
}

cv::RotatedRect CommonTool_CommonTransform::getRotatedRectOfOneRegion(HalconCpp::HObject hoRegion, bool bIsSwapLen12, bool bIsUseDeg, bool bIsFlipAngle)
{
	try
	{
		HalconCpp::HTuple hvRow, hvCol, hvPhi, hvLen1, hvLen2;
		HalconCpp::SmallestRectangle2(hoRegion, &hvRow, &hvCol, &hvPhi, &hvLen1, &hvLen2);

		if (hvRow.TupleLength().I() <= 0)
		{
			return cv::RotatedRect();
		}

		int iIdx = 0;

		cv::Point2f cvpfCenter;
		cvpfCenter.x = hvCol[iIdx].D();
		cvpfCenter.y = hvRow[iIdx].D();

		float fLen1 = 2 * hvLen1[iIdx].D();
		float fLen2 = 2 * hvLen2[iIdx].D();

		if (fLen1 < fLen2)
		{
			std::swap(fLen1, fLen2);
		}

		if (bIsSwapLen12)
		{
			std::swap(fLen1, fLen2);
		}

		cv::Size2f cvsfSize(fLen1, fLen2);

		float fAngle = hvPhi[iIdx].D();

		if (bIsUseDeg)
		{
			fAngle = fAngle * 180.0 / CV_PI;
		}

		if (bIsFlipAngle)
		{
			fAngle = -fAngle;
		}

		cv::RotatedRect cvrrRect = cv::RotatedRect(cvpfCenter, cvsfSize, fAngle);

		return cvrrRect;

	}
	catch (HalconCpp::HException& ex)
	{
		return cv::RotatedRect();
	}
}

std::vector<cv::Point> CommonTool_CommonTransform::getRegionContourPointsInt(HalconCpp::HObject hoRegion)
{
	try
	{
		HalconCpp::HObject hoContour;
		HalconCpp::GenContourRegionXld(hoRegion, &hoContour, "border");

		HalconCpp::HTuple hvRows, hvCols;
		HalconCpp::GetContourXld(hoContour, &hvRows, &hvCols);
		std::vector<cv::Point> veccvpiContour;
		for (int iIdx = 0; iIdx < hvRows.TupleLength(); ++iIdx)
		{
			veccvpiContour.push_back(cv::Point(hvCols[iIdx].D(), hvRows[iIdx].D()));
		}
		return veccvpiContour;
	}
	catch (HalconCpp::HException& ex)
	{
		return std::vector<cv::Point>();
	}
	catch (std::exception& ex)
	{
		return std::vector<cv::Point>();
	}
}

