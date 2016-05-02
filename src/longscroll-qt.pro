#Copyright (C) 2015-2016 Yannick Mueller
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

lessThan(QT_MAJOR_VERSION, 5): error("This library needs Qt 5.")
QT       += core gui widgets

CONFIG(debug, debug|release) {
    TARGET = longscroll-qtd
} else {
    TARGET = longscroll-qt
}
TEMPLATE = lib
VERSION = 2.0.0
CONFIG += dll

CONFIG += c++11
*-g++*|*-clang*|*-icc* {
	QMAKE_CXXFLAGS_RELEASE += -Ofast
	QMAKE_CXXFLAGS_WARN_ON += -Wextra -Wconversion -Werror=return-type   -Wpedantic -Wcast-qual -Wcast-align -Wwrite-strings
	clang: QMAKE_CXXFLAGS_WARN_ON += -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-documentation -Wno-padded -Wno-shadow -Wno-undefined-reinterpret-cast
} else:*-msvc* {
	QMAKE_CXXFLAGS_RELEASE += /O2
}

!isEmpty(CONTINUOUS_INTEGRATION) {
	msvc {
		QMAKE_CXXFLAGS += /WX
		QMAKE_LFLAGS += /WX
	} else {
		QMAKE_CXXFLAGS += -Werror
		QMAKE_LFLAGS += -Werror
	}
}

DEFINES += LONGSCROLLQT_LIBRARY

CONFIG(release, debug|release): RD = release
CONFIG(debug, debug|release):   RD = debug
include( longscroll-qt_dst-dir.pri )


docDistDir.target = $$PWD/../dist/doc
docDistDir.commands = mkdir -p $$docDistDir.target

docHtml.commands = cd $$PWD && doxygen ../doxygen/doxyfile-html
docHtml.depends = docDistDir

docQhp.commands = cd $$PWD && doxygen ../doxygen/doxyfile-qhp
docQhp.depends = docDistDir

docs.depends = docHtml docQhp

QMAKE_EXTRA_TARGETS += docDistDir docHtml docQhp docs


HEADERS += \
    contentwidget.h \
    contentwidgetitemfactory.h \
    imagewidget.h \
    longscroll-qt_global.h \
    longscroll-qt_qtincludes.h \
    navigatorwidget.h \
    notifyablescrollcontentwidget.h \
    notifyingscrollarea.h \
    contentiteminfo.h \
    imageitemwidget.h \
    imageinfowidget.h \
    longscrollwidget.h

SOURCES += \
    contentwidget.cpp \
    contentwidgetitemfactory.cpp \
    imagewidget.cpp \
    navigatorwidget.cpp \
    notifyablescrollcontentwidget.cpp \
    notifyingscrollarea.cpp \
    contentiteminfo.cpp \
    imageitemwidget.cpp \
    imageinfowidget.cpp \
    longscrollwidget.cpp

FORMS += \
    navigatorwidget.ui \
    imageinfowidget.ui


headersDataFiles.path = $$LONGSCROLLQT_DST_DIR/include/longscroll-qt
headersDataFiles.files = $$HEADERS
INSTALLS += headersDataFiles

target.path = $$LONGSCROLLQT_DST_DIR/lib
INSTALLS += target
