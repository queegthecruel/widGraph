#ifndef CONSTANTS_GLOBAL_H
#define CONSTANTS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CONSTANTS_LIBRARY)
#  define CONSTANTS_EXPORT Q_DECL_EXPORT
#else
#  define CONSTANTS_EXPORT Q_DECL_IMPORT
#endif
#endif // CONSTANTS_GLOBAL_H
