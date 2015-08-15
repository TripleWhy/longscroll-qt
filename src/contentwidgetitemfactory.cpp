#include "contentwidgetitemfactory.h"
#include "imageitemwidget.h"

ContentWidgetImageItemFactory::ContentWidgetImageItemFactory(bool fitImage, QObject * parent)
    : ContentWidgetItemFactory(parent),
      imageFit(fitImage)
{
}

QWidget * ContentWidgetImageItemFactory::createItemWidget(const ContentItemInfo & info, int width, int height)
{
	Q_UNUSED(width);
	Q_UNUSED(height);
	ImageItemWidget * item = new ImageItemWidget(info, imageFit);
	return item;
}
