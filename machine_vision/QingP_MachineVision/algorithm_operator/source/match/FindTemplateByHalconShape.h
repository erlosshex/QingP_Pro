#ifndef ALGORITHM_OPERATOR_MATCH_FINDTEMPLATEBYHALCONSHAPE_H
#define ALGORITHM_OPERATOR_MATCH_FINDTEMPLATEBYHALCONSHAPE_H

#include "../../algorithm_operator_global.h"

#include <QObject>

#include <halconcpp/HalconCpp.h>

class ALGORITHM_OPERATOR_EXPORT FindTemplateByHalconShape : public QObject
{
	Q_OBJECT
public:
	FindTemplateByHalconShape();
	~FindTemplateByHalconShape();

	typedef struct _FindTemplParam
	{
		bool bIsAvailable = false;
		HalconCpp::HObject hoTemplImage;
		HalconCpp::HTuple hvAngleStart = 0;
		HalconCpp::HTuple hvAngleExtent = 6.28;
		HalconCpp::HTuple hvModelID;

		HalconCpp::HTuple hvMinScore = 0.1;
		HalconCpp::HTuple hvScoreThres = 0.5;
		HalconCpp::HTuple hvNumMatches = 1;
		HalconCpp::HTuple hvMaxOverlap = 0.5;
		HalconCpp::HTuple hvNumLevels = 0;
		HalconCpp::HTuple hvGreediness = 0.9;
	} FindTemplParam;

	typedef struct _FindTemplResult
	{
		typedef struct _FindTemplOneResult
		{
			HalconCpp::HTuple hvMatchRow;
			HalconCpp::HTuple hvMatchCol;
			HalconCpp::HTuple hvMatchAngle;
			HalconCpp::HTuple hvMatchScore;
		} FindTemplOneResult;

		bool bIsOK = false;
		QList<FindTemplOneResult> lststFindTemplResult;
	} FindTemplResult;
	
	FindTemplParam m_stParam;

	/// <summary>
	/// 创建模板
	/// </summary>
	/// <param name="hoTeachImage">教授图像</param>
	void createTemplate(HalconCpp::HObject hoTeachImage);

	/// <summary>
	/// 查找模板
	/// </summary>
	/// <param name="hoImage">欲查找模板的图像</param>
	/// <param name="stResult">查找模板的结果</param>
	void findTemplate(HalconCpp::HObject cvmImage, FindTemplResult& stResult);

private:
	
};
#endif 

