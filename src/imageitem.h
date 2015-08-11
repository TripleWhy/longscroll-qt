#ifndef IMAGEITEM_H
#define IMAGEITEM_H

#include "notifyablescrollcontentwidget.h"
#include "imginfo.h"

class ImageWidget;

class ImageItem : public NotifyableScrollContentWidget
{
protected:
	ImageWidget * label = 0;
	QSize size;
	bool fit = false;

public:
	ImgInfo info;

public:
	ImageItem(ImgInfo const & info, bool fit = false);
	~ImageItem();
	void setHeight(int h);
	virtual QSize sizeHint() const override;

protected:
	virtual void setShowing(bool visible) override;
};

#endif // IMAGEITEM_H
