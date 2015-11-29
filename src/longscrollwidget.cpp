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

#include "longscrollwidget.h"
#include <QScrollBar>

LONGSCROLLQT_NAMESPACE_BEGIN

/*!
 * \class LongscrollWidget
 * \brief The longscroll widget.
 * Use \ref getContentWidget to customize the behavior.
 */

/*!
 * \brief LongscrollWidget::LongscrollWidget
 * \param parent
 */
LongscrollWidget::LongscrollWidget(QWidget * parent)
    : NotifyingScrollArea(parent)
{
	setWidgetResizable(true);
	cw = new ContentWidget;
	setWidget(cw);
	connect(cw, SIGNAL(scrollToRequest(int)), verticalScrollBar(), SLOT(setValue(int)));
}

/*!
 * \overloaded
 */
ContentWidget const * LongscrollWidget::getContentWidget() const
{
	return cw;
}

/*!
 * \brief Returns the ContentWidget.
 * \return ContentWidget
 */
ContentWidget * LongscrollWidget::getContentWidget()
{
	return cw;
}

LONGSCROLLQT_NAMESPACE_END
