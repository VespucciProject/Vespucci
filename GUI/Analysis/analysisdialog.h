/*******************************************************************************
    Copyright (C) 2014 Wright State University - All Rights Reserved

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

#ifndef ANALYSISDIALOG_H
#define ANALYSISDIALOG_H

#include <QDialog>
#include "Global/vespucciworkspace.h"

namespace Ui {
class AnalysisDialog;
}

class AnalysisDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AnalysisDialog(QWidget *parent, VespucciWorkspace *ws, int row);
    ~AnalysisDialog();

private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

    void on_predictionCheckBox_clicked(bool checked);

private:
    Ui::AnalysisDialog *ui;
    QComboBox *method_selection_box_;
    QSpinBox *components_box_;
    QSharedPointer<VespucciDataset> dataset_;
    VespucciWorkspace *workspace;

    ///
    /// \brief prediction_box_
    /// User selects whether or not to allow prediction of dimensionality
    QCheckBox *prediction_box_;
};

#endif // ANALYSISDIALOG_H
