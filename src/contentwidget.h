/*
Copyright (C) 2015-2016 Yannick Mueller

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

#ifndef CONTENTWIDGET_H
#define CONTENTWIDGET_H

#include "longscroll-qt_global.h"
#include "notifyablescrollcontentwidget.h"
#include "contentiteminfo.h"

LONGSCROLLQT_NAMESPACE_BEGIN

class NavigatorWidget;
struct ContentItemInfo;
class ContentWidgetItemFactory;

#ifndef CONTENTWIDGET_VARIABLE_ROW_HEIGHT
# define CONTENTWIDGET_VARIABLE_ROW_HEIGHT 1
#endif

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
	ContentWidget(int rowHeight, int itemWidth = 0, QWidget *parent = 0);
	~ContentWidget();

	Q_PROPERTY(ContentWidgetItemFactory * itemFactory READ getItemFactory WRITE setItemFactory)
	ContentWidgetItemFactory * getItemFactory();
	void setItemFactory(ContentWidgetItemFactory * factory);

	Q_PROPERTY(NavigatorWidget * navigatorWidget READ getNavigatorWidget WRITE setNavigatorWidget)
	NavigatorWidget * getNavigatorWidget();
	void setNavigatorWidget(NavigatorWidget * nav);
	Q_PROPERTY(int navigatorHeight READ getNavigatorHeight WRITE setNavigatorHeight)
	int getNavigatorHeight() const;
	void setNavigatorHeight(int height);

	Q_PROPERTY(bool itemTrackingEnabled READ isItemTrackingEnabled WRITE setItemTrackingEnabled)
	bool isItemTrackingEnabled() const;
	void setItemTrackingEnabled(bool enabled);
	uchar getItemTrackingScreenPositionPercentageX() const;
	uchar getItemTrackingScreenPositionPercentageY() const;
	void setItemTrackingScreenPositionPercentage(uchar percentX, uchar percentY);

	Q_PROPERTY(int prefetchRowsBefore READ getPrefetchRowsBefore WRITE setPrefetchRowsBefore)
	int getPrefetchRowsBefore() const;
	void setPrefetchRowsBefore(int rows);
	Q_PROPERTY(int prefetchRowsAfter READ getPrefetchRowsAfter WRITE setPrefetchRowsAfter)
	int getPrefetchRowsAfter() const;
	void setPrefetchRowsAfter(int rows);

	Q_PROPERTY(int horizontalSpacing READ getHorizontalSpacing WRITE setHorizontalSpacing)
	int getHorizontalSpacing() const;
	void setHorizontalSpacing(int spacing);
	Q_PROPERTY(int verticalSpacing READ getVerticalSpacing WRITE setVerticalSpacing)
	int getVerticalSpacing() const;
	void setVerticalSpacing(int spacing);
	Q_PROPERTY(int rowHeight READ getRowHeight WRITE setRowHeight)
	int getRowHeight() const;
	void setRowHeight(int height);
	Q_PROPERTY(int itemWidth READ getItemWidth WRITE setItemWidth)
	int getItemWidth() const;
	void setItemWidth(int width);
	Q_PROPERTY(bool allowOverfill READ getAllowOverfill WRITE setAllowOverfill)
	bool getAllowOverfill() const;
	void setAllowOverfill(bool allow);
	Q_PROPERTY(bool stretchRows READ getStretchRows WRITE setStretchRows)
	bool getStretchRows() const;
	void setStretchRows(bool stretch);
	Q_PROPERTY(bool stretchLastRow READ getStretchLastRow WRITE setStretchLastRow)
	bool getStretchLastRow() const;
	void setStretchLastRow(bool stretch);
#if CONTENTWIDGET_VARIABLE_ROW_HEIGHT
	Q_PROPERTY(bool scaleRows READ getScaleRows WRITE setScaleRows)
	bool getScaleRows() const;
	void setScaleRows(bool scale);
#endif

	Q_PROPERTY(bool handleMouseEvents READ getHandleMouseEvents WRITE setHandleMouseEvents)
	bool getHandleMouseEvents() const;
	void setHandleMouseEvents(bool handle);
	Q_PROPERTY(bool showNavigatorOnClick READ getShowNavigatorOnClick WRITE setShowNavigatorOnClick)
	bool getShowNavigatorOnClick() const;
	void setShowNavigatorOnClick(bool show);
	Q_PROPERTY(bool dragEnabled READ isDragEnabled WRITE setDragEnabled)
	bool isDragEnabled() const;
	void setDragEnabled(bool enabled);
	Q_PROPERTY(QAbstractItemView::SelectionMode selectionMode READ getSelectionMode WRITE setSelectionMode)
	QAbstractItemView::SelectionMode getSelectionMode() const;
	void setSelectionMode(QAbstractItemView::SelectionMode mode);
	Q_PROPERTY(QList<int> selectedItems READ getSelectedItems WRITE setSelectedItems)
	QList<int> getSelectedItems() const;
	void setSelectedItems(QList<int> const & indexes);
	Q_PROPERTY(int currentItem READ getCurrentItem WRITE setCurrentItem)
	int getCurrentItem() const;
	void setCurrentItem(int index);

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
	virtual QWidget * createItemWidget(ContentItemInfo const & info, int itemIndex);

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
	void refresh();
	void rebuild();
	bool calculateSize(bool const calculateChanges = false);
	void updateRows();
#if CONTENTWIDGET_VARIABLE_ROW_HEIGHT
	template<bool scale = false>
#endif
	void alignRow(RowInfo & row);	//template function if CONTENTWIDGET_VARIABLE_ROW_HEIGHT
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
	void scrollToRequest(int y);
	void itemPressed(int row, int col, int itemIndex);
	void itemReleased(int row, int col, int itemIndex);
	void itemClicked(int row, int col, int itemIndex);
	void itemDoubleClicked(int row, int col, int itemIndex);
	void selectionChanged(QList<int> const & selection, QList<int> const & oldSelection);
	void currentItemChanged(int itemIndex, int oldItemIndex);

private:
	//TODO make unsigned?
	int prefetchBefore = 1;
	int prefetchAfter = 3;
	int xSpacing = 5;
	int ySpacing = 5;

	int rowHeight = 150;
	int itemWidth = 0;
	bool allowOverfill = true;
	bool align = true;
	bool alignLast = false;
	int navigatorHeight = 400;
#if CONTENTWIDGET_VARIABLE_ROW_HEIGHT
	bool scaleRows = false;
#endif
	ContentWidgetItemFactory * itemFactory = 0;

private:
	QSize size;
	QRect visibleRect;

	bool itemTrackingEnabled = true;
	//TODO: Implement the following two properties:
//	bool itemTrackingHeightChangeUpdateItem = true;
//	bool itmeTrackingHeightChangeScroll = false;
	int trackingItemIndex;
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
	QList<int> itemWidths;
	QList<RowInfo> rowInfos;
	QVector<QWidget *> rowWidgets;
	QVector<QWidget *> itemWidgets;

	bool handleMouseEvents = true;
	QMetaObject::Connection navigatorClickConnection;
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
