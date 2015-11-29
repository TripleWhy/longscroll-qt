#Copyright (C) 2015 Yannick Mueller
#
#This file is part of longscroll-qt.
#
#longscroll-qt is free software; you can redistribute it and/or modify it under
#the terms of the GNU Affero General Public License as published by the Free
#Software Foundation; either version 3, or (at your option) any later
#version.
#
#longscroll-qt is distributed in the hope that it will be useful, but WITHOUT ANY
#WARRANTY; without even the implied warranty of MERCHANTABILITY or
#FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License
#for more details.
#
#You should have received a copy of the GNU Affero General Public License
#along with longscroll-qt.  If not see <http://www.gnu.org/licenses/>.

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = demo
TEMPLATE = app
CONFIG += c++11

include($$PWD/../src/longscroll-qt_dst-dir.pri)
DEPENDPATH += $$LONGSCROLLQT_DST_DIR/include
INCLUDEPATH += $$LONGSCROLLQT_DST_DIR/include

CONFIG(release, debug|release): LIBNAME = longscroll-qt
CONFIG(debug, debug|release):   LIBNAME = longscroll-qtd

windows: LIBS += -L$$LONGSCROLLQT_DST_DIR/lib -l$${LIBNAME}2
else:    LIBS += -L$$LONGSCROLLQT_DST_DIR/lib -l$${LIBNAME}

macx {
    maclib.files = $$LONGSCROLLQT_DST_DIR/lib/lib$${LIBNAME}.2.dylib
    maclib.path = $$OUT_PWD/$${TARGET}.app/Contents/MacOS
    INSTALLS += maclib
}

HEADERS += \
    mainwindow.h \
    itemfactories.h \
    imageloader.h \
    loaderimageitem.h \
    valueeditdialog.h

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    itemfactories.cpp \
    imageloader.cpp \
    loaderimageitem.cpp \
    valueeditdialog.cpp

FORMS += \
    itemtype1.ui \
    mainwindow.ui \
    valueeditdialog.ui
