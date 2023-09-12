#include "CameraMultiFilesOpenCV.h"

#include "../CameraSingleFileOpenCV/CameraSingleFileOpenCV.h"

CameraMultiFilesOpenCV::CameraMultiFilesOpenCV(CameraCommonEnum::CameraCaptureType eCaptureType, CameraCommonEnum::CameraFrameType eImageType) : \
	CameraBase(eCaptureType, eImageType)
{

}

CameraMultiFilesOpenCV::~CameraMultiFilesOpenCV()
{

}

void CameraMultiFilesOpenCV::setCaptureImagePathnameList(const QStringList& lststrCaptureImagePathname)
{
	m_lststrCaptureImagePathname = lststrCaptureImagePathname;
}

uint CameraMultiFilesOpenCV::getCaptureImageList(QList<CameraCommonStruct::CameraCaptureFrame*>& lstpstCaptureImage)
{
	try
	{
		lstpstCaptureImage.clear();
		lstpstCaptureImage.squeeze();

		for (int iIdx = 0; iIdx < m_lststrCaptureImagePathname.size(); ++iIdx)
		{
			try
			{
				QString strCaptureImagePathname = m_lststrCaptureImagePathname[iIdx];

				CameraSingleFileOpenCV clsCameraSingleFileOpenCV(m_eCaptureType, m_eCaptureImageType);
				clsCameraSingleFileOpenCV.setCaptureImagePathname(strCaptureImagePathname);

				CameraCommonStruct::CameraCaptureFrame* pclsCameraCaptureImage = new CameraCommonStruct::CameraCaptureFrame;

				if (CAMERA_CAPTURE_OK != clsCameraSingleFileOpenCV.getCaptureImage(*pclsCameraCaptureImage))
				{
					delete pclsCameraCaptureImage;
					pclsCameraCaptureImage = nullptr;
				}

				lstpstCaptureImage.append(pclsCameraCaptureImage);
			}
			catch (...)
			{
				lstpstCaptureImage.append(nullptr);
			}
		}

		return CAMERA_CAPTURE_OK;
	}
	catch (std::exception& ex)
	{
		return CAMERA_CAPTURE_FAIL;
	}
}


