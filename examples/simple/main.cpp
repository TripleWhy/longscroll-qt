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

#include "longscroll-qt/longscrollwidget.h"
#include <QApplication>
#include <QDir>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	if (a.arguments().size() <= 1)
	{
		qDebug("Specify a directory containing images.");
		return 1;
	}
	QDir dir(a.arguments().at(1));
	if (!dir.exists())
	{
		qDebug("Directory does not exist.");
		return 1;
	}

	//Generate some items
	QList<longscroll::ContentItemInfo> infos;
	QImageReader ir;
	for (QFileInfo const & file : dir.entryInfoList(QDir::Files | QDir::Readable))
	{
		QString const & fileName = file.absoluteFilePath();
		ir.setFileName(fileName);
		QSize const & size = ir.size();
		if (size.isEmpty())
			continue;
		infos.append(longscroll::ContentItemInfo(fileName, size));
	}

	//Show longscroll widget
	longscroll::LongscrollWidget * lw = new longscroll::LongscrollWidget();
	lw->getContentWidget()->setItemInfos(infos);
	lw->show();

	return a.exec();
}
