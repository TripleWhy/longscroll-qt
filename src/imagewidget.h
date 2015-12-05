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

#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include "longscroll-qt_global.h"
#include "longscroll-qt_qtincludes.h"
#include <QIcon>

class QLabel;

LONGSCROLLQT_NAMESPACE_BEGIN

class LONGSCROLLQT_EXPORT ImageWidget : public QFrame
{
	Q_OBJECT

public:
	explicit ImageWidget(QWidget *parent = 0);
	~ImageWidget();

public:
	Q_PROPERTY(QPixmap pixmap READ getPixmap WRITE setPixmap)
	QPixmap getPixmap() const;
	Q_PROPERTY(bool fit READ getFit WRITE setFit)
	bool getFit() const;
	Q_PROPERTY(bool selected READ isSelected WRITE setSelected)
	bool isSelected() const;
public slots:
	void setPixmap(QPixmap const & px);
	void setFit(bool fit);
	virtual void setSelected(bool selected);

protected:
	virtual void paintEvent(QPaintEvent * e) override;

	virtual bool hasHeightForWidth() const override;
	virtual int heightForWidth(int w) const override;
	virtual QSize sizeHint() const override;

private:
	QIcon icon;
	QSize pxSize;
	bool fit = true;
	bool selected = false;
};

LONGSCROLLQT_NAMESPACE_END

#endif // IMAGEWIDGET_H
