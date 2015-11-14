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

	QRect const & visible = wid->visibleRegion().boundingRect();

	NotifyableScrollContentWidget * nscw = qobject_cast<NotifyableScrollContentWidget *>(wid);
	if (nscw != 0)
	{
		nscw->showingRect(visible);
		return;
	}

	QList<NotifyableScrollContentWidget *> const & widgets = wid->findChildren<NotifyableScrollContentWidget *>(QString(), Qt::FindDirectChildrenOnly);
	for (NotifyableScrollContentWidget * w : widgets)
	{
		QRect && r = visible.intersected(w->geometry());
		if (!r.isEmpty())
			r.moveTopLeft( w->mapFromParent(r.topLeft()) );
		// r == w->visibleRegion().boundingRect(), thus w could determine r on its own, but this method is a little less computationally intensive.
		w->showingRect(r);
	}
}

LONGSCROLLQT_NAMESPACE_END
