#ifndef NOTIFYABLESCROLLCONTENT_H
#define NOTIFYABLESCROLLCONTENT_H

#include "longscroll-qt_global.h"
#include <QFrame>

class LONGSCROLLQT_EXPORT NotifyableScrollContentWidget : public QFrame
{
public:
	using QFrame::QFrame;
	virtual void showingRect(QRect const & rect) = 0;
};

#endif // NOTIFYABLESCROLLCONTENT_H
