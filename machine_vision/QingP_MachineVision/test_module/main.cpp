#include <QtCore/QCoreApplication>

#include "../camera_capture/source/camera_manager/normal/CameraManagerNormal.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CameraManagerNormal clsNewCameraManager;

    qDebug() << "hello,world";

    return a.exec();
}
