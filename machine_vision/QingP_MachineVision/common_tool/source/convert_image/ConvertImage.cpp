#include "ConvertImage.h"

HalconCpp::HObject CommonTool_ConvertImage::Mat2HObject(cv::Mat cvmImage)
{
	HalconCpp::HObject hoImage = HalconCpp::HObject();
	if (true == cvmImage.empty())
	{
		return hoImage;
	}

	int iHeight = cvmImage.rows;
	int iWidth = cvmImage.cols;

	if (cvmImage.type() == CV_8UC3 || cvmImage.type() == CV_8UC4) {
		std::vector<cv::Mat> veccvmImageChannel;
		cv::split(cvmImage, veccvmImageChannel);
		cv::Mat cvmImageB = veccvmImageChannel[0];
		cv::Mat cvmImageG = veccvmImageChannel[1];
		cv::Mat cvmImageR = veccvmImageChannel[2];
		uchar* pucDataR = new uchar[iHeight * iWidth];
		uchar* pucDataG = new uchar[iHeight * iWidth];
		uchar* pucDataB = new uchar[iHeight * iWidth];
		for (int iIdx = 0; iIdx < iHeight; iIdx++)
		{
			memcpy(pucDataR + iWidth * iIdx, cvmImageR.data + cvmImageR.step * iIdx, iWidth);
			memcpy(pucDataG + iWidth * iIdx, cvmImageG.data + cvmImageG.step * iIdx, iWidth);
			memcpy(pucDataB + iWidth * iIdx, cvmImageB.data + cvmImageB.step * iIdx, iWidth);
		}
		HalconCpp::GenImage3(&hoImage, "byte", iWidth, iHeight, (Hlong)pucDataR, (Hlong)pucDataG, (Hlong)pucDataB);
		delete[]pucDataR;
		delete[]pucDataG;
		delete[]pucDataB;
		pucDataR = NULL;
		pucDataG = NULL;
		pucDataB = NULL;
	}
	else if (cvmImage.type() == CV_8UC1)
	{
		uchar* pucData = new uchar[iHeight * iWidth];
		for (int iIdx = 0; iIdx < iHeight; iIdx++) {
			memcpy(pucData + iWidth * iIdx, cvmImage.data + cvmImage.step * iIdx, iWidth);
		}
		HalconCpp::GenImage1(&hoImage, "byte", iWidth, iHeight, (Hlong)pucData);
		delete[] pucData;
		pucData = NULL;
	}
	return hoImage;
}

void CommonTool_ConvertImage::Mat2HObject(const cv::Mat& cvmImage, HalconCpp::HObject& hoImage)
{
	hoImage = Mat2HObject(cvmImage);
}

cv::Mat CommonTool_ConvertImage::HObject2Mat(const HalconCpp::HObject hoImage)
{
	HalconCpp::HObject hoObj = HalconCpp::HObject();
	HalconCpp::HTuple hvCh = HalconCpp::HTuple();
	HalconCpp::HString hsType;
	cv::Mat cvmImage;
	HalconCpp::ConvertImageType(hoImage, &hoObj, "byte");
	HalconCpp::CountChannels(hoObj, &hvCh);
	Hlong hlWidth = 0;
	Hlong hlHeight = 0;
	if (hvCh[0].I() == 1)
	{
		HalconCpp::HImage hiImg(hoObj);
		void* pvPointer = hiImg.GetImagePointer1(&hsType, &hlWidth, &hlHeight);//GetImagePointer1(Hobj, &ptr, &cType, &wid, &hgt);
		int iWidth = hlWidth;
		int iHeight = hlHeight;
		cvmImage.create(iHeight, iWidth, CV_8UC1);
		unsigned char* pucdata = static_cast<unsigned char*>(pvPointer);
		memcpy(cvmImage.data, pucdata, iWidth * iHeight);
	}
	else if (hvCh[0].I() == 3)
	{
		void* pvPointerR;
		void* pvPointerG;
		void* pvPointerB;
		HalconCpp::HImage hiImg(hoObj);
		hiImg.GetImagePointer3(&pvPointerR, &pvPointerG, &pvPointerB, &hsType, &hlWidth, &hlHeight);
		int iWidth = hlWidth;
		int iHeight = hlHeight;
		cvmImage.create(iHeight, iWidth, CV_8UC3);
		std::vector<cv::Mat> veccvmImage(3);
		veccvmImage[0].create(iHeight, iWidth, CV_8UC1);
		veccvmImage[1].create(iHeight, iWidth, CV_8UC1);
		veccvmImage[2].create(iHeight, iWidth, CV_8UC1);
		unsigned char* pucPointerR = (unsigned char*)pvPointerR;
		unsigned char* pucPointerG = (unsigned char*)pvPointerG;
		unsigned char* pucPointerB = (unsigned char*)pvPointerB;
		memcpy(veccvmImage[2].data, pucPointerR, iWidth * iHeight);
		memcpy(veccvmImage[1].data, pucPointerG, iWidth * iHeight);
		memcpy(veccvmImage[0].data, pucPointerB, iWidth * iHeight);
		cv::merge(veccvmImage, cvmImage);
	}
	return cvmImage;
}

void CommonTool_ConvertImage::HObject2Mat(const HalconCpp::HObject hoImage, cv::Mat& cvmImage)
{
	HObject2Mat(hoImage).copyTo(cvmImage);
}


