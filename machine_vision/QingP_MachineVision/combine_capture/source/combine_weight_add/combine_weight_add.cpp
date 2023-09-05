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

cv::Mat CombineWeightAdd::transform(const QList<cv::Mat>& lstcvmImage)
{
	if (lstcvmImage.size() <= 0)
	{
		return cv::Mat();
	}

	cv::Mat cvmImageResult = cv::Mat::zeros(lstcvmImage[0].size(), lstcvmImage[0].type());

	for (int iIdx = 0; iIdx < lstcvmImage.size(); ++iIdx)
	{
		cvmImageResult = cvmImageResult + lstcvmImage[iIdx];
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
