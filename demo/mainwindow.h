#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <longscroll-qt/imginfo.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(int demoNr = 0, QWidget *parent = 0);
	~MainWindow();

	void setImages(QList<ImgInfo> const & imgs);

private:
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
