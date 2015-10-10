#include "contentiteminfo.h"
#include <QFileInfo>
#include <QSize>

LONGSCROLLQT_NAMESPACE_BEGIN

ContentItemInfo::ContentItemInfo()
{
}

ContentItemInfo::ContentItemInfo(const QVariant & data, qreal ratio)
    : ratio(ratio),
      data(data)
{
}

ContentItemInfo::ContentItemInfo(QVariant const & data, int width, int height)
    : data(data)
{
	if (width != 0 && height != 0)
		ratio = qreal(width) / qreal(height);
}

ContentItemInfo::ContentItemInfo(const QVariant & data, const QSize & size)
    : ContentItemInfo(data, size.width(), size.height())
{
}

int ContentItemInfo::widthForHeight(int height) const
{
	return qRound(ratio * height);
}

int ContentItemInfo::heightForWidth(int width) const
{
	return qRound(width / ratio);
}

const QVariant & ContentItemInfo::getData() const
{
	return data;
}

bool operator==(const ContentItemInfo & lhs, const ContentItemInfo & rhs)
{
	return lhs.data == rhs.data;
}

bool operator!=(const ContentItemInfo & lhs, const ContentItemInfo & rhs)
{
	return !(lhs == rhs);
}

QDataStream &operator<<(QDataStream & ds, const ContentItemInfo & ci)
{
	ds << ci.ratio << ci.data;
	return ds;
}

QDataStream &operator>>(QDataStream & ds, ContentItemInfo & ci)
{
	ds >> ci.ratio >> ci.data;
	return ds;
}

LONGSCROLLQT_NAMESPACE_END
