#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(ALGORITHM_KIT_LIB)
#  define ALGORITHM_KIT_EXPORT Q_DECL_EXPORT
# else
#  define ALGORITHM_KIT_EXPORT Q_DECL_IMPORT
# endif
#else
# define ALGORITHM_KIT_EXPORT
#endif
