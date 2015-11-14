#ifndef NOTIFYABLESCROLLCONTENT_H
#define NOTIFYABLESCROLLCONTENT_H

#include "longscroll-qt_global.h"
#include "longscroll-qt_qtincludes.h"

LONGSCROLLQT_NAMESPACE_BEGIN

class LONGSCROLLQT_EXPORT NotifyableScrollContentWidget : public QFrame
{
	Q_OBJECT
public:
	using QFrame::QFrame;
	virtual void showingRect(QRect const & visible) = 0;
};

LONGSCROLLQT_NAMESPACE_END

#endif // NOTIFYABLESCROLLCONTENT_H
