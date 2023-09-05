#include "combine_weight_add.h"

CombineWeightAdd::CombineWeightAdd()
{
}

CombineWeightAdd::~CombineWeightAdd()
{
}

cv::Mat CombineWeightAdd::transform(cv::Mat cvmImage1, cv::Mat cvmImage2)
{
	cv::Mat cvmImageResult = cvmImage1 + cvmImage2;
	return cvmImageResult;
}

cv::Mat CombineWeightAdd::transform(cv::Mat cvmImage1, cv::Mat cvmImage2, cv::Mat cvmImage3)
{
	cv::Mat cvmImageResult = cvmImage1 + cvmImage2;
	cvmImageResult = cvmImageResult + cvmImage3;
	return cvmImageResult;
}

cv::Mat CombineWeightAdd::transform(const QVector<cv::Mat>& qveccvmImage)
{
	if (qveccvmImage.size() <= 0)
	{
		return cv::Mat();
	}

	cv::Mat cvmImageResult = cv::Mat::zeros(qveccvmImage[0].size(), qveccvmImage[0].type());

	for (int iIdx = 0; iIdx < qveccvmImage.size(); ++iIdx)
	{
		cvmImageResult = cvmImageResult + qveccvmImage[iIdx];
	}

	return cvmImageResult;
}

cv::Mat CombineWeightAdd::transform(cv::Mat cvmImage1, double dScale1, double dAdd)
{
	cv::Mat cvmImageResult = dScale1 * cvmImage1 + dAdd;
	return cvmImageResult;
}

cv::Mat CombineWeightAdd::transform(cv::Mat cvmImage1, double dScale1, cv::Mat cvmImage2, double dScale2, double dAdd)
{
	cv::Mat cvmImageResult = dScale1 * cvmImage1 + dScale2 * cvmImage2 + dAdd;
	return cvmImageResult;
}

cv::Mat CombineWeightAdd::transform(cv::Mat cvmImage1, double dScale1, cv::Mat cvmImage2, double dScale2, cv::Mat cvmImage3, double dScale3, double dAdd)
{
	cv::Mat cvmImageResult = dScale1 * cvmImage1 + dScale2 * cvmImage2 + dScale3 * cvmImage3 + dAdd;
	return cvmImageResult;
}

cv::Mat CombineWeightAdd::transform(const QMap<cv::Mat, double>& qmapImageScale, double dAdd)
{
	if (qmapImageScale.size() <= 0)
	{
		return cv::Mat();
	}

	cv::Mat cvmImageResult = cv::Mat::zeros(qmapImageScale.firstKey().size(), qmapImageScale.firstKey().type());

	for (auto iter = qmapImageScale.constKeyValueBegin(); iter != qmapImageScale.constKeyValueEnd(); ++iter)
	{
		cvmImageResult = cvmImageResult + iter.base().key() * iter.base().value();
	}

	cvmImageResult = cvmImageResult + dAdd;

	return cvmImageResult;
}
