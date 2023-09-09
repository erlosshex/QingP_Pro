#ifndef SAVELOADIMAGE_LOADONEIMAGE_H
#define SAVELOADIMAGE_LOADONEIMAGE_H

#include "../../save_load_image_global.h"

#include <QObject>
#include <QString>

#include <opencv2/opencv.hpp>

#include "../save_load_image_common/save_load_image_common_define.h"

class SAVE_LOAD_IMAGE_EXPORT LoadOneImage : public QObject
{
    Q_OBJECT
public:
    LoadOneImage();
    ~LoadOneImage();

    struct LoadOneImageParam
    {
        QString strImagePath = QString("");
        QString strImageName = QString("");
        QString strImagePathname = QString("");
        cv::ImreadModes cvstLoadImageMode = cv::ImreadModes::IMREAD_UNCHANGED;
    };

    struct LoadOneImageResult
    {
        bool bIsOK = false;

        QString strImagePath = QString("");
        QString strImageName = QString("");
        QString strImagePathname = QString("");

        cv::Mat cvmImage = cv::Mat();
        int iImageChannel = 1;
    };

    LoadOneImageParam m_stParam;

    /// <summary>
    /// 加载图片
    /// </summary>
    /// <param name="stResult">返回值，加载图片后的结果</param>
    /// <returns>错误代码</returns>
    uint loadImage(LoadOneImageResult& stResult);

};

#endif
