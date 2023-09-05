#ifndef ALGORITHM_OPERATOR_COMMON_BLOB_INSPECT_H
#define ALGORITHM_OPERATOR_COMMON_BLOB_INSPECT_H

#include "../../algorithm_operator_global.h"

#include <QObject>
#include <QList>

#include <opencv2/opencv.hpp>
#include <halconcpp/HalconCpp.h>

#include "../../../common_tool/source/common_transform/CommonTransform.h"
#include "../../../common_tool/source/normal_tool/NormalTool.h"

class ALGORITHM_OPERATOR_EXPORT CommonBlobInspect : public QObject
{
    Q_OBJECT
public:
    CommonBlobInspect();
    ~CommonBlobInspect();

    struct FindBlobParam
    {
        int iBlobGrayMin = 0;
        int iBlobGrayMax = 255;

        double dAreaMin = 0;
        double dWidthMin = 0;
        double dHeightMin = 0;

        bool bIsWidthHeightOr = true;

        bool bIsFilterSmallArea = false;
        double dFilterBlobArea = 3;

        bool bIsEntireBlob = false;
        bool bNeedContour = false;

        HalconCpp::HObject hoBlobRegion;
    };

    struct FindBlobResult
    {
        struct OneBlobResult
        {
            double dBlobArea = 0;
            double dBlobWidth = 0;
            double dBlobHeight = 0;

            cv::RotatedRect cvrrBlobRrect;
            std::vector<cv::Point> veccvpiContour;
        };

        bool bIsDetectOK = false;
        QList<OneBlobResult> lststBlobResult;
    };

    FindBlobParam m_stParam;

    /// <summary>
    /// 查找Blob信息
    /// </summary>
    /// <param name="hoImage">查找的图像，Halcon</param>
    /// <param name="stResult">返回查找的Blob结果信息</param>
    void findBlob(HalconCpp::HObject hoImage, FindBlobResult& stResult);

};

#endif
