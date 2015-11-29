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
