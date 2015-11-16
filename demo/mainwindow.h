#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <longscroll-qt/contentiteminfo.h>

class QActionGroup;

using namespace longscroll;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	void setItemInfos(QList<ContentItemInfo> const & infos);
	void setDemoNumber(int demoNo);

private slots:
	void updatePropertyMenu();
	void propertyMenuTriggered(QAction * action);
	void demoMenuTriggered(QAction * action);

private:
	Ui::MainWindow *ui;
	QActionGroup * demoGroup;
};

#endif // MAINWINDOW_H
