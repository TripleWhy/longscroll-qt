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

#include "notifyingscrollarea.h"
#include "notifyablescrollcontentwidget.h"
#include <QScrollBar>

LONGSCROLLQT_NAMESPACE_BEGIN

/*!
 * \class NotifyingScrollArea
 * \brief Scroll area that informs widgets inside about their visible region.
 * When the the content is scrolled, resized, shown or hidden, the widget searches for instances of NotifyableScrollContentWidget and
 * informs them about their current visible rectangle by calling NotifyableScrollContentWidget::showingRect().
 * However it does not informa all children. If the \ref widget() returns an instance of NotifyableScrollContentWidget, only that
 * one will be informed, otherwise all direct children of widget() that derive from NotifyableScrollContentWidget will be informed.
 *
 * Note: A widget can determine its visible region on its own, but it does not now when it changes.
 * It is also slightly more effective to determine the visible rectangle for all children than let it do every widget.
 */

/*!
 * \reimp{QScrollArea::scrollContentsBy}
 */
void NotifyingScrollArea::scrollContentsBy(int dx, int dy)
{
	QScrollArea::scrollContentsBy(dx, dy);
	findVisible();
}

/*!
 * \reimp{QScrollArea::resizeEvent}
 */
void NotifyingScrollArea::resizeEvent(QResizeEvent * event)
{
	QScrollArea::resizeEvent(event);
	findVisible();
}

/*!
 * \reimp{QScrollArea::showEvent}
 */
void NotifyingScrollArea::showEvent(QShowEvent * event)
{
	QScrollArea::showEvent(event);
	findVisible();
}

/*!
 * \reimp{QScrollArea::hideEvent}
 */
void NotifyingScrollArea::hideEvent(QHideEvent * event)
{
	QScrollArea::hideEvent(event);
	findVisible();
}

/*!
 * \brief Finds NotifyableScrollContentWidget and informs them about their visible region.
 * Called every thime the widget is scrolled, resized, shown or hidden.
 */
void NotifyingScrollArea::findVisible()
{
	QWidget * wid = widget();
	if (wid == 0)
		return;

	QRect const & visible = wid->visibleRegion().boundingRect();

	NotifyableScrollContentWidget * nscw = qobject_cast<NotifyableScrollContentWidget *>(wid);
	if (nscw != 0)
	{
		nscw->showingRect(visible);
		return;
	}

	QList<NotifyableScrollContentWidget *> const & widgets = wid->findChildren<NotifyableScrollContentWidget *>(QString(), Qt::FindDirectChildrenOnly);
	for (NotifyableScrollContentWidget * w : widgets)
	{
		QRect && r = visible.intersected(w->geometry());
		if (!r.isEmpty())
			r.moveTopLeft( w->mapFromParent(r.topLeft()) );
		// r == w->visibleRegion().boundingRect(), thus w could determine r on its own, but this method is a little less computationally intensive.
		w->showingRect(r);
	}
}

LONGSCROLLQT_NAMESPACE_END
