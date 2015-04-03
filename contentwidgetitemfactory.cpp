#include "contentwidgetitemfactory.h"
#include "imageitem.h"
#include "itemtype1.h"

ContentWidgetImageItemFactory::ContentWidgetImageItemFactory(bool fitImage, QObject * parent)
    : ContentWidgetItemFactory(parent),
      imageFit(fitImage)
{
}

QWidget * ContentWidgetImageItemFactory::createItemWidget(const ImgInfo & info, int width, int height)
{
	Q_UNUSED(width);
	Q_UNUSED(height);
	ImageItem * item = new ImageItem(info, imageFit);
	item->showing(true);
	return item;
}

QWidget * ContentWidgetImageInfoItemFactory::createItemWidget(const ImgInfo & info, int width, int height)
{
	Q_UNUSED(width);
	ItemType1 * item = new ItemType1(info, height);
	return item;
}

