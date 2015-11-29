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

#ifndef LONGSCROLLQT_GLOBAL_H
#define LONGSCROLLQT_GLOBAL_H

#include <QtCore/QtGlobal>

#if defined(LONGSCROLLQT_LIBRARY)
#  define LONGSCROLLQT_EXPORT Q_DECL_EXPORT
#else
#  define LONGSCROLLQT_EXPORT Q_DECL_IMPORT
#endif

#ifndef LONGSCROLLQT_NAMESPACE
#  define LONGSCROLLQT_NAMESPACE       longscroll
#endif
#define LONGSCROLLQT_NAMESPACE_BEGIN namespace LONGSCROLLQT_NAMESPACE {
#define LONGSCROLLQT_NAMESPACE_END   }
#define LONGSCROLLQT_NAMESPACE_USING using namespace LONGSCROLLQT_NAMESPACE;

#endif // LONGSCROLLQT_GLOBAL_H
