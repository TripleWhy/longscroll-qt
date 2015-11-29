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
