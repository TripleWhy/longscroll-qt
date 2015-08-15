#include "contentiteminfo.h"
#include <QFileInfo>

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
