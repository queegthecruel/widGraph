QT -= gui
QT += widgets

TEMPLATE = lib
DEFINES += WIDGRAPH_LIBRARY

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#CONFIG += c++17
CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dataGraph.cpp \
    dialogGraph.cpp \
    objectsGraph.cpp \
    widGraph.cpp

HEADERS += \
    dataGraph.h \
    dialogGraph.h \
    graph_global.h \
    objectsGraph.h \
    widGraph.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-widPretty-Desktop_Qt_6_4_1_MinGW_64_bit-Release/release/ -lwidPretty
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-widPretty-Desktop_Qt_6_4_1_MinGW_64_bit-Debug/debug/ -lwidPretty

INCLUDEPATH += $$PWD/../build-widPretty-Desktop_Qt_6_4_1_MinGW_64_bit-Release/release
DEPENDPATH += $$PWD/../build-widPretty-Desktop_Qt_6_4_1_MinGW_64_bit-Release/release

LIBS += -L$$PWD/../widPretty/ -lwidPretty

INCLUDEPATH += $$PWD/../widPretty
DEPENDPATH += $$PWD/../widPretty

