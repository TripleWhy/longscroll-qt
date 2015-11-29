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
#include <QDebug>
#include <QApplication>
#include <QFile>
#include <QDir>
#include <QImageReader>

/*
 * Run this program with one or more directories that contain image files as arguments.
 * Add another integer paramter at the end to enable different demo modes.
 *
 * Change the preprocesser #if at the bottom if you want to test an arbitrary amount of images.
 */
int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	MainWindow w;
	if (app.arguments().size() > 2)
	{
		int demoMode;
		bool ok;
		demoMode = app.arguments().at(2).toInt(&ok);
		if (ok)
			w.setDemoNumber(demoMode);
	}
	if (app.arguments().size() > 1)
		w.loadDir(QDir(app.arguments().at(1)), true);

	w.show();

	return app.exec();
}
