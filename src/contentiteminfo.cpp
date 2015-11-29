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

#include "contentiteminfo.h"
#include <QFileInfo>
#include <QSize>
#include <QDataStream>

LONGSCROLLQT_NAMESPACE_BEGIN

/*!
  \class ContentItemInfo
  \brief Represents a longscroll-qt entry.
  ContentItemInfo stores an aspect ratio and some data. The aspect ratio is used by ContentWidget to assign some space for the item.
  The data is used by the displaying widget, to determine how the item looks like. Depending on the chosen display widget, this might require different kind of data.
  All of longscroll-qts built-in displaying widgets expect data to be a QString containing a file name of an image file on the disk.
*/

/*!
 * \brief Constructs an emmpty item.
 */
ContentItemInfo::ContentItemInfo()
{
}

/*!
 * \brief Constructs an item with given data and aspect ratio.
 * \param data Item data.
 * \param ratio Aspect ratio.
 */
ContentItemInfo::ContentItemInfo(const QVariant & data, qreal ratio)
    : ratio(ratio),
      data(data)
{
}

/*!
 * \brief Constructs an item with given data and aspect ratio, derived from a width and height.
 * \param data Item data.
 * \param width With that is used to calculate the aspect ratio.
 * \param height Height that is used to calculate the aspect ratio.
 */
ContentItemInfo::ContentItemInfo(QVariant const & data, int width, int height)
    : data(data)
{
	if (width != 0 && height != 0)
		ratio = qreal(width) / qreal(height);
}

/*!
 * \brief Constructs an item with given data and aspect ratio, derived from a QSize.
 * \param data Item data.
 * \param size Size that is used to calculate the aspect ratio.
 */
ContentItemInfo::ContentItemInfo(const QVariant & data, const QSize & size)
    : ContentItemInfo(data, size.width(), size.height())
{
}

/*!
 * \brief Calculates the width for a given height according this item's aspect ratio.
 * \param height Height to calculate the width for.
 * \return Width for the given height.
 */
int ContentItemInfo::widthForHeight(int height) const
{
	return qRound(ratio * height);
}

/*!
 * \brief Calculates the height for a given width according this item's aspect ratio.
 * \param width Width to calculate the height for.
 * \return Height for the given width.
 */
int ContentItemInfo::heightForWidth(int width) const
{
	return qRound(width / ratio);
}

/*!
 * \brief Returns this item's data.
 * This is used by a displaying widget to determine the look of this item.
 * \return This item's data.
 */
const QVariant & ContentItemInfo::getData() const
{
	return data;
}

bool operator==(const ContentItemInfo & lhs, const ContentItemInfo & rhs)
{
	return lhs.data == rhs.data;
}

bool operator!=(const ContentItemInfo & lhs, const ContentItemInfo & rhs)
{
	return !(lhs == rhs);
}

QDataStream &operator<<(QDataStream & ds, const ContentItemInfo & ci)
{
	ds << ci.ratio << ci.data;
	return ds;
}

QDataStream &operator>>(QDataStream & ds, ContentItemInfo & ci)
{
	ds >> ci.ratio >> ci.data;
	return ds;
}

LONGSCROLLQT_NAMESPACE_END
