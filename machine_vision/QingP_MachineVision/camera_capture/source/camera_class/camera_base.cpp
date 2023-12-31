#include "camera_base.h"

CameraBase::CameraBase(CameraCommonEnum::CameraCaptureType eCaptureType, CameraCommonEnum::CameraFrameType eImageType) : \
m_eCaptureType(eCaptureType), m_eCaptureImageType(eImageType)
{

}

CameraBase::~CameraBase()
{

}

uint CameraBase::init()
{
	return uint();
}

uint CameraBase::uninit()
{
	return uint();
}

uint CameraBase::getCaptureImage(CameraCommonStruct::CameraCaptureFrame& stCaptureImage)
{
	return uint();
}

uint CameraBase::getCaptureImage(CameraCommonStruct::CameraCaptureFrame* pstCaptureImage)
{
	return uint();
}

uint CameraBase::getCaptureImageList(QList<CameraCommonStruct::CameraCaptureFrame>& lststCaptureImage)
{
	return uint();
}

uint CameraBase::getCaptureImageList(QList<CameraCommonStruct::CameraCaptureFrame*>& lstpstCaptureImage)
{
	return uint();
}

