#ifndef ALGORITHM_OPERATOR_MATCH_FINDTEMPLATEBYTWOLINES_H
#define ALGORITHM_OPERATOR_MATCH_FINDTEMPLATEBYTWOLINES_H

#include "../../algorithm_operator_global.h"

#include <QObject>
#include <QVariant>
#include <QVector>

#include <opencv2/opencv.hpp>
#include <halconcpp/HalconCpp.h>

#include "../../../common_tool/source/tool_common/tool_common_enum.h"

class ALGORITHM_OPERATOR_EXPORT FindTemplateByTwoLines : public QObject
{
	Q_OBJECT
public:
	FindTemplateByTwoLines();
	~FindTemplateByTwoLines();

	typedef struct _FindTemplParam
	{
		cv::Vec4f cvlTeachHoriLine = cv::Vec4f(0, 0, 0, 0);
		cv::Vec4f cvlTeachVertLine = cv::Vec4f(0, 0, 0, 0);
		ToolCommonEnum::PosType eTeachPosType = ToolCommonEnum::PosType::TopLeftPos;
		cv::Point2d cvpTeachCrossPoint = cv::Point2d(0, 0);
		double dTeachAngle = 0;
	} FindTemplParam;

	typedef struct _FindTemplResult
	{
		cv::Vec4f cvlInspectHoriLine = cv::Vec4f(0, 0, 0, 0);
		cv::Vec4f cvlInspectVertLine = cv::Vec4f(0, 0, 0, 0);
		ToolCommonEnum::PosType eInspectPosType = ToolCommonEnum::PosType::TopLeftPos;
		cv::Point2d cvpInspectCrossPoint = cv::Point2d(0, 0);
		double dInspectAngle = 0;

		HalconCpp::HTuple hvAffineMat;
		HalconCpp::HTuple hvAffineMatInv;

		cv::Point2d cvpTeachCenter = cv::Point2d(0, 0);
		cv::Point2d cvpMatchCenter = cv::Point2d(0, 0);
		double dMatchAngle = 0;
	} FindTemplResult;

	FindTemplParam m_stParam;

	/// <summary>
	/// 创建模板
	/// </summary>
	/// <param name="cvlHoriLine">水平线</param>
	/// <param name="cvlVertLine">垂直线</param>
	/// <param name="ePosType">相交点位置</param>
	void createTemplate(cv::Vec4f cvlHoriLine, cv::Vec4f cvlVertLine, ToolCommonEnum::PosType ePosType = ToolCommonEnum::PosType::TopLeftPos);

	/// <summary>
	/// 查找模板
	/// </summary>
	/// <param name="stResult">返回值，查找模板的结果</param>
	/// <param name="cvlHoriLine">水平线</param>
	/// <param name="cvlVertLine">垂直线</param>
	/// <param name="ePosType">相交点位置</param>
	void findTemplate(FindTemplResult& stResult, cv::Vec4f cvlHoriLine, cv::Vec4f cvlVertLine, ToolCommonEnum::PosType ePosType = ToolCommonEnum::PosType::TopLeftPos);


private:
	
};
#endif 

