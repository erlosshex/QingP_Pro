#include "CameraSingleFileOpenCV.h"

#include "../../../../common_tool/source/file_operator/FileOperator.h"

CameraSingleFileOpenCV::CameraSingleFileOpenCV(CameraCommonEnum::CameraCaptureType eCaptureType, CameraCommonEnum::CameraFrameType eImageType) : \
	CameraBase(eCaptureType, eImageType)
{

}

CameraSingleFileOpenCV::~CameraSingleFileOpenCV()
{

}

void CameraSingleFileOpenCV::setCaptureImagePathname(const QString& strCaptureImagePathname)
{
	m_strCaptureImagePathname = strCaptureImagePathname;
}

uint CameraSingleFileOpenCV::getCaptureImage(CameraCommonStruct::CameraCaptureFrame& stCaptureImage)
{
	try
	{
		stCaptureImage.bIsAvailable = false;

		if (false == CommonTool_FileOperator::isFileExist(m_strCaptureImagePathname))
		{
			return CAMERA_CAPTURE_FAIL;
		}

		cv::Mat cvmImage = cv::imread(m_strCaptureImagePathname.toLocal8Bit().toStdString(), cv::ImreadModes::IMREAD_UNCHANGED);

		stCaptureImage.bIsAvailable = true;
		stCaptureImage.eCameraFrameType = CameraCommonEnum::CameraFrameType::OpenCVMatType;
		stCaptureImage.cvmImage = cvmImage;
		stCaptureImage.iImageWidth = stCaptureImage.cvmImage.cols;
		stCaptureImage.iImageHeight = stCaptureImage.cvmImage.rows;
		stCaptureImage.uiChannelType = stCaptureImage.cvmImage.channels();

		return CAMERA_CAPTURE_OK;
	}
	catch (std::exception& ex)
	{
		return CAMERA_CAPTURE_FAIL;
	}
}


