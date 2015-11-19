#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <longscroll-qt/contentiteminfo.h>

class QActionGroup;
class QFileDialog;

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

	void setItemInfos(QList<ContentItemInfo> const & infos, int fixedSize = -1);
	void setFixedItemCount(int fixedSize);
	void setDemoNumber(int demoNo);
	void loadDir(QDir const & dir, bool useCache);

private slots:
	void updateFileMenu();
	void updatePropertyMenu();
	void propertyMenuTriggered(QAction * action);
	void demoMenuTriggered(QAction * action);
	void on_actionOpen_triggered();

	void on_actionCount_triggered();

private:
	Ui::MainWindow *ui;
	QActionGroup * demoGroup;
	QFileDialog * fileDialog = 0;
	QList<ContentItemInfo> itemInfos;
	int fixedInfoSize = 0;
};

#endif // MAINWINDOW_H
