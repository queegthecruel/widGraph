QT -= gui
QT += widgets

TEMPLATE = lib
DEFINES += WIDGRAPH_LIBRARY
DEFINES += WIDPRETTY_LIBRARY

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#CONFIG += c++17
CONFIG += c++20
CONFIG += optimize_full

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dataGraph.cpp \
    dialogGraph.cpp \
    objectsGraph.cpp \
    supWidgets.cpp \
    widGraph.cpp \
    widPretty.cpp

HEADERS += \
    dataGraph.h \
    dialogGraph.h \
    objectsGraph.h \
    supWidgets.h \
    widGraph.h \
    widGraph_global.h \
    widPretty.h \
    widPretty_global.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc

DISTFILES +=

