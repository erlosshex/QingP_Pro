#ifndef COMBINE_WEIGHT_ADD
#define COMBINE_WEIGHT_ADD

#include "../../combine_capture_global.h"

#include <QObject>
#include <QMap>
#include <QVector>

#include <opencv2/opencv.hpp>

class COMBINE_CAPTURE_EXPORT CombineWeightAdd : public QObject
{
    Q_OBJECT
public:
    CombineWeightAdd();
    ~CombineWeightAdd();

    /// <summary>
    /// 整合两张图像
    /// </summary>
    /// <param name="cvmImage1">图像1</param>
    /// <param name="cvmImage2">图像2</param>
    /// <returns>返回值，整合后的图像</returns>
    cv::Mat transform(cv::Mat cvmImage1, cv::Mat cvmImage2);

    /// <summary>
    /// 整合三张图像
    /// </summary>
    /// <param name="cvmImage1">图像1</param>
    /// <param name="cvmImage2">图像2</param>
    /// <param name="cvmImage3">图像3</param>
    /// <returns>返回值，整合后的图像</returns>
    cv::Mat transform(cv::Mat cvmImage1, cv::Mat cvmImage2, cv::Mat cvmImage3);

    /// <summary>
    /// 整合多张图像
    /// </summary>
    /// <param name="qveccvmImage">多张图像的Vector</param>
    /// <returns>返回值，整合后的图像</returns>
    cv::Mat transform(const QVector<cv::Mat>& qveccvmImage);

    /// <summary>
    /// 拉伸一张图像
    /// </summary>
    /// <param name="cvmImage1">图像1</param>
    /// <param name="dScale1">拉伸参数</param>
    /// <param name="dAdd">补偿参数</param>
    /// <returns>返回值，拉伸后的图像</returns>
    cv::Mat transform(cv::Mat cvmImage1, double dScale1, double dAdd);

    /// <summary>
    /// 带权重的整合两张图像
    /// </summary>
    /// <param name="cvmImage1">图像1</param>
    /// <param name="dScale1">权重1</param>
    /// <param name="cvmImage2">图像2</param>
    /// <param name="dScale2">权重2</param>
    /// <param name="dAdd">补偿参数</param>
    /// <returns>返回值，整合后的图像</returns>
    cv::Mat transform(cv::Mat cvmImage1, double dScale1, cv::Mat cvmImage2, double dScale2, double dAdd);

    /// <summary>
    /// 带权重的整合三张图像
    /// </summary>
    /// <param name="cvmImage1">图像1</param>
    /// <param name="dScale1">权重1</param>
    /// <param name="cvmImage2">图像2</param>
    /// <param name="dScale2">权重2</param>
    /// <param name="cvmImage3">图像3</param>
    /// <param name="dScale3">权重3</param>
    /// <param name="dAdd">补偿参数</param>
    /// <returns>返回值，整合后的图像</returns>
    cv::Mat transform(cv::Mat cvmImage1, double dScale1, cv::Mat cvmImage2, double dScale2, cv::Mat cvmImage3, double dScale3, double dAdd);

    /// <summary>
    /// 带权重的整合多张图像
    /// </summary>
    /// <param name="qmapImageScale">多张图像和权重的Map</param>
    /// <param name="dAdd">补偿参数</param>
    /// <returns>返回值，整合后的图像</returns>
    cv::Mat transform(const QMap<cv::Mat, double>& qmapImageScale, double dAdd);
};

#endif
