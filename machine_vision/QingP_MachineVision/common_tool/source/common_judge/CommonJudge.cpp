#include "CommonJudge.h"

void CommonTool_CommonJudge::isHoImageEmpty(HalconCpp::HObject hoImg, bool& isEmpty)
{
	try
	{
		isEmpty = false;

		HalconCpp::HObject hoEmptyObj;
		HalconCpp::GenEmptyObj(&hoEmptyObj);

		HalconCpp::HTuple hvIsEqual;
		HalconCpp::TestEqualObj(hoEmptyObj, hoImg, &hvIsEqual);

		if (hvIsEqual.I() == 1) isEmpty = true;
		else isEmpty = false;
	}
	catch (HalconCpp::HException& ex)
	{
		isEmpty = false;
	}
}



