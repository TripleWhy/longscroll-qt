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

#ifndef IMAGEINFOWIDGET_H
#define IMAGEINFOWIDGET_H

#include "longscroll-qt_global.h"
#include "contentiteminfo.h"
#include "longscroll-qt_qtincludes.h"

namespace Ui {
class ImageInfoWidget;
}

LONGSCROLLQT_NAMESPACE_BEGIN

class LONGSCROLLQT_EXPORT ImageInfoWidget : public QFrame
{
	Q_OBJECT

public:
	explicit ImageInfoWidget(QWidget *parent = 0);
	explicit ImageInfoWidget(ContentItemInfo const & info, QWidget *parent = 0);
	virtual ~ImageInfoWidget();

	void setItemInfo(ContentItemInfo const & info);

private:
	::Ui::ImageInfoWidget *ui;
};

LONGSCROLLQT_NAMESPACE_END

#endif // IMAGEINFOWIDGET_H
