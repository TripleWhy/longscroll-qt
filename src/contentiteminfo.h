#ifndef CONTENTITEMINFO_H
#define CONTENTITEMINFO_H

#include "longscroll-qt_global.h"
#include <QVariant>

LONGSCROLLQT_NAMESPACE_BEGIN

struct LONGSCROLLQT_EXPORT ContentItemInfo
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
LONGSCROLLQT_EXPORT bool operator==(ContentItemInfo const & lhs, ContentItemInfo const & rhs);
LONGSCROLLQT_EXPORT bool operator!=(ContentItemInfo const & lhs, ContentItemInfo const & rhs);
LONGSCROLLQT_EXPORT QDataStream &operator<<(QDataStream & ds, ContentItemInfo const & ci);
LONGSCROLLQT_EXPORT QDataStream &operator>>(QDataStream & ds, ContentItemInfo & ci);

struct LONGSCROLLQT_EXPORT ImageContentItemInfo : public ContentItemInfo
{
	ImageContentItemInfo();
	ImageContentItemInfo(QString const & getFileName, int width, int height);
	QString const getFileName() const;
	void setFileName(QString const & fileName);
};

LONGSCROLLQT_NAMESPACE_END

#endif // CONTENTITEMINFO_H
