#include "contentiteminfo.h"
#include <QFileInfo>
#include <QSize>

LONGSCROLLQT_NAMESPACE_BEGIN

ContentItemInfo::ContentItemInfo()
{
}

ContentItemInfo::ContentItemInfo(QVariant const & data, int width, int height)
	: width(width),
	  height(height),
	  data(data)
{
	if (width != 0 && height != 0)
		r = double(width) / double(height);
}

ContentItemInfo::ContentItemInfo(const QVariant & data, const QSize & size)
    : ContentItemInfo(data, size.width(), size.height())
{
}

int ContentItemInfo::widthForHeight(int height) const
{
	return qRound(r * height);
}

int ContentItemInfo::heightForWidth(int width) const
{
	return qRound(width / r);
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
	ds << ci.valid << ci.width << ci.height << ci.r << ci.data;
	return ds;
}

QDataStream &operator>>(QDataStream & ds, ContentItemInfo & ci)
{
	ds >> ci.valid >> ci.width >> ci.height >> ci.r >> ci.data;
	return ds;
}


ImageContentItemInfo::ImageContentItemInfo()
    : ContentItemInfo()
{
	valid = false;
}

ImageContentItemInfo::ImageContentItemInfo(const QString & fileName, int width, int height)
    : ContentItemInfo(fileName, width, height)
{
	QFileInfo fi(fileName);
	valid = fi.exists(fileName);
}

const QString ImageContentItemInfo::getFileName() const
{
	return data.toString();
}

void ImageContentItemInfo::setFileName(const QString & fileName)
{
	QFileInfo fi(fileName);
	valid = fi.exists(fileName);
	data = fileName;
}

LONGSCROLLQT_NAMESPACE_END
