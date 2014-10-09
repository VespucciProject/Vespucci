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
#include "univariatedialog.h"
#include "ui_univariatedialog.h"

///
/// \brief UnivariateDialog::UnivariateDialog
/// \param parent See QDialog
/// \param ws Current workspace
/// \param row Selected row in the dataset list widget
///
UnivariateDialog::UnivariateDialog(QWidget *parent, VespucciWorkspace *ws, int row) :
    QDialog(parent),
    ui(new Ui::UnivariateDialog)
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
    min_box_ = this->findChild<QLineEdit *>("minLineEdit");
    max_box_ = this->findChild<QLineEdit *>("maxLineEdit");
    name_box_ = this->findChild<QLineEdit *>("nameLineEdit");
    spectrum_plot_ = this->findChild<QCustomPlot *>("spectrumPlot");
    value_method_selector_ = this->findChild<QComboBox *>("peakComboBox");
    color_selector_ = this->findChild<QComboBox *>("gradientComboBox");
    negative_box_ = this->findChild<QCheckBox *>("negativeScoresCheckBox");

    integration_method_selector_ = this->findChild<QComboBox *>("integrationComboBox");
    integration_method_label_ = this->findChild<QLabel *>("integrationLabel");

    double min = workspace->GetWavelengthMin(row);
    double max = workspace->GetWavelengthMax(row);

    min_box_->setValidator(new QDoubleValidator(min, max, 2, this));
    max_box_->setValidator(new QDoubleValidator(min, max, 2, this));

    QVector<double> plot_data = data_->PointSpectrum(0);
    QVector<double> wavelength = data_->WavelengthQVector();

    spectrum_plot_->addGraph();
    spectrum_plot_->graph(0)->addData(wavelength, plot_data);


    spectrum_plot_->xAxis->setRange(data_->WavelengthRange());
    spectrum_plot_->yAxis->setRange(data_->PointSpectrumRange(0));
    spectrum_plot_->setInteraction(QCP::iRangeDrag, true);

}

UnivariateDialog::~UnivariateDialog()
{
    delete ui;
}

///
/// \brief UnivariateDialog::on_buttonBox_accepted
/// Call the appropriate method of the dataset when "Ok" is clicked
void UnivariateDialog::on_buttonBox_accepted()
{
    double entered_min = min_box_->text().toDouble();
    double entered_max = max_box_->text().toDouble();

    uvec range = data_->FindRange(entered_min, entered_max);

    int min = range[0];
    int max = range[1];

    QString name = name_box_->text();
    QString value_method = value_method_selector_->currentText();
    QString integration_method = integration_method_selector_->currentText();

    int color_index = color_selector_->currentIndex();

    if (entered_min < workspace->GetWavelengthMin(data_index_)){
        QMessageBox::warning(this, "Invalid Input!", "You have entered a left bound that is smaller than the smallest number on the spectral abcissa");
        return;
    }

    if (entered_max > workspace->GetWavelengthMax(data_index_)){
        QMessageBox::warning(this, "Invalid Input!", "You have entered a right bound that is larger than the largest number on the spectral abcissa");
        return;
    }
    try{
        data_->Univariate(min, max, name, value_method, integration_method, color_index);
    }
    catch(exception e){
        workspace->main_window()->DisplayExceptionWarning(e);
    }
    this->close();
    data_.clear();
}

///
/// \brief UnivariateDialog::on_peakComboBox_currentTextChanged
/// \param arg1
/// Turn options on and off when peak determination method is changed
void UnivariateDialog::on_peakComboBox_currentTextChanged(const QString &arg1)
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
/// \brief UnivariateDialog::on_buttonBox_rejected
/// Close window when "Cancel" clicked.
void UnivariateDialog::on_buttonBox_rejected()
{
    this->close();
    data_.clear();
}
