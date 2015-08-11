#ifndef SCROLLSHWABLEWIDGET_H
#define SCROLLSHWABLEWIDGET_H

#include <QWidget>

class ScrollShwableWidget : public QWidget
{
	using QWidget::QWidget;
public:
	inline void showing(bool visible)
	{
		if (visible == m_showing)
			return;
		m_showing = visible;
		setShowing(visible);
	}

protected:
	virtual void setShowing(bool visible) = 0;

private:
	bool m_showing = false;
};

#endif // SCROLLSHWABLEWIDGET_H
