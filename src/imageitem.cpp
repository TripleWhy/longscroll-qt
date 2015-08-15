#include "imageitem.h"
#include "imagewidget.h"
#include <QVBoxLayout>

ImageItem::ImageItem(const ContentItemInfo & info, bool fit, bool doLoadImage, QWidget * parent)
    : QFrame(parent),
      fit(fit),
      info(info)
{
	new QVBoxLayout(this);
	layout()->setMargin(0);
	layout()->setSpacing(0);

	label = new ImageWidget();
	label->setFit(fit);
	layout()->addWidget(label);

	if (doLoadImage)
		loadImage();
}

ImageItem::ImageItem(const ContentItemInfo & info, bool fit, QWidget * parent)
    : ImageItem(info, fit, true, parent)
{
}

ImageItem::~ImageItem()
{
}

void ImageItem::loadImage()
{
	QPixmap px;
	px.load(info.data.toString());
	label->setPixmap(px);
}

void ImageItem::setHeight(int h)
{
	size.setHeight(h);
	size.setWidth(info.widthForHeight(h));
	setFixedHeight(h);
	setMinimumHeight(h);
	resize(size);
}

QSize ImageItem::sizeHint() const
{
	return size;
}
