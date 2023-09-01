#ifndef CAMERAMANAGERBASE
#define CAMERAMANAGERBASE

#include "../../camera_capture_global.h"

#include <QObject>

class CAMERA_CAPTURE_EXPORT CameraManagerBase : public QObject
{
    Q_OBJECT
public:
    CameraManagerBase();
    ~CameraManagerBase();
};

#endif