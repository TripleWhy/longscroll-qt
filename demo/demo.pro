QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = demo
TEMPLATE = app
CONFIG += c++11
QMAKE_CXXFLAGS_RELEASE += -Ofast


include($$PWD/../src/longscroll-qt_dst-dir.pri)
message($$LONGSCROLLQT_DST_DIR)
DEPENDPATH += $$LONGSCROLLQT_DST_DIR/include
INCLUDEPATH += $$LONGSCROLLQT_DST_DIR/include

CONFIG(release, debug|release): LIBS += -L$$LONGSCROLLQT_DST_DIR/lib -llongscroll-qt
CONFIG(debug, debug|release):   LIBS += -L$$LONGSCROLLQT_DST_DIR/lib -llongscroll-qtd
message($$LIBS)

HEADERS += \
    itemtype1.h \
    mainwindow.h \
    itemfactories.h \
    imageloader.h \
    loaderimageitem.h \
    loadernavigatorwidget.h

SOURCES += \
    itemtype1.cpp \
    main.cpp \
    mainwindow.cpp \
    itemfactories.cpp \
    imageloader.cpp \
    loaderimageitem.cpp \
    loadernavigatorwidget.cpp

FORMS += \
    itemtype1.ui \
    mainwindow.ui
