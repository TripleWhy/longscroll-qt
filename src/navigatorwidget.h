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

#ifndef NAVIGATORWIDGET_H
#define NAVIGATORWIDGET_H

#include "longscroll-qt_global.h"
#include "contentiteminfo.h"
#include "longscroll-qt_qtincludes.h"

LONGSCROLLQT_NAMESPACE_BEGIN

class ImageInfoWidget;

class LONGSCROLLQT_EXPORT NavigatorWidget : public QFrame
{
	Q_OBJECT
public:
	using QFrame::QFrame;
	Q_PROPERTY(ContentItemInfo itemInfo READ getItemInfo WRITE setItemInfo)
	ContentItemInfo const & getItemInfo() const;
	virtual void setItemInfo(ContentItemInfo const & info);

signals:
	void previousImageRequested();
	void nextImageRequested();
	void closeRequested();

private:
	ContentItemInfo itemInfo;
};

LONGSCROLLQT_NAMESPACE_END


namespace Ui {
class BaseNavigatorWidget;
}

class QToolButton;
class QGridLayout;

LONGSCROLLQT_NAMESPACE_BEGIN

class LONGSCROLLQT_EXPORT BaseNavigatorWidget : public NavigatorWidget
{
	Q_OBJECT
public:
	explicit BaseNavigatorWidget(QWidget *parent = 0, Qt::WindowFlags f = 0);
	~BaseNavigatorWidget();

protected:
	Ui::BaseNavigatorWidget *ui;
};


class LONGSCROLLQT_EXPORT ImageNavigatorWidget : public BaseNavigatorWidget
{
	Q_OBJECT
public:
	explicit ImageNavigatorWidget(QWidget *parent = 0, Qt::WindowFlags f = 0);

	virtual void setItemInfo(ContentItemInfo const & info) override;

private:
	ImageInfoWidget * imgContent;
};

LONGSCROLLQT_NAMESPACE_END

#endif // NAVIGATORWIDGET_H
