QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = demo
TEMPLATE = app
CONFIG += c++11

include($$PWD/../src/longscroll-qt_dst-dir.pri)
DEPENDPATH += $$LONGSCROLLQT_DST_DIR/include
INCLUDEPATH += $$LONGSCROLLQT_DST_DIR/include

CONFIG(release, debug|release): LIBS += -L$$LONGSCROLLQT_DST_DIR/lib -llongscroll-qt
CONFIG(debug, debug|release):   LIBS += -L$$LONGSCROLLQT_DST_DIR/lib -llongscroll-qtd

HEADERS += \
    mainwindow.h \
    itemfactories.h \
    imageloader.h \
    loaderimageitem.h

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    itemfactories.cpp \
    imageloader.cpp \
    loaderimageitem.cpp

FORMS += \
    itemtype1.ui \
    mainwindow.ui
