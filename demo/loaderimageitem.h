#ifndef LOADERIMAGEITEM_H
#define LOADERIMAGEITEM_H

#include <longscroll-qt/imageitemwidget.h>

class ImageLoader;

class LoaderImageItem : public ImageItemWidget
{
	Q_OBJECT

private:
	ImageLoader * loader;
	bool imageSet = false;

public:
	LoaderImageItem(ContentItemInfo const & info, bool fit = false);
	~LoaderImageItem();

private slots:
	void requestFinished(QString const & /*fileName*/, QObject const * receiver, QPixmap const & px);
};

#endif // LOADERIMAGEITEM_H
