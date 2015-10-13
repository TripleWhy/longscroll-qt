#include "contentwidgetitemfactory.h"
#include "imageitemwidget.h"
#include "imageinfowidget.h"
#include "contentwidget.h"

LONGSCROLLQT_NAMESPACE_BEGIN

ContentWidgetImageItemFactory::ContentWidgetImageItemFactory(bool fitImage, QObject * parent)
    : ContentWidgetItemFactory(parent),
      imageFit(fitImage)
{
}

QWidget * ContentWidgetImageItemFactory::createItemWidget(const ContentItemInfo & info, int itemIndex, int /*width*/, int /*height*/, ContentWidget * cw)
{
	return new ImageItemWidget(info, itemIndex, imageFit, cw);
}


QWidget *ContentWidgetImageInfoFactory::createItemWidget(const ContentItemInfo & info, int /*itemIndex*/, int /*width*/, int /*height*/, ContentWidget * /*cw*/)
{
	return new ImageInfoWidget(info);
}

LONGSCROLLQT_NAMESPACE_END
