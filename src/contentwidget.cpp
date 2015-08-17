#include "contentwidget.h"
#include "navigatorwidget.h"
#include "contentiteminfo.h"
#include "contentwidgetitemfactory.h"
#include <QMouseEvent>

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

// This file was written the quick way, and could surely be opitmized to perform faster. However it does not seem to be neccessary.

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
			for (int r = 0, l = rowInfos.length(); r < l; ++r)
			{
				if (rowInfos[r].items.last().index >= trackingItem.index)
				{
					blockScroll = true;
					emit scrollToRequest(0, rowInfos[r].y + trackingItemOffset - (trackingPoint.y() - rect.y()));
					blockScroll = false;
					break;
				}
			}
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
#if CONTENTWIDGET_LAZY_ALIGN
		if (!row.aligned)
			alignRow(row);
#endif
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
	rowWidget->setGeometry(0, rowInfo.y, visibleRect.width(), rowHeight);

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
				itemWidget = createItemWidget(*item.img, item.width, rowHeight);
				itemWidgets[item.index] = itemWidget;
#if CONTENTWIDGET_DEBUG_VISUALIZE_TRACKINGITEM
				if (item.index == trackingItem.index)
					itemWidget->setStyleSheet("QWidget{ background: blue; }");
#endif
			}
		}
		itemWidget->setParent(rowWidget);
		itemWidget->setGeometry(item.x, 0, item.width, rowHeight);
	}
	rowWidget->setVisible(true);
}

QWidget *ContentWidget::createItemWidget(const ContentItemInfo & info, int width, int height)
{
	QWidget * widget = itemFactory->createItemWidget(info, width, height);
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
	imageWidths.clear();
	imageWidths.reserve(itemInfos.size());
	qDeleteAll(itemWidgets);
	itemWidgets.clear();
	itemWidgets.reserve(itemInfos.size());
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
		for (int i = 0; i < itemInfos.size(); ++i)
		{
			imageWidths.append( std::numeric_limits<decltype(itemWidth)>::max() / 4 );	// big value, but also leave some room for calculations.
			itemWidgets.append(0);
		}
	}
	else
	{
		for (int i = 0; i < itemInfos.size(); ++i)
		{
			imageWidths.append(itemWidth);
			itemWidgets.append(0);
		}
	}

#if CONTENTWIDGET_MEASURE_SETINFOS
	int elapsed = t.elapsed();
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
		item.img = &itemInfos[i];
		item.width = width;

		if (navigatorVisible && navigatorImg == item.img)
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
#if CONTENTWIDGET_LAZY_ALIGN
				row.aligned = !align;
#else
				if (align)
					alignRow(row);
#endif
				y += rowHeight + ySpacing;
				rowInfosNew.append(row);
				row = RowInfo();
				row.y = y;
				rowWidth = 0;
			}
			else	//underfill
			{
#if CONTENTWIDGET_LAZY_ALIGN
				row.aligned = !align;
#else
				if (align)
					alignRow(row);
#endif
				y += rowHeight + ySpacing;
				rowInfosNew.append(row);
				row = RowInfo();
				row.y = y;

				item.x = 0;
				row.items.append(item);
				rowWidth = width;

				if (navigatorVisible && navigatorImg == item.img)
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
#if CONTENTWIDGET_LAZY_ALIGN
		row.aligned = !alignLast;
#else
		if (alignLast)
			alignRow(row);
#endif
		rowInfosNew.append(row);
		size.setHeight(y + rowHeight);
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
				if (items1[j].img != items2[j].img)
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
	double x = 0;
	int ix = 0;
	for (int i = 0; i < n; ++i)
	{
		ItemInfo & item = row.items[i];
		double w = double(row.items[i].width) * width / double(sumWidth);
		item.x = ix;
		item.width = qRound(w + (x - ix));
		ix += item.width + xSpacing;
		x += w + xSpacing;
	}
	Q_ASSERT( (ix - xSpacing) == visibleRect.width() );
#if CONTENTWIDGET_LAZY_ALIGN
	row.aligned = true;
#endif
}

int ContentWidget::rowAt(int y, bool * onNavigator)
{
	if (onNavigator != 0)
		*onNavigator = false;
	int row = y / (rowHeight + ySpacing);
	if (navigatorVisible && row > navigatorRow)
	{
		row = (y - navigatorHeight) / (rowHeight + ySpacing);
		if (row <= navigatorRow)
		{
			if (onNavigator != 0)
				*onNavigator = true;
			row = navigatorRow;
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
	if (event->buttons() == Qt::LeftButton)
	{
		bool onNavigator;
		int row = rowAt(event->y(), &onNavigator);
		if (onNavigator)
		{
//			qDebug() << "navigator pressed";
			event->ignore();
			return;
		}

		int col = colAt(event->x(), row);
//		qDebug() << "mouse press" << row << col;

		showNavigator(row, col);

		event->accept();
	}
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
	navigatorImg = rowInfos[row].items[col].img;
	navigator->setItemInfo(*rowInfos[row].items[col].img);
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

	navigator->setGeometry(0, (navigatorRow + 1) * (rowHeight + ySpacing), visibleRect.width(), navigatorHeight);
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


bool operator==(const ContentWidget::ItemInfo & lhs, const ContentWidget::ItemInfo & rhs)
{
	return lhs.index == rhs.index;	//Probably not technically correct, but enugh for now.
}

bool operator==(const ContentWidget::RowInfo & lhs, const ContentWidget::RowInfo & rhs)
{
	return lhs.y == rhs.y && lhs.items == rhs.items;
}
