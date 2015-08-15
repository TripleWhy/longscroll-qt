#ifndef NOTIFYABLESCROLLCONTENT_H
#define NOTIFYABLESCROLLCONTENT_H

#include <QFrame>

class NotifyableScrollContentWidget : public QFrame
{
public:
	using QFrame::QFrame;
	virtual void showingRect(QRect const & rect) = 0;
};

#endif // NOTIFYABLESCROLLCONTENT_H
