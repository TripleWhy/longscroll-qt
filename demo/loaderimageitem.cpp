#include "loaderimageitem.h"
#include "imageloader.h"
#include <longscroll-qt/imagewidget.h>
#include <QVBoxLayout>

LoaderImageItem::LoaderImageItem(const ImgInfo & info, bool fit)
    : ImageItem(info, fit)
{
	loader = ImageLoader::instance();
	connect(loader, SIGNAL(imageLoaded(QString,const QObject*,QPixmap)), this, SLOT(requestFinished(QString,const QObject*,QPixmap)));
}

LoaderImageItem::~LoaderImageItem()
{
	if (!imageSet)
		loader->removeImageRequest(info.fileName, this);
}

void LoaderImageItem::setShowing(bool visible)
{
	if (visible)
	{
		if (label != 0)
			return;
		label = new ImageWidget();
		label->setFit(fit);
		layout()->addWidget(label);
		loader->addImageRequest(info.fileName, this);
	}
	else
	{
		if (!imageSet)
			loader->removeImageRequest(info.fileName, this);
		delete label;
		label = 0;
		imageSet = false;
	}
}

void LoaderImageItem::requestFinished(const QString &, const QObject * receiver, const QPixmap & px)
{
	if (receiver != this || label == 0)
		return;

	imageSet = true;
	label->setPixmap(px);
}
