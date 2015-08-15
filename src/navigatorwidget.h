#ifndef NAVIGATORWIDGET_H
#define NAVIGATORWIDGET_H

#include <QFrame>
#include "contentiteminfo.h"

namespace Ui {
class NavigatorWidget;
}

class NavigatorWidget : public QFrame
{
	Q_OBJECT

protected:
	ContentItemInfo img;

public:
	explicit NavigatorWidget(QWidget *parent = 0);
	~NavigatorWidget();

public:
	virtual void setImage(ContentItemInfo const & info);

signals:
	void previousImageRequested();
	void nextImageRequested();
	void closeRequested();

private:
	Ui::NavigatorWidget *ui;
};

#endif // NAVIGATORWIDGET_H
