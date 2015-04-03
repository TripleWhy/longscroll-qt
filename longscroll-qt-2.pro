#-------------------------------------------------
#
# Project created by QtCreator 2015-03-29T12:54:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = longscroll-qt-2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    itemtype1.cpp \
    imginfo.cpp \
    imagewidget.cpp \
    navigatorwidget.cpp \
    imageloader.cpp \
    contentwidget.cpp \
    imageitem.cpp \
    notifyingscrollarea.cpp \
    notifyablescrollcontentwidget.cpp \
    contentwidgetitemfactory.cpp

HEADERS  += mainwindow.h \
    itemtype1.h \
    imginfo.h \
    imagewidget.h \
    navigatorwidget.h \
    imageloader.h \
    contentwidget.h \
    imageitem.h \
    notifyingscrollarea.h \
    notifyablescrollcontentwidget.h \
    contentwidgetitemfactory.h

FORMS    += mainwindow.ui \
    itemtype1.ui \
    navigatorwidget.ui

CONFIG += c++11

DISTFILES +=

RESOURCES += \
    icons.qrc

QMAKE_CXXFLAGS_RELEASE += -Ofast
