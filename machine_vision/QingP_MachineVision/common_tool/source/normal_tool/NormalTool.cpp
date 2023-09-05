#include "NormalTool.h"

HalconCpp::HObject CommonTool_NormalTool::genRectRegion(cv::Rect cvrRect)
{
	try
	{
		HalconCpp::HTuple hvRow1, hvCol1, hvRow2, hvCol2;

		hvRow1 = cvrRect.y;
		hvCol1 = cvrRect.x;
		hvRow2 = cvrRect.y + cvrRect.height - 1;
		hvCol2 = cvrRect.x + cvrRect.width - 1;

		HalconCpp::HObject hoRegion;
		HalconCpp::GenEmptyObj(&hoRegion);
		HalconCpp::GenRectangle1(&hoRegion, hvRow1, hvCol1, hvRow2, hvCol2);

		return hoRegion;
	}
	catch (HalconCpp::HException)
	{
		return HalconCpp::HObject();
	}
}

HalconCpp::HObject CommonTool_NormalTool::genRectRegions(QList<cv::Rect> lstcvrRoi)
{
	try
	{
		HalconCpp::HObject hoRectRegions;
		HalconCpp::GenEmptyObj(&hoRectRegions);
		for (const auto& rect : lstcvrRoi)
		{
			HalconCpp::HObject hoTempRegion = genRectRegion(rect);
			HalconCpp::Union2(hoRectRegions, hoTempRegion, &hoRectRegions);
		}
		return hoRectRegions;
	}
	catch (HalconCpp::HException& ex)
	{
		return HalconCpp::HObject();
	}
}

HalconCpp::HObject CommonTool_NormalTool::genRectShieldRegion(cv::Rect cvrRoi, QList<cv::Rect> lstcvrShieldRoi)
{
	try
	{
		HalconCpp::HObject hoRectRegion = genRectRegion(cvrRoi);
		HalconCpp::HObject hoShieldRectRegion = genRectRegions(lstcvrShieldRoi);
		HalconCpp::HObject hoFinalRegion;
		HalconCpp::Difference(hoRectRegion, hoShieldRectRegion, &hoFinalRegion);
		return hoFinalRegion;
	}
	catch (HalconCpp::HException& ex)
	{
		return HalconCpp::HObject();
	}
}

HalconCpp::HObject CommonTool_NormalTool::genRectShieldRegion(QList<cv::Rect> lstcvrRoi, QList<cv::Rect> lstcvrShieldRoi)
{
	try
	{
		HalconCpp::HObject hoRectRegion = genRectRegions(lstcvrRoi);
		HalconCpp::HObject hoRectShieldRegion = genRectRegions(lstcvrShieldRoi);
		HalconCpp::HObject hoRegion;
		HalconCpp::Difference(hoRectRegion, hoRectShieldRegion, &hoRegion);
		return hoRegion;
	}
	catch (HalconCpp::HException& ex)
	{
		return HalconCpp::HObject();
	}
}

HalconCpp::HObject CommonTool_NormalTool::genRectShieldRegion(QList<QPair<cv::Rect, QList<cv::Rect>>> lstpairRectShield)
{
	try
	{
		HalconCpp::HObject hoRegion;
		HalconCpp::GenEmptyObj(&hoRegion);
		for (const auto& item : lstpairRectShield)
		{
			HalconCpp::HObject hoTempRegion = genRectShieldRegion(item.first, item.second);
			HalconCpp::Union2(hoRegion, hoTempRegion, &hoRegion);
		}
		return hoRegion;
	}
	catch (HalconCpp::HException& ex)
	{
		return HalconCpp::HObject();
	}
}

cv::Rect CommonTool_NormalTool::getOpenCVRectFromRegion(HalconCpp::HObject hoRegion)
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

double CommonTool_NormalTool::overlapOfTwoRect(const cv::Rect& box1, const cv::Rect& box2)
{
	if (box1.x > box2.x + box2.width) { return 0.0; }
	if (box1.y > box2.y + box2.height) { return 0.0; }
	if (box1.x + box1.width < box2.x) { return 0.0; }
	if (box1.y + box1.height < box2.y) { return 0.0; }
	float colInt = min(box1.x + box1.width, box2.x + box2.width) - max(box1.x, box2.x);
	float rowInt = min(box1.y + box1.height, box2.y + box2.height) - max(box1.y, box2.y);
	float intersection = colInt * rowInt;
	float area1 = box1.width * box1.height;
	float area2 = box2.width * box2.height;
	return intersection / (area1 + area2 - intersection);
}

double CommonTool_NormalTool::overlapOfTwoRectList(const QList<cv::Rect>& lstcvrRect1, const QList<cv::Rect>& lstcvrRect2)
{
	try
	{
		HalconCpp::HObject hoRectRegion1 = genRectRegions(lstcvrRect1);
		HalconCpp::HObject hoRectRegion2 = genRectRegions(lstcvrRect2);
		HalconCpp::HObject hoUnionRegion;
		HalconCpp::HObject hoIntersectionRegion;
		HalconCpp::Union2(hoRectRegion1, hoRectRegion2, &hoUnionRegion);
		HalconCpp::Intersection(hoRectRegion1, hoRectRegion2, &hoIntersectionRegion);
		HalconCpp::HTuple hvUnionRow, hvUnionCol, hvUnionArea;
		HalconCpp::HTuple hvIntersectionRow, hvIntersectionCol, hvIntersectionArea;
		HalconCpp::AreaCenter(hoUnionRegion, &hvUnionArea, &hvUnionRow, &hvUnionCol);
		HalconCpp::AreaCenter(hoIntersectionRegion, &hvIntersectionArea, &hvIntersectionRow, &hvIntersectionCol);
		if (hvUnionArea.Length() <= 0)
		{
			return -1;
		}
		if (hvIntersectionArea.Length() <= 0)
		{
			return -1;
		}
		if (hvUnionArea.D() <= 0.0001)
		{
			return -1;
		}
		return (hvIntersectionArea.D() / hvUnionArea.D());
	}
	catch (HalconCpp::HException& ex)
	{
		return -1;
	}
}



