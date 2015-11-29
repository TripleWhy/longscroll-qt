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

#ifndef NOTIFYINGSCROLLAREA_H
#define NOTIFYINGSCROLLAREA_H

#include "longscroll-qt_global.h"
#include "longscroll-qt_qtincludes.h"

LONGSCROLLQT_NAMESPACE_BEGIN

class LONGSCROLLQT_EXPORT NotifyingScrollArea : public QScrollArea
{
	Q_OBJECT
public:
	using QScrollArea::QScrollArea;

protected:
	virtual void scrollContentsBy(int dx, int dy) override;
	virtual void resizeEvent(QResizeEvent * event) override;
	virtual void showEvent(QShowEvent * event) override;
	virtual void hideEvent(QHideEvent * event) override;
	void findVisible();
};

LONGSCROLLQT_NAMESPACE_END

#endif // NOTIFYINGSCROLLAREA_H
