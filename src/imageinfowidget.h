#ifndef IMAGEINFOWIDGET_H
#define IMAGEINFOWIDGET_H

#include "longscroll-qt_global.h"
#include "contentiteminfo.h"
#include <QFrame>

namespace Ui {
class ImageInfoWidget;
}

class LONGSCROLLQT_EXPORT ImageInfoWidget : public QFrame
{
	Q_OBJECT

public:
	explicit ImageInfoWidget(QWidget *parent = 0);
	explicit ImageInfoWidget(ContentItemInfo const & info, QWidget *parent = 0);
	virtual ~ImageInfoWidget();

	void setItemInfo(ContentItemInfo const & info);

private:
	Ui::ImageInfoWidget *ui;
};

#endif // IMAGEINFOWIDGET_H
