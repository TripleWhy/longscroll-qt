#include "mainwindow.h"
#include <QDebug>
#include <QApplication>
#include <QFile>

static QString xmlAttributeData(QString const & name, QString const & line)
{
	int index = line.indexOf(QString(' ') + name);
	if (index < 0)
		return QString();
	int lIndex = index + name.length() + 3;
	int rIndex = line.indexOf('"', lIndex);
	if (rIndex < 0)
		return QString();
	return line.mid(lIndex, (rIndex - lIndex));
}

static ContentItemInfo parseLine(const QString & line)
{
	QString sWidth = xmlAttributeData("width", line);
	QString sHeight = xmlAttributeData("height", line);
	QString src = xmlAttributeData("src", line);
	QString id = xmlAttributeData("id", line);

	if (sWidth.isEmpty() || sHeight.isEmpty() || src.isEmpty() || id.isEmpty())
		return ImageContentItemInfo();

	bool ok;
	int width = sWidth.toInt(&ok);
	if (!ok)
		return ImageContentItemInfo();
	int height = sHeight.toInt(&ok);
	if (!ok)
		return ImageContentItemInfo();
#ifdef Q_OS_LINUX
	QString fileName = QString("/home/yan/tmp/thumb/download/%1_%2.jpg").arg(src).arg(id);
#elif defined(Q_OS_WIN32)
	QString fileName = QString("D:/Data/tmp/thumbs/download/merge/%1_%2.jpg").arg(src).arg(id);
#elif defined(Q_OS_MAC)
	QString fileName = QString("/Users/eckhard/projects/Qt-Long-Scroll/thumbs/merge/%1_%2.jpg").arg(src).arg(id);
#endif

	return ImageContentItemInfo(fileName, width, height);
}

static QList<ContentItemInfo> loadData()
{
	// This method uses an xml input file to generate a list of ImgInfos, but you can do anything you want.
	// Just create something big. My test uses 31000 thumbnail images.

	QList<ContentItemInfo> imgInfos;
#ifdef Q_OS_LINUX
	QFile f("/home/yan/tmp/thumb/17-45-14.hist");
#elif defined(Q_OS_WIN32)
	QFile f("D:/Data/tmp/thumbs/17-45-14.hist");
#elif defined(Q_OS_MAC)
	QFile f("/Users/eckhard/projects/Qt-Long-Scroll/thumbs/17-45-14.hist");
#endif
	f.open(QIODevice::ReadOnly);
	while (!f.atEnd())
	{
		QString const & line = f.readLine();
		ContentItemInfo && ii = parseLine(line);
		if (ii.valid)
			imgInfos.append(ii);
	}
	f.close();

	// If you want to, you can duplicate the loaded data...
	// This is exponential. From 31k items you'll get 248k items.
	//imgInfos << imgInfos << imgInfos << imgInfos;

	return imgInfos;
}


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	// Generate some data. You'll probably need to write something yourself here.
	QList<ContentItemInfo> const & imgInfos = loadData();

	qDebug() << "loaded items:" << imgInfos.size();

	MainWindow w(0);
	w.setItemInfos(imgInfos);
	w.show();

	return a.exec();
}
