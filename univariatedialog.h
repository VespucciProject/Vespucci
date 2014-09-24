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
#ifndef UNIVARIATEDIALOG_H
#define UNIVARIATEDIALOG_H

#include <QDialog>
#include "vespucciworkspace.h"

namespace Ui {
class UnivariateDialog;
}

class UnivariateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UnivariateDialog(QWidget *parent, VespucciWorkspace *ws, int row);
    ~UnivariateDialog();

private slots:
    void on_buttonBox_accepted();

    void on_peakComboBox_currentTextChanged(const QString &arg1);

    void on_buttonBox_rejected();

private:
    Ui::UnivariateDialog *ui;
    VespucciWorkspace *workspace;
    QCustomPlot *spectrum_plot_;
    QLineEdit *min_box_;
    QLineEdit *max_box_;
    QLineEdit *name_box_;
    QComboBox *color_selector_;
    QComboBox *value_method_selector_;
    QCheckBox *negative_box_;


    QComboBox *integration_method_selector_;
    QLabel *integration_method_label_;

    //index of the VespucciDataset object in the various lists
    int data_index_;

    //Pointer to the VespucciDataset object at data_index_
    QSharedPointer<VespucciDataset> data_;
};

#endif // UNIVARIATEDIALOG_H
