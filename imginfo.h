#ifndef IMGINFO_H
#define IMGINFO_H

#include <QFileInfo>

struct ImgInfo
{
	bool valid = false;
	int width = 0;
	int height = 0;
	double r = 1.0;
	QString fileName;

	ImgInfo();
	ImgInfo(QString fileName, int width, int height);

	int widthForHeight(int height) const;
	int heightForWidth(int width) const;
};

bool operator==(ImgInfo const& lhs, ImgInfo const& rhs);
bool operator!=(ImgInfo const& lhs, ImgInfo const& rhs);

#endif // IMGINFO_H
