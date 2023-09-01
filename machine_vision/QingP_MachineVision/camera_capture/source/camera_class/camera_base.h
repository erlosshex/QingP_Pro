#ifndef CAMERA_BASE
#define CAMERA_BASE

#include "../../camera_capture_global.h"

#include <QObject>

class CAMERA_CAPTURE_EXPORT CameraBase : public QObject
{
    Q_OBJECT
public:
    CameraBase();
    ~CameraBase();
};

#endif
