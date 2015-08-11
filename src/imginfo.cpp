#include "imginfo.h"


ImgInfo::ImgInfo()
{
}

ImgInfo::ImgInfo(QString fileName, int width, int height)
	: width(width),
	  height(height),
	  fileName(fileName)
{
	valid = QFileInfo(fileName).exists();
	if (width != 0 && height != 0)
		r = double(width) / double(height);
}

int ImgInfo::widthForHeight(int height) const
{
	return qRound(r * height);
}

int ImgInfo::heightForWidth(int width) const
{
	return qRound(width / r);
}


bool operator==(const ImgInfo & lhs, const ImgInfo & rhs)
{
	return lhs.fileName == rhs.fileName;
}


bool operator!=(const ImgInfo & lhs, const ImgInfo & rhs)
{
	return !(lhs == rhs);
}
