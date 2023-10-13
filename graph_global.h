#ifndef GRAPH_GLOBAL_H
#define GRAPH_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(WIDGRAPH_LIBRARY)
#  define WIDGRAPH_EXPORT Q_DECL_EXPORT
#else
#  define WIDGRAPH_EXPORT Q_DECL_IMPORT
#endif

#endif // GRAPH_GLOBAL_H
