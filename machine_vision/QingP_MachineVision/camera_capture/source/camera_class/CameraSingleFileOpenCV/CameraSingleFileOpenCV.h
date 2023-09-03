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

    /// <summary>
    /// 设置图像的路径
    /// </summary>
    /// <param name="strImagePathname">图像路径</param>
    void setCaptureImagePathname(const QString& strCaptureImagePathname);

    /// <summary>
    /// 获取采集的图像
    /// </summary>
    /// <param name="stCaptureImage">获取的采集图像信息</param>
    /// <returns>返回初始化状态</returns>
    uint getCaptureImage(CameraCommonStruct::CameraCaptureFrame& stCaptureImage) override;

private:
    QString m_strCaptureImagePathname;
};

#endif
