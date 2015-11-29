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

#include "notifyablescrollcontentwidget.h"

LONGSCROLLQT_NAMESPACE_BEGIN

/*!
 * \class NotifyableScrollContentWidget
 * \brief Base class used by NotifyingScrollArea to inform the widget about visible region changes.
 * \see NotifyingScrollArea
 */

/*!
 * \fn NotifyableScrollContentWidget::showingRect
 * \brief Called by NotifyingScrollArea whenever the visible region changes.
 * \param visible Visible rectangle.
 */

LONGSCROLLQT_NAMESPACE_END
