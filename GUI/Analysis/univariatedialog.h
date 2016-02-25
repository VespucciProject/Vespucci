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
#ifndef UNIVARIATEDIALOG_H
#define UNIVARIATEDIALOG_H

#include <QDialog>
#include "Global/vespucciworkspace.h"

namespace Ui {
class UnivariateDialog;
}

///
/// \brief The UnivariateDialog class
/// Class allowing user to create univariate images
class UnivariateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UnivariateDialog(QWidget *parent, VespucciWorkspace *ws, int row);
    ~UnivariateDialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_minLineEdit_textChanged(const QString &arg1);

    void on_maxLineEdit_textChanged(const QString &arg1);

private:
    Ui::UnivariateDialog *ui;

    QCheckBox *map_check_box_;
    ///
    /// \brief range_label_
    /// Displays the range of the spectral abscissa.
    QLabel *range_label_;

    ///
    /// \brief workspace
    /// The current workspace
    VespucciWorkspace *workspace;

    ///
    /// \brief spectrum_plot_
    /// The plot displaying a spectrum
    QCustomPlot *spectrum_plot_;

    ///
    /// \brief min_line_edit_
    /// The user enters her desired minimum value here.
    QLineEdit *min_line_edit_;

    ///
    /// \brief max_line_edit_
    /// The user enters her desired maximum value here.
    QLineEdit *max_line_edit_;

    ///
    /// \brief name_line_edit_
    /// The user enters the name of the dataset her
    QLineEdit *name_line_edit_;


    ///
    /// \brief value_method_combo_box_
    /// User selects value determination method here
    QComboBox *value_method_combo_box_;

    ///
    /// \brief search_window_spin_box_
    ///
    QSpinBox *search_window_spin_box_;

    ///
    /// \brief data_index_
    /// Index of current dataset in relevant lists
    int data_index_;

    ///
    /// \brief min_line_
    /// The line drawn on the spectrum corresponding to the left bound
    QCPItemStraightLine *min_line_;

    ///
    /// \brief max_line_
    /// The line drawn on the spectrum corresponding to the right bound
    QCPItemStraightLine *max_line_;

    ///
    /// \brief data_
    /// The current dataset
    QSharedPointer<VespucciDataset> data_;

};

#endif // UNIVARIATEDIALOG_H
