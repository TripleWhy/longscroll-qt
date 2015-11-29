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

#ifndef IMAGEITEMWIDGET_H
#define IMAGEITEMWIDGET_H

#include "longscroll-qt_global.h"
#include "contentiteminfo.h"
#include "longscroll-qt_qtincludes.h"

LONGSCROLLQT_NAMESPACE_BEGIN

class ImageWidget;
class ContentWidget;

class LONGSCROLLQT_EXPORT ImageItemWidget : public QFrame
{
	Q_OBJECT
protected:
	ImageItemWidget(ContentItemInfo const & info, int itemIndex, bool fit, bool loadImage, ContentWidget * cw, QWidget * parent);
public:
	ImageItemWidget(ContentItemInfo const & info, int itemIndex, bool fit = false, ContentWidget * cw = 0, QWidget * parent = 0);
	~ImageItemWidget();

public slots:
	virtual void updateSelection(QList<int> const & selection);

protected:
	void loadImage();

protected:
	virtual void paintEvent(QPaintEvent * e) override;

protected:
	ImageWidget * imageWidget = 0;
	ContentItemInfo info;
	int itemIndex = -1;
	bool selected = false;
};

LONGSCROLLQT_NAMESPACE_END

#endif // IMAGEITEMWIDGET_H
