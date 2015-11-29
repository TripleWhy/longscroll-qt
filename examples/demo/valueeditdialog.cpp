/*
Copyright (C) 2015 Yannick Mueller

This file is part of longscroll-qt.

longscroll-qt is free software; you can redistribute it and/or modify it under
the terms of the GNU Affero General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

longscroll-qt is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License
for more details.

You should have received a copy of the GNU Affero General Public License
along with longscroll-qt.  If not see <http://www.gnu.org/licenses/>.
*/

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
