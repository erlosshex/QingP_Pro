#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(CAMERA_CAPTURE_LIB)
#  define CAMERA_CAPTURE_EXPORT Q_DECL_EXPORT
# else
#  define CAMERA_CAPTURE_EXPORT Q_DECL_IMPORT
# endif
#else
# define CAMERA_CAPTURE_EXPORT
#endif
