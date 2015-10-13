#include "itemfactories.h"
#include "loaderimageitem.h"

ContentWidgetLoaderImageItemFactory::ContentWidgetLoaderImageItemFactory(bool fitImage, QObject * parent)
    : ContentWidgetItemFactory(parent),
      imageFit(fitImage)
{
}

QWidget * ContentWidgetLoaderImageItemFactory::createItemWidget(const ContentItemInfo & info, int itemIndex, int width, int height, ContentWidget * cw)
{
	Q_UNUSED(width);
	Q_UNUSED(height);
	ImageItemWidget * item = new LoaderImageItem(info, itemIndex, imageFit, cw);
	return item;
}
