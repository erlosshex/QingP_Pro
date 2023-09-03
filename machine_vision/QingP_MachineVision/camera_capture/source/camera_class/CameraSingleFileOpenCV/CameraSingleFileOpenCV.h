#ifndef CAMERASINGLEFILEOPENCV
#define CAMERASINGLEFILEOPENCV

#include "../../../camera_capture_global.h"

#include "../camera_base.h"

class CAMERA_CAPTURE_EXPORT CameraSingleFileOpenCV : public CameraBase
{
    Q_OBJECT
public:
    CameraSingleFileOpenCV(CameraCommonEnum::CameraCaptureType eCaptureType, CameraCommonEnum::CameraFrameType eImageType);
    ~CameraSingleFileOpenCV();
};

#endif
