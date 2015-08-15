#ifndef IMAGEITEMWIDGET_H
#define IMAGEITEMWIDGET_H

#include "contentiteminfo.h"
#include <QFrame>

class ImageWidget;

class ImageItemWidget : public QFrame
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

#endif // IMAGEITEMWIDGET_H
