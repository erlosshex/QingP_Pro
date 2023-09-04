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

    cv::Mat transform(cv::Mat cvmImage);
};

#endif
