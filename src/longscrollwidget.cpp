#include "longscrollwidget.h"
#include <QScrollBar>

LONGSCROLLQT_NAMESPACE_BEGIN

/*!
 * \class LongscrollWidget
 * \brief The longscroll widget.
 * Use \ref getContentWidget to customize the behavior.
 */

/*!
 * \brief LongscrollWidget::LongscrollWidget
 * \param parent
 */
LongscrollWidget::LongscrollWidget(QWidget * parent)
    : NotifyingScrollArea(parent)
{
	setWidgetResizable(true);
	cw = new ContentWidget;
	setWidget(cw);
	connect(cw, SIGNAL(scrollToRequest(int)), verticalScrollBar(), SLOT(setValue(int)));
}

/*!
 * \overloaded
 */
ContentWidget const * LongscrollWidget::getContentWidget() const
{
	return cw;
}

/*!
 * \brief Returns the ContentWidget.
 * \return ContentWidget
 */
ContentWidget * LongscrollWidget::getContentWidget()
{
	return cw;
}

LONGSCROLLQT_NAMESPACE_END
