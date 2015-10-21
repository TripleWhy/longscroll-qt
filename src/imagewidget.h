#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include "longscroll-qt_global.h"
#include "longscroll-qt_qtincludes.h"
#include <QIcon>

class QLabel;

LONGSCROLLQT_NAMESPACE_BEGIN

class LONGSCROLLQT_EXPORT ImageWidget : public QFrame
{
	Q_OBJECT

private:
	QIcon icon;
	QSize pxSize;
	bool fit = true;
	bool selected = false;

public:
	explicit ImageWidget(QWidget *parent = 0);
	~ImageWidget();

protected:
	virtual void paintEvent(QPaintEvent * e) override;

	virtual bool hasHeightForWidth() const override;
	virtual int heightForWidth(int w) const override;
	virtual QSize sizeHint() const override;

public slots:
	void setPixmap(QPixmap const & px);
	void setFit(bool fit);
	void setSelected(bool selected);

};

LONGSCROLLQT_NAMESPACE_END

#endif // IMAGEWIDGET_H
