/*******************************************************************************
    Copyright (C) 2015 Wright State University - All Rights Reserved
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
#ifndef RANGEDIALOG_H
#define RANGEDIALOG_H

#include <QDialog>
#include "Data/Dataset/vespuccidataset.h"

namespace Ui {
class RangeDialog;
}
///
/// \brief The RangeDialog class
///
class RangeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RangeDialog(QWidget *parent, double min, double max);
    ~RangeDialog();
    bool is_accepted();
    void GetRange(double &min, double &max);

signals:
    void DialogAccepted(double min, double max);


private slots:
    void on_buttonBox_accepted();

private:
    Ui::RangeDialog *ui;
    QDoubleSpinBox *min_box_;
    QDoubleSpinBox *max_box_;
    QLabel *range_label_;
    double min_;
    double max_;

    bool is_accepted_;
};

#endif // RANGEDIALOG_H
