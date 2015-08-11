#include "notifyingscrollarea.h"
#include "notifyablescrollcontentwidget.h"
#include <QScrollBar>

void NotifyingScrollArea::scrollBy(int dx, int dy)
{
	programmaticScroll = true;
	if (dx != 0)
		horizontalScrollBar()->setValue(horizontalScrollBar()->value() + dx);
	if (dy != 0)
		verticalScrollBar()->setValue(verticalScrollBar()->value() + dy);
	programmaticScroll = false;
}

void NotifyingScrollArea::scrollTo(int x, int y)
{
	programmaticScroll = true;
	horizontalScrollBar()->setValue(x);
	verticalScrollBar()->setValue(y);
	programmaticScroll = false;
}

void NotifyingScrollArea::scrollContentsBy(int dx, int dy)
{
	QScrollArea::scrollContentsBy(dx, dy);
	findVisible();
	if (!programmaticScroll)
		emit userScrolled();
}

void NotifyingScrollArea::resizeEvent(QResizeEvent * event)
{
	QScrollArea::resizeEvent(event);
	findVisible();
}

void NotifyingScrollArea::showEvent(QShowEvent * event)
{
	QScrollArea::showEvent(event);
	findVisible();
}

void NotifyingScrollArea::hideEvent(QHideEvent * event)
{
	QScrollArea::hideEvent(event);
	findVisible();
}

void NotifyingScrollArea::findVisible()
{
	QWidget * wid = widget();
	if (wid == 0)
		return;
	QRegion const & region = wid->visibleRegion();
	QList<NotifyableScrollContentWidget *> const & widgets = wid->findChildren<NotifyableScrollContentWidget *>(QString(), Qt::FindDirectChildrenOnly);
	for (NotifyableScrollContentWidget * it : widgets)
	{
		bool show = region.contains(it->geometry());
		it->showing( show );
		if (show && it->usesShowingRect())
		{
			QRect && r = region.boundingRect().intersected(it->geometry());
			r.setTopLeft( it->mapFromParent(r.topLeft()) );
			it->showingRect(r);
		}
	}
}
