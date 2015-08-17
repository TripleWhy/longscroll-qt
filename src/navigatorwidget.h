#ifndef NAVIGATORWIDGET_H
#define NAVIGATORWIDGET_H

#include <QFrame>
#include "contentiteminfo.h"

class NavigatorWidget : public QFrame
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

class AbstractNavigatorWidget : public NavigatorWidget
{
	Q_OBJECT
public:
	explicit AbstractNavigatorWidget(QWidget *parent = 0);
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


class ImageInfoWidget;

class ImageNavigatorWidget : public AbstractNavigatorWidget
{
	Q_OBJECT
public:
	explicit ImageNavigatorWidget(QWidget *parent = 0);

	virtual void setItemInfo(ContentItemInfo const & info) override;

private:
	ImageInfoWidget * imgContent;
};

#endif // NAVIGATORWIDGET_H
