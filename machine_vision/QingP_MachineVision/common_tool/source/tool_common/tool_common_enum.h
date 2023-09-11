#ifndef TOOL_COMMON_ENUM
#define TOOL_COMMON_ENUM

namespace ToolCommonEnum
{
	//位置类型
	enum PosType
	{
		TopLeftPos = 0x0,
		TopRightPos = 0x1,
		BottomLeftPos = 0x2,
		BottomRightPos = 0x3,
		TopPos = 0x4,
		BottomPos = 0x5,
		LeftPos = 0x6,
		RightPos = 0x7,
	};

	enum EnumPin1Direction
	{
		TopLeft = 0,
		TopRight = 1,
		BottomRight = 2,
		BottomLeft = 3,
	};

	enum EnumPin1Orientation
	{
		Orien0 = 0,
		Orien90 = 1,
		Orien180 = 2,
		Orien270 = 3,
		OrienUnknown = 4,
	};

	enum EDGEFINDER_DIRECTION
	{
		enumLEFT2RIGHT = 0,
		enumRIGHT2LEFT = 1,
		enumTOP2BOTTOM = 2,
		enumBOTTOM2TOP = 3,
		enumNODIRECTION = 4,
	};
}

#endif
