#ifndef CONTENTWIDGET_H
#define CONTENTWIDGET_H

#include "notifyablescrollcontentwidget.h"
#include <QLabel>

class NavigatorWidget;
class ImgInfo;
class ContentWidgetItemFactory;

#define LAZY_ALIGN 1
class ContentWidget : public NotifyableScrollContentWidget
{
	Q_OBJECT

private:
	struct ItemInfo
	{
		int index = -1;
		ImgInfo * img = 0;
		int x = 0;
		int width = 0;
	};

	struct RowInfo
	{
		QList<ItemInfo> items;
		int y = 0;
		bool displaying = false;
#if LAZY_ALIGN
		bool aligned = false;
#endif
	};

protected:
	static constexpr int prefetchBefore = 1;
	static constexpr int prefetchAfter = 3;
	static constexpr int xSpacing = 5;
	static constexpr int ySpacing = 5;

protected:
	int rowHeight = 200;
	int itemWidth = 0;
	bool allowOverfill = true;
	bool align = true;
	bool alignLast = false;
	int navigatorHeight = 500;
	ContentWidgetItemFactory * itemFactory;

private:
	QSize size;
	QRect visibleRect;

	int navigatorRow = 0;
	int navigatorColumn = 0;
	ImgInfo * navigatorImg = 0;
	bool navigatorVisible = false;
	NavigatorWidget * navigator = 0;

	QList<ImgInfo> images;
	QList<int> imageWidths;
	QList<RowInfo> rowInfos;
	QList<QWidget *> rowWidgets;
	QList<QWidget *> itemWidgets;

public:
	ContentWidget(QWidget * parent = 0);
	ContentWidget(int rowHeight, int itemWidth = 0, bool alignRows = true, bool alignLastRow = false, bool allowOverfill = true, int navigatorHeight = 500, QWidget *parent = 0);
	~ContentWidget();

	void setItemFactory(ContentWidgetItemFactory * factory);

	virtual QSize sizeHint() const override;
	virtual bool usesShowingRect() override;
	virtual void showingRect(QRect const & rect) override;

private:
	void updateRows();
	void hideRow(int i);
	void showRow(RowInfo const & rowInfo, int rowIndex);

protected:
	// Can be overridden. Behavior can also be changed by setting a different item factory.
	virtual QWidget * createItemWidget(ImgInfo const & info, int width, int height);

public:
	void setImages(const QList<ImgInfo> & imgs);

protected:
	void calculateSize();
	void alignRow(RowInfo & row);

protected:
	virtual void setShowing(bool visible) override;
	int rowAt(int y, bool * onNavigator = 0);
	int colAt(int x, int row);
	void nextImage(int & row, int & col);
	void previousImage(int & row, int & col);
	virtual void mousePressEvent(QMouseEvent * event) override;

private:
	void showNavigator(int const row, int const col, bool const blockUpdates = true);

private slots:
	void hideNavigator();
	void navigatorNext();
	void navigatorPrev();

private:
	void navigatorPrevNext(bool next);

signals:
	void scrollRequest(int dx, int dy);
	void scrollToRequest(int x, int y);
};

#endif // CONTENTWIDGET_H
