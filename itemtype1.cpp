#include "itemtype1.h"
#include "ui_itemtype1.h"

ItemType1::ItemType1(const ImgInfo & info, int height, QWidget *parent) :
	QWidget(parent),
	fileName(info.fileName),
	ui(new Ui::ItemType1)
{
	ui->setupUi(this);

	ui->imageWidget->setFixedSize(height, height);
	ui->imageWidget->setPixmap(fileName);
}

ItemType1::~ItemType1()
{
	delete ui;
}
