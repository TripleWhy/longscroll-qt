#include "contentwidgetitemfactory.h"
#include "imageitemwidget.h"
#include "imageinfowidget.h"

LONGSCROLLQT_NAMESPACE_BEGIN

ContentWidgetImageItemFactory::ContentWidgetImageItemFactory(bool fitImage, QObject * parent)
    : ContentWidgetItemFactory(parent),
      imageFit(fitImage)
{
}

QWidget * ContentWidgetImageItemFactory::createItemWidget(const ContentItemInfo & info, int /*width*/, int /*height*/)
{
	return new ImageItemWidget(info, imageFit);
}


QWidget *ContentWidgetImageInfoFactory::createItemWidget(const ContentItemInfo & info, int /*width*/, int /*height*/)
{
	return new ImageInfoWidget(info);
}

LONGSCROLLQT_NAMESPACE_END
