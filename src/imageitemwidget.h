#ifndef IMAGEITEMWIDGET_H
#define IMAGEITEMWIDGET_H

#include "longscroll-qt_global.h"
#include "contentiteminfo.h"
#include "longscroll-qt_qtincludes.h"

LONGSCROLLQT_NAMESPACE_BEGIN

class ImageWidget;
class ContentWidget;

class LONGSCROLLQT_EXPORT ImageItemWidget : public QFrame
{
	Q_OBJECT
protected:
	ImageItemWidget(ContentItemInfo const & info, int itemIndex, bool fit, bool loadImage, ContentWidget * cw, QWidget * parent);
public:
	ImageItemWidget(ContentItemInfo const & info, int itemIndex, bool fit = false, ContentWidget * cw = 0, QWidget * parent = 0);
	~ImageItemWidget();

public slots:
	virtual void updateSelection(QList<int> const & selection);

protected:
	void loadImage();

protected:
	virtual void paintEvent(QPaintEvent * e) override;

protected:
	ImageWidget * imageWidget = 0;
	ContentItemInfo info;
	int itemIndex = -1;
	bool selected = false;
};

LONGSCROLLQT_NAMESPACE_END

#endif // IMAGEITEMWIDGET_H
