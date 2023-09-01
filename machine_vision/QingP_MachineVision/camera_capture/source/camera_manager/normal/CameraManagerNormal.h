#ifndef CAMERAMANAGERNORMAL
#define CAMERAMANAGERNORMAL

#include "../../../camera_capture_global.h"

#include "../camera_manager_base.h"

class CAMERA_CAPTURE_EXPORT CameraManagerNormal : public CameraManagerBase
{
    Q_OBJECT
public:
    CameraManagerNormal();
    ~CameraManagerNormal();
};

#endif