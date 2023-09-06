#ifndef ALGORITHM_OPERATOR_COMMON_ENUM
#define ALGORITHM_OPERATOR_COMMON_ENUM

namespace AlgorithmOperatorCommonEnum
{
	enum EDGEFINDER_DIRECTION
	{
		enumLEFT2RIGHT = 0,
		enumRIGHT2LEFT = 1,
		enumTOP2BOTTOM = 2,
		enumBOTTOM2TOP = 3,
		enumNODIRECTION = 4,
	};

	enum EDGEFINDER_DIRECTION_NEW
	{
		enumLEFT2RIGHTNEW = 0,
		enumRIGHT2LEFTNEW = 1,
		enumTOP2BOTTOMNEW = 2,
		enumBOTTOM2TOPNEW = 3,
		enumHOROUT2IN_NEW = 4,
		enumHORIN2OUT_NEW = 5,
		enumVEROUT2IN_NEW = 6,
		enumVERIN2OUT_NEW = 7,
	};

	enum EDGEFINDER_MODE
	{
		enumBLACK2WHITE = 0,
		enumWHITE2BLACK = 1,
	};

	enum EDGEFINDER_SEARCHSTYLE
	{
		enumMAXGRAD = 0,
		enumFRISTGRAD = 1,
		enumAUTOGRAD = 2,
	};

	enum SPLINE_TYPE
	{
		enumCubicSpline = 0,
		enumBSpline = 1,
	};

	//查找边缘的方向类型
	enum FindEdgeDirectionType
	{
		Left2Right = 0x0,
		Right2Left = 0x1,
		Top2Bottom = 0x2,
		Bottom2Top = 0x3,
	};

	//查找边缘的双方向类型
	enum FindEdgeDualDirectionType
	{
		HoriOut2In = 0x0,
		HoriIn2Out = 0x1,
		VertOut2In = 0x2,
		VertIn2Out = 0x3,
	};

	//查找边缘的四方向类型
	enum FindEdgeFourDirectionType
	{
		Out2In = 0x0,
		In2Out = 0x1,
	};

	//查找边缘的模式类型
	enum FindEdgeModeType
	{
		Black2White = 0x0,
		White2Black = 0x1,
	};

	//查找边缘的搜索方式类型
	enum FindEdgeSearchStyleType
	{
		MaxGrad = 0x0,
		FirstGrad = 0x1,
		AutoGrad = 0x2,
	};

	//样条类型
	enum SplineType
	{
		CubicSpl = 0x0,
		BSpl = 0x1,
	};
}

#endif
