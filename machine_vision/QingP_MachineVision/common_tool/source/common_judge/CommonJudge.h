#ifndef COMMONTOOL_COMMONJUDGE_H
#define COMMONTOOL_COMMONJUDGE_H

#include "../../common_tool_global.h"

#include <QObject>

#include <opencv2/opencv.hpp>
#include <halconcpp/HalconCpp.h>

class COMMON_TOOL_EXPORT CommonTool_CommonJudge : public QObject
{
	Q_OBJECT
public:

	/// <summary>
	/// 判断halcon图像是否为空
	/// </summary>
	/// <param name="hoImg">halcon图像</param>
	/// <param name="isEmpty">true表示空图，false表示不是空图，返回值</param>
	static void isHoImageEmpty(HalconCpp::HObject hoImg, bool& isEmpty);
};

#endif 