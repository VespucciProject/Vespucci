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
#include "GUI/Analysis/bandratiodialog.h"
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
    range_label_ = this->findChild<QLabel *>("rangeLabel");

    data_index_ = row;

    first_min_line_ = new QCPItemStraightLine(spectrum_plot_);
    first_max_line_ = new QCPItemStraightLine(spectrum_plot_);
    second_min_line_ = new QCPItemStraightLine(spectrum_plot_);
    second_max_line_ = new QCPItemStraightLine(spectrum_plot_);

    first_min_line_->point1->setCoords(0, 0);
    first_min_line_->point2->setCoords(0, 1);
    first_max_line_->point1->setCoords(0, 0);
    first_max_line_->point2->setCoords(0, 1);
    second_min_line_->point1->setCoords(0, 0);
    second_min_line_->point2->setCoords(0, 1);
    second_max_line_->point1->setCoords(0, 0);
    second_max_line_->point2->setCoords(0, 1);

    second_min_line_->setPen(QPen(Qt::GlobalColor::red));
    second_max_line_->setPen(QPen(Qt::GlobalColor::red));

    double min = workspace->GetWavelengthMin(row);
    double max = workspace->GetWavelengthMax(row);

    QDoubleValidator *validator = new QDoubleValidator(min, max, 2, this);


    QString label_text = QString::number(min) + "–" + QString::number(max);
    range_label_->setText(label_text);

    uword origin = data_->FindOrigin();

    QVector<double> plot_data = data_->PointSpectrum(origin);
    QVector<double> wavelength = data_->WavelengthQVector();

    first_min_box_->setValidator(validator);
    first_max_box_->setValidator(validator);
    second_min_box_->setValidator(validator);
    second_max_box_->setValidator(validator);
    spectrum_plot_->addGraph();
    spectrum_plot_->graph(0)->addData(wavelength, plot_data);
    spectrum_plot_->rescaleAxes();
    spectrum_plot_->setInteraction(QCP::iRangeDrag, true);
    spectrum_plot_->setInteraction(QCP::iRangeZoom, true);
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
    if (first_min_box_->text().isEmpty() || first_max_box_->text().isEmpty() || second_min_box_->text().isEmpty() || second_max_box_->text().isEmpty()){
        QMessageBox::warning(this, "Invalid Input!", "You must enter numbers for left and right bounds.");
        return;
    }


    double first_entered_min = first_min_box_->text().toDouble();
    double second_entered_min = second_min_box_->text().toDouble();
    double first_entered_max = first_max_box_->text().toDouble();
    double second_entered_max = second_max_box_->text().toDouble();

    if (first_entered_min < workspace->GetWavelengthMin(data_index_) || second_entered_min < workspace->GetWavelengthMin(data_index_)){
        QMessageBox::warning(this, "Invalid Input!", "You have entered a left bound that is smaller than the smallest number on the spectral abscissa");
        return;
    }

    if (first_entered_max > workspace->GetWavelengthMax(data_index_) || second_entered_max > workspace->GetWavelengthMax(data_index_)){
        QMessageBox::warning(this, "Invalid Input!", "You have entered a right bound that is larger than the largest number on the spectral abscissa");
        return;
    }



    QString name = name_box_->text();
    QString value_method = value_method_selector_->currentText();
    //QString integration_method = integration_method_selector_->currentText();

    UnivariateMethod::Method method;
    if (value_method == "Area")
        method = UnivariateMethod::AreaRatio;
    else
        method = UnivariateMethod::IntensityRatio;

    int gradient_index = color_selector_->currentIndex();
    try{
        data_->BandRatio(first_entered_min,
                         first_entered_max,
                         second_entered_min,
                         second_entered_max,
                         name,
                         method,
                         gradient_index);
    }catch(exception e){
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

void BandRatioDialog::on_firstMinLineEdit_textChanged(const QString &arg1)
{
    bool ok;
    double value = arg1.toDouble(&ok);
    if (!ok)
        return;
    first_min_line_->point1->setCoords(value, 0);
    first_min_line_->point2->setCoords(value, 1);
    if (!spectrum_plot_->hasItem(first_min_line_))
        spectrum_plot_->addItem(first_min_line_);
    spectrum_plot_->repaint();
}

void BandRatioDialog::on_firstMaxLineEdit_textChanged(const QString &arg1)
{
    bool ok;
    double value = arg1.toDouble(&ok);
    if (!ok)
        return;
    first_max_line_->point1->setCoords(value, 0);
    first_max_line_->point2->setCoords(value, 1);
    if (!spectrum_plot_->hasItem(first_max_line_))
        spectrum_plot_->addItem(first_max_line_);
    spectrum_plot_->repaint();
}

void BandRatioDialog::on_secondMinLineEdit_textChanged(const QString &arg1)
{
    bool ok;
    double value = arg1.toDouble(&ok);
    if (!ok)
        return;
    second_min_line_->point1->setCoords(value, 0);
    second_min_line_->point2->setCoords(value, 1);
    if (!spectrum_plot_->hasItem(second_min_line_))
        spectrum_plot_->addItem(second_min_line_);
}

void BandRatioDialog::on_secondMaxLineEdit_textChanged(const QString &arg1)
{
    bool ok;
    double value = arg1.toDouble(&ok);
    if (!ok)
        return;
    second_max_line_->point1->setCoords(value, 0);
    second_max_line_->point2->setCoords(value, 1);
    if (!spectrum_plot_->hasItem(second_max_line_))
        spectrum_plot_->addItem(second_max_line_);
}
