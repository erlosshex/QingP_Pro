#include "save_one_image.h"

#include "../../../common_tool/source/file_operator/FileOperator.h"

SaveOneImage::SaveOneImage()
{

}

SaveOneImage::~SaveOneImage()
{

}

uint SaveOneImage::saveImage(SaveOneImageResult& stResult)
{
	try
	{
		stResult.bIsOK = false;
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
			return SAVEONEIMAGE_COMMON_PARAERROR;
		}

		QString strImagePath = m_stParam.strImagePath;
		QString strImageName = m_stParam.strImageName;
		if (strImagePath == "" || strImageName == "")
		{
			QStringList lststrSplitFilePathname = CommonTool_FileOperator::splitFilePathname(strImagePathname);
			if (lststrSplitFilePathname.size() == 2)
			{
				strImagePath = lststrSplitFilePathname[0];
				strImageName = lststrSplitFilePathname[1];
			}
		}

		if (strImagePath == "" || strImageName == "")
		{
			return SAVEONEIMAGE_COMMON_PARAERROR;
		}

		CommonTool_FileOperator::createPath(strImagePathname);

		if (false == CommonTool_FileOperator::isDirExist(strImagePath))
		{
			return SAVEONEIMAGE_COMMON_PARAERROR;
		}

		stResult.bIsOK = cv::imwrite(strImagePathname.toLocal8Bit().toStdString(), m_stParam.cvmImage);

		stResult.strImagePath = strImagePath;
		stResult.strImageName = strImageName;
		stResult.strImagePathname = strImagePathname;

		return SAVEONEIMAGE_COMMON_OK;
	}
	catch (std::exception& ex)
	{
		return SAVEONEIMAGE_COMMON_UNKNOWERROR;
	}
}
