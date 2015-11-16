#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "itemfactories.h"
#include "valueeditdialog.h"
#include <longscroll-qt/contentwidget.h>
#include <longscroll-qt/contentwidgetitemfactory.h>
#include <QScrollBar>
#include <QMetaProperty>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	ContentWidget * cw = ui->longscroll->getContentWidget();

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

	demoGroup = new QActionGroup(ui->menuProperties);
	for (int i = 0; i < 5; ++i)
	{
		QAction * a = new QAction(tr("Demo %1").arg(i+1), demoGroup);
		a->setData(i);
		a->setCheckable(true);
		a->setChecked(i == 0);
	}
	ui->menuDemo->addActions(demoGroup->actions());

	setDemoNumber(0);

	connect(ui->menuProperties, &QMenu::aboutToShow, this, &MainWindow::updatePropertyMenu);
	connect(ui->menuProperties, &QMenu::triggered, this, &MainWindow::propertyMenuTriggered);
	connect(demoGroup, &QActionGroup::triggered, this, &MainWindow::demoMenuTriggered);
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

void MainWindow::setDemoNumber(int demoNo)
{
	ContentWidget * cw = ui->longscroll->getContentWidget();
	ContentWidgetItemFactory * cwif = 0;
	switch (demoNo)
	{
		default:
		case 0:
			//Default Values
			cw->setRowHeight(200);
			cw->setItemWidth(0);
			cw->setStretchRows(true);
			cw->setStretchLastRow(false);
			cw->setScaleRows(false);
			cw->setAllowOverfill(true);
			cw->setNavigatorHeight(500);
			cwif = new ContentWidgetLoaderImageItemFactory(false);
			break;
		case 1:
			cw->setRowHeight(200);
			cw->setItemWidth(0);
			cw->setStretchRows(true);
			cw->setStretchLastRow(false);
			cw->setScaleRows(true);
			cw->setAllowOverfill(true);
			cw->setNavigatorHeight(500);
			cwif = new ContentWidgetLoaderImageItemFactory(false);
			break;
		case 2:
			cw->setRowHeight(100);
			cw->setItemWidth(100);
			cw->setStretchRows(false);
			cw->setStretchLastRow(false);
			cw->setScaleRows(false);
			cw->setAllowOverfill(false);
			cw->setNavigatorHeight(200);
			cwif = new ContentWidgetLoaderImageItemFactory(true);
			break;
		case 3:
			cw->setRowHeight(100);
			cw->setItemWidth(-1);
			cw->setStretchRows(true);
			cw->setStretchLastRow(true);
			cw->setScaleRows(false);
			cw->setAllowOverfill(false);
			cw->setNavigatorHeight(200);
			cwif = new ContentWidgetLoaderImageItemFactory(true);
			break;
		case 4:
			cw->setRowHeight(100);
			cw->setItemWidth(200);
			cw->setStretchRows(false);
			cw->setScaleRows(false);
			cw->setStretchLastRow(false);
			cw->setAllowOverfill(false);
			cw->setNavigatorHeight(200);
			cwif = new ContentWidgetImageInfoFactory();
			break;
	}
	cw->setItemFactory(cwif);
	demoGroup->blockSignals(true);
	QAction * a = demoGroup->actions().value(demoNo);
	a->setChecked(a);
	demoGroup->blockSignals(false);
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
	for (QAction * a : demoGroup->actions())
		a->setChecked(false);

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

void MainWindow::demoMenuTriggered(QAction * action)
{
	setDemoNumber(action->data().toInt());
}
