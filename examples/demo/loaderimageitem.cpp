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

#include "loaderimageitem.h"
#include "imageloader.h"
#include <longscroll-qt/imagewidget.h>
#include <QVBoxLayout>

LoaderImageItem::LoaderImageItem(const ContentItemInfo & info, int itemIndex, bool fit, ContentWidget * cw)
    : ImageItemWidget(info, itemIndex, fit, false, cw, 0)
{
	loader = ImageLoader::instance();
	connect(loader, SIGNAL(imageLoaded(QString,const QObject*,QPixmap)), this, SLOT(requestFinished(QString,const QObject*,QPixmap)));
	loader->addImageRequest(info.getData().toString(), this);
}

LoaderImageItem::~LoaderImageItem()
{
	if (!imageSet)
		loader->removeImageRequest(info.getData().toString(), this);
}

void LoaderImageItem::requestFinished(const QString & fileName, const QObject * receiver, const QPixmap & px)
{
	if (receiver != this || info.getData().toString() != fileName)
		return;

	imageSet = true;
	imageWidget->setPixmap(px);
}
