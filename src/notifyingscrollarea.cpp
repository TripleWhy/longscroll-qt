#include "notifyingscrollarea.h"
#include "notifyablescrollcontentwidget.h"
#include <QScrollBar>

LONGSCROLLQT_NAMESPACE_BEGIN

void NotifyingScrollArea::scrollContentsBy(int dx, int dy)
{
	QScrollArea::scrollContentsBy(dx, dy);
	findVisible();
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
	QList<NotifyableScrollContentWidget *> const & widgets = wid->findChildren<NotifyableScrollContentWidget *>(QString(), Qt::FindDirectChildrenOnly); //TODO: Can this be optimized, so that we don't have to call findChildren every time?
	for (NotifyableScrollContentWidget * it : widgets)
	{
		if (region.contains(it->geometry()))
		{
			QRect && r = region.boundingRect().intersected(it->geometry());
//			r.moveTopLeft( it->mapFromParent(r.topLeft()) );	// Apparently this line was unnecessary
			it->showingRect(r);
		}
	}
}

LONGSCROLLQT_NAMESPACE_END
