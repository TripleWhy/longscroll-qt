#ifndef LONGSCROLLWIDGET_H
#define LONGSCROLLWIDGET_H

#include "notifyingscrollarea.h"
#include "contentwidget.h"

LONGSCROLLQT_NAMESPACE_BEGIN

class LONGSCROLLQT_EXPORT LongscrollWidget : public NotifyingScrollArea
{
public:
	explicit LongscrollWidget(QWidget * parent = 0);
	ContentWidget const * getContentWidget() const;
	ContentWidget * getContentWidget();

private:
	ContentWidget * cw = 0;
};

LONGSCROLLQT_NAMESPACE_END

#endif // LONGSCROLLWIDGET_H
