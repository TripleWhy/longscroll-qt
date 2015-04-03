#ifndef IMAGEITEM_H
#define IMAGEITEM_H

#include "notifyablescrollcontentwidget.h"
#include "imginfo.h"

class ImageLoader;
class ImageWidget;

class ImageItem : public NotifyableScrollContentWidget
{
	Q_OBJECT

private:
	ImageLoader * loader;

	ImageWidget * label = 0;
	QSize size;

	bool imageSet = false;
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

private slots:
	void requestFinished(QString const & /*fileName*/, QObject const * receiver, QPixmap const & px);
};

#endif // IMAGEITEM_H
