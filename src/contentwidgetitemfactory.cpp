#include "contentwidgetitemfactory.h"
#include "imageitemwidget.h"
#include "imageinfowidget.h"
#include "contentwidget.h"

LONGSCROLLQT_NAMESPACE_BEGIN

ContentWidgetImageItemFactory::ContentWidgetImageItemFactory(bool fitImage)
    : ContentWidgetItemFactory(),
      imageFit(fitImage)
{
}

QWidget * ContentWidgetImageItemFactory::createItemWidget(const ContentItemInfo & info, int itemIndex, ContentWidget * cw)
{
	return new ImageItemWidget(info, itemIndex, imageFit, cw);
}


QWidget *ContentWidgetImageInfoFactory::createItemWidget(const ContentItemInfo & info, int /*itemIndex*/, ContentWidget * /*cw*/)
{
	return new ImageInfoWidget(info);
}

LONGSCROLLQT_NAMESPACE_END
