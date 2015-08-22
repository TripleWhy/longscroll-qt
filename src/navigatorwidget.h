#ifndef NAVIGATORWIDGET_H
#define NAVIGATORWIDGET_H

#include "longscroll-qt_global.h"
#include "contentiteminfo.h"
#include <QFrame>

class ImageInfoWidget;

class LONGSCROLLQT_EXPORT NavigatorWidget : public QFrame
{
	Q_OBJECT
public:
	using QFrame::QFrame;
	virtual void setItemInfo(ContentItemInfo const & info);

signals:
	void previousImageRequested();
	void nextImageRequested();
	void closeRequested();

protected:
	ContentItemInfo itemInfo;
};


namespace Ui {
class AbstractNavigatorWidget;
}

class QToolButton;
class QGridLayout;

class LONGSCROLLQT_EXPORT AbstractNavigatorWidget : public NavigatorWidget
{
	Q_OBJECT
public:
	explicit AbstractNavigatorWidget(QWidget *parent = 0, Qt::WindowFlags f = 0);
	~AbstractNavigatorWidget();

protected:
	QToolButton * closeButton;
	QToolButton * nextButton;
	QToolButton * prevButton;
	QWidget * infoWidget;
	QGridLayout * mainLayout;

private:
	Ui::AbstractNavigatorWidget *ui;
};


class LONGSCROLLQT_EXPORT ImageNavigatorWidget : public AbstractNavigatorWidget
{
	Q_OBJECT
public:
	explicit ImageNavigatorWidget(QWidget *parent = 0, Qt::WindowFlags f = 0);

	virtual void setItemInfo(ContentItemInfo const & info) override;

private:
	ImageInfoWidget * imgContent;
};

#endif // NAVIGATORWIDGET_H
