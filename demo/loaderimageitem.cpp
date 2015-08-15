#include "loaderimageitem.h"
#include "imageloader.h"
#include <longscroll-qt/imagewidget.h>
#include <QVBoxLayout>

LoaderImageItem::LoaderImageItem(const ContentItemInfo & info, bool fit)
    : ImageItem(info, fit, false, 0)
{
	loader = ImageLoader::instance();
	connect(loader, SIGNAL(imageLoaded(QString,const QObject*,QPixmap)), this, SLOT(requestFinished(QString,const QObject*,QPixmap)));
	loader->addImageRequest(info.data.toString(), this);
}

LoaderImageItem::~LoaderImageItem()
{
	if (!imageSet)
		loader->removeImageRequest(info.data.toString(), this);
}

void LoaderImageItem::requestFinished(const QString &, const QObject * receiver, const QPixmap & px)
{
	if (receiver != this)
		return;

	imageSet = true;
	label->setPixmap(px);
}
