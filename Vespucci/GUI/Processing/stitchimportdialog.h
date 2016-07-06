/*******************************************************************************
    Copyright (C) 2014-2016 Wright State University - All Rights Reserved
    Daniel P. Foose - Maintainer/Lead Developer

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
#ifndef STITCHIMPORTDIALOG_H
#define STITCHIMPORTDIALOG_H

#include <QDialog>

#include "Global/vespucciworkspace.h"
#include <mlpack/core.hpp>
using namespace arma;
using namespace std;

namespace Ui {
class StitchImportDialog;
}

class StitchImportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StitchImportDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws);
    ~StitchImportDialog();

private slots:
    void on_browsePushButton_clicked();

    void on_buttonBox_accepted();

private:
    Ui::StitchImportDialog *ui;
    QSharedPointer<VespucciWorkspace> workspace_;
    QLineEdit *x_line_edit_;
    QLineEdit *x_units_line_edit_;
    QLineEdit *y_line_edit_;
    QLineEdit *y_units_line_edit_;
    QLineEdit *filename_line_edit_;
    QComboBox *data_format_combo_box_;
    QLineEdit *name_line_edit_;
    QCheckBox *swap_spatial_check_box_;

    QString path_;

    bool LoadDatasets(field<string> filenames,
                      mat &spectra,
                      vec &x, vec &y, vec &abscissa,
                      bool swap_spatial, QString type);

};

#endif // STITCHIMPORTDIALOG_H
