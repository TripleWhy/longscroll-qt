#ifndef LOADERIMAGEITEM_H
#define LOADERIMAGEITEM_H

#include <longscroll-qt/imageitemwidget.h>

class ImageLoader;

using namespace longscroll;

class LoaderImageItem : public ImageItemWidget
{
	Q_OBJECT

private:
	ImageLoader * loader;
	bool imageSet = false;

public:
	LoaderImageItem(ContentItemInfo const & info, int itemIndex, bool fit = false, ContentWidget * cw = 0);
	~LoaderImageItem();

private slots:
	void requestFinished(QString const & /*fileName*/, QObject const * receiver, QPixmap const & px);
};

#endif // LOADERIMAGEITEM_H
