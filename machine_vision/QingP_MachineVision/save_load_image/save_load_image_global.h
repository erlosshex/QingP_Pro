#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(SAVE_LOAD_IMAGE_LIB)
#  define SAVE_LOAD_IMAGE_EXPORT Q_DECL_EXPORT
# else
#  define SAVE_LOAD_IMAGE_EXPORT Q_DECL_IMPORT
# endif
#else
# define SAVE_LOAD_IMAGE_EXPORT
#endif
