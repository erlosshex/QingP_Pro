#ifndef SAVELOADIMAGE_SAVEONEIMAGE_H
#define SAVELOADIMAGE_SAVEONEIMAGE_H

#include "../../save_load_image_global.h"

#include <QObject>

#include <QObject>
#include <QString>

#include <opencv2/opencv.hpp>

#include "../save_load_image_common/save_load_image_common_define.h"

class SAVE_LOAD_IMAGE_EXPORT SaveOneImage : public QObject
{
    Q_OBJECT
public:
    SaveOneImage();
    ~SaveOneImage();

    struct SaveOneImageParam
    {
        QString strImagePath = QString("");
        QString strImageName = QString("");
        QString strImagePathname = QString("");

        cv::Mat cvmImage = cv::Mat();
    };

    struct SaveOneImageResult
    {
        bool bIsOK = false;

        QString strImagePath = QString("");
        QString strImageName = QString("");
        QString strImagePathname = QString("");
    };

    SaveOneImageParam m_stParam;

    /// <summary>
    /// 保存图片
    /// </summary>
    /// <param name="stResult">返回值，保存图片后的结果</param>
    /// <returns>错误代码</returns>
    uint saveImage(SaveOneImageResult& stResult);
};

#endif
