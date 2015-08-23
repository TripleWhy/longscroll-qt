#include "imageitemwidget.h"
#include "imagewidget.h"
#include <QVBoxLayout>

LONGSCROLLQT_NAMESPACE_BEGIN

ImageItemWidget::ImageItemWidget(const ContentItemInfo & info, bool fit, bool doLoadImage, QWidget * parent)
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

ImageItemWidget::ImageItemWidget(const ContentItemInfo & info, bool fit, QWidget * parent)
    : ImageItemWidget(info, fit, true, parent)
{
}

ImageItemWidget::~ImageItemWidget()
{
}

void ImageItemWidget::loadImage()
{
	QPixmap px;
	px.load(info.data.toString());
	label->setPixmap(px);
}

LONGSCROLLQT_NAMESPACE_END
