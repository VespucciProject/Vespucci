/******************************************************************************
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
#include "bandratiodialog.h"
#include "ui_bandratiodialog.h"

BandRatioDialog::BandRatioDialog(QWidget *parent,
                                 VespucciWorkspace *ws,
                                 int row) :
    QDialog(parent),
    ui(new Ui::BandRatioDialog)
{
    ui->setupUi(this);


    workspace = ws;
    data_ = workspace->DatasetAt(row);
    if(data_->non_spatial()){
        QMessageBox::warning(this,
                             "Non-spatial or Non-contiguous Dataset",
                             "Images cannot be created from non-spatial or "
                             "non-contiguous datasets.");
        this->close();
        data_.clear();
    }
    spectrum_plot_ = this->findChild<QCustomPlot *>("spectrumPlot");
    first_min_box_ = this->findChild<QLineEdit *>("firstMinLineEdit");
    first_max_box_ = this->findChild<QLineEdit *>("firstMaxLineEdit");
    second_min_box_ = this->findChild<QLineEdit *>("secondMinLineEdit");
    second_max_box_ = this->findChild<QLineEdit *>("secondMaxLineEdit");

    name_box_ = this->findChild<QLineEdit *>("nameLineEdit");
    color_selector_ = this->findChild<QComboBox *>("gradientComboBox");
    value_method_selector_ = this->findChild<QComboBox *>("peakComboBox");
    integration_method_selector_ = this->findChild<QComboBox *>("integrationComboBox");
    integration_method_label_ = this->findChild<QLabel *>("integrationLabel");

    data_index_ = row;

    double min = workspace->GetWavelengthMin(row);
    double max = workspace->GetWavelengthMax(row);

    QDoubleValidator *validator = new QDoubleValidator(min, max, 2, this);

    first_min_box_->setValidator(validator);
    first_max_box_->setValidator(validator);
    second_min_box_->setValidator(validator);
    second_max_box_->setValidator(validator);

    QVector<double> plot_data = data_->PointSpectrum(0);
    QVector<double> wavelength = data_->WavelengthQVector();

    spectrum_plot_->addGraph();
    spectrum_plot_->graph(0)->addData(wavelength, plot_data);


    spectrum_plot_->xAxis->setRange(data_->WavelengthRange());
    spectrum_plot_->yAxis->setRange(data_->PointSpectrumRange(0));

    spectrum_plot_->setInteraction(QCP::iRangeDrag, true);
}

BandRatioDialog::~BandRatioDialog()
{
    delete ui;
}

///
/// \brief BandRatioDialog::on_buttonBox_accepted
/// After clicking Ok, this slot grabs the information from the dialog, then
/// instantiates the map data
void BandRatioDialog::on_buttonBox_accepted()
{
    double first_entered_min = first_min_box_->text().toDouble();
    double second_entered_min = second_min_box_->text().toDouble();
    double first_entered_max = first_max_box_->text().toDouble();
    double second_entered_max = second_max_box_->text().toDouble();

    if (first_entered_min < workspace->GetWavelengthMin(data_index_) || second_entered_min < workspace->GetWavelengthMin(data_index_)){
        QMessageBox::warning(this, "Invalid Input!", "You have entered a left bound that is smaller than the smallest number on the spectral abcissa");
        return;
    }

    if (first_entered_max > workspace->GetWavelengthMax(data_index_) || second_entered_max > workspace->GetWavelengthMax(data_index_)){
        QMessageBox::warning(this, "Invalid Input!", "You have entered a right bound that is larger than the largest number on the spectral abcissa");
        return;
    }


    uvec first_range = data_->FindRange(first_entered_min, first_entered_max);
    uvec second_range = data_->FindRange(second_entered_min, second_entered_max);

    int first_min = first_range[0];
    int first_max = first_range[1];
    int second_min = second_range[0];
    int second_max = second_range[1];

    QString name = name_box_->text();
    QString value_method = value_method_selector_->currentText();
    QString integration_method = integration_method_selector_->currentText();
    int color_index = color_selector_->currentIndex();
    try{
        data_->BandRatio(first_min,
                         first_max,
                         second_min,
                         second_max,
                         name,
                         value_method,
                         integration_method,
                         color_index);
    }
    catch(exception e){
        workspace->main_window()->DisplayExceptionWarning(e);
    }

    this->close();
    data_.clear();
}

///
/// \brief BandRatioDialog::on_peakComboBox_currentTextChanged
/// \param arg1
/// When the user changes the peak determination method, various options are
/// enabled or disabled
void BandRatioDialog::on_peakComboBox_currentTextChanged(const QString &arg1)
{
    if (arg1 == "Area"){
        integration_method_selector_->setEnabled(true);
        integration_method_label_->setEnabled(true);
    }
    else{
        integration_method_selector_->setEnabled(false);
        integration_method_label_->setEnabled(false);
    }
}

///
/// \brief BandRatioDialog::on_buttonBox_rejected
/// The window closes when the user clicks "Cancel"
void BandRatioDialog::on_buttonBox_rejected()
{
    this->close();
    data_.clear();
}
