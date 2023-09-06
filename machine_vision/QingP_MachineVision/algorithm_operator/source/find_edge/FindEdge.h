#ifndef IMGPROC_FINDEDGE_FINDEDGE_H
#define IMGPROC_FINDEDGE_FINDEDGE_H

#include "../../algorithm_operator_global.h"

#include <QObject>

#include "../algorithm_operator_common/algorithm_operator_common_define.h"
#include "../algorithm_operator_common/algorithm_operator_common_enum.h"

#include "EdgePointSpline.h"
#include "RansacAlgorithm.h"

#include "../../../common_tool/source/normal_tool/NormalTool.h"

class ALGORITHM_OPERATOR_EXPORT FindEdge : public QObject
{
    Q_OBJECT
public:
    FindEdge();

    /// <summary>
    /// 设置查找边缘的样条类型
    /// </summary>
    /// <param name="eSplineType">样条类型</param>
    void setSplineType(AlgorithmOperatorCommonEnum::SplineType eSplineType);

    /// <summary>
    /// 灰度图像在ROI中查找边缘点
    /// </summary>
    /// <param name="qveccvpEdgePoints">查找到的边缘点集，返回值</param>
    /// <param name="mImg">灰度图像</param>
    /// <param name="cvrRoi">ROI区域，矩形</param>
    /// <param name="eDirection">查找方向</param>
    /// <param name="eEdgeMode">查找模式</param>
    /// <param name="iStep">查找步长</param>
    /// <param name="iMinGrad">最小梯度</param>
    /// <param name="iEdgeValue">边缘灰度阈值</param>
    /// <param name="dAccuracy">准确度</param>
    /// <param name="eSearchStyle">搜索方式</param>
    /// <returns>错误代码</returns>
    uint findEdgePoints(QVector<cv::Point2d> &qveccvpEdgePoints, cv::Mat mImg, cv::Rect cvrRoi, AlgorithmOperatorCommonEnum::FindEdgeDirectionType eDirection, AlgorithmOperatorCommonEnum::FindEdgeModeType eEdgeMode, int iStep, int iMinGrad, int iEdgeValue, double dAccuracy, AlgorithmOperatorCommonEnum::FindEdgeSearchStyleType eSearchStyle);

    /// <summary>
    /// 灰度图像在ROI中双向查找边缘点
    /// </summary>
    /// <param name="qveccvpEdgePoints1">查找到的边缘点集1，返回值</param>
    /// <param name="qveccvpEdgePoints2">查找到的边缘点集2，返回值</param>
    /// <param name="mImg">灰度图像</param>
    /// <param name="cvrRoi">ROI区域，矩形</param>
    /// <param name="eDirection">双向查找方向</param>
    /// <param name="eEdgeMode">查找模式</param>
    /// <param name="iStep">查找步长</param>
    /// <param name="iMinGrad">最小梯度</param>
    /// <param name="iEdgeValue">边缘灰度阈值</param>
    /// <param name="dAccuracy">准确度</param>
    /// <param name="eSearchStyle">搜索方式</param>
    /// <returns></returns>
    uint findDualEdgePoints(QVector<cv::Point2d>& qveccvpEdgePoints1, QVector<cv::Point2d> qveccvpEdgePoints2, cv::Mat mImg, cv::Rect cvrRoi, AlgorithmOperatorCommonEnum::FindEdgeDualDirectionType eDirection, AlgorithmOperatorCommonEnum::FindEdgeModeType eEdgeMode, int iStep, int iMinGrad, int iEdgeValue, double dAccuracy, AlgorithmOperatorCommonEnum::FindEdgeSearchStyleType eSearchStyle);

    /// <summary>
    /// 随机一致性筛选点集
    /// </summary>
    /// <typeparam name="T">点的模板类型</typeparam>
    /// <param name="qveccvtpRetPoints">随机一致性筛选后的点集，返回值</param>
    /// <param name="qveccvtpPoints">待筛选的点集</param>
    /// <param name="dThreshold">阈值</param>
    /// <param name="iStep">步长</param>
    /// <param name="iMin">最小值</param>
    /// <param name="iMax">最大值</param>
    template<typename T>
    void  ransacFilter(QVector<cv::Point_<T>> qveccvtpRetPoints, const QVector<cv::Point_<T>>& qveccvtpPoints, const double& dThreshold, const int& iStep, const int& iMin = 100, const int& iMax = 100)
    {
        try
        {
            qveccvtpRetPoints.clear();
            std::vector<cv::Point_<T>> veccvtpPoints(qveccvtpPoints.size());
            for (auto p : qveccvtpPoints) veccvtpPoints.push_back(p);
            std::vector<cv::Point_<T>> veccvtpRansacFilterPoints = RansacAlgorithm::RansacFilter(veccvtpPoints, dThreshold, iStep, iMin, iMax);
            for (auto p : veccvtpRansacFilterPoints) qveccvtpRetPoints.push_back(p);
        }
        catch (std::exception& ex)
        {
            qveccvtpRetPoints.clear();
        }
    }

    /// <summary>
    /// 用于水平或垂直方向的随机一致性筛选点集
    /// </summary>
    /// <param name="qveccvpRetPoints">随机一致性筛选后的点集，返回值</param>
    /// <param name="qveccvpPoints">待筛选的点集</param>
    /// <param name="iThreshold">阈值</param>
    /// <param name="iStep">步长</param>
    /// <param name="bIsHor">是否是水平方向</param>
    /// <param name="iAngleScope">角度限制范围</param>
    void ransacFilterHorVer(QVector<cv::Point2d>& qveccvpRetPoints, QVector<cv::Point2d> qveccvpPoints, int iThreshold, int iStep, bool bIsHor, int iAngleScope = 30);

private:
    EdgePointSpline eps;
};

#endif // IMGPROC_FINDEDGE_FINDEDGE_H