QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG(debug, debug|release) {
    TARGET = longscroll-qtd
} else {
    TARGET = longscroll-qt
}
TEMPLATE = lib
VERSION = 2.0.0
CONFIG += dll

CONFIG += c++11
QMAKE_CXXFLAGS_RELEASE += -Ofast
QMAKE_CXXFLAGS_WARN_ON += -Wextra -Wconversion -Werror=return-type   -Wpedantic -Wcast-qual -Wcast-align -Wwrite-strings

DEFINES += LONGSCROLLQT_LIBRARY

CONFIG(release, debug|release): RD = release
CONFIG(debug, debug|release):   RD = debug
include( longscroll-qt_dst-dir.pri )

headersDataFiles.path = $$LONGSCROLLQT_DST_DIR/include/longscroll-qt
headersDataFiles.files = $$PWD/*.h

linux: libraryFiles.path = $$LONGSCROLLQT_DST_DIR/lib
win32 {
	libraryFiles.files = $$OUT_PWD/$$RD/*.a $$OUT_PWD/$$RD/*.prl $$OUT_PWD/$$RD/*.lib
} else {
	libraryFiles.files = $$OUT_PWD/*.so $$OUT_PWD/*.so.*
}

INSTALLS += headersDataFiles
INSTALLS += libraryFiles



HEADERS += \
    contentwidget.h \
    contentwidgetitemfactory.h \
    imagewidget.h \
    longscroll-qt_global.h \
    navigatorwidget.h \
    notifyablescrollcontentwidget.h \
    notifyingscrollarea.h \
    contentiteminfo.h \
    imageitemwidget.h

SOURCES += \
    contentwidget.cpp \
    contentwidgetitemfactory.cpp \
    imagewidget.cpp \
    navigatorwidget.cpp \
    notifyablescrollcontentwidget.cpp \
    notifyingscrollarea.cpp \
    contentiteminfo.cpp \
    imageitemwidget.cpp

FORMS += \
    navigatorwidget.ui

OTHER_FILES += \
    longscroll-qt_dst-dir.pri
