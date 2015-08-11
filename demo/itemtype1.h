#ifndef ITEMTYPE1_H
#define ITEMTYPE1_H

#include <QWidget>
#include <longscroll-qt/imginfo.h>

namespace Ui {
class ItemType1;
}

class ItemType1 : public QWidget
{
	Q_OBJECT

private:
	QString fileName;

public:
	explicit ItemType1(ImgInfo const & info, int height, QWidget *parent = 0);
	~ItemType1();

private:
	Ui::ItemType1 *ui;
};

#endif // ITEMTYPE1_H
