#ifndef CONTENTWIDGET_H
#define CONTENTWIDGET_H

#include "longscroll-qt_global.h"
#include "notifyablescrollcontentwidget.h"

class NavigatorWidget;
struct ContentItemInfo;
class ContentWidgetItemFactory;

#define CONTENTWIDGET_LAZY_ALIGN 1
class LONGSCROLLQT_EXPORT ContentWidget : public NotifyableScrollContentWidget
{
	Q_OBJECT

private:
	struct ItemInfo
	{
		int index = -1;
		ContentItemInfo * img = 0;
		int x = 0;
		int width = 0;
	};
	friend bool operator==(ContentWidget::ItemInfo const & lhs, ContentWidget::ItemInfo const & rhs);

	struct RowInfo
	{
		QList<ItemInfo> items;
		int y = 0;
		bool displaying = false;
#if CONTENTWIDGET_LAZY_ALIGN
		bool aligned = false;
#endif
	};
	friend bool operator==(ContentWidget::RowInfo const & lhs, ContentWidget::RowInfo const & rhs);

protected:
	static constexpr int prefetchBefore = 1;
	static constexpr int prefetchAfter = 3;
	static constexpr int xSpacing = 5;
	static constexpr int ySpacing = 5;

protected:
	int rowHeight = 200;
	int itemWidth = 0;
	bool allowOverfill = true;
	bool align = true;
	bool alignLast = false;
	int navigatorHeight = 500;
	ContentWidgetItemFactory * itemFactory;

private:
	QSize size;
	QRect visibleRect;

	bool itemTrackingEnabled = true;
	//TODO: Implement the following two properties:
//	bool itemTrackingHeightChangeUpdateItem = true;
//	bool itmeTrackingHeightChangeScroll = false;
	ItemInfo trackingItem;
	QPoint trackingPoint;
	int trackingItemOffset = 0;
	uchar itemTrackingX = 0;
	uchar itemTrackingY = 0;
	bool blockScroll = false;

	int navigatorRow = 0;
	int navigatorColumn = 0;
	ContentItemInfo * navigatorImg = 0;
	bool navigatorVisible = false;
	NavigatorWidget * navigator = 0;

	QList<ContentItemInfo> itemInfos;
	QList<int> imageWidths;
	QList<RowInfo> rowInfos;
	QList<QWidget *> rowWidgets;
	QList<QWidget *> itemWidgets;

public:
	ContentWidget(QWidget * parent = 0);
	ContentWidget(int rowHeight, int itemWidth = 0, bool alignRows = true, bool alignLastRow = false, bool allowOverfill = true, int navigatorHeight = 500, QWidget *parent = 0);
	~ContentWidget();

	void setItemFactory(ContentWidgetItemFactory * factory);
	//Item Tracking works better when this ContentWidget is placed inside a NotifyingScrollArea
	void setItemTrackingEnabled(bool enabled);
	void setItemTrackingScreenPositionPercentage(uchar percentX, uchar percentY);

	virtual QSize sizeHint() const override;
	virtual void showingRect(QRect const & rect) override;

private:
	void updateRows();
	void hideRow(int i);
	void showRow(RowInfo const & rowInfo, int rowIndex);

protected:
	// Can be overridden. Behavior can also be changed by setting a different item factory.
	virtual QWidget * createItemWidget(ContentItemInfo const & info, int width, int height);

private slots:
	void updateTrackingItem();
	void updateTrackingPoint();

public:
	void setItemInfos(const QList<ContentItemInfo> & infos);

protected:
	bool calculateSize(bool const calculateChanges = false);
	void alignRow(RowInfo & row);

protected:
	int rowAt(int y, bool * onNavigator = 0);
	int colAt(int x, int row);
	int colAt(int x, RowInfo const & row);
	void nextImage(int & row, int & col);
	void previousImage(int & row, int & col);
	virtual void mousePressEvent(QMouseEvent * event) override;

private:
	void showNavigator(int const row, int const col, bool const blockUpdates = true);
	void updateNavigator(int const row, int const col);

private slots:
	void hideNavigator();
	void navigatorNext();
	void navigatorPrev();

private:
	void navigatorPrevNext(bool next);

signals:
	void scrollRequest(int dx, int dy);
	void scrollToRequest(int x, int y);
};

bool operator==(ContentWidget::ItemInfo const & lhs, ContentWidget::ItemInfo const & rhs);
bool operator==(ContentWidget::RowInfo const & lhs, ContentWidget::RowInfo const & rhs);

#endif // CONTENTWIDGET_H
