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

#include "imageitemwidget.h"
#include "imagewidget.h"
#include "contentwidget.h"
#include <QVBoxLayout>

LONGSCROLLQT_NAMESPACE_BEGIN

/*!
  \class ImageItemWidget
  \brief Simple widget displays an image from a ContentItemInfo.
  The image is either fit inside the widget by leaving some widget space empty or by cropping the image (see ImageWidget for more details).
  This behavior can be controlled using the fit parameter on construction.

  ImageItemWidget can also visualize a selection state. This can be enabled by specifying a ContentWidget on construction,
  or connecting or calling the updateSelection() slot later.
*/

/*!
 * \brief Constructs an ImageItemWidget.
 * Use This constructor if you don't want this widget to load the image (yet).
 * \param info
 * \param itemIndex
 * \param fit
 * \param doLoadImage
 * \param cw
 * \param parent
 */
ImageItemWidget::ImageItemWidget(const ContentItemInfo & info, int itemIndex, bool fit, bool doLoadImage, ContentWidget * cw, QWidget * parent)
    : QFrame(parent),
      info(info),
      itemIndex(itemIndex)
{
	new QVBoxLayout(this);
	layout()->setMargin(0);
	layout()->setSpacing(0);

	imageWidget = new ImageWidget();
	imageWidget->setFit(fit);
	layout()->addWidget(imageWidget);

	if (cw != 0)
		connect(cw, &ContentWidget::selectionChanged, this, &ImageItemWidget::updateSelection);

	if (doLoadImage)
		loadImage();
}

/*!
 * \brief Constructs an ImageItemWidget.
 * \param info
 * \param itemIndex Used to check if the item this widget represents is selected.
 * \param fit Controls how the image is fit inside the widget. See ImageWidget.
 * \param cw If cw is set, this widget will automatically connect its selectionChanged() signal to this updateSelection() slot.
 * \param parent parent is passed to the QFrame constructor.
 */
ImageItemWidget::ImageItemWidget(const ContentItemInfo & info, int itemIndex, bool fit, ContentWidget * cw, QWidget * parent)
    : ImageItemWidget(info, itemIndex, fit, true, cw, parent)
{
}

ImageItemWidget::~ImageItemWidget()
{
}

/*!
 * \brief Updates the selection state.
 * The widget will be drawn as selected if the selection contains the itemIndex that was passed to the constructor.
 * \param selection
 */
void ImageItemWidget::updateSelection(const QList<int> & selection)
{
	selected = selection.contains(itemIndex);
	imageWidget->setSelected(selected);
}

/*!
 * \brief Loads and displays the image.
 * Loads the image referenced by the ContentItemInfo passed on construction, by interpreting its data as string file name.
 * You can use this if you did not load the image on construction.
 */
void ImageItemWidget::loadImage()
{
	QPixmap px;
	px.load(info.getData().toString());
	imageWidget->setPixmap(px);
}

/*!
 * \reimp{QFrame::paintEvent}
 */
void ImageItemWidget::paintEvent(QPaintEvent * e)
{
	if (selected)
	{
		QPainter painter(this);
		QStyleOptionViewItem opt;
		opt.initFrom(this);
		opt.backgroundBrush = Qt::transparent;
		opt.state |= QStyle::State_Selected;
		style()->drawControl(QStyle::CE_ItemViewItem, &opt, &painter, this);
	}
	QFrame::paintEvent(e);
}

/*!
 * \var ImageItemWidget::imageWidget
 * \brief Widget used to display the image.
 */

/*!
 * \var ImageItemWidget::info
 * \brief Info of the displayed item.
 */

/*!
 * \var ImageItemWidget::itemIndex
 * \brief Index of the displayed item.
 */

/*!
 * \var ImageItemWidget::selected
 * \brief Stores whether the item is selected.
 */

LONGSCROLLQT_NAMESPACE_END
