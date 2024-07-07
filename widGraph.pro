QT -= gui
QT += widgets

TEMPLATE = lib
CONFIG += dll

DEFINES += WIDGRAPH_LIBRARY
DEFINES += WIDPRETTY_LIBRARY
DEFINES += CONSTANTS_LIBRARY

CONFIG += c++20
CONFIG += optimize_full

SOURCES += \
    constants.cpp \
    dataGraph.cpp \
    dialogGraph.cpp \
    objectsGraph.cpp \
    supWidgets.cpp \
    widGraph.cpp \
    widPretty.cpp

HEADERS += \
    constants.h \
    constants_global.h \
    dataGraph.h \
    dialogGraph.h \
    objectsGraph.h \
    supWidgets.h \
    widGraph.h \
    widGraph_global.h \
    widPretty.h \
    widPretty_global.h

# Default rules for deployment.
#unix {
#    target.path = /usr/lib
#}
#!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc

DISTFILES +=

