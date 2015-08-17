#ifndef ITEMFACTORIES_H
#define ITEMFACTORIES_H

#include <longscroll-qt/contentwidgetitemfactory.h>

class ContentWidgetLoaderImageItemFactory : public ContentWidgetItemFactory
{
private:
	bool imageFit;
public:
	ContentWidgetLoaderImageItemFactory(bool fitImage = false, QObject * parent = 0);
	virtual QWidget * createItemWidget(ContentItemInfo const & info, int width, int height) override;
};

#endif // ITEMFACTORIES_H
