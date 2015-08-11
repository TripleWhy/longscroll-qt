#ifndef SCROLLABLEWIDGET_H
#define SCROLLABLEWIDGET_H

#include <QFrame>

class NotifyableScrollContentWidget : public QFrame
{
	using QFrame::QFrame;
public:
	inline void showing(bool visible)
	{
		if (visible == m_showing)
			return;
		m_showing = visible;
		setShowing(visible);
	}
	virtual bool usesShowingRect() { return false; }
	virtual void showingRect(QRect const & rect) { Q_UNUSED(rect); }

protected:
	virtual void setShowing(bool visible) = 0;

private:
	bool m_showing = false;
};

#endif // SCROLLABLEWIDGET_H
