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
#include "mapdialog.h"
#include "ui_mapdialog.h"

MapDialog::MapDialog(MainWindow *parent, QStringList data_keys) :
    QDialog(parent),
    ui(new Ui::MapDialog)
{
    ui->setupUi(this);

    if (data_keys.size() > 3 || data_keys.size() < 2){
        main_window_->DisplayWarning("Cannot map this object", "Can't map object");
        close();
    }
    main_window_ = parent;
    data_keys_ = data_keys;
    workspace = main_window_->workspace_ptr();
    uword col_count;
    try{
        col_count = workspace->GetMatrix(data_keys_).n_cols;
    }catch(exception e){
        main_window_->DisplayExceptionWarning("MapDialog constructor, VespucciWorkspace::GetMatrix", e);
        close();
    }

    try{
        dataset_ = workspace->data_model()->GetDataset(data_keys_[0]);
    }catch(exception e){
        main_window_->DisplayExceptionWarning("MapDialog constructor, DataModel::GetDataset", e);
        close();
    }



    gradient_combo_box_ = findChild<QComboBox*>("gradientComboBox");
    invert_check_box_ = findChild<QCheckBox*>("invertCheckBox");
    column_spin_box_ = findChild<QSpinBox*>("columnSpinBox");
    column_spin_box_->setRange(1, col_count);
    name_line_edit_ = findChild<QLineEdit*>("nameLineEdit");
    name_line_edit_->setText(data_keys_.last());
    QStringList color_list({"ColorBrewer BlueGreen",
                            "ColorBrewer BluePurple",
                            "ColorBrewer GreenBlue",
                            "ColorBrewer OrangeRed",
                            "ColorBrewer PurpleBlue",
                            "ColorBrewer PurpleBlueGreen",
                            "ColorBrewer PurpleRed",
                            "ColorBrewer RedPurple",
                            "ColorBrewer YellowGreen",
                            "ColorBrewer YellowGreenBlue",
                            "ColorBrewer YellowOrangeBrown",
                            "ColorBrewer YellowOrangeRed",
                            "ColorBrewer Blues",
                            "ColorBrewer Greens",
                            "ColorBrewer Oranges",
                            "ColorBrewer Purples",
                            "ColorBrewer Reds",
                            "ColorBrewer Grayscale",
                            "QCustomPlot Grayscale",
                            "QCustomPlot Night",
                            "QCustomPlot Candy",
                            "QCustomPlot Ion",
                            "QCustomPlot Thermal",
                            "↔QCustomPlot Polar",
                            "↔QCustomPlot Spectrum",
                            "QCustomPlot Jet",
                            "QCustomPlot Hues",
                            "QCustomPlot Hot",
                            "QCustomPlot Cold",
                            "↔ColorBrewer BrownBlueGreen",
                            "↔ColorBrewer PinkYellowGreen",
                            "↔ColorBrewer PurpleGreen",
                            "↔ColorBrewer PurpleOrange",
                            "↔ColorBrewer RedBlue",
                            "↔ColorBrewer RedGray",
                            "↔ColorBrewer RedYellowBlue",
                            "↔ColorBrewer RedYellowGreen",
                            "↔ColorBrewer Spectral",
                            "↔Vespucci Spectral",
                            "ColorBrewer Cluster"});
    gradient_combo_box_->addItems(color_list);
}

MapDialog::~MapDialog()
{
    delete ui;
}

void MapDialog::on_buttonBox_accepted()
{
    uint gradient_index = gradient_combo_box_->currentIndex();
    uint column_index = column_spin_box_->value() - 1;
    QString map_name = name_line_edit_->text();
    vec data;
    try{
        data = workspace->data_model()->GetMatrix(data_keys_).col(column_index);
    }catch(exception e){
        main_window_->DisplayExceptionWarning("DataModel::GetMatrix or mat::col", e);
        close();
    }

    QCPColorGradient gradient;
    uint tick_count;

    if (gradient_combo_box_->currentText() == "ColorBrewer Cluster"){
        vec unique_values = unique(data);
        tick_count = unique_values.n_rows;
        gradient = dataset_->GetClusterGradient(tick_count);

    }
    else{
        tick_count = 6;
        gradient = dataset_->GetGradient(gradient_index);
    }

    try{
        if (data_keys_.size() == 2)
            dataset_->CreateMap(map_name,
                                data_keys_[1],
                                column_index,
                                gradient,
                                tick_count);
        else
            dataset_->CreateMap(map_name,
                                data_keys_[1],
                                data_keys_[2],
                                column_index,
                                gradient,
                                tick_count);
    }catch(exception e){
        main_window_->DisplayExceptionWarning("VespucciDataset::CreateMap()", e);
        close();
    }
    close();
}
