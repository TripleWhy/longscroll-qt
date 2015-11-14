#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "itemfactories.h"
#include <longscroll-qt/contentwidget.h>
#include <longscroll-qt/contentwidgetitemfactory.h>
#include <QScrollBar>

MainWindow::MainWindow(int demoNr, QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	ContentWidget * cw = ui->longscroll->getContentWidget();
	ContentWidgetItemFactory * cwif = 0;
	switch (demoNr)
	{
		default:
		case 0:
			cwif = new ContentWidgetLoaderImageItemFactory(false);
			break;
		case 1:
			cw->setRowHeight(100);
			cw->setItemWidth(100);
			cw->setStretchRows(false);
			cw->setStretchLastRow(false);
			cw->setAllowOverfill(false);
			cw->setNavigatorHeight(200);
			cwif = new ContentWidgetLoaderImageItemFactory(true);
			break;
		case 2:
			cw->setRowHeight(100);
			cw->setItemWidth(-1);
			cw->setStretchRows(true);
			cw->setStretchLastRow(true);
			cw->setAllowOverfill(false);
			cw->setNavigatorHeight(200);
			cwif = new ContentWidgetLoaderImageItemFactory(true);
			break;
		case 3:
			cw->setRowHeight(100);
			cw->setItemWidth(200);
			cw->setStretchRows(false);
			cw->setStretchLastRow(false);
			cw->setAllowOverfill(false);
			cw->setNavigatorHeight(200);
			cwif = new ContentWidgetImageInfoFactory();
			break;
		case 4:
			cw->setScaleRows(true);
			cwif = new ContentWidgetLoaderImageItemFactory(false);
			break;
	}
	if (cwif != 0)
		cw->setItemFactory(cwif);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::setItemInfos(const QList<ContentItemInfo> & infos)
{
	ContentWidget * cw = ui->longscroll->getContentWidget();
	cw->setItemInfos(infos);
}
