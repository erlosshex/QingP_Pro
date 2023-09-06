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

int CommonTool_NormalTool::isRoiValid(bool& bIsValid, int iImageWidth, int iImageHeight, cv::Rect rRoi)
{
	try
	{
		bIsValid = false;
		cv::Rect cvrImageRect(0, 0, iImageWidth, iImageHeight);
		if (cvrImageRect.contains(rRoi.tl()) &&
			cvrImageRect.contains(cv::Point(rRoi.br().x - 1, rRoi.tl().y)) &&
			cvrImageRect.contains(cv::Point(rRoi.tl().x, rRoi.br().y - 1)) &&
			cvrImageRect.contains(rRoi.br() - cv::Point(1, 1)))
		{
			bIsValid = true;
		}
		else
		{
			bIsValid = false;
		}
		return TOOL_COMMON_OK;
	}
	catch (cv::Exception& ex)
	{
		bIsValid = false;
		return TOOL_COMMON_UNKNOWERROR;
	}
}

int CommonTool_NormalTool::isRoiValid(bool& bIsValid, cv::Mat mImage, cv::Rect rRoi)
{
	try
	{
		bIsValid = false;
		if (mImage.empty()) return TOOL_COMMON_PARAERROR;
		return isRoiValid(bIsValid, mImage.cols, mImage.rows, rRoi);
	}
	catch (cv::Exception& ex)
	{
		bIsValid = false;
		return TOOL_COMMON_UNKNOWERROR;
	}
	catch (std::exception& ex)
	{
		bIsValid = false;
		return TOOL_COMMON_UNKNOWERROR;
	}
}

void CommonTool_NormalTool::ensureRectROIOfImage(const cv::Rect _crRoi, const int _iImageWidth, const int _iImageHeight, cv::Rect& _crEnsureRoi)
{
	//记录传入的ROI区域的左上角和右下角的拐点信息
	int iRoiTLX = _crRoi.x;
	int iRoiTLY = _crRoi.y;
	int iRoiBRX = _crRoi.x + _crRoi.width - 1;
	int iRoiBRY = _crRoi.y + _crRoi.height - 1;

	//修改后的ROI区域的左上角和右下角的拐点信息
	int iEnsureTLX = 0, iEnsureTLY = 0, iEnsureBRX = 0, iEnsureBRY = 0;

	//判断是否溢出图片区域，并做对应修正
	if (iRoiTLX < 0)
		iEnsureTLX = 0;
	else if (iRoiTLX >= _iImageWidth)
	{
		_crEnsureRoi = cv::Rect(-1, -1, -1, -1);
		return;
	}
	else
		iEnsureTLX = iRoiTLX;

	if (iRoiTLY < 0)
		iEnsureTLY = 0;
	else if (iRoiTLY >= _iImageHeight)
	{
		_crEnsureRoi = cv::Rect(-1, -1, -1, -1);
		return;
	}
	else
		iEnsureTLY = iRoiTLY;

	if (iRoiBRX >= _iImageWidth)
		iEnsureBRX = _iImageWidth - 1;
	else if (iRoiBRX < 0)
	{
		_crEnsureRoi = cv::Rect(-1, -1, -1, -1);
		return;
	}
	else
		iEnsureBRX = iRoiBRX;

	if (iRoiBRY >= _iImageHeight)
		iEnsureBRY = _iImageHeight - 1;
	else if (iRoiBRY < 0)
	{
		_crEnsureRoi = cv::Rect(-1, -1, -1, -1);
		return;
	}
	else
		iEnsureBRY = iRoiBRY;

	//重新设定修改后的ROI区域
	_crEnsureRoi = cv::Rect(iEnsureTLX, iEnsureTLY, abs(iEnsureBRX - iEnsureTLX) + 1, abs(iEnsureBRY - iRoiTLY) + 1);
}

HalconCpp::HObject CommonTool_NormalTool::ensureRegionOfImage(HalconCpp::HObject hoImage, HalconCpp::HObject hoRegion)
{
	try
	{
		HalconCpp::HObject hoImageRegion;
		HalconCpp::HObject hoValidRegion;
		hoImageRegion = getEntireImageRoi(hoImage);
		HalconCpp::Intersection(hoRegion, hoImageRegion, &hoValidRegion);
		return hoValidRegion;
	}
	catch (HalconCpp::HException& ex)
	{
		return hoRegion;
	}

}

void CommonTool_NormalTool::isHoImageEmpty(HalconCpp::HObject hoImg, bool& isEmpty)
{
	try
	{
		isEmpty = false;

		HalconCpp::HObject hoEmptyObj;
		HalconCpp::GenEmptyObj(&hoEmptyObj);

		HalconCpp::HTuple hvIsEqual;
		HalconCpp::TestEqualObj(hoEmptyObj, hoImg, &hvIsEqual);

		if (hvIsEqual.I() == 1) isEmpty = true;
		else isEmpty = false;
	}
	catch (HalconCpp::HException& ex)
	{
		isEmpty = false;
	}
}

bool CommonTool_NormalTool::isHObjectInit(HalconCpp::HObject hoObj)
{
	try
	{
		return hoObj.IsInitialized();
	}
	catch (HalconCpp::HException& ex)
	{
		return false;
	}
	catch (std::exception& ex)
	{
		return false;
	}
}

bool CommonTool_NormalTool::isHObjectEmpty(HalconCpp::HObject hoObj)
{
	try
	{
		HalconCpp::HObject hoEmptyObj;
		HalconCpp::GenEmptyObj(&hoEmptyObj);

		HalconCpp::HTuple hvIsEqual;
		HalconCpp::TestEqualObj(hoEmptyObj, hoObj, &hvIsEqual);

		if (hvIsEqual.I() == 1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	catch (HalconCpp::HException& ex)
	{
		return true;
	}
	catch (std::exception& ex)
	{
		return true;
	}
}

bool CommonTool_NormalTool::isHObjectValid(HalconCpp::HObject hoObj)
{
	if (false == isHObjectInit(hoObj))
	{
		return false;
	}

	if (true == isHObjectEmpty(hoObj))
	{
		return false;
	}

	return true;
}

HalconCpp::HObject CommonTool_NormalTool::getEntireImageRoi(cv::Mat cvmImage)
{
	try
	{
		int iImageWidth = cvmImage.cols;
		int iImageHeight = cvmImage.rows;
		cv::Rect cvrRect(0, 0, iImageWidth, iImageHeight);
		return genRectRegion(cvrRect);
	}
	catch (HalconCpp::HException& ex)
	{
		return HalconCpp::HObject();
	}
	catch (std::exception& ex)
	{
		return HalconCpp::HObject();
	}
}

HalconCpp::HObject CommonTool_NormalTool::getEntireImageRoi(HalconCpp::HObject hoImage)
{
	try
	{
		HalconCpp::HTuple hvImageWidth, hvImageHeight;
		HalconCpp::GetImageSize(hoImage, &hvImageWidth, &hvImageHeight);
		cv::Rect cvrRect(0, 0, hvImageWidth.I(), hvImageHeight.I());
		return genRectRegion(cvrRect);
	}
	catch (HalconCpp::HException& ex)
	{
		return HalconCpp::HObject();
	}
	catch (std::exception& ex)
	{
		return HalconCpp::HObject();
	}
}

void CommonTool_NormalTool::getEntireImageRoi(HalconCpp::HObject hoImage, cv::Rect& cvrEntireImageRoi)
{
	try
	{
		HalconCpp::HTuple hvImageWidth, hvImageHeight;
		HalconCpp::GetImageSize(hoImage, &hvImageWidth, &hvImageHeight);
		cvrEntireImageRoi.x = 0;
		cvrEntireImageRoi.y = 0;
		cvrEntireImageRoi.width = hvImageWidth.D();
		cvrEntireImageRoi.height = hvImageHeight.D();
	}
	catch (HalconCpp::HException& ex)
	{
		cvrEntireImageRoi = cv::Rect();
	}
}

cv::Rect CommonTool_NormalTool::getEntireImageCvRoi(cv::Mat cvmImage)
{
	try
	{
		int iImageWidth = cvmImage.cols;
		int iImageHeight = cvmImage.rows;
		return cv::Rect(0, 0, iImageWidth, iImageHeight);
	}
	catch (std::exception& ex)
	{
		return cv::Rect();
	}
}

cv::Rect CommonTool_NormalTool::getEntireImageCvRoi(HalconCpp::HObject hoImage)
{
	try
	{
		HalconCpp::HTuple hvImageWidth, hvImageHeight;
		HalconCpp::GetImageSize(hoImage, &hvImageWidth, &hvImageHeight);
		return cv::Rect(0, 0, hvImageWidth.I(), hvImageHeight.I());
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

void CommonTool_NormalTool::RGB2HSVFull(cv::Mat cvmRgbImg, std::vector<cv::Mat>& veccvmHsvImg)
{
	cv::Mat cvmImgHSV;
	cv::cvtColor(cvmRgbImg, cvmImgHSV, cv::ColorConversionCodes::COLOR_RGB2HSV_FULL);
	cv::split(cvmImgHSV, veccvmHsvImg);
}

void CommonTool_NormalTool::RGB2HSVFull(cv::Mat cvmRgbImg, QVector<cv::Mat>& veccvmHsvImg)
{
	veccvmHsvImg.clear();
	veccvmHsvImg.squeeze();
	std::vector<cv::Mat> veccvmImg;
	RGB2HSVFull(cvmRgbImg, veccvmImg);
	for (int iIdx = 0; iIdx < veccvmImg.size(); ++iIdx)
	{
		veccvmHsvImg.append(veccvmImg[iIdx]);
	}
}

void CommonTool_NormalTool::BGR2HSVFull(cv::Mat cvmBgrImg, std::vector<cv::Mat>& veccvmHsvImg)
{
	cv::Mat cvmImgHSV;
	cv::cvtColor(cvmBgrImg, cvmImgHSV, cv::ColorConversionCodes::COLOR_BGR2HSV_FULL);
	cv::split(cvmImgHSV, veccvmHsvImg);
}

void CommonTool_NormalTool::BGR2HSVFull(cv::Mat cvmBgrImg, QVector<cv::Mat>& veccvmHsvImg)
{
	veccvmHsvImg.clear();
	veccvmHsvImg.squeeze();
	std::vector<cv::Mat> veccvmImg;
	BGR2HSVFull(cvmBgrImg, veccvmImg);
	for (int iIdx = 0; iIdx < veccvmImg.size(); ++iIdx)
	{
		veccvmHsvImg.append(veccvmImg[iIdx]);
	}
}


