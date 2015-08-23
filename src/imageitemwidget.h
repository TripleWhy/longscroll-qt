#ifndef IMAGEITEMWIDGET_H
#define IMAGEITEMWIDGET_H

#include "longscroll-qt_global.h"
#include "contentiteminfo.h"
#include <QFrame>

LONGSCROLLQT_NAMESPACE_BEGIN

class ImageWidget;

class LONGSCROLLQT_EXPORT ImageItemWidget : public QFrame
{
protected:
	ImageWidget * label = 0;
	bool fit = false;

public:
	ContentItemInfo info;

protected:
	ImageItemWidget(ContentItemInfo const & info, bool fit, bool loadImage, QWidget * parent);
public:
	ImageItemWidget(ContentItemInfo const & info, bool fit = false, QWidget * parent = 0);
	~ImageItemWidget();

protected:
	void loadImage();
};

LONGSCROLLQT_NAMESPACE_END

#endif // IMAGEITEMWIDGET_H
