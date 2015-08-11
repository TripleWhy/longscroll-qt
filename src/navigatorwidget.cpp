#include "navigatorwidget.h"
#include "ui_navigatorwidget.h"

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

	img = info;

	ui->label_2->setText(info.fileName);
	ui->label_3->setText(QString(tr("Size: %1 x %2").arg(info.width).arg(info.height)));

	QPixmap px;
	px.load(img.fileName);
	ui->imageWidget->setPixmap(px);
}
