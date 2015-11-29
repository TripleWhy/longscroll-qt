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

#ifndef CONTENTITEMINFO_H
#define CONTENTITEMINFO_H

#include "longscroll-qt_global.h"
#include "longscroll-qt_qtincludes.h"

LONGSCROLLQT_NAMESPACE_BEGIN

struct LONGSCROLLQT_EXPORT ContentItemInfo
{
private:
	qreal ratio = 1.0;
	QVariant data;

public:
	ContentItemInfo();
	ContentItemInfo(QVariant const & data, qreal ratio);
	ContentItemInfo(QVariant const & data, int width, int height);
	ContentItemInfo(QVariant const & data, QSize const & size);

	int widthForHeight(int height) const;
	int heightForWidth(int width) const;

	QVariant const & getData() const;

private:
	LONGSCROLLQT_EXPORT friend bool operator==(ContentItemInfo const & lhs, ContentItemInfo const & rhs);
	LONGSCROLLQT_EXPORT friend bool operator!=(ContentItemInfo const & lhs, ContentItemInfo const & rhs);
	LONGSCROLLQT_EXPORT friend QDataStream &operator<<(QDataStream & ds, ContentItemInfo const & ci);
	LONGSCROLLQT_EXPORT friend QDataStream &operator>>(QDataStream & ds, ContentItemInfo & ci);
};

LONGSCROLLQT_NAMESPACE_END

#endif // CONTENTITEMINFO_H
