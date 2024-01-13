QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AlgorithmUtils/Algorithms/AStar.cpp \
    AlgorithmUtils/Algorithms/DFS.cpp \
    AlgorithmUtils/Algorithms/Dijkstra.cpp \
    AlgorithmUtils/Algorithms/Floyd.cpp \
    AlgorithmUtils/Algorithms/Gene.cpp \
    AlgorithmUtils/Algorithms/Heap.cpp \
    AlgorithmUtils/Algorithms/SPFA.cpp \
    AlgorithmUtils/getshortestpath.cpp \
    CampusMap/campusmap.cpp \
    CampusMap/edge.cpp \
    CampusMap/point.cpp \
    DrawingItems/drawingedge.cpp \
    DrawingItems/drawingpoint.cpp \
    DrawingItems/mapbackground.cpp \
    main.cpp \
    mainwindow.cpp


HEADERS += \
    AlgorithmUtils/getshortestpath.h \
    CampusMap/campusmap.h \
    CampusMap/edge.h \
    CampusMap/point.h \
    DrawingItems/drawingedge.h \
    DrawingItems/drawingpoint.h \
    DrawingItems/mapbackground.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    RunningResources.qrc
