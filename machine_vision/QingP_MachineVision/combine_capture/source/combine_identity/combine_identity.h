#ifndef COMBINE_IDENTITY
#define COMBINE_IDENTITY

#include "../../combine_capture_global.h"

#include <QObject>

#include <opencv2/opencv.hpp>

class COMBINE_CAPTURE_EXPORT CombineIdentity : public QObject
{
    Q_OBJECT
public:
    CombineIdentity();
    ~CombineIdentity();

    /// <summary>
    /// 恒等变换
    /// </summary>
    /// <param name="cvmImage">输入图像</param>
    /// <returns>返回值，恒等变换后的图像</returns>
    cv::Mat transform(cv::Mat cvmImage);
};

#endif
