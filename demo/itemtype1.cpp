#include "itemtype1.h"
#include "ui_itemtype1.h"
#include <QFileInfo>

ItemType1::ItemType1(const ContentItemInfo & info, int height, QWidget *parent) :
	QWidget(parent),
	fileName(info.data.toString()),
	ui(new Ui::ItemType1)
{
	ui->setupUi(this);

	ui->imageWidget->setFixedSize(height, height);
	ui->imageWidget->setPixmap(fileName);
	ui->label_2->setText(QFileInfo(fileName).fileName());
}

ItemType1::~ItemType1()
{
	delete ui;
}
