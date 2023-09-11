#ifndef ALGORITHM_OPERATOR_MATCH_FINDTEMPLATEBYGRAYHISTO_H
#define ALGORITHM_OPERATOR_MATCH_FINDTEMPLATEBYGRAYHISTO_H

#include "../../algorithm_operator_global.h"

#include <QObject>
#include <QVariant>
#include <QVector>

#include <opencv2/opencv.hpp>
#include <halconcpp/HalconCpp.h>

class ALGORITHM_OPERATOR_EXPORT FindTemplateByGrayHisto : public QObject
{
	Q_OBJECT
public:
	FindTemplateByGrayHisto();
	~FindTemplateByGrayHisto();

	typedef struct _FindTemplParam
	{
		cv::Rect cvrTemplRect;
		double dScaleRatio = 0.5;
		bool bIsTemplAvaliable = false;
		HalconCpp::HTuple hvTemplAbsolutionHisto = HalconCpp::HTuple();
		HalconCpp::HTuple hvTemplRelativeHisto = HalconCpp::HTuple();

		cv::Rect cvrSearchRect;
		double dScoreThres = 0.6;
		int dFindRowStep = 5;
		int dFindColStep = 5;
	} FindTemplParam;

	typedef struct _FindTemplResult
	{
		typedef struct _MatchResult
		{
			double dMatchScore = 0;
			cv::Point2d cvpMatchCenter = cv::Point2d(0, 0);
		} MatchResult;

		bool bIsMatchOK = false;
		QList<MatchResult> lststMatchResult;
	} FindTemplResult;

	void setFindParam(const FindTemplParam& stParam);

	void getFindParam(FindTemplParam& stParam);

	void createTempl(cv::Mat cvmImage);

	void findTempl(cv::Mat cvmImage, FindTemplResult& stResult);

private:
	FindTemplParam m_stFindTemplParam;

	void ggsCreateTemplByHisto(HalconCpp::HObject ho_Image, HalconCpp::HTuple hv_templRow1, HalconCpp::HTuple hv_templCol1,
		HalconCpp::HTuple hv_templRow2, HalconCpp::HTuple hv_templCol2, HalconCpp::HTuple hv_scaleRatio, HalconCpp::HTuple* hv_isOK,
		HalconCpp::HTuple* hv_templAbsolutionHisto, HalconCpp::HTuple* hv_templRelativeHisto);

	void ggsFindTemplByHisto(HalconCpp::HObject ho_Image, HalconCpp::HTuple hv_searchRow1, HalconCpp::HTuple hv_searchCol1,
		HalconCpp::HTuple hv_searchRow2, HalconCpp::HTuple hv_searchCol2, HalconCpp::HTuple hv_scoreThres, HalconCpp::HTuple hv_templAbsolutionHisto,
		HalconCpp::HTuple hv_templRelativeHisto, HalconCpp::HTuple hv_maskWidth, HalconCpp::HTuple hv_maskHeight, HalconCpp::HTuple hv_rowStep,
		HalconCpp::HTuple hv_colStep, HalconCpp::HTuple hv_scaleRatio, HalconCpp::HTuple* hv_isOK, HalconCpp::HTuple* hv_findRows,
		HalconCpp::HTuple* hv_findCols, HalconCpp::HTuple* hv_findScores);

	void ggsGetCosSimilarity(HalconCpp::HTuple hv_vec1, HalconCpp::HTuple hv_vec2, HalconCpp::HTuple* hv_isOK, HalconCpp::HTuple* hv_cosSimilarity);

};
#endif //IMGPROC_MATCH_FINDTEMPLATEBYGRAYHISTO_H

