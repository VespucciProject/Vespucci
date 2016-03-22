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
#include "GUI/Analysis/bandratiodialog.h"
#include "ui_bandratiodialog.h"

BandRatioDialog::BandRatioDialog(QWidget *parent,
                                 VespucciWorkspace *ws,
                                 const QModelIndex &dataset_index) :
    QDialog(parent),
    ui(new Ui::BandRatioDialog)
{
    ui->setupUi(this);

    workspace = ws;
    data_ = workspace->DatasetAt(dataset_index);

    spectrum_custom_plot_ = findChild<QCustomPlot *>("spectrumPlot");
    first_min_line_edit_ = findChild<QLineEdit *>("firstMinLineEdit");
    first_max_line_edit_ = findChild<QLineEdit *>("firstMaxLineEdit");
    second_min_line_edit_ = findChild<QLineEdit *>("secondMinLineEdit");
    second_max_line_edit_ = findChild<QLineEdit *>("secondMaxLineEdit");
    name_line_edit_ = findChild<QLineEdit *>("nameLineEdit");
    method_combo_box_ = findChild<QComboBox *>("peakComboBox");
    range_label_ = findChild<QLabel *>("rangeLabel");
    search_window_spin_box_ = findChild<QSpinBox*>("searchWindowSpinBox");

    first_min_line_ = new QCPItemStraightLine(spectrum_custom_plot_);
    first_max_line_ = new QCPItemStraightLine(spectrum_custom_plot_);
    second_min_line_ = new QCPItemStraightLine(spectrum_custom_plot_);
    second_max_line_ = new QCPItemStraightLine(spectrum_custom_plot_);

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

    double min = data_->abscissa_ptr()->min();
    double max = data_->abscissa_ptr()->max();

    QDoubleValidator *validator = new QDoubleValidator(min, max, 2, this);


    QString label_text = QString::number(min) + "–" + QString::number(max);
    range_label_->setText(label_text);

    uword origin = data_->FindOrigin();

    QVector<double> plot_data = data_->PointSpectrum(origin);
    QVector<double> wavelength = data_->WavelengthQVector();
    if (plot_data.isEmpty()){plot_data = data_->PointSpectrum(0);}

    first_min_line_edit_->setValidator(validator);
    first_max_line_edit_->setValidator(validator);
    second_min_line_edit_->setValidator(validator);
    second_max_line_edit_->setValidator(validator);
    spectrum_custom_plot_->addGraph();
    spectrum_custom_plot_->graph(0)->addData(wavelength, plot_data);
    spectrum_custom_plot_->rescaleAxes();
    spectrum_custom_plot_->setInteraction(QCP::iRangeDrag, true);
    spectrum_custom_plot_->setInteraction(QCP::iRangeZoom, true);
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
    if (first_min_line_edit_->text().isEmpty() || first_max_line_edit_->text().isEmpty() || second_min_line_edit_->text().isEmpty() || second_max_line_edit_->text().isEmpty()){
        QMessageBox::warning(this, "Invalid Input!", "You must enter numbers for left and right bounds.");
        return;
    }


    double first_entered_min = first_min_line_edit_->text().toDouble();
    double second_entered_min = second_min_line_edit_->text().toDouble();
    double first_entered_max = first_max_line_edit_->text().toDouble();
    double second_entered_max = second_max_line_edit_->text().toDouble();

    if (first_entered_min < data_->abscissa_ptr()->min() || second_entered_min < data_->abscissa_ptr()->min()){
        QMessageBox::warning(this, "Invalid Input!", "You have entered a left bound that is smaller than the smallest number on the spectral abscissa");
        return;
    }

    if (first_entered_max > data_->abscissa_ptr()->min() || second_entered_max > data_->abscissa_ptr()->min()){
        QMessageBox::warning(this, "Invalid Input!", "You have entered a right bound that is larger than the largest number on the spectral abscissa");
        return;
    }
    uint bound_window = search_window_spin_box_->value();


    QString name = name_line_edit_->text();
    QString value_method = method_combo_box_->currentText();

    if (value_method == "Riemann Sum"){
        try{
            data_->BandRatio(name, first_entered_min, first_entered_max,
                             second_entered_min, second_entered_max,
                             bound_window);
        }catch(exception e){
            workspace->main_window()->DisplayExceptionWarning(e);
        }
    }
    else if (value_method == "Gaussian Fit"){
        try{

        }catch(exception e){
            workspace->main_window()->DisplayExceptionWarning(e);
        }
    }
    else{
        QMessageBox::warning(this, "Error Occurred", "A non-fatal error occurred: invalid input from method_combo_box_");
    }

    close();
    data_.clear();
}


///
/// \brief BandRatioDialog::on_buttonBox_rejected
/// The window closes when the user clicks "Cancel"
void BandRatioDialog::on_buttonBox_rejected()
{
    close();
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
    if (!spectrum_custom_plot_->hasItem(first_min_line_))
        spectrum_custom_plot_->addItem(first_min_line_);
    spectrum_custom_plot_->repaint();
}

void BandRatioDialog::on_firstMaxLineEdit_textChanged(const QString &arg1)
{
    bool ok;
    double value = arg1.toDouble(&ok);
    if (!ok)
        return;
    first_max_line_->point1->setCoords(value, 0);
    first_max_line_->point2->setCoords(value, 1);
    if (!spectrum_custom_plot_->hasItem(first_max_line_))
        spectrum_custom_plot_->addItem(first_max_line_);
    spectrum_custom_plot_->repaint();
}

void BandRatioDialog::on_secondMinLineEdit_textChanged(const QString &arg1)
{
    bool ok;
    double value = arg1.toDouble(&ok);
    if (!ok)
        return;
    second_min_line_->point1->setCoords(value, 0);
    second_min_line_->point2->setCoords(value, 1);
    if (!spectrum_custom_plot_->hasItem(second_min_line_))
        spectrum_custom_plot_->addItem(second_min_line_);
}

void BandRatioDialog::on_secondMaxLineEdit_textChanged(const QString &arg1)
{
    bool ok;
    double value = arg1.toDouble(&ok);
    if (!ok)
        return;
    second_max_line_->point1->setCoords(value, 0);
    second_max_line_->point2->setCoords(value, 1);
    if (!spectrum_custom_plot_->hasItem(second_max_line_))
        spectrum_custom_plot_->addItem(second_max_line_);
}

