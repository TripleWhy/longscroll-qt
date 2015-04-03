#ifndef NAVIGATORWIDGET_H
#define NAVIGATORWIDGET_H

#include <QFrame>
#include "imginfo.h"

class ImageLoader;

namespace Ui {
class NavigatorWidget;
}

class NavigatorWidget : public QFrame
{
	Q_OBJECT

private:
	ImageLoader * loader = 0;
	ImgInfo img;

public:
	explicit NavigatorWidget(QWidget *parent = 0);
	~NavigatorWidget();

public:
	void setImage(ImgInfo const & info);

private slots:
	void requestFinished(QString const & fileName, QObject const * receiver, QPixmap const & px);

signals:
	void previousImageRequested();
	void nextImageRequested();
	void closeRequested();

private:
	Ui::NavigatorWidget *ui;
};

#endif // NAVIGATORWIDGET_H
