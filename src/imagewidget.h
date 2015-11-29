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

public:
	explicit ImageWidget(QWidget *parent = 0);
	~ImageWidget();

public:
	Q_PROPERTY(QPixmap pixmap READ getPixmap WRITE setPixmap)
	QPixmap const & getPixmap() const;
	Q_PROPERTY(bool fit READ getFit WRITE setFit)
	bool getFit() const;
	Q_PROPERTY(bool selected READ isSelected WRITE setSelected)
	bool isSelected() const;
public slots:
	void setPixmap(QPixmap const & px);
	void setFit(bool fit);
	virtual void setSelected(bool selected);

protected:
	virtual void paintEvent(QPaintEvent * e) override;

	virtual bool hasHeightForWidth() const override;
	virtual int heightForWidth(int w) const override;
	virtual QSize sizeHint() const override;

private:
	QIcon icon;
	QSize pxSize;
	bool fit = true;
	bool selected = false;
};

LONGSCROLLQT_NAMESPACE_END

#endif // IMAGEWIDGET_H
