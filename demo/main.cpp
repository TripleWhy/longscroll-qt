#include "mainwindow.h"
#include <QDebug>
#include <QApplication>
#include <QFile>
#include <QDir>
#include <QImageReader>

/*
 * Run this program with one or more directories that contain image files as arguments.
 * Add another integer paramter at the end to enable different demo modes.
 *
 * Change the preprocesser #if at the bottom if you want to test an arbitrary amount of images.
 */
int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	MainWindow w;
	if (app.arguments().size() > 2)
	{
		int demoMode;
		bool ok;
		demoMode = app.arguments().at(2).toInt(&ok);
		if (ok)
			w.setDemoNumber(demoMode);
	}
	if (app.arguments().size() > 1)
		w.loadDir(QDir(app.arguments().at(1)), true);

	w.show();

	return app.exec();
}
