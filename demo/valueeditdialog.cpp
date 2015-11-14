#include "valueeditdialog.h"
#include "ui_valueeditdialog.h"

ValueEditDialog::ValueEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ValueEditDialog)
{
	ui->setupUi(this);
	ui->spinBox->setRange(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
}

ValueEditDialog::~ValueEditDialog()
{
	delete ui;
}

void ValueEditDialog::setValue(QString const & name, const QVariant & val)
{
	ui->nameLabel->setText(name);
	value = val;
	switch (val.userType())
	{
		case QMetaType::Int:
			ui->spinBox->setValue(val.toInt());
			ui->stackedWidget->setCurrentWidget(ui->intPage);
			break;
		default:
			ui->unknownLabel->setText(val.toString());
			ui->stackedWidget->setCurrentWidget(ui->unknownPage);
			break;
	}
}

const QVariant &ValueEditDialog::getValue()
{
	switch (value.userType())
	{
		case QMetaType::Int:
			value = ui->spinBox->value();
			break;
		default:
			break;
	}
	return value;
}
