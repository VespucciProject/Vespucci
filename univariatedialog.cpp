/*******************************************************************************
    Copyright (C) 2014 Wright State University - All Rights Reserved
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
    file_name_box_ = this->findChild<QLineEdit *>("filenameLineEdit");
    spectrum_plot_ = this->findChild<QCustomPlot *>("spectrumPlot");
    value_method_selector_ = this->findChild<QComboBox *>("peakComboBox");
    color_selector_ = this->findChild<QComboBox *>("gradientComboBox");
    negative_box_ = this->findChild<QCheckBox *>("negativeScoresCheckBox");

    integration_method_selector_ = this->findChild<QComboBox *>("integrationComboBox");
    integration_method_label_ = this->findChild<QLabel *>("integrationLabel");
    range_label_ = this->findChild<QLabel *>("rangeLabel");

    min_line_ = new QCPItemStraightLine(spectrum_plot_);
    min_line_->point1->setCoords(0, 0);
    min_line_->point2->setCoords(0, 1);
    max_line_ = new QCPItemStraightLine(spectrum_plot_);
    max_line_->point1->setCoords(0, 0);
    max_line_->point2->setCoords(0, 1);
    double min, max;
    try{
        min = data_->wavelength_ptr()->min();
        max = data_->wavelength_ptr()->max();
    }
    catch(exception e){
        cerr << e.what();
        workspace->main_window()->DisplayExceptionWarning(e);
        min = 0;
        max = 0;
    }

    QString label_text = QString::number(min) + "–" + QString::number(max);
    range_label_->setText(label_text);

    min_box_->setValidator(new QDoubleValidator(min, max, 2, this));
    max_box_->setValidator(new QDoubleValidator(min, max, 2, this));

    uword origin = data_->FindOrigin();
    QVector<double> plot_data, wavelength;

    try{
        plot_data = data_->PointSpectrum(origin);
        wavelength = data_->WavelengthQVector();
    }
    catch(exception e){
        workspace->main_window()->DisplayExceptionWarning(e);
    }

    spectrum_plot_->addGraph();
    spectrum_plot_->graph(0)->addData(wavelength, plot_data);
    spectrum_plot_->rescaleAxes();
    spectrum_plot_->setInteraction(QCP::iRangeDrag, true);
    spectrum_plot_->setInteraction(QCP::iRangeZoom, true);

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
    if (min_box_->text().isEmpty() || max_box_->text().isEmpty()){
        QMessageBox::warning(this, "Invalid Input!", "You must enter numbers for left and right bounds.");
        return;
    }
    double entered_min = min_box_->text().toDouble();
    double entered_max = max_box_->text().toDouble();

    QString name = name_box_->text();
    QString value_method = value_method_selector_->currentText();
    cout << "Method selection" << endl;
    UnivariateMethod::Method method;
    if (value_method == "Area")
        method = UnivariateMethod::Area;
    else if (value_method == "Bandwidth")
        method = UnivariateMethod::FWHM;
    else if (value_method == "Correlation")
        method = UnivariateMethod::Correlation;
    else
        method = UnivariateMethod::Intensity;

    QString integration_method = integration_method_selector_->currentText();
    int gradient_index = color_selector_->currentIndex();
/*
    if (entered_min < workspace->GetWavelengthMin(data_index_)){
        QMessageBox::warning(this, "Invalid Input!", "You have entered a left bound that is smaller than the smallest number on the spectral abscissa");
        return;
    }

    if (entered_max > workspace->GetWavelengthMax(data_index_)){
        QMessageBox::warning(this, "Invalid Input!", "You have entered a right bound that is larger than the largest number on the spectral abscissa");
        return;
    }
*/
    if (method != UnivariateMethod::Correlation){
        try{
            data_->Univariate(entered_min, entered_max, name, method, integration_method, gradient_index);
        }
        catch(exception e){
            workspace->main_window()->DisplayExceptionWarning(e);
        }
    }
    else{
        vec control;
        QFileInfo file_info(file_name_box_->text());
        workspace->set_directory(file_info.dir().path());
        try{
            control.load(file_name_box_->text().toStdString());
            data_->CorrelationMap(control, name, gradient_index);
        }
        catch(exception e){
            workspace->main_window()->DisplayExceptionWarning(e);
        }
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
        data_.clear();
}

void UnivariateDialog::on_minLineEdit_textChanged(const QString &arg1)
{
    bool ok;
    double value = arg1.toDouble(&ok);
    if (!ok)
        return;

    min_line_->point1->setCoords(value, 0);
    min_line_->point2->setCoords(value, 1);
    if(!spectrum_plot_->hasItem(min_line_))
        spectrum_plot_->addItem(min_line_);
}

void UnivariateDialog::on_maxLineEdit_textChanged(const QString &arg1)
{
    bool ok;
    double value = arg1.toDouble(&ok);
    if (!ok)
        return;

    max_line_->point1->setCoords(value, 0);
    max_line_->point2->setCoords(value, 1);

    if(!spectrum_plot_->hasItem(max_line_))
        spectrum_plot_->addItem(max_line_);
}

void UnivariateDialog::on_browseButton_clicked()
{
    QString filename =
            QFileDialog::getOpenFileName(this,
                                         "Select Spectrum File",
                                         workspace->directory(),
                                         "Vespucci Spectrum Files (*.arma);;"
                                         "Comma-separated Variables (*.csv);;"
                                         "Tab-delimited Text (*.txt)");
    file_name_box_->setText(filename);
}
