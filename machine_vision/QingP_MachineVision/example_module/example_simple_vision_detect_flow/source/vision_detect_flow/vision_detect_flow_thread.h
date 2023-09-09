#ifndef VISION_DETECT_FLOW_THREAD_H
#define VISION_DETECT_FLOW_THREAD_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <QThread>

#include <opencv2/opencv.hpp>

#include "../../../../../camera_capture/source/camera_class/CameraSingleFileOpenCV/CameraSingleFileOpenCV.h"
#include "../../../../../combine_capture/source/combine_identity/combine_identity.h"
#include "../../../../../save_load_image/source/save_image/save_one_image.h"
#include "../../../../../save_load_image/source/load_image/load_one_image.h"

class VisionDetectFlowThread : public QThread
{
    Q_OBJECT
public:
    VisionDetectFlowThread();
    ~VisionDetectFlowThread();

    void run() override;

    /// <summary>
    /// 简单的检测流程：
    /// 1.简单的取像（读取本地图片）
    /// 2.简单的图像预处理（返回原本的图像）
    /// 3.简单的图像显示（使用imshow，显示5s）
    /// 4.简单的vision检测方式（在图像上画一个圆形）
    /// 5.简单的检测结果再显示（使用imshow，显示5s)
    /// 6.简单的保存原图
    /// 7.简单的保存结果图
    /// 8.简单的加载原图
    /// 9.简单显示加载的原图（使用imshow，显示5s）
    /// </summary>
    void detect();

private:
    CameraSingleFileOpenCV* m_pclsCameraSingleFileOpenCV = nullptr;
    CombineIdentity* m_pclsCombineIdentity = nullptr;
    SaveOneImage* m_pclsSaveOneImage = nullptr;
    LoadOneImage* m_pclsLoadOneImage = nullptr;

    /// <summary>
    /// 显示一张图片，显示5s
    /// </summary>
    /// <param name="cvmImage">要显示的图片</param>
    void showImage(cv::Mat cvmImage);

    /// <summary>
    /// 检测一张图片，在图片上画圆形
    /// </summary>
    /// <param name="cvmImage">待检测的图片</param>
    /// <returns>返回值，绘制好圆形的图片</returns>
    cv::Mat detectImage(cv::Mat cvmImage);
};

#endif