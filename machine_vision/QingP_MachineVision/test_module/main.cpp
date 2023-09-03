#include <QtCore/QCoreApplication>

#include "../camera_capture/source/camera_manager/normal/CameraManagerNormal.h"
#include "../camera_capture/source/camera_class/CameraSingleFileOpenCV/CameraSingleFileOpenCV.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CameraManagerNormal clsNewCameraManager;

    qDebug() << "hello,world";

    CameraSingleFileOpenCV clsCam(CameraCommonEnum::CameraCaptureType::SingleFileOpenCV, CameraCommonEnum::CameraFrameType::OpenCVMatType);
    clsCam.setCaptureImagePathname("D:/lena.bmp");
    CameraCommonStruct::CameraCaptureFrame stCaptureImage;
    uint uiRet = clsCam.getCaptureImage(stCaptureImage);

    return a.exec();
}
