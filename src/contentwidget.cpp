#include "contentwidget.h"
#include "navigatorwidget.h"
#include "contentiteminfo.h"
#include "contentwidgetitemfactory.h"
#include <QMouseEvent>
#include <QApplication>

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

LONGSCROLLQT_NAMESPACE_BEGIN

ContentWidget::ContentWidget(QWidget * parent)
	: NotifyableScrollContentWidget(parent)
{
	itemFactory = new ContentWidgetImageItemFactory(false, this);

	setNavigatorWidget(new ImageNavigatorWidget(this));
	navigator->setVisible(false);

#if CONTENTWIDGET_DEMO_STYLESHEETS
	setStyleSheet("QWidget { background: green; } ImageItem { background: blue; } QLabel { background: red; }");
	navigator->setStyleSheet("QWidget { background: darkgray; }");
#endif

	connect(this, SIGNAL(itemPressed(int,int,int,Qt::MouseButtons)), this, SLOT(showNavigator(int,int)));
}

ContentWidget::ContentWidget(int _rowHeight, int _itemWidth, bool alignRows, bool alignLastRow, bool _allowOverfill, int _navigatorHeight, QWidget * parent)
	: ContentWidget(parent)
{
	rowHeight = _rowHeight;
	itemWidth = _itemWidth;
	align = alignRows;
	alignLast = alignLastRow;
	allowOverfill = _allowOverfill;
	navigatorHeight = _navigatorHeight;
}

ContentWidget::~ContentWidget()
{
}

void ContentWidget::setItemFactory(ContentWidgetItemFactory * factory)
{
	delete itemFactory;
	factory->setParent(this);
	itemFactory = factory;
}

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

void ContentWidget::setItemTrackingEnabled(bool enabled)
{
	if (enabled == itemTrackingEnabled)
		return;
	itemTrackingEnabled = enabled;
	trackingItem = ItemInfo();
}

void ContentWidget::setItemTrackingScreenPositionPercentage(uchar percentX, uchar percentY)
{
	percentX = qBound(uchar(0), percentX, uchar(100));
	percentY = qBound(uchar(0), percentY, uchar(100));
	if (percentX == itemTrackingX && percentY == itemTrackingY)
		return;
	itemTrackingX = percentX;
	itemTrackingY = percentY;
	trackingItem = ItemInfo();
}

QSize ContentWidget::sizeHint() const
{
	return size;
}

void ContentWidget::showingRect(const QRect & rect)
{
#if CONTENTWIDGET_MEASURE_SHOWINGRECT
	static quint64 tSum = 0;
	static quint64 tCount = 0;
	QElapsedTimer t;
	t.start();
#endif

	bool needsUpdate = itemTrackingEnabled && !blockScroll && rect.size() == visibleRect.size() && rect.topLeft() != visibleRect.topLeft();

	setUpdatesEnabled(false);

	if (rect.width() != visibleRect.width())
	{
		if (itemTrackingEnabled)
		{
			if (trackingItem.index < 0)
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
		if (changed && trackingItem.index >= 0)
		{
			int const r = findRow(trackingItem.index);
			blockScroll = true;
			emit scrollToRequest(0, rowInfos[r].y + trackingItemOffset - (trackingPoint.y() - rect.y()));
			blockScroll = false;
		}
	}
	else
		visibleRect = rect;

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

void ContentWidget::updateRows()
{
	int firstRow = qMax(0, rowAt(visibleRect.y())  -  prefetchBefore);
	int bottom = visibleRect.bottom();
	if (navigatorVisible)
		bottom += navigatorHeight;
	int lastRow = qMin(rowInfos.length() - 1, rowAt(bottom)  +  prefetchAfter);

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
				itemWidget->setVisible(true);
			}
			else
			{
				itemWidget = createItemWidget(*item.item, item.index, item.width, rowH);
				itemWidgets[item.index] = itemWidget;
#if CONTENTWIDGET_DEBUG_VISUALIZE_TRACKINGITEM
				if (item.index == trackingItem.index)
					itemWidget->setStyleSheet("QWidget{ background: blue; }");
#endif
			}
		}
		itemWidget->setParent(rowWidget);
		itemWidget->setGeometry(item.x, 0, item.width, rowH);
	}
	rowWidget->setVisible(true);
}

QWidget *ContentWidget::createItemWidget(const ContentItemInfo & info, int itemIndex,int width, int height)
{
	QWidget * widget = itemFactory->createItemWidget(info, itemIndex, width, height, this);
#if CONTENTWIDGET_DEMO_STYLESHEETS
	widget->setStyleSheet("QWidget{ background: blue; } QLabel { background: red; }");
#endif
	return widget;
}

void ContentWidget::updateTrackingItem()
{
#if CONTENTWIDGET_DEBUG_VISUALIZE_TRACKINGITEM
	QWidget * trackingWidget = itemWidgets.value(trackingItem.index);
	if (trackingWidget != 0)
		trackingWidget->setStyleSheet(QString());
#endif

	if (rowInfos.isEmpty() > 0)
	{
		trackingItem = ItemInfo();
		trackingItemOffset = 0;
	}
	else
	{
		updateTrackingPoint();
		RowInfo const & row = rowInfos.at(rowAt(trackingPoint.y()));
		int colIndex = colAt(trackingPoint.x(), row);
		trackingItem = row.items.at(colIndex);

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

void ContentWidget::setItemInfos(const QList<ContentItemInfo> & infos)
{
#if CONTENTWIDGET_MEASURE_SETINFOS
	QElapsedTimer t;
	t.start();
#endif

	itemInfos = infos;
	int const size = itemInfos.size();
	imageWidths.clear();
	imageWidths.reserve(size);
	qDeleteAll(itemWidgets);
	itemWidgets.clear();
	itemWidgets.reserve(size);
	if (itemWidth == 0)
	{
		for (ContentItemInfo const & img : infos)
		{
			imageWidths.append(img.widthForHeight(rowHeight));
			itemWidgets.append(0);
		}
	}
	else if (itemWidth < 0)
	{
		for (int i = 0; i < size; ++i)
		{
			imageWidths.append( std::numeric_limits<decltype(itemWidth)>::max() / 4 );	// big value, but also leave some room for calculations.
			itemWidgets.append(0);
		}
	}
	else
	{
		for (int i = 0; i < size; ++i)
		{
			imageWidths.append(itemWidth);
			itemWidgets.append(0);
		}
	}

#if CONTENTWIDGET_MEASURE_SETINFOS
	qint64 elapsed = t.elapsed();
	qDebug() << "ContentWidget::setItemInfos took" << elapsed << "ms";
#endif
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
		int width = imageWidths[i];

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
					scaleRow(row);
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
					scaleRow(row);
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
			scaleRow(row);
		size.setHeight(y + row.height);
#else
		size.setHeight(y + rowHeight);
#endif
		rowInfosNew.append(row);
	}
	else
	{
		size.setHeight(y - ySpacing);
	}
	setMinimumSize(size);


	int const is = rowInfosNew.size();
	int const ws = rowWidgets.size();
	for (int i = 0, l = qMin(ws, rowInfosNew.size()); i < l; ++i)
	{
		bool equal = true;
		QList<ItemInfo> const & items1 = rowInfos[i].items;
		QList<ItemInfo> const & items2 = rowInfosNew[i].items;
		if (items1.size() != items2.size())
			equal = false;
		else
		{
			for (int j = 0, m = items1.size(); j < m; ++j)
			{
				if (items1[j].item != items2[j].item)
					equal = false;
			}
		}
		if (!equal)
		{
			if (rowWidgets[i] != 0)
			{
				QObjectList children = rowWidgets[i]->children();	//copy list
				for (QObject * o : children)
				{
					static_cast<QWidget *>(o)->setVisible(false);
					o->setParent(this);
				}
				delete rowWidgets[i];
				rowWidgets[i] = 0;
			}
		}
	}
	for (int i = is; i < ws; ++i)
	{
		QWidget * last = rowWidgets.takeLast();
		if (last == 0)
			continue;
		QObjectList children = last->children();	//copy list
		for (QObject * o : children)
		{
			static_cast<QWidget *>(o)->setVisible(false);
			o->setParent(this);
		}
		delete last;
	}
	rowWidgets.reserve(is);
	for (int i = ws; i < is; ++i)
		rowWidgets.append(0);
	Q_ASSERT(rowWidgets.size() == rowInfosNew.size());
	rowInfos.swap(rowInfosNew);

	if (navigatorVisible)
		updateNavigator(navRow, navCol);

	if (calculateChanges)
		return rowInfos != rowInfosNew;
	else
		return false;
}

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
	Q_ASSERT( (ix - xSpacing) == visibleRect.width() );
	row.aligned = true;
}

#if CONTENTWIDGET_VARIABLE_ROW_HEIGHT
void ContentWidget::scaleRow(ContentWidget::RowInfo & row)
{
	int const n = row.items.size();
	double const width = double(visibleRect.width() - xSpacing * (row.items.size() - 1));
	int sumWidth = 0;
	for (int i = 0; i < n; ++i)
		sumWidth += row.items[i].width;

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
	row.height = qRound(row.height * r);
	Q_ASSERT( (ix - xSpacing) == visibleRect.width() );
	row.aligned = true;
}
#endif

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

int ContentWidget::colAt(int x, int row)
{
	return colAt(x, rowInfos.at(row));
}

int ContentWidget::colAt(int x, const ContentWidget::RowInfo & row)
{
	for (int i = row.items.length() - 1; i >= 0; --i)
		if (row.items.at(i).x <= x)
			return i;
	return 0;
}

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

void ContentWidget::mousePressEvent(QMouseEvent * event)
{
	if (!handleMouseEvents)
	{
		event->ignore();
		return;
	}
	event->accept();
	mouseState = 0;
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

	emit itemPressed(row, col, item->index, event->buttons());
}

void ContentWidget::mouseMoveEvent(QMouseEvent * event)
{
	if (!handleMouseEvents)
	{
		event->ignore();
		return;
	}
	event->accept();
	if (event->buttons() == Qt::LeftButton)
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

void ContentWidget::mouseReleaseEvent(QMouseEvent * event)
{
	if (!handleMouseEvents)
	{
		event->ignore();
		return;
	}

	event->accept();

	bool onNavigator;
	int row = rowAt(event->y(), &onNavigator);
	if (onNavigator)
	{
		mouseState = -1;
		return;
	}
	int col = colAt(event->x(), row);
	ItemInfo * item = &rowInfos[row].items[col];
	emit itemReleased(row, col, item->index, event->buttons());

	if (event->buttons() == Qt::NoButton && mouseState == 1 &&
	        row == mousePressRow && col == mousePressCol && item == mousePressItem)
		emit itemClicked(row, col, item->index);
}

void ContentWidget::mouseDoubleClickEvent(QMouseEvent * event)
{
	event->accept();
	mouseState = -2;
	bool onNavigator;
	int row = rowAt(event->y(), &onNavigator);
	if (onNavigator)
		return;
	int col = colAt(event->x(), row);
	int itemIndex = rowInfos[row].items[col].index;
	emit itemDoubleClicked(row, col, itemIndex);
}

void ContentWidget::startDrag(int /*row*/, int /*col*/, int /*itemIndex*/)
{
}

void ContentWidget::showNavigator(int itemIndex)
{
	int row, col;
	findRowCol(row, col, itemIndex);
	showNavigator(row, col);
}

void ContentWidget::showNavigator(const int row, const int col)
{
	showNavigator(row, col, true);
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
		emit scrollRequest( 0, navigator->y() - oldNaviY );
		blockScroll = false;
	}
}

int ContentWidget::findRow(int itemIndex)
{
	int const row = std::lower_bound(rowInfos.begin(), rowInfos.end(), itemIndex, [](RowInfo const & ri, int index){ return ri.items.last().index < index; }) - rowInfos.begin();
	Q_ASSERT(rowInfos[row].items.first().index <= itemIndex);
	Q_ASSERT(rowInfos[row].items.last().index >= itemIndex);
	return row;
}

void ContentWidget::findRowCol(int & row, int & col, int itemIndex)
{
	row = findRow(itemIndex);
	col = itemIndex - rowInfos[row].items.first().index;
	Q_ASSERT(rowInfos[row].items[col].index == itemIndex);
}

void ContentWidget::updateSelection(int itemIndex, bool dragging, bool controlPressed, bool shiftPressed)
{
	if (selectionMode == QAbstractItemView::NoSelection)
		return;
	QList<int> oldSelection;
	int oldCurrentItemIndex = currentItemIndex;
	selection.swap(oldSelection);
	switch (selectionMode)
	{
		case QAbstractItemView::NoSelection:
			break;
		case QAbstractItemView::SingleSelection:
			selection.append(itemIndex);
			currentItemIndex = itemIndex;
			break;
		case QAbstractItemView::ExtendedSelection:
			if (!controlPressed)
			{
				if (dragging || shiftPressed)
				{
					Q_ASSERT(currentItemIndex != -1);
					Q_ASSERT(!oldSelection.isEmpty());
					if (itemIndex > currentItemIndex)
					{
						for (int i = currentItemIndex; i <= itemIndex; ++i)
							selection.append(i);
					}
					else
					{
						for (int i = currentItemIndex; i >= itemIndex; --i)
							selection.append(i);
					}
				}
				else
				{
					selection.append(itemIndex);
					currentItemIndex = itemIndex;
				}
				break;
			}
			//no break
		case QAbstractItemView::MultiSelection:
			currentItemIndex = itemIndex;
			selection = oldSelection;
			if (dragging)
			{
				Q_ASSERT(dragStartItemIndex != -1);
				selection = dragStartSelection;
				bool select = selection.contains(dragStartItemIndex);
				for (int i = qMin(dragStartItemIndex, itemIndex); i <= qMax(dragStartItemIndex, itemIndex); ++i)
				{
					if (!select)
						selection.removeOne(i);
					else if (!selection.contains(i))
						selection.append(i);
				}
			}
			else
			{
				if (selection.contains(itemIndex))
					selection.removeOne(itemIndex);
				else
					selection.append(itemIndex);
				dragStartSelection = selection;
				dragStartItemIndex = itemIndex;
			}
			currentItemIndex = itemIndex;
			break;
		case QAbstractItemView::ContiguousSelection:
			if (dragging || controlPressed || shiftPressed)
			{
				Q_ASSERT(currentItemIndex != -1);
				Q_ASSERT(!oldSelection.isEmpty());
				Q_ASSERT(oldSelection.first() == currentItemIndex);
				if (itemIndex > currentItemIndex)
				{
					for (int i = currentItemIndex; i <= itemIndex; ++i)
						selection.append(i);
				}
				else
				{
					for (int i = currentItemIndex; i >= itemIndex; --i)
						selection.append(i);
				}
			}
			else
			{
				currentItemIndex = itemIndex;
				selection.append(itemIndex);
			}
			break;
	}

	if (selection != oldSelection)
		emit selectionChanged(selection);
	if (currentItemIndex != oldCurrentItemIndex)
		emit currentItemChanged(currentItemIndex);
}


bool operator==(const ContentWidget::ItemInfo & lhs, const ContentWidget::ItemInfo & rhs)
{
	return lhs.index == rhs.index;	//Probably not technically correct, but enugh for now.
}

bool operator==(const ContentWidget::RowInfo & lhs, const ContentWidget::RowInfo & rhs)
{
	return lhs.y == rhs.y && lhs.items == rhs.items;
}

LONGSCROLLQT_NAMESPACE_END
