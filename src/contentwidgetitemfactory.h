/*
Copyright (C) 2015-2016 Yannick Mueller

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

#ifndef CONTENTWIDGETITEMFACTORY_H
#define CONTENTWIDGETITEMFACTORY_H

#include "longscroll-qt_global.h"
#include "contentiteminfo.h"
#include <QObject>
#include <QWidget>

LONGSCROLLQT_NAMESPACE_BEGIN

class ContentWidget;

class LONGSCROLLQT_EXPORT ContentWidgetItemFactory
{
public:
	virtual ~ContentWidgetItemFactory();
	virtual QWidget * createItemWidget(ContentItemInfo const & info, int itemIndex, ContentWidget * cw) = 0;
};

class LONGSCROLLQT_EXPORT ContentWidgetImageItemFactory : public ContentWidgetItemFactory
{
private:
	bool imageFit;
public:
	ContentWidgetImageItemFactory(bool fitImage = false);
	virtual QWidget * createItemWidget(ContentItemInfo const & info, int itemIndex, ContentWidget * cw) override;
};

class LONGSCROLLQT_EXPORT ContentWidgetImageInfoFactory : public ContentWidgetItemFactory
{
public:
	using ContentWidgetItemFactory::ContentWidgetItemFactory;
	virtual QWidget * createItemWidget(ContentItemInfo const & info, int itemIndex, ContentWidget * cw) override;
};

LONGSCROLLQT_NAMESPACE_END

#endif // CONTENTWIDGETITEMFACTORY_H
