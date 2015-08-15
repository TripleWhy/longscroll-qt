#ifndef CONTENTWIDGETITEMFACTORY_H
#define CONTENTWIDGETITEMFACTORY_H

#include "contentiteminfo.h"
#include <QObject>
#include <QWidget>

class ContentWidgetItemFactory : public QObject
{
public:
	using QObject::QObject;
	virtual QWidget * createItemWidget(ContentItemInfo const & info, int width, int height) = 0;
};

class ContentWidgetImageItemFactory : public ContentWidgetItemFactory
{
private:
	bool imageFit;
public:
	ContentWidgetImageItemFactory(bool fitImage = false, QObject * parent = 0);
	virtual QWidget * createItemWidget(ContentItemInfo const & info, int width, int height) override;
};

#endif // CONTENTWIDGETITEMFACTORY_H
