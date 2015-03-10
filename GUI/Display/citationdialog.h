/*******************************************************************************
    Copyright (C) 2014 Wright State University - All Rights Reserved
    Daniel P. Foose - Author

    This file is part of Vespucci.

    Vespucci is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Vespucci is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Vespucci.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************************/
#ifndef CITATIONDIALOG_H
#define CITATIONDIALOG_H

#include <QDialog>

namespace Ui {
class CitationDialog;
}

///
/// \brief The CitationDialog class
/// The dialog that displays how to cite Vespucci
class CitationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CitationDialog(QWidget *parent = 0);
    ~CitationDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::CitationDialog *ui;
};

#endif // CITATIONDIALOG_H
