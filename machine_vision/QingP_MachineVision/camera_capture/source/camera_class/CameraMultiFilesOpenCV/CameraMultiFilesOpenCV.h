#ifndef CAMERAMULTIFILESOPENCV
#define CAMERAMULTIFILESOPENCV

#include "../../../camera_capture_global.h"

#include "../camera_base.h"

class CAMERA_CAPTURE_EXPORT CameraMultiFilesOpenCV : public CameraBase
{
    Q_OBJECT
public:
    CameraMultiFilesOpenCV();
    ~CameraMultiFilesOpenCV();
};

#endif
