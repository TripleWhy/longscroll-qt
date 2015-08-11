#include "imageitem.h"
#include "imagewidget.h"
#include <QVBoxLayout>

ImageItem::ImageItem(const ImgInfo & info, bool fit)
    : NotifyableScrollContentWidget(),
      fit(fit),
      info(info)
{
	new QVBoxLayout(this);
	layout()->setMargin(0);
	layout()->setSpacing(0);
}

ImageItem::~ImageItem()
{
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

void ImageItem::setShowing(bool visible)
{
	if (visible)
	{
		if (label != 0)
			return;
		label = new ImageWidget();
		label->setFit(fit);
		QPixmap px;
		px.load(info.fileName);
		label->setPixmap(px);
		layout()->addWidget(label);
	}
	else
	{
		delete label;
		label = 0;
	}
}
