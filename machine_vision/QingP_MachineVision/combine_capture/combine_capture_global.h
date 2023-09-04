#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(COMBINE_CAPTURE_LIB)
#  define COMBINE_CAPTURE_EXPORT Q_DECL_EXPORT
# else
#  define COMBINE_CAPTURE_EXPORT Q_DECL_IMPORT
# endif
#else
# define COMBINE_CAPTURE_EXPORT
#endif
