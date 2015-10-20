#ifndef CONTENTWIDGET_H
#define CONTENTWIDGET_H

#include "longscroll-qt_global.h"
#include "notifyablescrollcontentwidget.h"
#include "contentiteminfo.h"
#include <QAbstractItemView>

LONGSCROLLQT_NAMESPACE_BEGIN

class NavigatorWidget;
struct ContentItemInfo;
class ContentWidgetItemFactory;

#define CONTENTWIDGET_VARIABLE_ROW_HEIGHT 1
class LONGSCROLLQT_EXPORT ContentWidget : public NotifyableScrollContentWidget
{
	Q_OBJECT

private:
	struct ItemInfo
	{
		int index = -1;
		ContentItemInfo * item = 0;
		int x = 0;
		int width = 0;
	};
	friend bool operator==(ContentWidget::ItemInfo const & lhs, ContentWidget::ItemInfo const & rhs);

	struct RowInfo
	{
		QList<ItemInfo> items;
		int y = 0;
		bool displaying = false;
		bool aligned = false;
#if CONTENTWIDGET_VARIABLE_ROW_HEIGHT
		int height;
#endif
	};
	friend bool operator==(ContentWidget::RowInfo const & lhs, ContentWidget::RowInfo const & rhs);

public:
	ContentWidget(QWidget * parent = 0);
	ContentWidget(int rowHeight, int itemWidth = 0, bool alignRows = true, bool alignLastRow = false, bool allowOverfill = true, int navigatorHeight = 500, QWidget *parent = 0);
	~ContentWidget();

	Q_PROPERTY(ContentWidgetItemFactory * itemFactory READ getItemFactory WRITE setItemFactory)
	ContentWidgetItemFactory * getItemFactory();
	void setItemFactory(ContentWidgetItemFactory * factory);

	Q_PROPERTY(NavigatorWidget * navigatorWidget READ getNavigatorWidget WRITE setNavigatorWidget)
	NavigatorWidget * getNavigatorWidget();
	void setNavigatorWidget(NavigatorWidget * nav);

	Q_PROPERTY(bool itemTrackingEnabled READ isItemTrackingEnabled WRITE setItemTrackingEnabled)
	bool isItemTrackingEnabled() const;
	void setItemTrackingEnabled(bool enabled);
	uchar getItemTrackingScreenPositionPercentageX() const;
	uchar getItemTrackingScreenPositionPercentageY() const;
	void setItemTrackingScreenPositionPercentage(uchar percentX, uchar percentY);

	Q_PROPERTY(QList<ContentItemInfo> itemInfos READ getItemInfos WRITE setItemInfos)
	QList<ContentItemInfo> const & getItemInfos() const;
	void setItemInfos(QList<ContentItemInfo> const & infos);

	int findRow(int itemIndex);
	void findRowCol(int & row, int & col, int itemIndex);

public slots:
	void showNavigator(int itemIndex);
	void showNavigator(int row, int col);

	virtual QSize sizeHint() const override;
	virtual void showingRect(QRect const & rect) override;

protected:
	// Can be overridden. Behavior can also be changed by setting a different item factory.
	virtual QWidget * createItemWidget(ContentItemInfo const & info, int itemIndex, int width, int height);

	bool calculateSize(bool const calculateChanges = false);
	void alignRow(RowInfo & row);
#if CONTENTWIDGET_VARIABLE_ROW_HEIGHT
	void scaleRow(RowInfo & row);
#endif

	int rowAt(int y, bool * onNavigator = 0);
	int colAt(int x, int row);
	int colAt(int x, RowInfo const & row);
	void nextImage(int & row, int & col);
	void previousImage(int & row, int & col);

	virtual void mousePressEvent(QMouseEvent * event) override;
	virtual void mouseMoveEvent(QMouseEvent * event) override;
	virtual void mouseReleaseEvent(QMouseEvent * event) override;
	virtual void mouseDoubleClickEvent(QMouseEvent * event) override;
	virtual void startDrag(int row, int col, int itemIndex);

private:
	void updateRows();
	void hideRow(int i);
	void showRow(RowInfo const & rowInfo, int rowIndex);

	void updateSelection(int itemIndex, bool dragging, bool controlPressed, bool shiftPressed);

	void showNavigator(int const row, int const col, bool const blockUpdates);
	void updateNavigator(int const row, int const col);
	void navigatorPrevNext(bool next);

private slots:
	void hideNavigator();
	void navigatorNext();
	void navigatorPrev();

	void updateTrackingItem();
	void updateTrackingPoint();

signals:
	void scrollRequest(int dx, int dy);
	void scrollToRequest(int x, int y);
	void itemPressed(int row, int col, int itemIndex, Qt::MouseButtons buttons);
	void itemReleased(int row, int col, int itemIndex, Qt::MouseButtons buttons);
	void itemClicked(int row, int col, int itemIndex);
	void itemDoubleClicked(int row, int col, int itemIndex);
	void selectionChanged(QList<int> const & selection);
	void currentItemChanged(int itemIndex);

protected:
	static constexpr int prefetchBefore = 1;
	static constexpr int prefetchAfter = 3;
	static constexpr int xSpacing = 5;
	static constexpr int ySpacing = 5;

	int rowHeight = 200;
	int itemWidth = 0;
	bool allowOverfill = true;
	bool align = true;
	bool alignLast = false;
	int navigatorHeight = 500;
#if CONTENTWIDGET_VARIABLE_ROW_HEIGHT
	bool scaleRows = false;
#endif
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

	bool handleMouseEvents = true;
	bool dragEnabled = false;
	QPoint mousePressPoint;
	int mousePressRow = -1;
	int mousePressCol = -1;
	ItemInfo * mousePressItem = 0;
	signed char mouseState = 0;	//0: none, 1: pressed, 3: dragging, -1: navigator pressed, -2: double clicked

	QAbstractItemView::SelectionMode selectionMode = QAbstractItemView::NoSelection;
	QList<int> selection;
	int currentItemIndex = -1;
	QList<int> dragStartSelection;
	int dragStartItemIndex = -1;
};

LONGSCROLLQT_NAMESPACE_END

#endif // CONTENTWIDGET_H
