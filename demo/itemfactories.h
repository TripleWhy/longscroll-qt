#ifndef ITEMFACTORIES_H
#define ITEMFACTORIES_H

#include <longscroll-qt/contentwidgetitemfactory.h>

using namespace longscroll;

class ContentWidgetLoaderImageItemFactory : public ContentWidgetItemFactory
{
private:
	bool imageFit;
public:
	ContentWidgetLoaderImageItemFactory(bool fitImage = false);
	virtual QWidget * createItemWidget(ContentItemInfo const & info, int itemIndex, int width, int height, ContentWidget * cw) override;
};

#endif // ITEMFACTORIES_H
