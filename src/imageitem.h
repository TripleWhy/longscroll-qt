#ifndef IMAGEITEM_H
#define IMAGEITEM_H

#include "contentiteminfo.h"
#include <QFrame>

class ImageWidget;

class ImageItem : public QFrame
{
protected:
	ImageWidget * label = 0;
	QSize size;
	bool fit = false;

public:
	ContentItemInfo info;

protected:
	ImageItem(ContentItemInfo const & info, bool fit, bool loadImage, QWidget * parent);
public:
	ImageItem(ContentItemInfo const & info, bool fit = false, QWidget * parent = 0);
	~ImageItem();

protected:
	void loadImage();

public:
	void setHeight(int h);
	virtual QSize sizeHint() const override;
};

#endif // IMAGEITEM_H
