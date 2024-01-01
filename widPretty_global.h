#ifndef WIDPRETTY_GLOBAL_H
#define WIDPRETTY_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(WIDPRETTY_LIBRARY)
#  define WIDPRETTY_EXPORT Q_DECL_EXPORT
#else
#  define WIDPRETTY_EXPORT Q_DECL_IMPORT
#endif

#endif // WIDPRETTY_GLOBAL_H
