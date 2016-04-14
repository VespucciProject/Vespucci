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
#include "GUI/Processing/baselinedialog.h"
#include "ui_baselinedialog.h"

///
/// \brief BaselineDialog::BaselineDialog
/// \param parent Parent widget, required for QDialog
/// \param ws The current workspace
/// \param row The currently selected row in the dataset list widget
///
BaselineDialog::BaselineDialog(QWidget *parent, VespucciWorkspace *ws, const QString &dataset_key) :
    QDialog(parent),
    ui(new Ui::BaselineDialog)
{
    ui->setupUi(this);
    workspace = ws;
    dataset_ = workspace->GetDataset(dataset_key);

    param_0_spin_box_ = findChild<QSpinBox *>("param_0SpinBox");
    method_box_ = findChild<QComboBox *>("methodComboBox");
    param_1_spin_box_ = findChild<QSpinBox *>("param_1SpinBox");
    param_2_double_spin_box_ = findChild<QDoubleSpinBox *>("param_2DoubleSpinBox");
    param_3_double_spin_box_ = findChild<QDoubleSpinBox *>("param_3DoubleSpinBox");

    param_0_label_ = findChild<QLabel *>("param_0Label");
    param_1_label_ = findChild<QLabel *>("param_1Label");
    param_2_label_ = findChild<QLabel *>("param_2Label");
    param_3_label_ = findChild<QLabel *>("param_3Label");

    spectrum_plot_ = findChild<QCustomPlot *>("spectrumPlot");

    param_0_label_->setText("Window Size");
    param_1_label_->setText("Iterations");
    param_0_label_->setVisible(true);
    param_1_label_->setVisible(true);
    param_0_spin_box_->setVisible(true);
    param_1_spin_box_->setVisible(true);

    param_2_double_spin_box_->setVisible(false);
    param_3_double_spin_box_->setVisible(false);
    param_2_label_->setVisible(false);
    param_3_label_->setVisible(false);

    spectrum_q_ = dataset_->PointSpectrum(0);
    abscissa_q_ = dataset_->WavelengthQVector();
    arma::uvec ind = {0};
    spectrum_ = dataset_->spectra(ind);
    abscissa_ = dataset_->abscissa();

    spectrum_plot_->setInteraction(QCP::iRangeDrag, true);
    spectrum_plot_->setInteraction(QCP::iRangeZoom, true);
    spectrum_plot_->addGraph();
    spectrum_plot_->graph(0)->setData(abscissa_q_, spectrum_q_);
    spectrum_plot_->xAxis->setRange(dataset_->WavelengthRange());
    spectrum_plot_->yAxis->setRange(0, spectrum_.max());
    spectrum_plot_->replot();
}

BaselineDialog::~BaselineDialog()
{
    delete ui;
}

///
/// \brief BaselineDialog::on_buttonBox_accepted
/// When the user clicks "Ok", the baseline method of the dataset is called to
/// correct the baseline of each spectrum.
void BaselineDialog::on_buttonBox_accepted()
{

    close();
    QString method = method_box_->currentText();

    if (method == "Median Filter"){
        int window_size = param_0_spin_box_->value();
        int iterations = param_1_spin_box_->value();
        try{
            dataset_->MFBaseline(window_size, iterations);
        }
        catch(exception e){
            workspace->main_window()->DisplayExceptionWarning(e);
        }
    }
    else if (method == "Vancouver Raman Algorithm (IModPoly)"){
        int poly_order = param_0_spin_box_->value();
        int max_it = param_1_spin_box_->value();
        double threshold = param_2_double_spin_box_->value();
        try{
            dataset_->IModPolyBaseline(poly_order, max_it, threshold);
        }catch(exception e){
            workspace->main_window()->DisplayExceptionWarning(e);
        }
    }
    else if (method == "CWT"){
        int lambda = param_0_spin_box_->value();
        int penalty_order = param_1_spin_box_->value();
        double SNR_threshold = param_2_double_spin_box_->value();
        double peak_shape_threshold = param_3_double_spin_box_->value();
        try{
            dataset_->CWTBaseline(lambda, penalty_order, SNR_threshold, peak_shape_threshold);
        }catch(exception e){
            workspace->main_window()->DisplayExceptionWarning(e);
        }
    }
    else{
        QMessageBox::warning(this, "Error", "An Error Occurred in BaselineDialog");
    }

    dataset_.clear();
}

///
/// \brief BaselineDialog::on_buttonBox_rejected
/// Window closes when user clicks "Cancel"
void BaselineDialog::on_buttonBox_rejected()
{
    close();
    dataset_.clear();
}

void BaselineDialog::on_methodComboBox_currentTextChanged(const QString &arg1)
{
    if (arg1 == "Median Filter"){
        param_0_label_->setText("Window Size");
        param_1_label_->setText("Iterations");
        param_0_label_->setVisible(true);
        param_1_label_->setVisible(true);
        param_0_spin_box_->setVisible(true);
        param_1_spin_box_->setVisible(true);
        param_1_spin_box_->setValue(1);
        param_0_spin_box_->setValue(35);
        param_0_spin_box_->setSingleStep(2);
        param_0_spin_box_->setToolTip("Only odd window sizes are allowed. "
                                      "If you enter an even number, "
                                      "it will be rounded up.");
        param_0_spin_box_->setMinimum(3);

        param_2_double_spin_box_->setVisible(false);
        param_3_double_spin_box_->setVisible(false);
        param_2_label_->setVisible(false);
        param_3_label_->setVisible(false);
    }
    else if (arg1 == "Vancouver Raman Algorithm (IModPoly)"){
        param_0_label_->setText("Polynomial Order");
        param_0_label_->setVisible(true);
        param_0_spin_box_->setVisible(true);
        param_0_spin_box_->setValue(5);
        param_0_spin_box_->setSingleStep(1);
        param_0_spin_box_->setMinimum(1);

        param_1_label_->setText("Maximum Iterations");
        param_1_label_->setVisible(true);
        param_1_spin_box_->setVisible(true);
        param_1_spin_box_->setRange(1, 1000);
        param_1_spin_box_->setValue(100);


        param_2_label_->setText("Threshold");
        param_2_label_->setVisible(true);
        param_2_double_spin_box_->setVisible(true);
        param_2_double_spin_box_->setRange(0, 1);
        param_2_double_spin_box_->setValue(0.005);

        param_3_label_->setVisible(false);
        param_3_double_spin_box_->setVisible(false);
    }
    else if (arg1 == "CWT"){
        param_0_label_->setText("Whittaker Lambda");
        param_0_label_->setVisible(true);
        param_0_spin_box_->setVisible(true);

        param_1_label_->setText("Whittaker Penalty Order");
        param_1_label_->setVisible(true);
        param_1_spin_box_->setVisible(true);

        param_2_label_->setText("Peak SNR Threshold");
        param_2_label_->setVisible(true);
        param_2_double_spin_box_->setVisible(true);
        param_2_double_spin_box_->setValue(3.0);
        param_2_double_spin_box_->setRange(0, 10000);

        param_3_label_->setText("Peak Shape Threshold");
        param_3_label_->setVisible(false);
        param_3_double_spin_box_->setVisible(false);
        param_3_double_spin_box_->setValue(0.50);
        param_3_double_spin_box_->setRange(0, 1);
    }
}

void BaselineDialog::on_pushButton_clicked()
{
    typedef std::vector<double> stdvec;
    typedef QVector<double> qvec;
    using namespace arma;
    QString method = method_box_->currentText();
    if (spectrum_plot_->graph(1))
        spectrum_plot_->graph(1)->clearData();
    else
        spectrum_plot_->addGraph(spectrum_plot_->graph(0)->keyAxis(), spectrum_plot_->graph(0)->valueAxis());
    if (spectrum_plot_->graph(2))
        spectrum_plot_->graph(2)->clearData();
    else
        spectrum_plot_->addGraph(spectrum_plot_->graph(0)->keyAxis(), spectrum_plot_->graph(0)->valueAxis());


    if (method == "Median Filter"){
        int window_size = param_0_spin_box_->value();
        if (!(window_size % 2)){window_size += 1;} //round up if even.
        int iterations = param_1_spin_box_->value();
        vec baseline = spectrum_;

        try{
            for (uword i = 0; i < iterations; ++i){

                baseline = Vespucci::Math::Smoothing::MedianFilter(baseline, window_size);
            }
            vec corrected = spectrum_ - baseline;
            qvec baseline_q = qvec::fromStdVector(conv_to<stdvec>::from(baseline));
            qvec corrected_q = qvec::fromStdVector(conv_to<stdvec>::from(corrected));
            spectrum_plot_->graph(1)->setPen(QColor("black"));
            spectrum_plot_->graph(1)->setData(abscissa_q_, baseline_q);
            spectrum_plot_->graph(2)->setPen(QColor("red"));
            spectrum_plot_->graph(2)->setData(abscissa_q_, corrected_q);
        }
        catch(exception e){
            workspace->main_window()->DisplayExceptionWarning(e);
        }
    }
    else if (method == "Vancouver Raman Algorithm (IModPoly)"){
        int poly_order = param_0_spin_box_->value();
        int max_it = param_1_spin_box_->value();
        double threshold = param_2_double_spin_box_->value();
        vec baseline, corrected;
        double err;
        try{
            corrected = Vespucci::Math::LinLeastSq::IModPoly(spectrum_, abscissa_,
                                                             baseline, corrected,
                                                             err, poly_order,
                                                             max_it, threshold);
            qvec baseline_q = qvec::fromStdVector(conv_to<stdvec>::from(baseline));
            qvec corrected_q = qvec::fromStdVector(conv_to<stdvec>::from(corrected));
            spectrum_plot_->graph(1)->setPen(QColor("black"));
            spectrum_plot_->graph(1)->setData(abscissa_q_, baseline_q);
            spectrum_plot_->graph(2)->setPen(QColor("red"));
            spectrum_plot_->graph(2)->setData(abscissa_q_, corrected_q);
        }catch(exception e){
            workspace->main_window()->DisplayExceptionWarning(e);
        }
    }
    else if (method == "CWT"){
        //not implemented yet

        /*
        int lambda = param_0_spin_box_->value();
        int penalty_order = param_1_spin_box_->value();
        double SNR_threshold = param_2_double_spin_box_->value();
        double peak_shape_threshold = param_3_double_spin_box_->value();
        try{
            dataset_->CWTBaseline(lambda, penalty_order, SNR_threshold, peak_shape_threshold);
        }catch(exception e){
            workspace->main_window()->DisplayExceptionWarning(e);
        }
        */
    }
    else{
        QMessageBox::warning(this, "Error", "An Error Occurred in BaselineDialog");
    }
    spectrum_plot_->replot();
}
