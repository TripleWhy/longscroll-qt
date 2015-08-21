#ifndef CONTENTITEMINFO_H
#define CONTENTITEMINFO_H

#include <QVariant>

struct ContentItemInfo
{
	bool valid = true;
	int width = 0;
	int height = 0;
	double r = 1.0;
	QVariant data;

	ContentItemInfo();
	ContentItemInfo(QVariant const & data, int width, int height);
	ContentItemInfo(QVariant const & data, QSize const & size);

	int widthForHeight(int height) const;
	int heightForWidth(int width) const;
};
bool operator==(ContentItemInfo const & lhs, ContentItemInfo const & rhs);
bool operator!=(ContentItemInfo const & lhs, ContentItemInfo const & rhs);
QDataStream &operator<<(QDataStream & ds, ContentItemInfo const & ci);
QDataStream &operator>>(QDataStream & ds, ContentItemInfo & ci);

struct ImageContentItemInfo : public ContentItemInfo
{
	ImageContentItemInfo();
	ImageContentItemInfo(QString const & getFileName, int width, int height);
	QString const getFileName() const;
	void setFileName(QString const & fileName);
};

#endif // CONTENTITEMINFO_H
