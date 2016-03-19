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

#include "contentwidgetitemfactory.h"
#include "imageitemwidget.h"
#include "imageinfowidget.h"
#include "contentwidget.h"

LONGSCROLLQT_NAMESPACE_BEGIN

/*!
 * \interface ContentWidgetItemFactory
 * \brief Interface for item factories used by ContentWidget to create display items.
 */

ContentWidgetItemFactory::~ContentWidgetItemFactory()
{
}

/*!
 * \fn ContentWidgetItemFactory::createItemWidget
 * \brief Creates a display widget for given item info.
 * This method receives a pointer to the ContentWidget, this is useful for example if the widget vizualizes the selection in some way, to connect to selection changes.
 * The ContentWidget will take ownership of the returned widget.
 * \param info Item info to be displayed.
 * \param itemIndex Item info index in ContentWidget's item info list.
 * \param cw The ContentWidget.
 * \return A newly created QWidget.
 * \see ContentWidget::createItemWidget
 */


/*!
 * \class ContentWidgetImageItemFactory
 * \brief ContentWidgetItemFactory that creates \ref ImageItemWidget "ImageItemWidget"s.
 */

/*!
 * \brief Constructs a ContentWidgetImageItemFactory.
 * \param fitImage \c fit parameter passed to ImageItemWidget.
 */
ContentWidgetImageItemFactory::ContentWidgetImageItemFactory(bool fitImage)
    : ContentWidgetItemFactory(),
      imageFit(fitImage)
{
}

/*!
 * \reimp{ContentWidgetItemFactory::createItemWidget}
 */
QWidget * ContentWidgetImageItemFactory::createItemWidget(const ContentItemInfo & info, int itemIndex, ContentWidget * cw)
{
	return new ImageItemWidget(info, itemIndex, imageFit, cw);
}


/*!
 * \class ContentWidgetImageInfoFactory
 * \brief ContentWidgetImageInfoFactory that creates \ref ImageInfoWidget "ImageInfoWidget"s.
 */

/*!
 * \reimp{ContentWidgetItemFactory::createItemWidget}
 */
QWidget *ContentWidgetImageInfoFactory::createItemWidget(const ContentItemInfo & info, int /*itemIndex*/, ContentWidget * /*cw*/)
{
	return new ImageInfoWidget(info);
}

LONGSCROLLQT_NAMESPACE_END
