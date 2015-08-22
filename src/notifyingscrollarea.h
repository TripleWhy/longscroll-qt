#ifndef NOTIFYINGSCROLLAREA_H
#define NOTIFYINGSCROLLAREA_H

#include "longscroll-qt_global.h"
#include <QScrollArea>

class LONGSCROLLQT_EXPORT NotifyingScrollArea : public QScrollArea
{
	Q_OBJECT
public:
	using QScrollArea::QScrollArea;

public slots:
	void scrollBy(int dx, int dy);
	void scrollTo(int x, int y);

protected:
	virtual void scrollContentsBy(int dx, int dy) override;
	virtual void resizeEvent(QResizeEvent * event) override;
	virtual void showEvent(QShowEvent * event) override;
	virtual void hideEvent(QHideEvent * event) override;
	void findVisible();
};

#endif // NOTIFYINGSCROLLAREA_H
