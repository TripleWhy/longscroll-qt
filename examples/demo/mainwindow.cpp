/*
Copyright (C) 2015 Yannick Mueller

This file is part of longscroll-qt.

longscroll-qt is free software; you can redistribute it and/or modify it under
the terms of the GNU Affero General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

longscroll-qt is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License
for more details.

You should have received a copy of the GNU Affero General Public License
along with longscroll-qt.  If not see <http://www.gnu.org/licenses/>.
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "itemfactories.h"
#include "valueeditdialog.h"
#include <longscroll-qt/contentwidget.h>
#include <longscroll-qt/contentwidgetitemfactory.h>
#include <QScrollBar>
#include <QMetaProperty>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QDebug>
#include <algorithm>
#include <random>

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
	for (int i = 0; i < 7; ++i)
	{
		QAction * a = new QAction(tr("Demo %1").arg(i+1), demoGroup);
		a->setData(i);
		a->setCheckable(true);
		a->setChecked(i == 0);
	}
	ui->menuDemo->addActions(demoGroup->actions());

	setDemoNumber(0);

	connect(ui->menuFile, &QMenu::aboutToShow, this, &MainWindow::updateFileMenu);
	connect(ui->menuProperties, &QMenu::aboutToShow, this, &MainWindow::updatePropertyMenu);
	connect(ui->menuProperties, &QMenu::triggered, this, &MainWindow::propertyMenuTriggered);
	connect(demoGroup, &QActionGroup::triggered, this, &MainWindow::demoMenuTriggered);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::setItemInfos(const QList<ContentItemInfo> & infos, int fixedSize, bool shuffle)
{
	ContentWidget * cw = ui->longscroll->getContentWidget();
	QList<ContentItemInfo> displayInfos = infos;

	if (fixedSize == -1)
		fixedSize = fixedInfoSize;
	if (fixedSize > 0 && !infos.isEmpty())
	{
		qDebug() << "creating items ...";
		displayInfos.clear();
		for (int i = 0; i < fixedSize; ++i)
			displayInfos.append(infos.at(i % infos.count()));
		qDebug() << "created" << fixedSize << "items";
	}
	if (shuffle)
	{
		std::random_device rd;
		std::mt19937 rng(rd());
		std::shuffle(displayInfos.begin(), displayInfos.end(), rng);
		qDebug() << "shuffled";
	}

	fixedInfoSize = fixedSize;
	itemInfos = infos;
	cw->setItemInfos(displayInfos);
}

void MainWindow::setFixedItemCount(int fixedSize)
{
	setItemInfos(itemInfos, fixedSize);
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
			cw->setRowHeight(150);
			cw->setItemWidth(0);
			cw->setStretchRows(true);
			cw->setStretchLastRow(false);
			cw->setScaleRows(false);
			cw->setAllowOverfill(true);
			cw->setNavigatorHeight(400);
			cwif = new ContentWidgetLoaderImageItemFactory(false);
			break;
		case 1:
			cw->setRowHeight(150);
			cw->setItemWidth(0);
			cw->setStretchRows(true);
			cw->setStretchLastRow(false);
			cw->setScaleRows(true);
			cw->setAllowOverfill(true);
			cw->setNavigatorHeight(400);
			cwif = new ContentWidgetLoaderImageItemFactory(false);
			break;
		case 2:
			cw->setRowHeight(150);
			cw->setItemWidth(0);
			cw->setStretchRows(false);
			cw->setStretchLastRow(false);
			cw->setScaleRows(false);
			cw->setAllowOverfill(false);
			cw->setNavigatorHeight(400);
			cwif = new ContentWidgetLoaderImageItemFactory(false);
			break;
		case 3:
			cw->setRowHeight(100);
			cw->setItemWidth(100);
			cw->setStretchRows(false);
			cw->setStretchLastRow(false);
			cw->setScaleRows(false);
			cw->setAllowOverfill(false);
			cw->setNavigatorHeight(150);
			cwif = new ContentWidgetLoaderImageItemFactory(true);
			break;
		case 4:
			cw->setRowHeight(100);
			cw->setItemWidth(100);
			cw->setStretchRows(false);
			cw->setStretchLastRow(false);
			cw->setScaleRows(false);
			cw->setAllowOverfill(false);
			cw->setNavigatorHeight(150);
			cwif = new ContentWidgetLoaderImageItemFactory(false);
			break;
		case 5:
			cw->setRowHeight(100);
			cw->setItemWidth(-1);
			cw->setStretchRows(true);
			cw->setStretchLastRow(true);
			cw->setScaleRows(false);
			cw->setAllowOverfill(false);
			cw->setNavigatorHeight(150);
			cwif = new ContentWidgetLoaderImageItemFactory(true);
			break;
		case 6:
			cw->setRowHeight(100);
			cw->setItemWidth(400);
			cw->setStretchRows(false);
			cw->setScaleRows(false);
			cw->setStretchLastRow(false);
			cw->setAllowOverfill(false);
			cw->setNavigatorHeight(150);
			cwif = new ContentWidgetImageInfoFactory();
			break;
	}
	cw->setItemFactory(cwif);
	demoGroup->blockSignals(true);
	QAction * a = demoGroup->actions().value(demoNo);
	a->setChecked(a);
	demoGroup->blockSignals(false);
}

void MainWindow::loadDir(const QDir & dir, bool useCache)
{
	if (!dir.exists())
	{
		qDebug() << dir.absolutePath() << "does not exist";
		return;
	}
	QMap<QString, ContentItemInfo> cache, loadedCache;
	QFile cacheFile(dir.absoluteFilePath(".imagecache"));
	if (cacheFile.exists())
	{
		if (cacheFile.open(QIODevice::ReadOnly))
		{
			QByteArray const & buffer = qUncompress(cacheFile.readAll());
			cacheFile.close();
			QDataStream ds(buffer);
			ds >> loadedCache;
		}
		else
		{
			qWarning() << "could not open cache file:" << cacheFile.fileName();
		}
	}
	cache = loadedCache;
	qDebug() << "cached images:" << cache.size();

	QStringList fileNamefilters;
	for (QString const & format : QImageReader::supportedImageFormats())
		fileNamefilters.append(QString("*.%1").arg(format));

	QImageReader ir;
	qDebug() << "loading file list for" << dir.absolutePath() << "...";
	QFileInfoList const & files = dir.entryInfoList(fileNamefilters, QDir::Files | QDir::Readable);
	qDebug() << "loading images from" << dir.absolutePath() << "...";
	for (QFileInfo const & file : files)
	{
		QString const & fileName = file.absoluteFilePath();
		if (cache.contains(fileName))
			continue;

		ir.setFileName(fileName);
		cache.insert(fileName, ContentItemInfo(fileName, ir.size()));
	}

	qDebug() << "checking file existence ...";
	for (auto it = cache.begin(); it != cache.end(); )
	{
		QFileInfo fi(it.key());
		if (fi.exists() && fi.absoluteDir() == dir)
			++it;
		else
			it = cache.erase(it);
	}

	if (useCache && cache != loadedCache)
	{
		qDebug() << "storing cache file ...";
		QByteArray buffer;
		if (cacheFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
		{
			QDataStream ds(&buffer, QIODevice::WriteOnly);
			ds << cache;
			cacheFile.write(qCompress(buffer, 9));
			cacheFile.close();
		}
		else
		{
			qWarning() << "Could not open" << cacheFile.fileName() << "for write.";
		}
	}

	qDebug() << "loaded" << cache.size() << "images";
	qDebug() << "loading gui ...";

	setItemInfos(cache.values(), fixedInfoSize);
}

void MainWindow::updateFileMenu()
{
	ui->actionCount->setText(tr("Count = %1").arg(fixedInfoSize));
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

void MainWindow::on_actionOpen_triggered()
{
	if (fileDialog == 0)
	{
		fileDialog = new QFileDialog(this);
		fileDialog->setAcceptMode(QFileDialog::AcceptOpen);
		fileDialog->setFileMode(QFileDialog::Directory);
		fileDialog->setOption(QFileDialog::ShowDirsOnly, true);
	}
	if (fileDialog->exec() != QDialog::Accepted)
		return;
	QDir dir(fileDialog->selectedFiles().first());
	dir.makeAbsolute();
	QFileInfo cacheInfo(dir.filePath(".imagecache"));

	qDebug() << cacheInfo.absoluteFilePath() << cacheInfo.exists() << cacheInfo.isFile() << cacheInfo.isWritable() << cacheInfo.permissions();
	QFileInfo dirInfo(dir.absolutePath());
	qDebug() << dirInfo.absoluteFilePath() << dirInfo.isWritable() << dirInfo.permissions();

	bool useCache = ((!cacheInfo.exists() && QFileInfo(dir.absolutePath()).isWritable()) || (cacheInfo.isFile() && cacheInfo.isWritable()));
	if (useCache && QMessageBox::question(this, tr("Write Cache File?"), tr("Write a cache file?\nThis is useful if you open a directory with a large number of files more than once.")) != QMessageBox::Yes)
		useCache = false;
	loadDir(dir, useCache);
}

void MainWindow::on_actionCount_triggered()
{
	bool ok;
	int val = QInputDialog::getInt(this, "Count", "Fixed number of images", fixedInfoSize, 0, std::numeric_limits<int>::max(), 1, &ok);
	if (ok)
		setFixedItemCount(val);
}

void MainWindow::on_actionShuffle_triggered()
{
	setItemInfos(itemInfos, fixedInfoSize, true);
}

void MainWindow::on_actionClear_triggered()
{
	setItemInfos({});
}
