/************************************************************************************
    Copyright (C) 2014 Daniel P. Foose - All Rights Reserved

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
***************************************************************************************/
#ifndef PRINCIPALCOMPONENTSDIALOG_H
#define PRINCIPALCOMPONENTSDIALOG_H

#include <QDialog>
#include "vespucciworkspace.h"


namespace Ui {
class PrincipalComponentsDialog;
}

class PrincipalComponentsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PrincipalComponentsDialog(QWidget *parent, VespucciWorkspace *ws, int row);
    ~PrincipalComponentsDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::PrincipalComponentsDialog *ui;
    VespucciWorkspace *workspace;
    QSpinBox *component_selector_;
    QComboBox *color_selector_;
    QCheckBox *negative_score_box_;
    QLineEdit *name_box_;

    //index of the SpecMap object in the various lists
    int data_index_;

    //Pointer to the SpecMap object at data_index_
    QSharedPointer<SpecMap> data_;
};

#endif // PRINCIPALCOMPONENTSDIALOG_H
