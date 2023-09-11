#ifndef ALGORITHM_OPERATOR_MATCH_FINDTEMPLATEBYOPENCV_H
#define ALGORITHM_OPERATOR_MATCH_FINDTEMPLATEBYOPENCV_H

#include "../../algorithm_operator_global.h"

#include <QObject>
#include <QList>

#include <opencv2/opencv.hpp>

class ALGORITHM_OPERATOR_EXPORT FindTemplateByOpenCV : public QObject
{
	Q_OBJECT
public:
	FindTemplateByOpenCV();
	~FindTemplateByOpenCV();

	typedef struct _FindTemplParam
	{
		bool bIsAvailable = false;
		cv::Rect cvrTemplRoi = cv::Rect();
		cv::Mat cvmTemplImage = cv::Mat();
		QList<cv::Rect> lstcvrSearchRoi;
		double dMatchScoreThres = 0.5;
		cv::TemplateMatchModes cvtmmMatchMode = cv::TemplateMatchModes::TM_CCORR_NORMED;
		bool bIsBestMatch = true;
		int iStepX = 1;
		int iStepY = 1;
	} FindTemplParam;

	typedef struct _FindTemplResult
	{
		typedef struct _FindTemplOneResult
		{
			cv::Point2d cvpMatchCenter = cv::Point2d(0, 0);
			cv::Rect cvrMatchRoi = cv::Rect();
			double dMatchScore = 0;
		} FindTemplOneResult;

		bool bIsOK = false;
		QList<FindTemplOneResult> lststFindTemplResult;
	} FindTemplResult;

	FindTemplParam m_stParam;

	/// <summary>
	/// 创建模板
	/// </summary>
	/// <param name="cvmTeachImage">教授图像</param>
	void createTemplate(cv::Mat cvmTeachImage);

	/// <summary>
	/// 查找模板
	/// </summary>
	/// <param name="cvmImage">欲查找模板的图像</param>
	/// <param name="stResult">查找模板的结果</param>
	void findTemplate(cv::Mat cvmImage, FindTemplResult& stResult);

private:
	
	

};
#endif 

