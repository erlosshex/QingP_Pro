#ifndef CAMERA_COMMON_ENUM
#define CAMERA_COMMON_ENUM

namespace CameraCommon
{
	enum class CameraCaptureType
	{
		SingleFileOpenCV = 0x0,
		SingleFileHalcon = 0x1,
		MultiFilesOpenCV = 0x2,
		MultiFilesHalcon = 0x3,
		RealCaptureOpenCV = 0x4,
		RealCaptureHalcon = 0x5,

		DefaultMode = 0xFFFF,
	};
}

#endif
