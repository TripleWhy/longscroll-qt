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
