#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(COMMON_TOOL_LIB)
#  define COMMON_TOOL_EXPORT Q_DECL_EXPORT
# else
#  define COMMON_TOOL_EXPORT Q_DECL_IMPORT
# endif
#else
# define COMMON_TOOL_EXPORT
#endif
