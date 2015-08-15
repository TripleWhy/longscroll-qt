#include "itemfactories.h"
#include "loaderimageitem.h"
#include "itemtype1.h"

ContentWidgetLoaderImageItemFactory::ContentWidgetLoaderImageItemFactory(bool fitImage, QObject * parent)
    : ContentWidgetItemFactory(parent),
      imageFit(fitImage)
{
}

QWidget * ContentWidgetLoaderImageItemFactory::createItemWidget(const ImgInfo & info, int width, int height)
{
	Q_UNUSED(width);
	Q_UNUSED(height);
	ImageItem * item = new LoaderImageItem(info, imageFit);
	return item;
}


QWidget * ContentWidgetImageInfoItemFactory::createItemWidget(const ImgInfo & info, int width, int height)
{
	Q_UNUSED(width);
	ItemType1 * item = new ItemType1(info, height);
	return item;
}
