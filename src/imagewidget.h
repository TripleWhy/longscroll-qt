#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QFrame>
class QLabel;

class ImageWidget : public QFrame
{
	Q_OBJECT

private:
	QLabel * imgLabel;
	QSize pxSize;
	double pxr = qQNaN();
	bool fit = true;

public:
	explicit ImageWidget(QWidget *parent = 0);
	~ImageWidget();

protected:
	virtual void resizeEvent(QResizeEvent * event) override;
	virtual bool event(QEvent * event) override;
	void relayout(QSize const & size);

	virtual bool hasHeightForWidth() const override;
	virtual int heightForWidth(int w) const override;
	virtual QSize sizeHint() const override;

public slots:
	void setPixmap(QPixmap const & px);
	void setFit(bool fit);
};

#endif // IMAGEWIDGET_H
