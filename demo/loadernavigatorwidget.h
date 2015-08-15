#ifndef LOADERNAVIGATORWIDGET_H
#define LOADERNAVIGATORWIDGET_H

#include "imageloader.h"
#include <longscroll-qt/navigatorwidget.h>

class LoaderNavigatorWidget : public NavigatorWidget
{
	Q_OBJECT

public:
	using NavigatorWidget::NavigatorWidget;

	virtual void setImage(ContentItemInfo const & info) override;

private slots:
	void requestFinished(QString const & fileName, QObject const * receiver, QPixmap const & px);

private:
	ImageLoader * loader = 0;
};

#endif // LOADERNAVIGATORWIDGET_H
