#include "navigatorwidget.h"
#include "ui_navigatorwidget.h"
#include "imageinfowidget.h"

LONGSCROLLQT_NAMESPACE_BEGIN

/*!
 * \class NavigatorWidget
 * \brief Base class for navigator widgets used by ContentWidget.
 */

/*!
 * \property NavigatorWidget::itemInfo
 * \brief The item currently being displayed.
 */

/*!
 * \see NavigatorWidget::itemInfo
 */
const ContentItemInfo &NavigatorWidget::getItemInfo() const
{
	return itemInfo;
}

/*!
 * \see NavigatorWidget::itemInfo
 */
void NavigatorWidget::setItemInfo(const ContentItemInfo & info)
{
	if (itemInfo == info)
		return;
	itemInfo = info;
}

/*!
 * \fn NavigatorWidget::previousImageRequested
 * \brief This signal is emitted when navigator should display the previous image.
 */

/*!
 * \fn NavigatorWidget::nextImageRequested
 * \brief This signal is emitted when navigator should display the next image.
 */

/*!
 * \fn NavigatorWidget::closeRequested
 * \brief This signal is emitted when the navigator should be closed.
 */


/*!
 * \class BaseNavigatorWidget
 * \brief NavigatorWidget that provides a basic layout without actually processing information.
 * This class is ment to be derived further.
 */

/*!
 * \brief Constructs a BaseNavigatorWidget.
 * \param parent
 * \param f
 */
BaseNavigatorWidget::BaseNavigatorWidget(QWidget *parent, Qt::WindowFlags f)
    : NavigatorWidget(parent, f),
      ui(new Ui::BaseNavigatorWidget)
{
	ui->setupUi(this);

	ui->prevButton->setShortcut(QKeySequence("Left"));
	ui->nextButton->setShortcut(QKeySequence("Right"));
	ui->closeButton->setShortcut(QKeySequence("Esc"));

	connect(ui->prevButton, SIGNAL(clicked()), this, SIGNAL(previousImageRequested()));
	connect(ui->nextButton, SIGNAL(clicked()), this, SIGNAL(nextImageRequested()));
	connect(ui->closeButton, SIGNAL(clicked()), this, SIGNAL(closeRequested()));
}

/*!
 * \brief Destroys the widget.
 */
BaseNavigatorWidget::~BaseNavigatorWidget()
{
	delete ui;
}

/*!
 * \var BaseNavigatorWidget::ui;
 * \brief Designer UI.
 */


/*!
 * \class ImageNavigatorWidget
 * \brief NavigatorWidget that uses a ImageInfoWidget to display item info.
 */

/*!
 * \brief Constructs an ImageNavigatorWidget.
 * \param parent
 * \param f
 */
ImageNavigatorWidget::ImageNavigatorWidget(QWidget * parent, Qt::WindowFlags f)
    : BaseNavigatorWidget(parent, f)
{
	imgContent = new ImageInfoWidget();
	ui->infoLayout->addWidget(imgContent);
}

/*!
 * \reimp{BaseNavigatorWidget::setItemInfo}
 */
void ImageNavigatorWidget::setItemInfo(const ContentItemInfo & info)
{
	BaseNavigatorWidget::setItemInfo(info);
	imgContent->setItemInfo(info);
}

LONGSCROLLQT_NAMESPACE_END
