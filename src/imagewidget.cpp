#include "imagewidget.h"
#include <QPainter>

LONGSCROLLQT_NAMESPACE_BEGIN

ImageWidget::ImageWidget(QWidget *parent) : QFrame(parent)
{
}

ImageWidget::~ImageWidget()
{
}

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

bool ImageWidget::hasHeightForWidth() const
{
	return true;
}

int ImageWidget::heightForWidth(int w) const
{
	if (pxSize.isEmpty())
		return -1;
	return qRound(qreal(w * pxSize.height()) / qreal(pxSize.width()));
}

QSize ImageWidget::sizeHint() const
{
	return pxSize;
}

void ImageWidget::setPixmap(const QPixmap &px)
{
	icon = QIcon(px);
	pxSize = px.size();
	update();
}

void ImageWidget::setFit(bool _fit)
{
	if (_fit == fit)
		return;
	fit = _fit;
	update();
}

void ImageWidget::setSelected(bool _selected)
{
	if (_selected == selected)
		return;
	selected = _selected;
	update();
}

LONGSCROLLQT_NAMESPACE_END
