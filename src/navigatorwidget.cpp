#include "navigatorwidget.h"
#include "ui_navigatorwidget.h"
#include "imageinfowidget.h"

void NavigatorWidget::setItemInfo(const ContentItemInfo & info)
{
	if (itemInfo == info)
		return;
	itemInfo = info;
}

AbstractNavigatorWidget::AbstractNavigatorWidget(QWidget *parent) :
    NavigatorWidget(parent),
    ui(new Ui::AbstractNavigatorWidget)
{
	ui->setupUi(this);

	closeButton = ui->closeButton;
	nextButton = ui->nextButton;
	prevButton = ui->prevButton;
	infoWidget = ui->infoWidget;
	mainLayout = ui->mainLayout;

	ui->prevButton->setShortcut(QKeySequence("Left"));
	ui->nextButton->setShortcut(QKeySequence("Right"));
	ui->closeButton->setShortcut(QKeySequence("Esc"));

	connect(ui->prevButton, SIGNAL(clicked()), this, SIGNAL(previousImageRequested()));
	connect(ui->nextButton, SIGNAL(clicked()), this, SIGNAL(nextImageRequested()));
	connect(ui->closeButton, SIGNAL(clicked()), this, SIGNAL(closeRequested()));
}

AbstractNavigatorWidget::~AbstractNavigatorWidget()
{
	delete ui;
}


ImageNavigatorWidget::ImageNavigatorWidget(QWidget * parent)
    : AbstractNavigatorWidget(parent)
{
	imgContent = new ImageInfoWidget();
	infoWidget->layout()->addWidget(imgContent);
}

void ImageNavigatorWidget::setItemInfo(const ContentItemInfo & info)
{
	AbstractNavigatorWidget::setItemInfo(info);
	imgContent->setItemInfo(info);
}
