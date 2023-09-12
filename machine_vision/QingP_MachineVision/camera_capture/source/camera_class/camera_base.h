#ifndef CAMERA_BASE
#define CAMERA_BASE

#include "../../camera_capture_global.h"

#include <QObject>

#include "../camera_common/camera_common_define.h"
#include "../camera_common/camera_common_enum.h"
#include "../camera_common/camera_common_struct.h"

class CAMERA_CAPTURE_EXPORT CameraBase : public QObject
{
    Q_OBJECT
public:
    CameraBase(CameraCommonEnum::CameraCaptureType eCaptureType, CameraCommonEnum::CameraFrameType eImageType);
    ~CameraBase();
    
    /// <summary>
    /// 初始化相机
    /// </summary>
    /// <returns>返回初始化状态</returns>
    virtual uint init();

    /// <summary>
    /// 断开相机
    /// </summary>
    /// <returns>返回断开相机状态</returns>
    virtual uint uninit();

    /// <summary>
    /// 获取采集的图像
    /// </summary>
    /// <param name="stCaptureImage">获取的采集图像信息</param>
    /// <returns>返回初始化状态</returns>
    virtual uint getCaptureImage(CameraCommonStruct::CameraCaptureFrame& stCaptureImage);

    /// <summary>
    /// 获取采集的图像
    /// </summary>
    /// <param name="stCaptureImage">获取的采集图像信息，指针</param>
    /// <returns>返回初始化状态</returns>
    virtual uint getCaptureImage(CameraCommonStruct::CameraCaptureFrame* pstCaptureImage);

    /// <summary>
    /// 获取采集的图像列表
    /// </summary>
    /// <param name="lststCaptureImage">获取的采集图像信息</param>
    /// <returns>返回初始化状态</returns>
    virtual uint getCaptureImageList(QList<CameraCommonStruct::CameraCaptureFrame>& lststCaptureImage);

    /// <summary>
    /// 获取采集的图像
    /// </summary>
    /// <param name="lstpstCaptureImage">获取的采集图像信息，指针</param>
    /// <returns>返回初始化状态</returns>
    virtual uint getCaptureImageList(QList<CameraCommonStruct::CameraCaptureFrame*>& lstpstCaptureImage);

protected:
    CameraCommonEnum::CameraCaptureType m_eCaptureType = CameraCommonEnum::CameraCaptureType::DefaultMode;
    CameraCommonEnum::CameraFrameType m_eCaptureImageType = CameraCommonEnum::CameraFrameType::DefaultMode;
};

#endif
