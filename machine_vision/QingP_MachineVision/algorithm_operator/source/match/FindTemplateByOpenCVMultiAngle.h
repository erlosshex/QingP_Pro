#ifndef ALGORITHM_OPERATOR_MATCH_FINDTEMPLATEBYOPENCVMULTIANGLE_H
#define ALGORITHM_OPERATOR_MATCH_FINDTEMPLATEBYOPENCVMULTIANGLE_H

#include <QMap>

#include "../../algorithm_operator_global.h"

#include <QObject>

#include "FindTemplateByOpenCV.h"

class ALGORITHM_OPERATOR_EXPORT FindTemplateByOpenCVMultiAngle : public QObject
{
	Q_OBJECT
public:
	FindTemplateByOpenCVMultiAngle();
	~FindTemplateByOpenCVMultiAngle();

	typedef struct _FindTemplParam
	{
		typedef struct _AngleTempl
		{
			double dAngle;
			FindTemplateByOpenCV clsTempl;
		} AngleTempl;

		bool bIsAvailable = false;
		cv::Mat cvmTeachImage = cv::Mat();
		cv::Rect cvrTemplRoi = cv::Rect();
		cv::Mat cvmTemplImage = cv::Mat();
		QList<double> lstdTemplAngle;
		QList<cv::Rect> lstcvrSearchRoi;
		double dMatchScoreThres = 0.5;
		cv::TemplateMatchModes cvtmmMatchMode = cv::TemplateMatchModes::TM_CCORR_NORMED;
		bool bIsFirstMatch = true;
		bool bIsBestMatch = true;
		int iStepX = 1;
		int iStepY = 1;

		QList<AngleTempl> lststAngleTempl;
	} FindTemplParam;

	typedef struct _FindTemplResult
	{
		typedef struct _FindTemplOneResult
		{
			cv::Point2d cvpMatchCenter = cv::Point2d(0, 0);
			cv::Rect cvrMatchRoi = cv::Rect();
			cv::RotatedRect cvrrMatchRoi = cv::RotatedRect();
			double dMatchAngle = 0;
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
	
	/// <summary>
	/// 生成多角度模板
	/// </summary>
	/// <param name="stParam">查找模板的参数</param>
	/// <returns>返回值，是否成功</returns>
	bool generateMultiAngleTempl(FindTemplParam& stParam);

	/// <summary>
	/// 旋转匹配结果中的矩形
	/// </summary>
	/// <param name="stResult">匹配的结果</param>
	void rotateMatchRectInResult(FindTemplResult& stResult);
};
#endif 

