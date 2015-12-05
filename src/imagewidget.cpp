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

#include "imagewidget.h"

LONGSCROLLQT_NAMESPACE_BEGIN

/*!
 * \class ImageWidget
 * \brief A widget that displays an image.
 * The image is scaled to the size of the widget keeping its aspect ratio, either by leaving widget space empty or cropping the image.
 * This can be controlled by using setFit().
 */

/*!
 * \brief Constructs an empty ImageWidget.
 * \param parent Parent widget.
 */
ImageWidget::ImageWidget(QWidget *parent)
    : QFrame(parent)
{
}

/*!
 * \brief Destructor.
 */
ImageWidget::~ImageWidget()
{
}

/*!
 * \property ImageWidget::pixmap
 * \brief The displayed Pixmap.
 * \accessors getPixmap(), setPixmap()
 */

/*!
 * \see ImageWidget::pixmap
 */
QPixmap ImageWidget::getPixmap() const
{
	return icon.pixmap(pxSize, QIcon::Normal, QIcon::Off);
}

/*!
 * \see ImageWidget::pixmap
 */
void ImageWidget::setPixmap(const QPixmap &px)
{
	icon = QIcon(px);
	pxSize = px.size();
	update();
}

/*!
 * \property ImageWidget::fit
 * \brief Fit the image entireley inside the widget.
 * Decides wheter the image is fit entirely inside the widget, leaving empty widget space (true) or fills the complete widget space cropping image information (false).
 * \default true
 * \accessors getFit(), setFit()
 */

/*!
 * \see ImageWidget::fit
 */
bool ImageWidget::getFit() const
{
	return fit;
}

/*!
 * \see ImageWidget::fit
 */
void ImageWidget::setFit(bool _fit)
{
	if (_fit == fit)
		return;
	fit = _fit;
	update();
}

/*!
 * \property ImageWidget::selected
 * \brief Draw the widget as selected.
 * \default false
 * \accessors isSelected(), setSelected()
 */

/*!
 * \see ImageWidget::selected
 */
bool ImageWidget::isSelected() const
{
	return selected;
}

/*!
 * \see ImageWidget::selected
 */
void ImageWidget::setSelected(bool _selected)
{
	if (_selected == selected)
		return;
	selected = _selected;
	update();
}

/*!
 * \reimp{QFrame::paintEvent}
 */
void ImageWidget::paintEvent(QPaintEvent * e)
{
	QFrame::paintEvent(e);

	QPainter painter(this);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

	QIcon::Mode mode = QIcon::Normal;
	if (!isEnabled())
		mode = QIcon::Disabled;
	else if (selected)
		mode = QIcon::Selected;

	QPixmap const & pix = icon.pixmap(pxSize, mode, QIcon::Off);
	QSize && s = size();
	s.scale(pxSize, fit ? Qt::KeepAspectRatioByExpanding : Qt::KeepAspectRatio);
	QRect r(QPoint(), s);
	r.moveCenter(pix.rect().center());

	painter.drawPixmap(rect(), pix, r);
}

/*!
 * \reimp{QFrame::hasHeightForWidth}
 * \return true
 */
bool ImageWidget::hasHeightForWidth() const
{
	return true;
}

/*!
 * \reimp{QFrame::heightForWidth}
 */
int ImageWidget::heightForWidth(int w) const
{
	if (pxSize.isEmpty())
		return -1;
	return qRound(qreal(w * pxSize.height()) / qreal(pxSize.width()));
}

/*!
 * \reimp{QFrame::sizeHint}
 */
QSize ImageWidget::sizeHint() const
{
	return pxSize;
}

LONGSCROLLQT_NAMESPACE_END
