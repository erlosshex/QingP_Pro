#include "CommonAffineOperator.h"

#include "../normal_tool/NormalTool.h"

void CommonAffineOperator::rotateRect1Orien(const cv::Rect& cvrRect, const cv::Point2d& cvpRotateCenter, double dRotateAngle, cv::Rect& cvr1OrienRect)
{
	try
	{
		HalconCpp::HTuple hvAffineMat;
		HalconCpp::HomMat2dIdentity(&hvAffineMat);
		HalconCpp::HomMat2dRotate(hvAffineMat, HalconCpp::HTuple(dRotateAngle).TupleRad(), cvpRotateCenter.y, cvpRotateCenter.x, &hvAffineMat);

		HalconCpp::HTuple hvTopLeftRow, hvTopLeftCol;
		HalconCpp::HTuple hvTopRightRow, hvTopRightCol;
		HalconCpp::HTuple hvBottomLeftRow, hvBottomLeftCol;
		HalconCpp::HTuple hvBottomRightRow, hvBottomRightCol;

		hvTopLeftRow = cvrRect.tl().y;
		hvTopLeftCol = cvrRect.tl().x;
		hvBottomRightRow = cvrRect.br().y - 1;
		hvBottomRightCol = cvrRect.br().x - 1;
		hvTopRightRow = hvTopLeftRow;
		hvTopRightCol = hvBottomRightCol;
		hvBottomLeftRow = hvBottomRightRow;
		hvBottomLeftCol = hvTopLeftCol;

		HalconCpp::HTuple hvAffineTopLeftRow, hvAffineTopLeftCol;
		HalconCpp::HTuple hvAffineTopRightRow, hvAffineTopRightCol;
		HalconCpp::HTuple hvAffineBottomLeftRow, hvAffineBottomLeftCol;
		HalconCpp::HTuple hvAffineBottomRightRow, hvAffineBottomRightCol;
		HalconCpp::AffineTransPoint2d(hvAffineMat, hvTopLeftRow, hvTopLeftCol, &hvAffineTopLeftRow, &hvAffineTopLeftCol);
		HalconCpp::AffineTransPoint2d(hvAffineMat, hvTopRightRow, hvTopRightCol, &hvAffineTopRightRow, &hvAffineTopRightCol);
		HalconCpp::AffineTransPoint2d(hvAffineMat, hvBottomLeftRow, hvBottomLeftCol, &hvAffineBottomLeftRow, &hvAffineBottomLeftCol);
		HalconCpp::AffineTransPoint2d(hvAffineMat, hvBottomRightRow, hvBottomRightCol, &hvAffineBottomRightRow, &hvAffineBottomRightCol);

		HalconCpp::HTuple hvRows, hvCols;
		hvRows.Append(hvAffineTopLeftRow);
		hvRows.Append(hvAffineTopRightRow);
		hvRows.Append(hvAffineBottomRightRow);
		hvRows.Append(hvAffineBottomLeftRow);
		hvRows.Append(hvAffineTopLeftRow);
		hvCols.Append(hvAffineTopLeftCol);
		hvCols.Append(hvAffineTopRightCol);
		hvCols.Append(hvAffineBottomRightCol);
		hvCols.Append(hvAffineBottomLeftCol);
		hvCols.Append(hvAffineTopLeftCol);

		HalconCpp::HObject hoAffineContour;
		HalconCpp::GenContourPolygonXld(&hoAffineContour, hvRows, hvCols);

		HalconCpp::HTuple hvRow1, hvCol1, hvRow2, hvCol2;
		HalconCpp::SmallestRectangle1Xld(hoAffineContour, &hvRow1, &hvCol1, &hvRow2, &hvCol2);

		cv::Rect cvrAffineRect;
		cvrAffineRect.x = hvCol1.D();
		cvrAffineRect.y = hvRow1.D();
		cvrAffineRect.width = hvCol2.D() - hvCol1.D() + 1;
		cvrAffineRect.height = hvRow2.D() - hvRow1.D() + 1;

		cvr1OrienRect = cvrAffineRect;
	}
	catch (HalconCpp::HException& ex)
	{
		cvr1OrienRect = cv::Rect();
	}
	catch (std::exception& ex)
	{
		cvr1OrienRect = cv::Rect();
	}
}

cv::Rect CommonAffineOperator::rotateRect1Orien(const cv::Rect& cvrRect, const cv::Point2d& cvpRotateCenter, double dRotateAngle)
{
	cv::Rect cvrRect1Orien;
	rotateRect1Orien(cvrRect, cvpRotateCenter, dRotateAngle, cvrRect1Orien);
	return cvrRect1Orien;
}

void CommonAffineOperator::rotateRect4Orien(const cv::Rect& cvrRect, const cv::Point2d& cvpRotateCenter, QList<cv::Rect>& lstcvr4OrienRect)
{
	try
	{
		lstcvr4OrienRect.clear();

		lstcvr4OrienRect.append(cvrRect);

		cv::Rect cvrRect90, cvrRect180, cvrRect270;
		rotateRect1Orien(cvrRect, cvpRotateCenter, 90, cvrRect90);
		rotateRect1Orien(cvrRect, cvpRotateCenter, 180, cvrRect180);
		rotateRect1Orien(cvrRect, cvpRotateCenter, 270, cvrRect270);
		
		lstcvr4OrienRect.append(cvrRect90);
		lstcvr4OrienRect.append(cvrRect180);
		lstcvr4OrienRect.append(cvrRect270);
	}
	catch (HalconCpp::HException& ex)
	{
		lstcvr4OrienRect.clear();
	}
	catch (std::exception& ex)
	{
		lstcvr4OrienRect.clear();
	}
}

QList<cv::Rect>& CommonAffineOperator::rotateRect4Orien(const cv::Rect& cvrRect, const cv::Point2d& cvpRotateCenter)
{
	QList<cv::Rect> lstcvr4OrienRect;
	rotateRect4Orien(cvrRect, cvpRotateCenter, lstcvr4OrienRect);
	return lstcvr4OrienRect;
}

void CommonAffineOperator::rotatePoint1Orien(const cv::Point2d& cvpPoint, const cv::Point2d& cvpRotateCenter, double dRotateAngle, cv::Point2d& cvp1OrienPoint)
{
	try
	{
		HalconCpp::HTuple hvAffineMat;
		HalconCpp::HomMat2dIdentity(&hvAffineMat);
		HalconCpp::HomMat2dRotate(hvAffineMat, HalconCpp::HTuple(dRotateAngle).TupleRad(), cvpRotateCenter.y, cvpRotateCenter.x, &hvAffineMat);

		HalconCpp::HTuple hvPointRow, hvPointCol;

		hvPointRow = cvpPoint.y;
		hvPointCol = cvpPoint.x;

		HalconCpp::HTuple hvAffinePointRow, hvAffinePointCol;
		HalconCpp::AffineTransPoint2d(hvAffineMat, hvPointRow, hvPointCol, &hvAffinePointRow, &hvAffinePointCol);

		cvp1OrienPoint.y = hvAffinePointRow.D();
		cvp1OrienPoint.x = hvAffinePointCol.D();
	}
	catch (HalconCpp::HException& ex)
	{
		cvp1OrienPoint = cv::Point2d();
	}
	catch (std::exception& ex)
	{
		cvp1OrienPoint = cv::Point2d();
	}
}

cv::Point2d CommonAffineOperator::rotatePoint1Orien(const cv::Point2d& cvpPoint, const cv::Point2d& cvpRotateCenter, double dRotateAngle)
{
	cv::Point2d cvp1OrienPoint;
	rotatePoint1Orien(cvpPoint, cvpRotateCenter, dRotateAngle, cvp1OrienPoint);
	return cvp1OrienPoint;
}

cv::Point2d CommonAffineOperator::getPin1RotatedRectCorner(const cv::Point2d& cvpTopLeftPoint, const cv::Point2d& cvpTopRightPoint, const cv::Point2d& cvpBottomLeftPoint, const cv::Point2d& cvpBottomRightPoint, ToolCommonEnum::EnumPin1Orientation ePin1Orien, ToolCommonEnum::EnumPin1Direction ePin1Direction)
{
	cv::Point2d cvpRotatedTopLeftPoint(0, 0);
	cv::Point2d cvpRotatedTopRightPoint(0, 0);
	cv::Point2d cvpRotatedBottomLeftPoint(0, 0);
	cv::Point2d cvpRotatedBottomRightPoint(0, 0);
	switch (ePin1Orien)
	{
	case ToolCommonEnum::EnumPin1Orientation::Orien0:
		cvpRotatedTopLeftPoint = cvpTopLeftPoint;
		cvpRotatedTopRightPoint = cvpTopRightPoint;
		cvpRotatedBottomLeftPoint = cvpBottomLeftPoint;
		cvpRotatedBottomRightPoint = cvpBottomRightPoint;
		break;
	case ToolCommonEnum::EnumPin1Orientation::Orien180:
		cvpRotatedTopLeftPoint = cvpBottomRightPoint;
		cvpRotatedTopRightPoint = cvpBottomLeftPoint;
		cvpRotatedBottomLeftPoint = cvpTopRightPoint;
		cvpRotatedBottomRightPoint = cvpTopLeftPoint;
		break;
	case ToolCommonEnum::EnumPin1Orientation::Orien90:
		cvpRotatedTopLeftPoint = cvpBottomLeftPoint;
		cvpRotatedTopRightPoint = cvpTopLeftPoint;
		cvpRotatedBottomLeftPoint = cvpBottomRightPoint;
		cvpRotatedBottomRightPoint = cvpTopRightPoint;
		break;
	case ToolCommonEnum::EnumPin1Orientation::Orien270:
		cvpRotatedTopLeftPoint = cvpTopRightPoint;
		cvpRotatedTopRightPoint = cvpBottomRightPoint;
		cvpRotatedBottomLeftPoint = cvpTopLeftPoint;
		cvpRotatedBottomRightPoint = cvpBottomLeftPoint;
		break;
	default:
		cvpRotatedTopLeftPoint = cvpTopLeftPoint;
		cvpRotatedTopRightPoint = cvpTopRightPoint;
		cvpRotatedBottomLeftPoint = cvpBottomLeftPoint;
		cvpRotatedBottomRightPoint = cvpBottomRightPoint;
		break;
	}

	switch (ePin1Direction)
	{
	case ToolCommonEnum::EnumPin1Direction::TopLeft:
		return cvpRotatedTopLeftPoint;
	case ToolCommonEnum::EnumPin1Direction::TopRight:
		return cvpRotatedTopRightPoint;
	case ToolCommonEnum::EnumPin1Direction::BottomLeft:
		return cvpRotatedBottomLeftPoint;
	case ToolCommonEnum::EnumPin1Direction::BottomRight:
		return cvpRotatedBottomRightPoint;
	default:
		return cvpRotatedTopLeftPoint;
	}
}

void CommonAffineOperator::getPin1RotatedRectCorner(ToolCommonStruct::RectFourCornerPoints stRectFourCornerPoints, ToolCommonStruct::RectFourCornerPoints* pstRectFourCornerPoints, ToolCommonEnum::EnumPin1Orientation ePin1Orien)
{
	switch (ePin1Orien)
	{
	case ToolCommonEnum::EnumPin1Orientation::Orien0:
		pstRectFourCornerPoints->cvpTopLeftPoint = stRectFourCornerPoints.cvpTopLeftPoint;
		pstRectFourCornerPoints->cvpTopRightPoint = stRectFourCornerPoints.cvpTopRightPoint;
		pstRectFourCornerPoints->cvpBottomLeftPoint = stRectFourCornerPoints.cvpBottomLeftPoint;
		pstRectFourCornerPoints->cvpBottomRightPoint = stRectFourCornerPoints.cvpBottomRightPoint;
		break;
	case ToolCommonEnum::EnumPin1Orientation::Orien180:
		pstRectFourCornerPoints->cvpTopLeftPoint = stRectFourCornerPoints.cvpBottomRightPoint;
		pstRectFourCornerPoints->cvpTopRightPoint = stRectFourCornerPoints.cvpBottomLeftPoint;
		pstRectFourCornerPoints->cvpBottomLeftPoint = stRectFourCornerPoints.cvpTopRightPoint;
		pstRectFourCornerPoints->cvpBottomRightPoint = stRectFourCornerPoints.cvpTopLeftPoint;
		break;
	case ToolCommonEnum::EnumPin1Orientation::Orien90:
		pstRectFourCornerPoints->cvpTopLeftPoint = stRectFourCornerPoints.cvpBottomLeftPoint;
		pstRectFourCornerPoints->cvpTopRightPoint = stRectFourCornerPoints.cvpTopLeftPoint;
		pstRectFourCornerPoints->cvpBottomLeftPoint = stRectFourCornerPoints.cvpBottomRightPoint;
		pstRectFourCornerPoints->cvpBottomRightPoint = stRectFourCornerPoints.cvpTopRightPoint;
		break;
	case ToolCommonEnum::EnumPin1Orientation::Orien270:
		pstRectFourCornerPoints->cvpTopLeftPoint = stRectFourCornerPoints.cvpTopRightPoint;
		pstRectFourCornerPoints->cvpTopRightPoint = stRectFourCornerPoints.cvpBottomRightPoint;
		pstRectFourCornerPoints->cvpBottomLeftPoint = stRectFourCornerPoints.cvpTopLeftPoint;
		pstRectFourCornerPoints->cvpBottomRightPoint = stRectFourCornerPoints.cvpBottomLeftPoint;
		break;
	default:
		pstRectFourCornerPoints->cvpTopLeftPoint = stRectFourCornerPoints.cvpTopLeftPoint;
		pstRectFourCornerPoints->cvpTopRightPoint = stRectFourCornerPoints.cvpTopRightPoint;
		pstRectFourCornerPoints->cvpBottomLeftPoint = stRectFourCornerPoints.cvpBottomLeftPoint;
		pstRectFourCornerPoints->cvpBottomRightPoint = stRectFourCornerPoints.cvpBottomRightPoint;
		break;
	}
}

cv::Mat CommonAffineOperator::rotateImage(cv::Mat cvmImage, cv::Point2d cvpRotateCenter, double dRotateAngle)
{
	if (std::abs(dRotateAngle) < 0.0001 || true == cvmImage.empty())
	{
		return cvmImage;
	}

	cv::Mat cvmRotateMat = cv::getRotationMatrix2D(cv::Point2f(cvpRotateCenter.x, cvpRotateCenter.y), dRotateAngle, 1);
	
	cv::Mat cvmRotateImage;
	cv::warpAffine(cvmImage, cvmRotateImage, cvmRotateMat, cvmImage.size());

	return cvmRotateImage;
}

HalconCpp::HObject CommonAffineOperator::rotateImage(HalconCpp::HObject hoImage, HalconCpp::HTuple hvRotateCenterRow, HalconCpp::HTuple hvRotateCenterCol, HalconCpp::HTuple hvRotateAngle)
{
	if (std::abs(hvRotateAngle.D()) < 0.0001 ||
		false == CommonTool_NormalTool::isHObjectValid(hoImage))
	{
		return hoImage;
	}

	HalconCpp::HTuple hvAffineMat;
	HalconCpp::HomMat2dIdentity(&hvAffineMat);
	HalconCpp::HomMat2dRotate(hvAffineMat, hvRotateAngle.TupleRad(), hvRotateCenterRow, hvRotateCenterCol, &hvAffineMat);
	
	HalconCpp::HObject hoAffineImage;
	HalconCpp::AffineTransImage(hoImage, &hoAffineImage, hvAffineMat, "constant", "false");

	return hoAffineImage;
}

cv::Mat CommonAffineOperator::moveImage(cv::Mat cvmImage, cv::Point2d cvpMove)
{
	if (cvmImage.empty() ||
		(cvpMove.x == 0 && cvpMove.y == 0)) 
	{
		return cvmImage;
	}

	cv::Mat cvmMoveImage;

	cv::Mat cvmAffineMat = cv::Mat::zeros(2, 3, CV_32FC1);
	cvmAffineMat.at<float>(0, 0) = 1;
	cvmAffineMat.at<float>(0, 2) = cvpMove.x;
	cvmAffineMat.at<float>(1, 1) = 1;
	cvmAffineMat.at<float>(1, 2) = cvpMove.y;

	warpAffine(cvmImage, cvmMoveImage, cvmAffineMat, cvmImage.size());

	return cvmMoveImage;
}

HalconCpp::HObject CommonAffineOperator::moveImage(HalconCpp::HObject hoImage, HalconCpp::HTuple hvMoveRow, HalconCpp::HTuple hvMoveCol)
{
	HalconCpp::HTuple hvAffineMat;
	HalconCpp::HomMat2dIdentity(&hvAffineMat);
	HalconCpp::HomMat2dTranslate(hvAffineMat, hvMoveRow, hvMoveCol, &hvAffineMat);

	HalconCpp::HObject hoAffineImage;
	HalconCpp::AffineTransImage(hoImage, &hoAffineImage, hvAffineMat, "constant", "false");

	return hoAffineImage;
}

cv::Point2d CommonAffineOperator::rotatePoint(cv::Point2d cvpPoint, cv::Point2d cvpRotateCenter, double dRotateAngle)
{
	if (std::abs(dRotateAngle) < 0.001)
	{
		return cvpPoint;
	}

	double dRadAngle = dRotateAngle * CV_PI / 180.0;

	cv::Point2d cvpRotatePoint;
	cvpRotatePoint.x = (cvpPoint.x - cvpRotateCenter.x) * std::cos(dRadAngle) - (cvpPoint.y - cvpRotateCenter.y) * std::sin(dRadAngle) + cvpRotateCenter.x;
	cvpRotatePoint.y = (cvpPoint.x - cvpRotateCenter.x) * std::sin(dRadAngle) + (cvpPoint.y - cvpRotateCenter.y) * std::cos(dRadAngle) + cvpRotateCenter.y;

	return cvpRotatePoint;

}

cv::Point2d CommonAffineOperator::rotatePoint1(cv::Point2d cvpPoint, cv::Point2d cvpRotateCenter, double dRotateAngle)
{
	return rotatePoint(cvpPoint, cvpRotateCenter, -dRotateAngle);
}

void CommonAffineOperator::rotatePoint(HalconCpp::HTuple hvPointRow, HalconCpp::HTuple hvPointCol, HalconCpp::HTuple hvRotateCenterRow, HalconCpp::HTuple hvRotateCenterCol, HalconCpp::HTuple hvRotateAngle, HalconCpp::HTuple& hvRotatePointRow, HalconCpp::HTuple& hvRotatePointCol)
{
	if (std::abs(hvRotateAngle.D()) < 0.0001)
	{
		hvRotatePointRow = hvPointRow;
		hvRotatePointCol = hvPointCol;
		return;
	}

	HalconCpp::HTuple hvAffineMat;
	HalconCpp::HomMat2dIdentity(&hvAffineMat);
	HalconCpp::HomMat2dRotate(hvAffineMat, hvRotateAngle.TupleRad(), hvRotateCenterRow, hvRotateCenterCol, &hvAffineMat);

	HalconCpp::AffineTransPoint2d(hvAffineMat, hvPointRow, hvPointCol, &hvRotatePointRow, &hvRotatePointCol);
}

cv::Point2d CommonAffineOperator::movePoint(cv::Point2d cvpPoint, cv::Point2d cvpMove)
{
	cv::Point2d cvpMovePoint(0, 0);
	cvpMovePoint = cvpPoint + cvpMove;
	return cvpMovePoint;
}

void CommonAffineOperator::movePoint(HalconCpp::HTuple hvPointRow, HalconCpp::HTuple hvPointCol, HalconCpp::HTuple hvMoveRow, HalconCpp::HTuple hvMoveCol, HalconCpp::HTuple& hvMovePointRow, HalconCpp::HTuple& hvMovePointCol)
{
	hvMovePointRow = hvPointRow + hvMoveRow;
	hvMovePointCol = hvPointCol + hvMoveCol;
}

cv::RotatedRect CommonAffineOperator::rotateRect(cv::Rect cvrRect, cv::Point2d cvpRotateCenter, double dRotateAngle)
{
	double dTopLeftX = cvrRect.x;
	double dTopLeftY = cvrRect.y;
	double dBottomRightX = (double)cvrRect.x + (double)cvrRect.width - 1.0;
	double dBottomRightY = (double)cvrRect.y + (double)cvrRect.height - 1.0;

	cv::Point2d cvpTopLeft(dTopLeftX, dTopLeftY);
	cv::Point2d cvpTopRight(dBottomRightX, dTopLeftY);
	cv::Point2d cvpBottomRight(dBottomRightX, dBottomRightY);
	cv::Point2d cvpBottomLeft(dTopLeftX, dBottomRightY);

	cv::Point2d cvpRotateTopLeft = rotatePoint(cvpTopLeft, cvpRotateCenter, dRotateAngle);
	cv::Point2d cvpRotateTopRight = rotatePoint(cvpTopRight, cvpRotateCenter, dRotateAngle);
	cv::Point2d cvpRotateBottomRight = rotatePoint(cvpBottomRight, cvpRotateCenter, dRotateAngle);
	cv::Point2d cvpRotateBottomLeft = rotatePoint(cvpBottomLeft, cvpRotateCenter, dRotateAngle);

	return cv::RotatedRect(
		cv::Point2f(cvpRotateTopLeft.x, cvpRotateTopLeft.y),
		cv::Point2f(cvpRotateTopRight.x, cvpRotateTopRight.y),
		cv::Point2f(cvpRotateBottomRight.x, cvpRotateBottomRight.y)
	);
}

cv::Rect CommonAffineOperator::getBoundingRectFromRotateRect(cv::Rect cvrRect, HalconCpp::HTuple hvAffineMat)
{
	try
	{
		HalconCpp::HObject hoRect = CommonTool_NormalTool::genRectRegion(cvrRect);
		HalconCpp::HObject hoAffineRect;
		HalconCpp::AffineTransRegion(hoRect, &hoAffineRect, hvAffineMat, "constant");
		HalconCpp::HTuple hvRow1, hvCol1, hvRow2, hvCol2;
		HalconCpp::SmallestRectangle1(hoAffineRect, &hvRow1, &hvCol1, &hvRow2, &hvCol2);
		cv::Rect cvrAffineRect;
		cvrAffineRect.x = hvCol1.D();
		cvrAffineRect.y = hvRow1.D();
		cvrAffineRect.width = hvCol2.D() - hvCol1.D() + 1;
		cvrAffineRect.height = hvRow2.D() - hvRow1.D() + 1;
		return cvrAffineRect;
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
