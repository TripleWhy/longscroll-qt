#ifndef VALUEEDITDIALOG_H
#define VALUEEDITDIALOG_H

#include <QDialog>
#include <QVariant>

namespace Ui {
class ValueEditDialog;
}

class ValueEditDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ValueEditDialog(QWidget *parent = 0);
	~ValueEditDialog();

	void setValue(QString const & name, QVariant const & val);
	QVariant const & getValue();

private:
	Ui::ValueEditDialog *ui;
	QVariant value;
};

#endif // VALUEEDITDIALOG_H
