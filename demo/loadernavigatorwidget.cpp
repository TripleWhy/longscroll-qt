#include "loadernavigatorwidget.h"

void LoaderNavigatorWidget::setImage(const ImgInfo & info)
{
	if (img == info)
		return;

	img = info;

	if (loader == 0)
	{
		loader = ImageLoader::instance();
		connect(loader, SIGNAL(imageLoaded(QString,const QObject*,QPixmap)), this, SLOT(requestFinished(QString,const QObject*,QPixmap)));
	}
	//TODO
//	ui->label_2->setText(info.fileName);
//	ui->label_3->setText(QString(tr("Size: %1 x %2").arg(info.width).arg(info.height)));

	loader->addImageRequest(info.fileName, this);
}

void LoaderNavigatorWidget::requestFinished(const QString & /*fileName*/, const QObject * receiver, const QPixmap & px)
{
	if (receiver != this)
		return;

	//TODO
	Q_UNUSED(px);
//	ui->imageWidget->setPixmap(px);
}
