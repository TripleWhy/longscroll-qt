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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <longscroll-qt/contentiteminfo.h>

class QActionGroup;
class QFileDialog;

using namespace longscroll;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	void setItemInfos(QList<ContentItemInfo> const & infos, int fixedSize = -1, bool shuffle = false);
	void setFixedItemCount(int fixedSize);
	void setDemoNumber(int demoNo);
	void loadDir(QDir const & dir, bool useCache);

private slots:
	void updateFileMenu();
	void updatePropertyMenu();
	void propertyMenuTriggered(QAction * action);
	void demoMenuTriggered(QAction * action);
	void on_actionOpen_triggered();
	void on_actionCount_triggered();
	void on_actionShuffle_triggered();
	void on_actionClear_triggered();

private:
	Ui::MainWindow *ui;
	QActionGroup * demoGroup;
	QFileDialog * fileDialog = 0;
	QList<ContentItemInfo> itemInfos;
	int fixedInfoSize = 0;
};

#endif // MAINWINDOW_H
