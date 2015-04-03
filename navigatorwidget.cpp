#include "navigatorwidget.h"
#include "ui_navigatorwidget.h"
#include "imageloader.h"

NavigatorWidget::NavigatorWidget(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::NavigatorWidget)
{
	ui->setupUi(this);

	connect(ui->prevButton, SIGNAL(clicked()), this, SIGNAL(previousImageRequested()));
	connect(ui->nextButton, SIGNAL(clicked()), this, SIGNAL(nextImageRequested()));
	connect(ui->closeButton, SIGNAL(clicked()), this, SIGNAL(closeRequested()));
}

NavigatorWidget::~NavigatorWidget()
{
	delete ui;
}

void NavigatorWidget::setImage(const ImgInfo &info)
{
	if (img == info)
		return;

	if (loader == 0)
	{
		loader = ImageLoader::instance();
		connect(loader, SIGNAL(imageLoaded(QString,const QObject*,QPixmap)), this, SLOT(requestFinished(QString,const QObject*,QPixmap)));
	}
	ui->label_2->setText(img.fileName);
	ui->label_3->setText(QString(tr("Size: %1 x %2").arg(info.width).arg(info.height)));

	loader->addImageRequest(info.fileName, this);
}

void NavigatorWidget::requestFinished(const QString &/*fileName*/, const QObject *receiver, const QPixmap &px)
{
	if (receiver != this)
		return;

	ui->imageWidget->setPixmap(px);
}
