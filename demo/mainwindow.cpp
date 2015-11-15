#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "itemfactories.h"
#include "valueeditdialog.h"
#include <longscroll-qt/contentwidget.h>
#include <longscroll-qt/contentwidgetitemfactory.h>
#include <QScrollBar>
#include <QMetaProperty>

MainWindow::MainWindow(int demoNr, QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	ContentWidget * cw = ui->longscroll->getContentWidget();

	connect(ui->menuProperties, &QMenu::aboutToShow, this, &MainWindow::updatePropertyMenu);
	connect(ui->menuProperties, &QMenu::triggered, this, &MainWindow::propertyMenuTriggered);

	const QMetaObject * mo = cw->metaObject();
	QStringList blacklist;
	blacklist << "dragEnabled" << "selectedItems" << "currentItem";
	for (int i = mo->superClass()->propertyCount(), count = mo->propertyCount(); i < count; ++i)
	{
		QMetaProperty const & prop = mo->property(i);
		if (prop.userType() == QMetaType::UnknownType || !prop.isReadable() || !prop.isWritable() || blacklist.contains(prop.name()))
			continue;
		ui->menuProperties->addAction(QString())->setData(i);
	}

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

void MainWindow::updatePropertyMenu()
{
	ContentWidget * cw = ui->longscroll->getContentWidget();
	const QMetaObject * mo = cw->metaObject();

	for (QAction * a : ui->menuProperties->actions())
	{
		QMetaProperty const & prop = mo->property(a->data().toInt());
		QString const & name = prop.name();
		int type = prop.userType();
		QVariant const & value = prop.read(cw);

		QString const & displayName = QString("%1 = %2").arg(name, prop.read(cw).toString());
		a->setText(displayName);
		switch (type)
		{
			case QMetaType::Bool:
				a->setCheckable(true);
				a->setChecked(value.toBool());
				break;
			default:
				break;
		}
	}
}

void MainWindow::propertyMenuTriggered(QAction * action)
{
	ContentWidget * cw = ui->longscroll->getContentWidget();
	const QMetaObject * mo = cw->metaObject();
	QMetaProperty const & prop = mo->property(action->data().toInt());
	switch (prop.userType())
	{
		case QMetaType::Bool:
			prop.write(cw, action->isChecked());
			break;
		default:
			ValueEditDialog ved(this);
			ved.setValue(prop.name(), prop.read(cw));
			if (ved.exec() == QDialog::Accepted)
				prop.write(cw, ved.getValue());
			break;
	}
}
