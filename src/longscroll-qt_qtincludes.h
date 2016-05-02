/*
Copyright (C) 2015 Yannick Mueller

This file is part of longscroll-qt.

longscroll-qt is free software; you can redistribute it and/or modify it under
the terms of the GNU Affero General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

longscroll-qt is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License
for more details.

You should have received a copy of the GNU Affero General Public License
along with longscroll-qt.  If not see <http://www.gnu.org/licenses/>.
*/

#ifndef LONGSCROLLQT_QTINCLUDESL_H
#define LONGSCROLLQT_QTINCLUDESL_H

#include <QtGlobal>

#ifdef Q_CC_GNU
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wconversion"
# if defined(__GNUC__) && defined(__GNUC_MINOR__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 9))
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
# endif
#endif
#ifdef Q_CC_CLANG
# pragma GCC diagnostic ignored "-Wdeprecated"
# pragma GCC diagnostic ignored "-Wused-but-marked-unused"
# pragma GCC diagnostic ignored "-Wfloat-equal"
# pragma GCC diagnostic ignored "-Wsign-conversion"
# pragma GCC diagnostic ignored "-Wexit-time-destructors"
# pragma GCC diagnostic ignored "-Wcovered-switch-default"
# pragma GCC diagnostic ignored "-Wdisabled-macro-expansion"
# ifdef Q_OS_MAC
#  pragma GCC diagnostic ignored "-Wdeprecated"
# else
# endif
#endif

#include <QScrollArea>
#include <QFrame>
#include <QVariant>
#include <QPainter>
#include <QPainter>
#include <QMouseEvent>
#include <QImageReader>
#include <QAbstractItemView>

#ifdef Q_CC_GNU
#pragma GCC diagnostic pop
#endif

#endif // LONGSCROLLQT_QTINCLUDESL_H
