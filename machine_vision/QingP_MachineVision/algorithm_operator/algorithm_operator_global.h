#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(ALGORITHM_OPERATOR_LIB)
#  define ALGORITHM_OPERATOR_EXPORT Q_DECL_EXPORT
# else
#  define ALGORITHM_OPERATOR_EXPORT Q_DECL_IMPORT
# endif
#else
# define ALGORITHM_OPERATOR_EXPORT
#endif
