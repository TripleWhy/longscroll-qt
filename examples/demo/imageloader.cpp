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

#include "imageloader.h"

bool ImageLoader::QueueEntry::operator==(const ImageLoader::QueueEntry & other) const
{
	return (fileName == other.fileName);
}


ImageLoader * ImageLoader::inst = 0;

ImageLoader::ImageLoader()
    : QThread()
{
}

ImageLoader *ImageLoader::instance()
{
	if (inst == 0)
	{
		inst = new ImageLoader;
		inst->start();
	}
	return inst;
}

void ImageLoader::addImageRequest(const QString & fileName, QObject * receiver)
{
	QueueEntry entry;
	entry.fileName = fileName;
	entry.receivers.insert(receiver);

	QMutexLocker locker(&queueMutex);
	int index = queue.indexOf(entry);
	if (index >= 0)
		queue[index].receivers.insert(receiver);
	else
		queue.enqueue(entry);
	locker.unlock();
}

void ImageLoader::removeImageRequest(const QString & fileName, QObject * receiver)
{
	QueueEntry entry;
	entry.fileName = fileName;

	QMutexLocker locker(&queueMutex);
	int index = queue.indexOf(entry);
	if (index >= 0)
	{
		queue[index].receivers.remove(receiver);
		if (queue[index].receivers.isEmpty())
			queue.removeAt(index);
	}
	locker.unlock();
}

void ImageLoader::run()
{
	while (true)
	{
		QMutexLocker locker(&queueMutex);
		if (queue.isEmpty())
		{
			locker.unlock();
			msleep(100);
			continue;
		}

		QueueEntry entry = queue.dequeue();
		locker.unlock();

		QPixmap p;
		p.load(entry.fileName);
		msleep(10);

		for (QObject * receiver : entry.receivers)
			emit imageLoaded(entry.fileName, receiver, p);
		yieldCurrentThread();
	}
}
