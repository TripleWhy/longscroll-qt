#ifndef CONTENTITEMINFO_H
#define CONTENTITEMINFO_H

#include "longscroll-qt_global.h"
#include <QVariant>

LONGSCROLLQT_NAMESPACE_BEGIN

struct LONGSCROLLQT_EXPORT ContentItemInfo
{
private:
	qreal ratio = 1.0;
	QVariant data;

public:
	ContentItemInfo();
	ContentItemInfo(QVariant const & data, qreal ratio);
	ContentItemInfo(QVariant const & data, int width, int height);
	ContentItemInfo(QVariant const & data, QSize const & size);

	int widthForHeight(int height) const;
	int heightForWidth(int width) const;

	QVariant const & getData() const;

private:
	friend bool operator==(ContentItemInfo const & lhs, ContentItemInfo const & rhs);
	friend bool operator!=(ContentItemInfo const & lhs, ContentItemInfo const & rhs);
	friend QDataStream &operator<<(QDataStream & ds, ContentItemInfo const & ci);
	friend QDataStream &operator>>(QDataStream & ds, ContentItemInfo & ci);
};

LONGSCROLLQT_EXPORT bool operator==(ContentItemInfo const & lhs, ContentItemInfo const & rhs);
LONGSCROLLQT_EXPORT bool operator!=(ContentItemInfo const & lhs, ContentItemInfo const & rhs);
LONGSCROLLQT_EXPORT QDataStream &operator<<(QDataStream & ds, ContentItemInfo const & ci);
LONGSCROLLQT_EXPORT QDataStream &operator>>(QDataStream & ds, ContentItemInfo & ci);


LONGSCROLLQT_NAMESPACE_END

#endif // CONTENTITEMINFO_H
