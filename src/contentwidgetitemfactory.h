#ifndef CONTENTWIDGETITEMFACTORY_H
#define CONTENTWIDGETITEMFACTORY_H

#include "longscroll-qt_global.h"
#include "contentiteminfo.h"
#include <QObject>
#include <QWidget>

LONGSCROLLQT_NAMESPACE_BEGIN

class ContentWidget;

class LONGSCROLLQT_EXPORT ContentWidgetItemFactory : public QObject
{
public:
	using QObject::QObject;
	virtual QWidget * createItemWidget(ContentItemInfo const & info, int itemIndex, int width, int height, ContentWidget * cw) = 0;
};

class LONGSCROLLQT_EXPORT ContentWidgetImageItemFactory : public ContentWidgetItemFactory
{
private:
	bool imageFit;
public:
	ContentWidgetImageItemFactory(bool fitImage = false, QObject * parent = 0);
	virtual QWidget * createItemWidget(ContentItemInfo const & info, int itemIndex, int width, int height, ContentWidget * cw) override;
};

class LONGSCROLLQT_EXPORT ContentWidgetImageInfoFactory : public ContentWidgetItemFactory
{
private:
	bool imageFit;
public:
	using ContentWidgetItemFactory::ContentWidgetItemFactory;
	virtual QWidget * createItemWidget(ContentItemInfo const & info, int itemIndex, int width, int height, ContentWidget * cw) override;
};

LONGSCROLLQT_NAMESPACE_END

#endif // CONTENTWIDGETITEMFACTORY_H
