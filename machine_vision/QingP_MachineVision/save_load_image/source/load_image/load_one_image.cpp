#include "load_one_image.h"

#include "../../../common_tool/source/file_operator/FileOperator.h"

LoadOneImage::LoadOneImage()
{

}

LoadOneImage::~LoadOneImage()
{

}

uint LoadOneImage::loadImage(LoadOneImageResult& stResult)
{
	try
	{
		stResult.bIsOK = false;
		stResult.cvmImage.release();
		stResult.strImagePath = "";
		stResult.strImageName = "";
		stResult.strImagePathname = "";

		QString strImagePathname = m_stParam.strImagePathname;
		if (strImagePathname == "")
		{
			strImagePathname = m_stParam.strImagePath + m_stParam.strImageName;
		}

		if (strImagePathname == "")
		{
			return LOADONEIMAGE_COMMON_PARAERROR;
		}

		if (false == CommonTool_FileOperator::isFileExist(strImagePathname))
		{
			return LOADONEIMAGE_COMMON_PARAERROR;
		}

		stResult.cvmImage = cv::imread(strImagePathname.toLocal8Bit().toStdString(), m_stParam.cvstLoadImageMode);
		stResult.iImageChannel = stResult.cvmImage.channels();

		QStringList lststrSplitFilePathname = CommonTool_FileOperator::splitFilePathname(strImagePathname);
		if (lststrSplitFilePathname.size() == 2)
		{
			stResult.strImagePath = lststrSplitFilePathname[0];
			stResult.strImageName = lststrSplitFilePathname[1];
		}
		stResult.strImagePathname = strImagePathname;

		if (false == stResult.cvmImage.empty())
		{
			stResult.bIsOK = true;
		}
		else
		{
			stResult.bIsOK = false;
		}

		return LOADONEIMAGE_COMMON_OK;
	}
	catch (std::exception& ex)
	{
		return LOADONEIMAGE_COMMON_UNKNOWERROR;
	}
}
