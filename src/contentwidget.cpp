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

#include "contentwidget.h"
#include "navigatorwidget.h"
#include "contentiteminfo.h"
#include "contentwidgetitemfactory.h"
#include "longscroll-qt_qtincludes.h"
#include <QApplication>

#ifdef Q_CC_CLANG
# define FALLTHROUGH [[clang::fallthrough]];
#else
# define FALLTHROUGH
#endif


#define CONTENTWIDGET_MEASURE_SHOWINGRECT   0
#define CONTENTWIDGET_MEASURE_CALCULATESIZE 0
#define CONTENTWIDGET_MEASURE_SETINFOS      0

#define CONTENTWIDGET_DEMO_STYLESHEETS              0
#define CONTENTWIDGET_DEBUG_VISUALIZE_TRACKINGITEM  0
#define CONTENTWIDGET_DEBUG_VISUALIZE_TRACKINGPOINT 0

#if CONTENTWIDGET_MEASURE_SHOWINGRECT || CONTENTWIDGET_MEASURE_CALCULATESIZE || CONTENTWIDGET_MEASURE_SETINFOS
# include <QDebug>
# include <QElapsedTimer>
#endif

//TODO: Keyboard selection
//TODO: Create custom layout class to have standard margin and spacing?
//TODO: Stylesheets don't work with the class names.
//TODO: Column layouts. Might need a layout abstraction layer for this.
//TODO: Use Qt::LayoutDirection

LONGSCROLLQT_NAMESPACE_BEGIN

/*!
 * \class ContentWidget
 * \brief Longscroll-qt's central widget.
 * \note You probably want to use LongscrollWidget instead of instantiating this class directly.
 *
 * Implements a widget that can display a lot of items, similar to a QAbstractItemView, but allows dynamic item positions and uses real widgets to display the items,
 * thus there is no MVC concept.
 *
 * Items can be any kind of data and their number is not limited by this widget, however you may face other limits. For example, any QWidget can only have a size of
 * \ref QWidget#QWIDGETSIZE_MAX "QWIDGETSIZE_MAX" pixels in each dimension.
 * An item is visualized by a widget that is created when the item becomes visible and destroyed when it's hidden. This item widget determines how the item looks and
 * what its data mean. The widget is created using the createItemWidget() function, which again calls ContentWidgetItemFactory::createItemWidget().
 * This can be customized by setting a different \ref itemFactory or by overriding createItemWidget(), so that a custom widget is returned.
 *
 * Items are always aligned in rows. The height of the rows can be fixed or dynamic, the width of the items inside a row can also be fixed or dynamic.
 * That way different kind of layouts can be achieved: Tables, lists or Goolge-Image-like views.
 * Set the properties to achieve a layout before seetting \ref itemInfos, since each property change causes re-layouting.
 * The following properties affect the layout:
 * \ref rowHeight,
 * \ref itemWidth,
 * \ref allowOverfill,
 * \ref stretchRows,
 * \ref stretchLastRow,
 * \ref scaleRows,
 * \ref horizontalSpacing,
 * \ref verticalSpacing,
 * \ref itemFactory,
 * \ref navigatorHeight.
 *
 * <DIV class="images">
 * \image html img/layout1.png
 * \image html img/layout2.png
 * \image html img/layout3.png
 * \image html img/layout4.png
 * \image html img/layout5.png
 * \image html img/layout6.png
 * <DIV class="caption">Some possible layouts.</DIV>
 * </DIV>
 *
 * ContentWidget offers a so called navigator widget. The \ref navigatorWidget represents a single item and is shown below the row the item is in. It always has a
 * fixed height of \ref navigatorHeight and the full width of this widget.
 * The navigator widget can for example provide additional information about the selected item.
 * By default the navigator widget is shown when an item is clicked, this can be disabled using showNavigatorOnClick.
 */

/*!
 * \brief Constructs a ContentWidget.
 * \param parent
 */
ContentWidget::ContentWidget(QWidget * parent)
	: NotifyableScrollContentWidget(parent)
{
	itemFactory = new ContentWidgetImageItemFactory(false);

	setNavigatorWidget(new ImageNavigatorWidget(this));
	navigator->setVisible(false);
	setShowNavigatorOnClick(true);

#if CONTENTWIDGET_DEMO_STYLESHEETS
	setStyleSheet("QWidget { background: green; } ImageItem { background: blue; } QLabel { background: red; }");
	navigator->setStyleSheet("QWidget { background: darkgray; }");
#endif
}

/*!
 * \brief Constructs a ContentWidget.
 * \param rowHeight
 * \param itemWidth
 * \param parent
 */
ContentWidget::ContentWidget(int rowHeight, int itemWidth, QWidget * parent)
    : ContentWidget(parent)
{
	setRowHeight(rowHeight);
	setItemWidth(itemWidth);
}

/*!
 * \brief Destroys the widget.
 */
ContentWidget::~ContentWidget()
{
	delete itemFactory;
}

/*!
 * \property ContentWidget::itemFactory
 * \brief ContentWidgetItemFactory used to create displaying widgets.
 * This ContentWidget takes ownership of the ContentWidgetItemFactory.
 * \accessors getItemFactory(), setItemFactory()
 * \default{new ContentWidgetImageItemFactory(false, this)}
 * \see ContentWidgetImageItemFactory
 */

/*!
 * \see itemFactory
 */
ContentWidgetItemFactory * ContentWidget::getItemFactory()
{
	return itemFactory;
}

/*!
 * \see itemFactory
 */
void ContentWidget::setItemFactory(ContentWidgetItemFactory * factory)
{
	delete itemFactory;
	itemFactory = factory;
	rebuild();
}

/*!
 * \property ContentWidget::navigatorWidget
 * \brief NavigatorWidget.
 * \default{new ImageNavigatorWidget(this)}
 * \accessors getNavigatorWidget(), setNavigatorWidget()
 * \see navigatorHeight
 * \see ImageNavigatorWidget
 */

/*!
 * \see navigatorWidget
 */
NavigatorWidget * ContentWidget::getNavigatorWidget()
{
	return navigator;
}

/*!
 * \see navigatorWidget
 */
void ContentWidget::setNavigatorWidget(NavigatorWidget * nav)
{
	if (navigator == nav)
		return;

	bool updatesBlocked = false;
	if (navigator != 0 && navigator->isVisible() && updatesEnabled())
	{
		updatesBlocked = true;
		setUpdatesEnabled(false);
	}

	nav->setParent(this);
	if (navigator != 0)
	{
		nav->setGeometry(navigator->geometry());
		nav->setVisible(navigator->isVisible());
		navigator->setVisible(false);
		navigator->deleteLater();
	}
	navigator = nav;

	connect(navigator, SIGNAL(closeRequested()), this, SLOT(hideNavigator()));
	connect(navigator, SIGNAL(nextImageRequested()), this, SLOT(navigatorNext()));
	connect(navigator, SIGNAL(previousImageRequested()), this, SLOT(navigatorPrev()));

	if (updatesBlocked)
		setUpdatesEnabled(true);
}

/*!
 * \property ContentWidget::navigatorHeight
 * \brief Navigator widget height.
 * The navigator widget always has a fixed height.
 * \default 400
 * \accessors getNavigatorHeight(), setNavigatorHeight()
 * \see navigatorWidget
 */

/*!
 * \see navigatorWidget
 */
int ContentWidget::getNavigatorHeight() const
{
	return navigatorHeight;
}

/*!
 * \see navigatorWidget
 */
void ContentWidget::setNavigatorHeight(int height)
{
	if (height <= 0)
		return;
	navigatorHeight = height;
	refresh();
}

/*!
 * \property ContentWidget::itemTrackingEnabled
 * \brief Tries to keep one item at the same screen y-coordinate while resizing.
 * If item tracking is enabled, an item is memorized when the user scrolls the widget.
 * When the widget is resized, it tries to keep that tracking item at the same screen y-coordinate.
 * \default true
 * \accessors isItemTrackingEnabled(), setItemTrackingEnabled()
 * \see setItemTrackingScreenPositionPercentage
 */

/*!
 * \see itemTrackingEnabled
 */
bool ContentWidget::isItemTrackingEnabled() const
{
	return itemTrackingEnabled;
}

/*!
 * \see itemTrackingEnabled
 */
void ContentWidget::setItemTrackingEnabled(bool enabled)
{
	if (enabled == itemTrackingEnabled)
		return;
	itemTrackingEnabled = enabled;
	trackingItemIndex = -1;
}

/*!
 * \brief ContentWidget::getItemTrackingScreenPositionPercentageX
 * \return Tracking item x-coordinate percentage
 * \see setItemTrackingScreenPositionPercentage
 */
uchar ContentWidget::getItemTrackingScreenPositionPercentageX() const
{
	return itemTrackingX;
}

/*!
 * \brief ContentWidget::getItemTrackingScreenPositionPercentageY
 * \return Tracking item y-coordinate percentage
 * \see setItemTrackingScreenPositionPercentage
 */
uchar ContentWidget::getItemTrackingScreenPositionPercentageY() const
{
	return itemTrackingY;
}

/*!
 * \brief Sets the position where the tracking item is searched.
 * If item tracking is enabled, the tracking item is the item that is found at this position of the visible region.
 * The values are in percent. For example, <tt>(0, 0)</tt> is the item at the top left corner, <tt>(0, 100)</tt> is the item at the bottom left corner
 * and <tt>(50, 50)</tt> is the item at the center.
 * \default{(0, 0)}
 * \param percentX X-coordinate percentage.
 * \param percentY Y-coordinate percentage.
 * \see itemTrackingEnabled
 * \see getItemTrackingScreenPositionPercentageX
 * \see getItemTrackingScreenPositionPercentageY
 */
void ContentWidget::setItemTrackingScreenPositionPercentage(uchar percentX, uchar percentY)
{
	percentX = qBound(uchar(0), percentX, uchar(100));
	percentY = qBound(uchar(0), percentY, uchar(100));
	if (percentX == itemTrackingX && percentY == itemTrackingY)
		return;
	itemTrackingX = percentX;
	itemTrackingY = percentY;
	trackingItemIndex = -1;
}

/*!
 * \property ContentWidget::prefetchRowsBefore
 * \brief Number of rows before the first visible row that should be kept ready for displaying.
 * These rows are not currently visible, but their widgets still or already exist anyway.
 * This can be useful for example if a widget needs some time to load data from the disk or network.
 * \default 1
 * \accessors getPrefetchRowsBefore(), setPrefetchRowsBefore()
 */

/*!
 * \see prefetchRowsBefore
 */
int ContentWidget::getPrefetchRowsBefore() const
{
	return prefetchBefore;
}

/*!
 * \see prefetchRowsBefore
 */
void ContentWidget::setPrefetchRowsBefore(int rows)
{
	prefetchBefore = rows;
}

/*!
 * \property ContentWidget::prefetchRowsAfter
 * \brief Number of rows after the last visible row that should be kept ready for displaying.
 * These rows are not currently visible, but their widgets still or already exist anyway.
 * This can be useful for example if a widget needs some time to load data from the disk or network.
 * \default 3
 * \accessors getPrefetchRowsAfter(), setPrefetchRowsAfter()
 */

/*!
 * \see prefetchRowsAfter
 */
int ContentWidget::getPrefetchRowsAfter() const
{
	return prefetchAfter;
}

/*!
 * \see prefetchRowsAfter
 */
void ContentWidget::setPrefetchRowsAfter(int rows)
{
	prefetchAfter = rows;
}

/*!
 * \property ContentWidget::horizontalSpacing
 * \brief Horizontal spacing between items.
 * \default 5
 * \accessors getHorizontalSpacing(), setHorizontalSpacing()
 */

/*!
 * \see horizontalSpacing
 */
int ContentWidget::getHorizontalSpacing() const
{
	return xSpacing;
}

/*!
 * \see horizontalSpacing
 */
void ContentWidget::setHorizontalSpacing(int spacing)
{
	xSpacing = spacing;
	refresh();
}

/*!
 * \property ContentWidget::verticalSpacing
 * \brief Vertical spacing between items.
 * \default 5
 * \accessors getVerticalSpacing(), setVerticalSpacing()
 */

/*!
 * \see verticalSpacing
 */
int ContentWidget::getVerticalSpacing() const
{
	return ySpacing;
}

/*!
 * \see verticalSpacing
 */
void ContentWidget::setVerticalSpacing(int spacing)
{
	ySpacing = spacing;
	refresh();
}

/*!
 * \property ContentWidget::rowHeight
 * \brief Row base height.
 * If \ref scaleRows is not set, every row will have this height. If \c scaleRows is set, this will be the base value to calculate the actual size from.
 * The value must be > 0.
 * \default 150
 * \accessors getRowHeight(), setRowHeight()
 * \see scaleRows
 */

/*!
 * \see rowHeight
 */
int ContentWidget::getRowHeight() const
{
	return rowHeight;
}

/*!
 * \see rowHeight
 */
void ContentWidget::setRowHeight(int height)
{
	if (height <= 0)
		return;
	rowHeight = height;
	rebuild();
}

/*!
 * \property ContentWidget::itemWidth
 * \brief Item base width.
 * If <tt>itemWidth > 0</tt>, all items will have this width, unless \ref stretchRows is set. \n
 * If <tt>itemWidth < 0</tt>, all items will have some large width. Only use this to gether with \ref stretchRows and without \ref scaleRows. That has the effect that one item takes exactly one row. \n
 * If <tt>itemWidth == 0</tt>, the items will have their size assigned by using <TT>item.widthForHeight(rowHeight)</TT>, i.e. each item has the width that fits the \ref rowHeight best.
 * \default 0
 * \accessors getItemWidth(), setItemWidth()
 * \see stretchRows
 */

/*!
 * \see itemWidth
 */
int ContentWidget::getItemWidth() const
{
	return itemWidth;
}

/*!
 * \see itemWidth
 */
void ContentWidget::setItemWidth(int width)
{
	itemWidth = width;
	rebuild();
}

/*!
 * \property ContentWidget::allowOverfill
 * \brief Allows the last item in a row to exceed the width of that row.
 * If \c allowOverfill is not set, a row is never wider than the widget.\n
 * If \c allowOverfill is set, the widget decides for the first item that does not fit inside a row anymore, if it adds the item to the row anyway or creates a new row.
 * The item is added to the row if more than half of the item fits inside the row.
 * This is usually used together with \ref stretchRows or \ref scaleRows.
 * \default true
 * \accessors getAllowOverfill(), setAllowOverfill()
 * \see stretchRows
 * \see scaleRows
 */

/*!
 * \see allowOverfill
 */
bool ContentWidget::getAllowOverfill() const
{
	return allowOverfill;
}

/*!
 * \see allowOverfill
 */
void ContentWidget::setAllowOverfill(bool allow)
{
	allowOverfill = allow;
	refresh();
}

/*!
 * \property ContentWidget::stretchRows
 * \brief Stretch row widths to the actual widget's width.
 * If \c stretchRows is set, all rows will be stretched to the actual widget's width. This is done by stretching all items of a row proportionally to their widths.
 * If \c stretchRows is not set, all items will have the size specified by \ref itemWidth.
 * This value does not affect the last row, which is controlled by \ref stretchLastRow.
 * \default true
 * \accessors getStretchRows(), setStretchRows()
 * \see stretchLastRow
 * \see scaleRows
 */

/*!
 * \see stretchRows
 */
bool ContentWidget::getStretchRows() const
{
	return align;
}

/*!
 * \see stretchRows
 */
void ContentWidget::setStretchRows(bool stretch)
{
	align = stretch;
	refresh();
}

/*!
 * \property ContentWidget::stretchLastRow
 * \brief Stretch last row.
 * The last row can be stretched independently from the other rows. This is because the last row usually is not completely filled and stretching it looks strange.
 * \default false
 * \accessors getStretchLastRow(), setStretchLastRow()
 * \see stretchRows
 */

/*!
 * \see stretchLastRow
 */
bool ContentWidget::getStretchLastRow() const
{
	return alignLast;
}

/*!
 * \see stretchLastRow
 */
void ContentWidget::setStretchLastRow(bool stretch)
{
	alignLast = stretch;
	refresh();
}

#if CONTENTWIDGET_VARIABLE_ROW_HEIGHT
/*!
 * \property ContentWidget::scaleRows
 * \brief Scale rows to the actual widget's width, keeping the items' aspect ratio.
 * Similar to \ref stretchRows, but keeps the aspect ratios of the items. Instead it changes the row height. If enabled, overrides \ref stretchRows, but respects \ref stretchLastRow.
 * For (theoretically) better performance you should disable \ref stretchRows when \c scaleRows is set.
 *
 * Enabling \c scaleRows means that rows don't have fixed row heights anymore, which causes some operations to be a bit more complex.
 * For maximum performance, this feature can be disabled completely by compiling the library with \c CONTENTWIDGET_VARIABLE_ROW_HEIGHT=0 defined.
 * However I found the effective performance impact hardly mensurable.
 *
 * Don't use \c scaleRows with \ref itemWidth <tt>< 0</tt>.
 * \default false
 * \accessors getScaleRows(), setScaleRows()
 * \see stretchRows
 * \see stretchLastRow
 */

/*!
 * \see scaleRows
 */
bool ContentWidget::getScaleRows() const
{
	return scaleRows;
}

/*!
 * \see scaleRows
 */
void ContentWidget::setScaleRows(bool scale)
{
	scaleRows = scale;
	refresh();
}
#endif

/*!
 * \property ContentWidget::handleMouseEvents
 * \brief Handle mouse events.
 * Handle press, release and move events to change the selection, start dragging, emit events or show navigator.
 * \default true
 * \accessors getHandleMouseEvents(), setHandleMouseEvents()
 * \see showNavigatorOnClick
 * \see selectionMode
 * \see dragEnabled
 */

/*!
 * \see ContentWidget::handleMouseEvents
 */
bool ContentWidget::getHandleMouseEvents() const
{
	return handleMouseEvents;
}

/*!
 * \see ContentWidget::handleMouseEvents
 */
void ContentWidget::setHandleMouseEvents(bool handle)
{
	handleMouseEvents = handle;
}

/*!
 * \property ContentWidget::showNavigatorOnClick
 * \brief Show the navigator widget on item click, requires \ref handleMouseEvents to be set.
 * \default true
 * \accessors getShowNavigatorOnClick(), setShowNavigatorOnClick()
 * \see handleMouseEvents
 */

/*!
 * \see ContentWidget::showNavigatorOnClick
 */
bool ContentWidget::getShowNavigatorOnClick() const
{
	return navigatorClickConnection;
}

/*!
 * \see ContentWidget::showNavigatorOnClick
 */
void ContentWidget::setShowNavigatorOnClick(bool show)
{
	if (bool(navigatorClickConnection) == show)
		return;
	if (show)
		navigatorClickConnection = connect(this, SIGNAL(itemClicked(int,int,int)), this, SLOT(showNavigator(int,int)));
	else
		disconnect(navigatorClickConnection);
}

/*!
 * \property ContentWidget::dragEnabled
 * \brief Enables drag detection, requires \ref handleMouseEvents to be set.
 * If \c dragEnabled is set, the widget detects mouse dragging.
 * If a drag was detected, \ref startDrag is called. Any actual actions need to be implemented by overriding it.
 * \default false
 * \accessors isDragEnabled(), setDragEnabled()
 * \see handleMouseEvents
 * \see startDrag
 */

/*!
 * \see ContentWidget::dragEnabled
 */
bool ContentWidget::isDragEnabled() const
{
	return dragEnabled;
}

/*!
 * \see ContentWidget::dragEnabled
 */
void ContentWidget::setDragEnabled(bool enabled)
{
	dragEnabled = enabled;
}

/*!
 * \property ContentWidget::selectionMode
 * \brief Selection mode.
 * If \c selectionMode is set to \c QAbstractItemView::NoSelection, the selection is cleared and disabled.
 * Requires \ref handleMouseEvents for mouse selection to work.
 *
 * Note that selection is not automatically displayed. The item widget has to visualize the selection.
 * ImageItemWidget for example can be configured to visualize selections.
 *
 * Currently only mouse selection is implemented.
 * \accessors getSelectionMode(), setSelectionMode()
 * \see handleMouseEvents
 */

/*!
 * \see ContentWidget::selectionMode
 */
QAbstractItemView::SelectionMode ContentWidget::getSelectionMode() const
{
	return selectionMode;
}

/*!
 * \see ContentWidget::selectionMode
 */
void ContentWidget::setSelectionMode(QAbstractItemView::SelectionMode mode)
{
	selectionMode = mode;
	if (mode == QAbstractItemView::NoSelection)
		setSelectedItems(QList<int>());
}

/*!
 * \property ContentWidget::selectedItems
 * \brief Selected item indexes.
 * \accessors getSelectedItems(), setSelectedItems()
 */

/*!
 * \see ContentWidget::selectedItems
 */
QList<int> ContentWidget::getSelectedItems() const
{
	return selection;
}

/*!
 * \see ContentWidget::selectedItems
 */
void ContentWidget::setSelectedItems(QList<int> const & indexes)
{
	if (indexes == selection)
		return;
	QList<int> oldSelection = indexes;
	selection.swap(oldSelection);
	emit selectionChanged(selection, oldSelection);
}

/*!
 * \property ContentWidget::currentItem
 * \brief Current item index.
 * \accessors getcurrentItem(), setcurrentItem()
 */

/*!
 * \see ContentWidget::currentItem
 */
int ContentWidget::getCurrentItem() const
{
	return currentItemIndex;
}

/*!
 * \see ContentWidget::currentItem
 */
void ContentWidget::setCurrentItem(int index)
{
	if (index == currentItemIndex)
		return;
	int oldItemindex = currentItemIndex;
	currentItemIndex = index;
	emit currentItemChanged(currentItemIndex, oldItemindex);
}

/*!
 * \property ContentWidget::itemInfos
 * \brief The list of ContentItemInfos being displayed.
 * \accessors getItemInfos(), setItemInfos()
 */

/*!
 * \see itemInfos
 */
const QList<ContentItemInfo> &ContentWidget::getItemInfos() const
{
	return itemInfos;
}

/*!
 * \see itemInfos
 */
void ContentWidget::setItemInfos(const QList<ContentItemInfo> & infos)
{
#if CONTENTWIDGET_MEASURE_SETINFOS
	QElapsedTimer t;
	t.start();
#endif

	hideNavigator();
	itemInfos = infos;
	int const size = itemInfos.size();
	itemWidths.clear();
	itemWidths.reserve(size);
	qDeleteAll(itemWidgets);
	itemWidgets.fill(0, size); //resize and fill
	qDeleteAll(rowWidgets);
	rowWidgets.clear();
	rowInfos.clear();
	trackingItemIndex = -1;
	if (itemWidth == 0)
	{
		for (ContentItemInfo const & img : infos)
			itemWidths.append(img.widthForHeight(rowHeight));
	}
	else if (itemWidth < 0)
	{
		for (int i = 0; i < size; ++i)
			itemWidths.append( std::numeric_limits<decltype(itemWidth)>::max() / 4 );	// big value, but also leave some room for calculations.
	}
	else
	{
		for (int i = 0; i < size; ++i)
			itemWidths.append(itemWidth);
	}

	refresh();

#if CONTENTWIDGET_MEASURE_SETINFOS
	qint64 elapsed = t.elapsed();
	qDebug() << "ContentWidget::setItemInfos took" << elapsed << "ms";
#endif
}

/*!
 * \brief Searches the row of a given item index.
 * Uses binary search using \c std::lower_bound() to find the row.
 * The \c itemIndex must be in the range of the item info list, this is not checked.
 * \param itemIndex
 * \return row
 * \see itemInfos
 * \see findRowCol
 */
int ContentWidget::findRow(int itemIndex)
{
	int const row = std::lower_bound(rowInfos.begin(), rowInfos.end(), itemIndex, [](RowInfo const & ri, int index){ return ri.items.last().index < index; }) - rowInfos.begin();
	Q_ASSERT(rowInfos[row].items.first().index <= itemIndex);
	Q_ASSERT(rowInfos[row].items.last().index >= itemIndex);
	return row;
}

/*!
 * \brief Searches the row and column of a given item index.
 * Uses binary search using \c std::lower_bound() to find the row, the column is calculated directly.
 * The itemIndex must be in the range of the item info list, this is not checked.
 * \param row Returns the row.
 * \param col Returns the column.
 * \param itemIndex
 * \see itemInfos
 * \see findRow
 */
void ContentWidget::findRowCol(int & row, int & col, int itemIndex)
{
	row = findRow(itemIndex);
	col = itemIndex - rowInfos[row].items.first().index;
	Q_ASSERT(rowInfos[row].items[col].index == itemIndex);
}

/*!
 * \brief Shows the \ref navigatorWidget for the given item index.
 * The index must exist in the item info list.
 * \param itemIndex
 * \see itemInfos
 * \see findRowCol
 */
void ContentWidget::showNavigator(int itemIndex)
{
	int row, col;
	findRowCol(row, col, itemIndex);
	showNavigator(row, col);
}

/*!
 * \brief Shows the \ref navigatorWidget for the given row and column.
 * The row and column must exist.
 * \param row
 * \param col
 */
void ContentWidget::showNavigator(int row, int col)
{
	showNavigator(row, col, true);
}

/*!
 * \reimp{NotifyableScrollContentWidget::sizeHint}
 */
QSize ContentWidget::sizeHint() const
{
	return size;
}

/*!
 * \reimp{NotifyableScrollContentWidget::showingRect}
 */
void ContentWidget::showingRect(const QRect & rect)
{
#if CONTENTWIDGET_MEASURE_SHOWINGRECT
	static quint64 tSum = 0;
	static quint64 tCount = 0;
	QElapsedTimer t;
	t.start();
#endif

	if (rect == visibleRect || itemInfos.isEmpty())
		return;

	bool needsUpdate = itemTrackingEnabled && !blockScroll && rect.size() == visibleRect.size() && rect.topLeft() != visibleRect.topLeft();

	setUpdatesEnabled(false);

	if (rect.width() != visibleRect.width())
	{
		if (itemTrackingEnabled)
		{
			if (trackingItemIndex < 0)
				updateTrackingItem();
			else
				updateTrackingPoint();
		}

		visibleRect = rect;

#if CONTENTWIDGET_MEASURE_CALCULATESIZE
		QElapsedTimer t2;
		t2.start();
#endif
		bool changed = calculateSize(itemTrackingEnabled);
#if CONTENTWIDGET_MEASURE_CALCULATESIZE
		auto elapsed2 = t2.elapsed();
		qDebug() << "calculating size took" << elapsed2 << "ms";
#endif

		// changed == true implies itemTrackingEnabled == true
		if (changed && trackingItemIndex >= 0)
		{
			int const r = findRow(trackingItemIndex);
			blockScroll = true;
			emit scrollToRequest(rowInfos[r].y + trackingItemOffset - (trackingPoint.y() - rect.y()));
			blockScroll = false;
		}
	}
	else
		visibleRect = rect;

	if (rect.width() <= 0)
		return;

	updateRows();

	setUpdatesEnabled(true);

	if (needsUpdate)
		updateTrackingItem();

#if CONTENTWIDGET_MEASURE_SHOWINGRECT
	auto elapsed = t.elapsed();
	tSum += elapsed;
	++tCount;
	qDebug() << "showingRect took" << elapsed << "ms" << " average:" << (tSum / tCount) << "ms";
#endif
}

/*!
 * \brief Creates an item widget.
 * This method creates a widget to visualize an item. These widgets are created when the respective item becomes close to the visible region
 * and destroyed when it moves away. For fine control when the widget is created, use \ref prefetchRowsBefore and \ref prefetchRowsAfter.
 * This widget takes ownership of the returned widget.
 *
 * This method can be overridden. However, the same effect can be achieved by changing the \ref itemFactory.
 *
 * \param info The item info to be displayed.
 * \param itemIndex List index of the item info.
 * \return The newly created widget.
 * \see itemFactory
 * \see prefetchBefore
 * \see prefetchAfter
 */
QWidget *ContentWidget::createItemWidget(const ContentItemInfo & info, int itemIndex)
{
	QWidget * widget = itemFactory->createItemWidget(info, itemIndex, this);
#if CONTENTWIDGET_DEMO_STYLESHEETS
	widget->setStyleSheet("QWidget{ background: blue; } QLabel { background: red; }");
#endif
	return widget;
}

/*!
 * \brief Calculates which row a given y-coordinate belongs to.
 * If the navigator widget is shown and the y-coordinate is on the navigator, the returned row will be the number of the row above the navigator.
 * The optional parameter onNavigator returns if the y-coordinate is on the navigator.
 * \param y Y-Coordinate.
 * \param onNavigator Returns if y is on the navigator widget.
 * \return Row index.
 */
int ContentWidget::rowAt(int y, bool * onNavigator)
{
	if (onNavigator != 0)
		*onNavigator = false;
	int row = y / (rowHeight + ySpacing);
#if CONTENTWIDGET_VARIABLE_ROW_HEIGHT
	if (scaleRows)
	{
		int const size = rowInfos.size();
		while (row < size && rowInfos[row].y < y)
			row++;
		while (row >= size || rowInfos[row].y > y)
			row--;

		if (onNavigator != 0)
		{
			RowInfo const & rowInfo = rowInfos[row];
			if (y > rowInfo.y + rowInfo.height)
				*onNavigator = true;
		}
	}
	else
#endif
	{
		if (navigatorVisible && row > navigatorRow)
		{
			row = (y - navigatorHeight - ySpacing) / (rowHeight + ySpacing);
			if (row <= navigatorRow)
			{
				if (onNavigator != 0)
					*onNavigator = true;
				row = navigatorRow;
			}
		}
	}
	return row;
}

/*!
 * \overloaded
 * \see colAt(int x, const ContentWidget::RowInfo & row)
 */
int ContentWidget::colAt(int x, int row)
{
	return colAt(x, rowInfos.at(row));
}

/*!
 * \brief Calculates which column of a row a given x-coordinate belongs to.
 * Since items can have different widths, a row is needed.
 * \param x X-Coordinate.
 * \param row Row.
 * \return Column.
 * \see rowAt
 */
int ContentWidget::colAt(int x, const ContentWidget::RowInfo & row)
{
	for (int i = row.items.length() - 1; i >= 0; --i)
		if (row.items.at(i).x <= x)
			return i;
	return 0;
}

/*!
 * \brief Takes a row and column and changes them to point at the next item.
 * The input row and column have to exist. If there is no next item, row and col will be -1.
 * \param row Row.
 * \param col Column.
 */
void ContentWidget::nextImage(int & row, int & col)
{
	RowInfo const & rowInfo = rowInfos.at(row);
	++col;
	if (col < rowInfo.items.size())
		return;
	++row;
	if (row < rowInfos.size())
	{
		col = 0;
		return;
	}
	col = -1;
	row = -1;
}

/*!
 * \brief Takes a row and column and changes them to point at the previous item.
 * The input row and column have to exist. If there is no previous item, row and col will be -1.
 * \param row Row.
 * \param col Column.
 */
void ContentWidget::previousImage(int & row, int & col)
{
	--col;
	if (col >= 0)
		return;
	--row;
	if (row >= 0)
	{
		RowInfo const & prevRow = rowInfos.at(row);
		col = prevRow.items.size() - 1;
		return;
	}
	col = -1;
	row = -1;
}

/*!
 * \reimp{NotifyableScrollContentWidget::mousePressEvent}
 */
void ContentWidget::mousePressEvent(QMouseEvent * event)
{
	if (!handleMouseEvents)
	{
		event->ignore();
		return;
	}
	event->accept();
	mouseState = 0;

	if (itemInfos.isEmpty())
		return;

	bool onNavigator;
	int row = rowAt(event->y(), &onNavigator);
	if (onNavigator)
	{
		mouseState = -1;
		return;
	}
	
	int col = colAt(event->x(), row);
	mouseState = 1;
	ItemInfo * item = &rowInfos[row].items[col];

	if (event->buttons() == Qt::LeftButton)
	{
		mousePressPoint = event->pos();
		mousePressRow = row;
		mousePressCol = col;
		mousePressItem = item;

		updateSelection(item->index, false, event->modifiers() & Qt::ControlModifier, event->modifiers() & Qt::ShiftModifier);
	}
	else
	{
		mousePressPoint = QPoint(-1, -1);
		mousePressRow = -1;
		mousePressCol = -1;
		mousePressItem = 0;
	}

	emit itemPressed(row, col, item->index);
}

/*!
 * \reimp{NotifyableScrollContentWidget::mouseMoveEvent}
 */
void ContentWidget::mouseMoveEvent(QMouseEvent * event)
{
	if (!handleMouseEvents)
	{
		event->ignore();
		return;
	}
	event->accept();
	if (event->buttons() == Qt::LeftButton && !itemInfos.isEmpty())
	{
		if (dragEnabled)
		{
			if (mouseState == 1
			        && (mousePressPoint - event->pos()).manhattanLength() >= QApplication::startDragDistance())
			{
				mouseState = 2;
				startDrag(mousePressRow, mousePressCol, mousePressItem->index);
			}
		}
		else
		{
			bool onNavigator;
			int row = rowAt(event->y(), &onNavigator);
			if (onNavigator)
				return;

			int col = colAt(event->x(), row);
			ItemInfo * item = &rowInfos[row].items[col];

			updateSelection(item->index, true, event->modifiers() & Qt::ControlModifier, event->modifiers() & Qt::ShiftModifier);
		}
	}
}

/*!
 * \reimp{NotifyableScrollContentWidget::mouseReleaseEvent}
 */
void ContentWidget::mouseReleaseEvent(QMouseEvent * event)
{
	if (!handleMouseEvents)
	{
		event->ignore();
		return;
	}

	event->accept();

	if (itemInfos.isEmpty())
		return;

	bool onNavigator;
	int row = rowAt(event->y(), &onNavigator);
	if (onNavigator)
	{
		mouseState = -1;
		return;
	}
	int col = colAt(event->x(), row);
	ItemInfo * item = &rowInfos[row].items[col];
	emit itemReleased(row, col, item->index);

	if (event->buttons() == Qt::NoButton && mouseState == 1 &&
	        row == mousePressRow && col == mousePressCol && item == mousePressItem)
		emit itemClicked(row, col, item->index);
}

/*!
 * \reimp{NotifyableScrollContentWidget::mouseDoubleClickEvent}
 */
void ContentWidget::mouseDoubleClickEvent(QMouseEvent * event)
{
	event->accept();
	mouseState = -2;

	if (itemInfos.isEmpty())
		return;

	bool onNavigator;
	int row = rowAt(event->y(), &onNavigator);
	if (onNavigator)
		return;
	int col = colAt(event->x(), row);
	int itemIndex = rowInfos[row].items[col].index;
	emit itemDoubleClicked(row, col, itemIndex);
}

/*!
 * \brief Begins a drag operation.
 * If dragging is enabled (dragEnabled), and the user tries to drag an item, this method is called.
 * If you wish to implement some kind of drag support, you have to override this moethod.
 * The default implementation does nothing.
 * \see dragEnabled
 */
void ContentWidget::startDrag(int /*row*/, int /*col*/, int /*itemIndex*/)
{
}

void ContentWidget::refresh()
{
	QRect r = visibleRect;
	visibleRect = QRect();
	showingRect(r);
}

void ContentWidget::rebuild()
{
	setItemInfos(itemInfos);
}

bool ContentWidget::calculateSize(const bool calculateChanges)
{
	QList<RowInfo> rowInfosNew;
	rowInfosNew.reserve(rowInfos.size());
	RowInfo row;
	int rowWidth = 0;
	int y = 0;
	int navRow = -1, navCol = -1;
	for (int i = 0, l = itemInfos.size(); i < l; ++i)
	{
		int width = itemWidths[i];

		ItemInfo item;
		item.index = i;
		item.item = &itemInfos[i];
		item.width = width;

		if (navigatorVisible && navigatorImg == item.item)
		{
			navRow = rowInfosNew.size();
			navCol = row.items.size();
		}

		if (rowWidth == 0)
		{
			item.x = 0;
			row.items.append(item);
			rowWidth += width;
		}
		else if(rowWidth + width + xSpacing <= visibleRect.width())
		{
			item.x = rowWidth + xSpacing;
			row.items.append(item);
			rowWidth += width + xSpacing;
		}
		else
		{
			if (allowOverfill && ( ((rowWidth + width + xSpacing) - visibleRect.width()) < (visibleRect.width() - rowWidth) ))	//overfill
			{
				item.x = rowWidth + xSpacing;
				row.items.append(item);
				row.aligned = !align;
#if CONTENTWIDGET_VARIABLE_ROW_HEIGHT
				row.height = rowHeight;
				if (scaleRows)
					alignRow<true>(row);
				y += row.height + ySpacing;
#else
				y += rowHeight + ySpacing;
#endif
				rowInfosNew.append(row);
				row = RowInfo();
				row.y = y;
				rowWidth = 0;
			}
			else	//underfill
			{
				row.aligned = !align;
#if CONTENTWIDGET_VARIABLE_ROW_HEIGHT
				row.height = rowHeight;
				if (scaleRows)
					alignRow<true>(row);
				y += row.height + ySpacing;
#else
				y += rowHeight + ySpacing;
#endif
				rowInfosNew.append(row);
				row = RowInfo();
				row.y = y;

				item.x = 0;
				row.items.append(item);
				rowWidth = width;

				if (navigatorVisible && navigatorImg == item.item)
				{
					++navRow;
					navCol = 0;
				}
			}
			if (navigatorVisible && rowInfosNew.size()-1 == navRow)
			{
				y += navigatorHeight + ySpacing;
				row.y += navigatorHeight + ySpacing;
			}
		}
	}
	size.setWidth(visibleRect.width());
	if (!row.items.isEmpty())
	{
		row.aligned = !alignLast;
#if CONTENTWIDGET_VARIABLE_ROW_HEIGHT
		row.height = rowHeight;
		if (scaleRows && alignLast)
			alignRow<true>(row);
		size.setHeight(y + row.height);
#else
		size.setHeight(y + rowHeight);
#endif
		rowInfosNew.append(row);
	}
	else
	{
		size.setHeight(qMax(0, y - ySpacing));
	}
	setMinimumSize(size);


	int const is = rowInfosNew.size();
	int const ws = rowWidgets.size();
	for (int i = 0, l = qMin(ws, is); i < l; ++i)
	{
		if (rowWidgets[i] == 0)
			continue;
		QList<ItemInfo> const & items1 = rowInfos[i].items;
		QList<ItemInfo> const & items2 = rowInfosNew[i].items;
		if (items1 != items2)
		{
			QWidget *& rowWidget = rowWidgets[i];
			QObjectList children = rowWidget->children();	//copy list
			for (QObject * o : children)
			{
				QWidget * w = static_cast<QWidget *>(o);
				w->setVisible(false);
				w->setParent(this);
			}
			delete rowWidget;
			rowWidget = 0;
		}
	}
	for (int i = is; i < ws; ++i)
	{
		QWidget *& rowWidget = rowWidgets[i];
		if (rowWidget == 0)
			continue;
		QObjectList children = rowWidget->children();	//copy list
		for (QObject * o : children)
		{
			QWidget * w = static_cast<QWidget *>(o);
			w->setVisible(false);
			w->setParent(this);
		}
		delete rowWidget;
		rowWidget = 0;
	}
	rowWidgets.resize(is);
	Q_ASSERT(rowWidgets.size() == rowInfosNew.size());
	rowInfos.swap(rowInfosNew);

	if (navigatorVisible)
		updateNavigator(navRow, navCol);

	if (calculateChanges)
		return rowInfos != rowInfosNew;
	else
		return false;
}

void ContentWidget::updateRows()
{
	int bottom = visibleRect.bottom();
	if (navigatorVisible)
		bottom += navigatorHeight;
	int lastRow = qMin(rowInfos.length() - 1, rowAt(bottom)  +  prefetchAfter);
	int firstRow = qBound(0, rowAt(visibleRect.y())  -  prefetchBefore, lastRow);

	for (int i = 0; i < firstRow; i++)
	{
		RowInfo & row = rowInfos[i];
		if (row.displaying)
			hideRow(i);
		row.displaying = false;
	}
	for (int i = lastRow + 1, l = rowInfos.size(); i < l; i++)
	{
		RowInfo & row = rowInfos[i];
		if (row.displaying)
			hideRow(i);
		row.displaying = false;
	}
	for (int i = firstRow; i <= lastRow; i++)
	{
		RowInfo & row = rowInfos[i];
		if (row.displaying)
			continue;
		row.displaying = true;
		if (!row.aligned)
			alignRow(row);
		showRow(row, i);
	}
}

#if CONTENTWIDGET_VARIABLE_ROW_HEIGHT
template<bool scale>
#endif
void ContentWidget::alignRow(ContentWidget::RowInfo & row)
{
	int const n = row.items.size();
	double const width = double(visibleRect.width() - xSpacing * (row.items.size() - 1));
	int sumWidth = 0;
	for (int i = 0; i < n; ++i)
		sumWidth += row.items[i].width;

	//TODO: is the same result possible using integers?
	double const r = double(width) / double(sumWidth);
	double x = 0;
	int ix = 0;
	for (int i = 0; i < n; ++i)
	{
		ItemInfo & item = row.items[i];
		double w = double(row.items[i].width) * r;
		item.x = ix;
		item.width = qRound(w + (x - ix));
		ix += item.width + xSpacing;
		x += w + xSpacing;
	}
#if CONTENTWIDGET_VARIABLE_ROW_HEIGHT
	if (scale)
		row.height = qRound(row.height * r);
#endif
	Q_ASSERT( (ix - xSpacing) == visibleRect.width() );
	row.aligned = true;
}

void ContentWidget::hideRow(int i)
{
	if (rowWidgets[i] != 0)
	{
		for (ItemInfo const & info : rowInfos[i].items)
			itemWidgets[info.index] = 0;
		delete rowWidgets[i];
		rowWidgets[i] = 0;
	}
}

void ContentWidget::showRow(const ContentWidget::RowInfo & rowInfo, int rowIndex)
{
	QWidget * rowWidget = rowWidgets[rowIndex];
	bool updateOnly = true;
	if (rowWidget == 0)
	{
		updateOnly = false;
		rowWidget = new QWidget(this);
		rowWidgets[rowIndex] = rowWidget;
	}
#if CONTENTWIDGET_VARIABLE_ROW_HEIGHT
	int const & rowH = rowInfo.height;
#else
	int const & rowH = rowHeight;
#endif
	rowWidget->setGeometry(0, rowInfo.y, visibleRect.width(), rowH);

	for (int i = 0; i < rowInfo.items.size(); ++i)
	{
		ItemInfo const & item = rowInfo.items[i];
		QWidget * itemWidget;
		if (updateOnly)
		{
			itemWidget = static_cast<QWidget *>(rowWidget->children().at(i));
		}
		else
		{
			if (itemWidgets[item.index] != 0)
			{
				itemWidget = itemWidgets[item.index];
			}
			else
			{
				itemWidget = createItemWidget(*item.item, item.index);
				itemWidgets[item.index] = itemWidget;
#if CONTENTWIDGET_DEBUG_VISUALIZE_TRACKINGITEM
				if (item.index == trackingItem.index)
					itemWidget->setStyleSheet("QWidget{ background: blue; }");
#endif
			}
			itemWidget->setParent(rowWidget);
			itemWidget->setVisible(true);
		}
		itemWidget->setGeometry(item.x, 0, item.width, rowH);
	}
	rowWidget->setVisible(true);
}

void ContentWidget::updateSelection(int itemIndex, bool dragging, bool controlPressed, bool shiftPressed)
{
	if (selectionMode == QAbstractItemView::NoSelection)
		return;
	QList<int> newSelection;
	int newCurrentItemIndex = currentItemIndex;

	switch (selectionMode)
	{
		case QAbstractItemView::NoSelection:
			break;
		case QAbstractItemView::SingleSelection:
			newSelection.append(itemIndex);
			newCurrentItemIndex = itemIndex;
			break;
		case QAbstractItemView::ExtendedSelection:
		{
			if (!controlPressed)
			{
				if (dragging || shiftPressed)
				{
					Q_ASSERT(newCurrentItemIndex != -1);
					Q_ASSERT(!selection.isEmpty());
					if (itemIndex > newCurrentItemIndex)
					{
						for (int i = newCurrentItemIndex; i <= itemIndex; ++i)
							newSelection.append(i);
					}
					else
					{
						for (int i = newCurrentItemIndex; i >= itemIndex; --i)
							newSelection.append(i);
					}
				}
				else
				{
					newSelection.append(itemIndex);
					newCurrentItemIndex = itemIndex;
				}
				break;
			}
			FALLTHROUGH
		}
		case QAbstractItemView::MultiSelection:
			newCurrentItemIndex = itemIndex;
			newSelection = selection;
			if (dragging)
			{
				Q_ASSERT(dragStartItemIndex != -1);
				newSelection = dragStartSelection;
				bool select = newSelection.contains(dragStartItemIndex);
				for (int i = qMin(dragStartItemIndex, itemIndex); i <= qMax(dragStartItemIndex, itemIndex); ++i)
				{
					if (!select)
						newSelection.removeOne(i);
					else if (!newSelection.contains(i))
						newSelection.append(i);
				}
			}
			else
			{
				if (newSelection.contains(itemIndex))
					newSelection.removeOne(itemIndex);
				else
					newSelection.append(itemIndex);
				dragStartSelection = newSelection;
				dragStartItemIndex = itemIndex;
			}
			newCurrentItemIndex = itemIndex;
			break;
		case QAbstractItemView::ContiguousSelection:
			if (dragging || controlPressed || shiftPressed)
			{
				Q_ASSERT(newCurrentItemIndex != -1);
				Q_ASSERT(!selection.isEmpty());
				Q_ASSERT(selection.first() == newCurrentItemIndex);
				if (itemIndex > newCurrentItemIndex)
				{
					for (int i = newCurrentItemIndex; i <= itemIndex; ++i)
						newSelection.append(i);
				}
				else
				{
					for (int i = newCurrentItemIndex; i >= itemIndex; --i)
						newSelection.append(i);
				}
			}
			else
			{
				newCurrentItemIndex = itemIndex;
				newSelection.append(itemIndex);
			}
			break;
	}

	setSelectedItems(newSelection);
	setCurrentItem(newCurrentItemIndex);
}

void ContentWidget::showNavigator(const int row, const int col, const bool blockUpdates)
{
	if (navigatorVisible && navigatorRow == row && navigatorColumn == col)
		return;

	if (blockUpdates)
		setUpdatesEnabled(false);

	bool needsTrackingUpdate = itemTrackingEnabled && !navigatorVisible;
	if (navigatorVisible)
	{
		for (int i = navigatorRow+1; i <= row; ++i)
		{
			rowInfos[i].y -= navigatorHeight + ySpacing;
			if (rowWidgets[i] != 0)
				rowWidgets[i]->move(0, rowInfos[i].y);
		}
		for (int i = row+1; i <= navigatorRow; ++i)
		{
			rowInfos[i].y += navigatorHeight + ySpacing;
			if (rowWidgets[i] != 0)
				rowWidgets[i]->move(0, rowInfos[i].y);
		}
	}
	else
	{
		for (int i = row+1; i < rowInfos.size(); ++i)
		{
			rowInfos[i].y += navigatorHeight + ySpacing;
			if (rowWidgets[i] != 0)
				rowWidgets[i]->move(0, rowInfos[i].y);
		}

		size.setHeight(size.height() + navigatorHeight + ySpacing);
		setMinimumSize(size);
	}

	navigatorVisible = true;
	navigatorImg = rowInfos[row].items[col].item;
	navigator->setItemInfo(*rowInfos[row].items[col].item);
	updateNavigator(row, col);
	navigator->setVisible(true);

	if (needsTrackingUpdate)
		updateTrackingItem();

	if (blockUpdates)
		setUpdatesEnabled(true);
}

void ContentWidget::updateNavigator(const int row, const int col)
{
	navigatorRow = row;
	navigatorColumn = col;

	RowInfo const & rowInfo = rowInfos.at(row);
#if CONTENTWIDGET_VARIABLE_ROW_HEIGHT
	int const & rowH = rowInfo.height;
#else
	int const & rowH = rowHeight;
#endif
	int const y = rowInfo.y + rowH + ySpacing;
	navigator->setGeometry(0, y, visibleRect.width(), navigatorHeight);
}

void ContentWidget::navigatorPrevNext(bool next)
{
	int row = navigatorRow;
	int col = navigatorColumn;
	if (next)
		nextImage(row, col);
	else
		previousImage(row, col);
	if (row < 0 || col < 0)
		return;

	int oldNaviY = navigator->y();
	showNavigator(row, col);
	if (navigator->y() != oldNaviY)
	{
		blockScroll = true;
		emit scrollToRequest( visibleRect.top() + (navigator->y() - oldNaviY) );
		blockScroll = false;
	}
}

void ContentWidget::hideNavigator()
{
	if (!navigatorVisible)
		return;

	setUpdatesEnabled(false);

	for (int i = navigatorRow+1; i < rowInfos.size(); ++i)
	{
		rowInfos[i].y -= navigatorHeight + ySpacing;
		if (rowWidgets[i] != 0)
			rowWidgets[i]->move(0, rowInfos[i].y);
	}

	navigatorVisible = false;
	navigator->setVisible(false);

	size.setHeight(size.height() - (navigatorHeight + ySpacing));
	setMinimumSize(size);

	setUpdatesEnabled(true);
}

void ContentWidget::navigatorNext()
{
	navigatorPrevNext(true);
}

void ContentWidget::navigatorPrev()
{
	navigatorPrevNext(false);
}

void ContentWidget::updateTrackingItem()
{
#if CONTENTWIDGET_DEBUG_VISUALIZE_TRACKINGITEM
	QWidget * trackingWidget = itemWidgets.value(trackingItem.index);
	if (trackingWidget != 0)
		trackingWidget->setStyleSheet(QString());
#endif

	if (rowInfos.isEmpty())
	{
		trackingItemIndex = -1;
		trackingItemOffset = 0;
	}
	else
	{
		updateTrackingPoint();
		RowInfo const & row = rowInfos.at(rowAt(trackingPoint.y()));
		int colIndex = colAt(trackingPoint.x(), row);
		trackingItemIndex = row.items.at(colIndex).index;

		trackingItemOffset = trackingPoint.y() - row.y;
	}

#if CONTENTWIDGET_DEBUG_VISUALIZE_TRACKINGITEM
	trackingWidget = itemWidgets.value(trackingItem.index);
	if (trackingWidget != 0)
		trackingWidget->setStyleSheet("QWidget{ background: yellow; }");
#endif
}

void ContentWidget::updateTrackingPoint()
{
	trackingPoint.setX(visibleRect.x() + (visibleRect.width() * itemTrackingX * 100 / (100 * 100)));
	trackingPoint.setY(visibleRect.y() + (visibleRect.height() * itemTrackingY * 100 / (100 * 100)));
#if CONTENTWIDGET_DEBUG_VISUALIZE_TRACKINGPOINT
	static QFrame * w = 0;
	delete w;
	w = new QFrame(this);
	w->setStyleSheet("QFrame { background: red; }");
	w->setGeometry(trackingPoint.x() - 5, trackingPoint.y() - 5, 10, 10);
	w->setVisible(true);
#endif
}


bool operator==(const ContentWidget::ItemInfo & lhs, const ContentWidget::ItemInfo & rhs)
{
	return lhs.index == rhs.index;	//Probably not technically correct, but enugh for now.
}

bool operator==(const ContentWidget::RowInfo & lhs, const ContentWidget::RowInfo & rhs)
{
	return lhs.y == rhs.y && lhs.items == rhs.items;
}

//TODO: Remove link in overview of scrollToRequest somehow.
/*!
 * \fn ContentWidget::scrollToRequest
 * \brief This signal is emitted when the widget wants to scroll somewhere.
 * This happens when the navigator switches a line to keep it steady on the screen,
 * or when itemTrackingEnabled is enabled and the widget is resized and tries to keep one item at one y-coordinate.
 * Can be connected to the setValue() signal of the vertical scroll bar of the containing scroll area.
 * \param y The y-coordiate the widget wants to be visible on the top edge.
 */

/*!
 * \fn ContentWidget::itemPressed
 * \brief This signal is emitted whenever an item is pressed.
 * The row, column and item inex specify is the item that was pressed.
 * Use the QApplication::mouseButtons() function to get the state of the mouse buttons.
 * \param row
 * \param col
 * \param itemIndex
 *
 * \see itemReleased
 * \see itemClicked
 * \see itemDoubleClicked
 * \see QGuiApplication::mouseButtons()
 */

/*!
 * \fn ContentWidget::itemReleased
 * \brief This signal is emitted whenever an item is released.
 * The row, column and item inex specify is the item that was released.
 * Use the QApplication::mouseButtons() function to get the state of the mouse buttons.
 * \param row
 * \param col
 * \param itemIndex
 * 
 * \see itemPressed
 * \see itemClicked
 * \see itemDoubleClicked
 * \see QGuiApplication::mouseButtons()
 */

/*!
 * \fn ContentWidget::itemClicked
 * \brief This signal is emitted whenever an item is left-clicked.
 * The row, column and item inex specify is the item that was clicked.
 * \param row
 * \param col
 * \param itemIndex
 * 
 * \see itemPressed
 * \see itemReleased
 * \see itemDoubleClicked
 */

/*!
 * \fn ContentWidget::itemDoubleClicked
 * \brief This signal is emitted whenever an item is double clicked.
 * The row, column and item inex specify is the item that was double clicked.
 * \param row
 * \param col
 * \param itemIndex
 *
 * \see itemPressed
 * \see itemReleased
 * \see itemClicked
 */

/*!
 * \fn ContentWidget::selectionChanged
 * \brief This signal is emitted whenever the selection changes.
 * \param selection New selection.
 * \param oldSelection Old selection.
 */

/*!
 * \fn ContentWidget::currentItemChanged
 * \brief This signal is emitted whenever the current item changes.
 * The previous model item index is replaced by the current index as the selection's current item.
 * \param itemIndex Current item index.
 * \param oldItemIndex Old current item index.
 */

LONGSCROLLQT_NAMESPACE_END
