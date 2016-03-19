/*
Copyright (C) 2015-2016 Yannick Mueller

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

#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <QThread>
#include <QQueue>
#include <QMutex>
#include <QMutexLocker>
#include <QSet>
#include <QPixmap>

class ImageLoader : public QThread
{
	Q_OBJECT

private:
	struct QueueEntry
	{
		QString fileName;
		QSet<QObject *> receivers;

		bool operator==(QueueEntry const & other) const;
	};

	QQueue<QueueEntry> queue;
	QMutex queueMutex;

private:
	static ImageLoader * inst;
	ImageLoader();

public:
	static ImageLoader * instance();

	void addImageRequest(QString const & fileName, QObject * receiver);
	void removeImageRequest(QString const & fileName, QObject * receiver);

protected:
	void run() override;

signals:
	void imageLoaded(QString const & fileName, QObject const * receiver, QPixmap const & px);
};

#endif // IMAGELOADER_H
