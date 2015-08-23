#include "imagewidget.h"
#include <QEvent>
#include <QResizeEvent>
#include <QLabel>

LONGSCROLLQT_NAMESPACE_BEGIN

ImageWidget::ImageWidget(QWidget *parent) : QFrame(parent)
{
	imgLabel = new QLabel(this);
	imgLabel->setScaledContents(true);
}

ImageWidget::~ImageWidget()
{
}

void ImageWidget::resizeEvent(QResizeEvent *event)
{
	relayout(event->size());
}

bool ImageWidget::event(QEvent *event)
{
	if (event->type() == QEvent::LayoutRequest)
	{
		relayout(size());
		event->accept();
		return true;
	}
	else
		return QWidget::event(event);
}

void ImageWidget::relayout(const QSize &size)
{
	int w = size.width();
	int h = size.height();
	int lw = w;
	int lh = h;
	double r = double(w) / double(h);
	if (qIsNaN(pxr) || pxr == r)
		imgLabel->setGeometry(0, 0, size.width(), size.height());
	else if ((fit && pxr > r) || (!fit && pxr < r))
	{
		lh = qRound(w / pxr);
		imgLabel->setGeometry(0, (h - lh) / 2, lw, lh);
	}
	else
	{
		lw = qRound(pxr * h);
		imgLabel->setGeometry((w - lw) / 2, 0, lw, lh);
	}
}

bool ImageWidget::hasHeightForWidth() const
{
	return true;
}

int ImageWidget::heightForWidth(int w) const
{
	if (qIsNaN(pxr))
		return -1;
	return qRound(w / pxr);
}

QSize ImageWidget::sizeHint() const
{
	return pxSize;
}

void ImageWidget::setPixmap(const QPixmap &px)
{
	imgLabel->setPixmap(px);
	pxr = double(px.width()) / double(px.height());
	pxSize = px.size();
	relayout(size());
}

void ImageWidget::setFit(bool _fit)
{
	fit = _fit;
	relayout(size());
}

LONGSCROLLQT_NAMESPACE_END
