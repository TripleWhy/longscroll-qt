#ifndef LOADERIMAGEITEM_H
#define LOADERIMAGEITEM_H

#include <longscroll-qt/imageitem.h>

class ImageLoader;

class LoaderImageItem : public ImageItem
{
	Q_OBJECT

private:
	ImageLoader * loader;
	bool imageSet = false;

public:
	LoaderImageItem(ImgInfo const & info, bool fit = false);
	~LoaderImageItem();

protected:
	virtual void setShowing(bool visible) override;

private slots:
	void requestFinished(QString const & /*fileName*/, QObject const * receiver, QPixmap const & px);
};

#endif // LOADERIMAGEITEM_H
