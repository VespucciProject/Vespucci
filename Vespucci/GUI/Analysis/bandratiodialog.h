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

#ifndef BANDRATIODIALOG_H
#define BANDRATIODIALOG_H

#include <QDialog>
#include "GUI/Analysis/univariatedialog.h"

namespace Ui {
class BandRatioDialog;
}

///
/// \brief The BandRatioDialog class
/// The dialog that allows the user to create a band-ratio map
class BandRatioDialog : public QDialog
{
    Q_OBJECT

public:
   explicit BandRatioDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws, QSharedPointer<AbstractDataAnalyzer> analyzer);
    ~BandRatioDialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_firstMinLineEdit_textChanged(const QString &arg1);

    void on_firstMaxLineEdit_textChanged(const QString &arg1);

    void on_secondMinLineEdit_textChanged(const QString &arg1);

    void on_secondMaxLineEdit_textChanged(const QString &arg1);

    void on_indexSpinBox_editingFinished();

private:
    Ui::BandRatioDialog *ui;
    ///
    /// \brief data_index_
    /// Index of the VespucciDataset object in the various lists
    int data_index_;

    ///
    /// \brief first_min_line_
   ///
    QCPItemStraightLine *first_min_line_;
    ///
    /// \brief first_max_line_
    ///
    QCPItemStraightLine *first_max_line_;
    ///
    /// \brief second_min_line_
    ///
    QCPItemStraightLine *second_min_line_;
    ///
    /// \brief second_max_line_
    ///
    QCPItemStraightLine *second_max_line_;

    ///
    /// \brief dataset_
    /// Points to the dataset we're working with.
    QSharedPointer<VespucciWorkspace> workspace_;
    QSharedPointer<AbstractDataAnalyzer> analyzer_;

    QStringList dataset_keys_;

};

#endif // BANDRATIODIALOG_H
