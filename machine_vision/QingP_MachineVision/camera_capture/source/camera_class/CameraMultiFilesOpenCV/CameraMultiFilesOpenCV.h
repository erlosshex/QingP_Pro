#ifndef CAMERAMULTIFILESOPENCV
#define CAMERAMULTIFILESOPENCV

#include "../../../camera_capture_global.h"

#include "../camera_base.h"

class CAMERA_CAPTURE_EXPORT CameraMultiFilesOpenCV : public CameraBase
{
    Q_OBJECT
public:
    CameraMultiFilesOpenCV(CameraCommonEnum::CameraCaptureType eCaptureType, CameraCommonEnum::CameraFrameType eImageType);
    ~CameraMultiFilesOpenCV();

    /// <summary>
    /// 设置图像的路径列表
    /// </summary>
    /// <param name="strImagePathname">图像路径</param>
    void setCaptureImagePathnameList(const QStringList& lststrCaptureImagePathname);

    /// <summary>
    /// 获取采集的图像列表
    /// </summary>
    /// <param name="stCaptureImage">获取的采集图像信息</param>
    /// <returns>返回初始化状态</returns>
    uint getCaptureImageList(QList<CameraCommonStruct::CameraCaptureFrame*>& lstpstCaptureImage) override;

private:
    QStringList m_lststrCaptureImagePathname;
};

#endif
