#include "imageitem.h"
#include "imageloader.h"
#include "imagewidget.h"
#include <QVBoxLayout>

ImageItem::ImageItem(const ImgInfo & info, bool fit)
    : NotifyableScrollContentWidget(),
      fit(fit),
      info(info)
{
	loader = ImageLoader::instance();
	connect(loader, SIGNAL(imageLoaded(QString,const QObject*,QPixmap)), this, SLOT(requestFinished(QString,const QObject*,QPixmap)));

	new QVBoxLayout(this);
	layout()->setMargin(0);
	layout()->setSpacing(0);
	setStyleSheet("QWidget{ background: blue; } QLabel { background: red; }");
}

ImageItem::~ImageItem()
{
	//		qDebug("ImageItem::~ImageItem");
	if (!imageSet)
		loader->removeImageRequest(info.fileName, this);
}

void ImageItem::setHeight(int h)
{
	size.setHeight(h);
	size.setWidth(info.widthForHeight(h));
	setFixedHeight(h);
	setMinimumHeight(h);
	resize(size);
}

QSize ImageItem::sizeHint() const
{
	return size;
}

void ImageItem::setShowing(bool visible)
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

void ImageItem::requestFinished(const QString &, const QObject * receiver, const QPixmap & px)
{
	if (receiver != this || label == 0)
		return;

	imageSet = true;
	label->setPixmap(px);
}
