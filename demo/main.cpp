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

	if (app.arguments().size() < 2)
	{
		qWarning() << "Please specify one or more directories.";
		return 1;
	}

	QDir mainDir(app.arguments().at(1));
	if (!mainDir.exists())
	{
		qWarning() << app.arguments().at(1) << "does not exist or is not a directory.";
		return 1;
	}

	QMap<QString, ContentItemInfo> cache, loadedCache;
	QFile cacheFile(mainDir.absoluteFilePath("images.cache"));
	if (cacheFile.exists())
	{
		if (cacheFile.open(QIODevice::ReadOnly))
		{
			QByteArray const & buffer = qUncompress(cacheFile.readAll());
			cacheFile.close();
			QDataStream ds(buffer);
			ds >> loadedCache;
		}
		else
		{
			qWarning() << "could not open cache file:" << cacheFile.fileName();
		}
	}
	cache = loadedCache;
	qDebug() << "cached images:" << cache.size();

	QStringList fileNamefilters;
	for (QString const & format : QImageReader::supportedImageFormats())
		fileNamefilters.append(QString("*.%1").arg(format));

	QImageReader ir;
	for (int i = 1; i < app.arguments().size(); ++i)
	{
		QDir dir(app.arguments().at(i));
		if (!dir.exists())
			continue;

		qDebug() << "loading file list for" << dir.absolutePath() << "...";
		QFileInfoList const & files = dir.entryInfoList(fileNamefilters, QDir::Files | QDir::Readable);
		qDebug() << "loading images from" << dir.absolutePath() << "...";
		for (QFileInfo const & file : files)
		{
			QString const & fileName = file.absoluteFilePath();
			if (cache.contains(fileName))
				continue;

			ir.setFileName(fileName);
			cache.insert(fileName, ContentItemInfo(fileName, ir.size()));
		}
	}

	qDebug() << "checking file existence ...";
	for (auto it = cache.begin(); it != cache.end(); )
	{
		QFileInfo fi(it.key());
		if (fi.exists())
			++it;
		else
			it = cache.erase(it);
	}

	if (cache != loadedCache)
	{
		qDebug() << "storing cache file ...";
		QByteArray buffer;
		if (cacheFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
		{
			QDataStream ds(&buffer, QIODevice::WriteOnly);
			ds << cache;
			cacheFile.write(qCompress(buffer, 9));
			cacheFile.close();
		}
		else
		{
			qWarning() << "Could not open" << cacheFile.fileName() << "for write.";
		}
	}

	qDebug() << "loaded" << cache.size() << "images";
	qDebug() << "loading gui...";


	int demoMode = 0;
	if (app.arguments().size() >=2)
		demoMode = app.arguments().last().toInt();
	MainWindow w(demoMode);

#if 1
	w.setItemInfos(cache.values());
#else
	//Enable this if you want to test with an arbitrary number of images. 10,000,000 works a little slow for me.
	int count = 10000000;
	qDebug() << "creating" << count << "items ...";
	QList<ContentItemInfo> const & values = cache.values();
	QList<ContentItemInfo> infos;
	for (int i = 0; i < count; ++i)
		infos.append(values.at(i % values.count()));

	w.setItemInfos(infos);
	qDebug() << "starting ...";
#endif
	w.show();

	return app.exec();
}
