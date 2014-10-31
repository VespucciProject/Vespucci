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
*******************************************************************************/#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QDialog>
#include "vespucciworkspace.h"
#include "vespuccidataset.h"

class VespucciWorkspace;
class VespucciDataset;
namespace Ui {
class FilterDialog;
}

///
/// \brief The FilterDialog class
/// This dialog allows the user to apply filtering, smoothing or derivatization
/// to the dataset.
class FilterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FilterDialog(QWidget *parent, VespucciWorkspace *ws, int row);
    ~FilterDialog();

private slots:


    void on_methodComboBox_currentIndexChanged(int index);

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::FilterDialog *ui;
    VespucciWorkspace *workspace;
    QSharedPointer<VespucciDataset> dataset_;

    QComboBox *method_box_;
    QSpinBox *derivative_box_;
    QSpinBox *polynomial_box_;
    QSpinBox *window_box_;
    QSpinBox *singular_values_box_;
};

#endif // FILTERDIALOG_H
