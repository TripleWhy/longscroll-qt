#include "itemfactories.h"
#include "loaderimageitem.h"

ContentWidgetLoaderImageItemFactory::ContentWidgetLoaderImageItemFactory(bool fitImage, QObject * parent)
    : ContentWidgetItemFactory(parent),
      imageFit(fitImage)
{
}

QWidget * ContentWidgetLoaderImageItemFactory::createItemWidget(const ContentItemInfo & info, int width, int height)
{
	Q_UNUSED(width);
	Q_UNUSED(height);
	ImageItemWidget * item = new LoaderImageItem(info, imageFit);
	return item;
}
