#include "itemfactories.h"
#include "loaderimageitem.h"

ContentWidgetLoaderImageItemFactory::ContentWidgetLoaderImageItemFactory(bool fitImage)
    : ContentWidgetItemFactory(),
      imageFit(fitImage)
{
}

QWidget * ContentWidgetLoaderImageItemFactory::createItemWidget(const ContentItemInfo & info, int itemIndex, ContentWidget * cw)
{
	ImageItemWidget * item = new LoaderImageItem(info, itemIndex, imageFit, cw);
	return item;
}
