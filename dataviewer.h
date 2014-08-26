/*******************************************************************************
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
*******************************************************************************/
#ifndef DATAVIEWER_H
#define DATAVIEWER_H

#include <QDialog>
#include "vespucciworkspace.h"
#include "vespuccitablemodel.h"
#include "specmap.h"
class SpecMap;
class VespucciWorkspace;
namespace Ui {
class DataViewer;
class VespucciTableModel;
}

class DataViewer : public QDialog
{
    Q_OBJECT

public:
    explicit DataViewer(QWidget *parent = 0, VespucciWorkspace *ws = 0, int row = 0);
    ~DataViewer();

private slots:
    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_plotPushButton_clicked();

private:
    Ui::DataViewer *ui;
    QString current_text_;
    QString directory_;
    QSharedPointer<SpecMap> dataset_;
    VespucciWorkspace *workspace;
    QTableView *table_;
    QComboBox *data_selector_;
    mat *current_data_;
    bool constructor_in_progress_;
    QPushButton *export_button_;
    QPushButton *plot_button_;
    SpectrumViewer *spectrum_viewer_;

};

#endif // DATAVIEWER_H
