#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScrollArea>
#include <QEvent>
#include <QFileInfo>
#include <QDebug>
#include <QElapsedTimer>
#include <QScrollBar>
#include <QLabel>
#include <QVBoxLayout>
#include <QResizeEvent>
#include "itemtype1.h"
#include "imginfo.h"
#include "imagewidget.h"
#include "imageloader.h"
#include "imageitem.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

private:
	QList<ImageItem *> imgItems;
	QList<QHBoxLayout *> rows;
	QList<QWidget *> rowWidgets;

public:
	explicit MainWindow(int demoNr = 0, QWidget *parent = 0);
	~MainWindow();

	void setImages(QList<ImgInfo> const & imgs);

private:
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
