#-------------------------------------------------
#
# Project created by QtCreator 2015-06-23T22:20:00
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = DXLProg
CONFIG   += console
CONFIG   -= app_bundle
QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS_RELEASE *= -O3
TEMPLATE = app


SOURCES += \
    dxllibclass/dxlhal.cpp \
    dxllibclass/dynamixelclass.cpp \
    dclass/dmodeldetails.cpp \
    dclass/dxmotorsystem.cpp \
    dclass/dxsetupmotor.cpp \
    dclass/dxsinglemotor.cpp \
    dclass/dxsystem.cpp \
    dtools/additionaltools.cpp \
    dtools/bilateralcontrol.cpp \
    dtools/readfile.cpp \
    dtools/setup.cpp \
    dtools/threadedreadfile.cpp \
    src/main.cpp \
    src/userinputfn.cpp \
    src/manualinput.cpp \
    src/characterizerw.cpp \
    qtgui/mainwindow.cpp \
    qtgui/bilateralcontrolwidget.cpp \
    qtgui/closedloopwidget.cpp \
    qtgui/multireadwidget.cpp \
    qtgui/readfilewidget.cpp \
    dstopwatch/dstopwatch.cpp

HEADERS += \
    dxllibclass/dxlhal.h \
    dxllibclass/dynamixelclass.h \
    dclass/control_table_constants.h \
    dclass/dconstants.h \
    dclass/dmodeldetails.h \
    dclass/dxmotorsystem.h \
    dclass/dxsetupmotor.h \
    dclass/dxsinglemotor.h \
    dclass/dxsystem.h \
    dtools/additionaltools.h \
    dtools/bilateralcontrol.h \
    dtools/readfile.h \
    dtools/setup.h \
    dtools/threadedreadfile.h \
    src/userinputfn.h \
    src/user_settings.h \
    src/manualinput.h \
    src/characterizerw.h \
    qtgui/mainwindow.h \
    qtgui/bilateralcontrolwidget.h \
    qtgui/closedloopwidget.h \
    qtgui/readfilewidget.h \
    qtgui/multireadwidget.h \
    dstopwatch/dstopwatch.h


FORMS += \
    qtgui/bilateralcontrolwidget.ui \
    qtgui/closedloopwidget.ui \
    qtgui/multireadwidget.ui \
    qtgui/readfilewidget.ui \
    qtgui/mainwindow.ui
