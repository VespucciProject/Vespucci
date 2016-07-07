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
BaselineDialog::BaselineDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws, const QString &dataset_key) :
    QDialog(parent),
    ui(new Ui::BaselineDialog)
{
    ui->setupUi(this);
    workspace_ = ws;
    dataset_ = workspace_->GetDataset(dataset_key);

    ui->param_0Label->setText("Window Size");
    ui->param_1Label->setText("Iterations");
    ui->param_0Label->setVisible(true);
    ui->param_1Label->setVisible(true);
    ui->param_0SpinBox->setVisible(true);
    ui->param_1SpinBox->setVisible(true);

    ui->param_2DoubleSpinBox->setVisible(false);
    ui->param_3DoubleSpinBox->setVisible(false);
    ui->param_2Label->setVisible(false);
    ui->param_3Label->setVisible(false);

    spectrum_q_ = dataset_->PointSpectrum(0);
    abscissa_q_ = dataset_->WavelengthQVector();
    arma::uvec ind = {0};
    spectrum_ = dataset_->spectra(ind);
    abscissa_ = dataset_->abscissa();

    ui->spectrumPlot->setInteraction(QCP::iRangeDrag, true);
    ui->spectrumPlot->setInteraction(QCP::iRangeZoom, true);
    ui->spectrumPlot->addGraph();
    ui->spectrumPlot->graph(0)->setData(abscissa_q_, spectrum_q_);
    ui->spectrumPlot->xAxis->setRange(dataset_->WavelengthRange());
    ui->spectrumPlot->yAxis->setRange(0, spectrum_.max());
    ui->spectrumPlot->replot();
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
    QString method = ui->methodComboBox->currentText();

    if (method == "Median Filter"){
        int window_size = ui->param_0SpinBox->value();
        int iterations = ui->param_1SpinBox->value();
        try{
            dataset_->MFBaseline(window_size, iterations);
        }
        catch(exception e){
            workspace_->main_window()->DisplayExceptionWarning(e);
        }
    }
    else if (method == "Vancouver Raman Algorithm (IModPoly)"){
        int poly_order = ui->param_0SpinBox->value();
        int max_it = ui->param_1SpinBox->value();
        double threshold = ui->param_2DoubleSpinBox->value();
        try{
            dataset_->IModPolyBaseline(poly_order, max_it, threshold);
        }catch(exception e){
            workspace_->main_window()->DisplayExceptionWarning(e);
        }
    }
    else if (method == "CWT"){
        int lambda = ui->param_0SpinBox->value();
        int penalty_order = ui->param_1SpinBox->value();
        double SNR_threshold = ui->param_2DoubleSpinBox->value();
        double peak_shape_threshold = ui->param_3DoubleSpinBox->value();
        try{
            dataset_->CWTBaseline(lambda, penalty_order, SNR_threshold, peak_shape_threshold);
        }catch(exception e){
            workspace_->main_window()->DisplayExceptionWarning(e);
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
        ui->param_0Label->setText("Window Size");
        ui->param_1Label->setText("Iterations");
        ui->param_0Label->setVisible(true);
        ui->param_1Label->setVisible(true);
        ui->param_0SpinBox->setVisible(true);
        ui->param_1SpinBox->setVisible(true);
        ui->param_1SpinBox->setValue(1);
        ui->param_0SpinBox->setValue(35);
        ui->param_0SpinBox->setSingleStep(2);
        ui->param_0SpinBox->setToolTip("Only odd window sizes are allowed. "
                                      "If you enter an even number, "
                                      "it will be rounded up.");
        ui->param_0SpinBox->setMinimum(3);

        ui->param_2DoubleSpinBox->setVisible(false);
        ui->param_3DoubleSpinBox->setVisible(false);
        ui->param_2Label->setVisible(false);
        ui->param_3Label->setVisible(false);
    }
    else if (arg1 == "Vancouver Raman Algorithm (IModPoly)"){
        ui->param_0Label->setText("Polynomial Order");
        ui->param_0Label->setVisible(true);
        ui->param_0SpinBox->setVisible(true);
        ui->param_0SpinBox->setValue(5);
        ui->param_0SpinBox->setSingleStep(1);
        ui->param_0SpinBox->setMinimum(1);

        ui->param_1Label->setText("Maximum Iterations");
        ui->param_1Label->setVisible(true);
        ui->param_1SpinBox->setVisible(true);
        ui->param_1SpinBox->setRange(1, 1000);
        ui->param_1SpinBox->setValue(100);


        ui->param_2Label->setText("Threshold");
        ui->param_2Label->setVisible(true);
        ui->param_2DoubleSpinBox->setVisible(true);
        ui->param_2DoubleSpinBox->setRange(0, 1);
        ui->param_2DoubleSpinBox->setValue(0.005);

        ui->param_3Label->setVisible(false);
        ui->param_3DoubleSpinBox->setVisible(false);
    }
    else if (arg1 == "CWT"){
        ui->param_0Label->setText("Whittaker Lambda");
        ui->param_0Label->setVisible(true);
        ui->param_0SpinBox->setVisible(true);

        ui->param_1Label->setText("Whittaker Penalty Order");
        ui->param_1Label->setVisible(true);
        ui->param_1SpinBox->setVisible(true);

        ui->param_2Label->setText("Peak SNR Threshold");
        ui->param_2Label->setVisible(true);
        ui->param_2DoubleSpinBox->setVisible(true);
        ui->param_2DoubleSpinBox->setValue(3.0);
        ui->param_2DoubleSpinBox->setRange(0, 10000);

        ui->param_3Label->setText("Peak Shape Threshold");
        ui->param_3Label->setVisible(false);
        ui->param_3DoubleSpinBox->setVisible(false);
        ui->param_3DoubleSpinBox->setValue(0.50);
        ui->param_3DoubleSpinBox->setRange(0, 1);
    }
}

void BaselineDialog::on_pushButton_clicked()
{
    typedef std::vector<double> stdvec;
    typedef QVector<double> qvec;
    using namespace arma;
    QString method = ui->methodComboBox->currentText();
    if (ui->spectrumPlot->graph(1))
        ui->spectrumPlot->graph(1)->clearData();
    else
        ui->spectrumPlot->addGraph(ui->spectrumPlot->graph(0)->keyAxis(), ui->spectrumPlot->graph(0)->valueAxis());
    if (ui->spectrumPlot->graph(2))
        ui->spectrumPlot->graph(2)->clearData();
    else
        ui->spectrumPlot->addGraph(ui->spectrumPlot->graph(0)->keyAxis(), ui->spectrumPlot->graph(0)->valueAxis());


    if (method == "Median Filter"){
        int window_size = ui->param_0SpinBox->value();
        if (!(window_size % 2)){window_size += 1;} //round up if even.
        int iterations = ui->param_1SpinBox->value();
        vec baseline = spectrum_;

        try{
            for (uword i = 0; i < iterations; ++i){

                baseline = Vespucci::Math::Smoothing::MedianFilter(baseline, window_size);
            }
            vec corrected = spectrum_ - baseline;
            qvec baseline_q = qvec::fromStdVector(conv_to<stdvec>::from(baseline));
            qvec corrected_q = qvec::fromStdVector(conv_to<stdvec>::from(corrected));
            ui->spectrumPlot->graph(1)->setPen(QColor("black"));
            ui->spectrumPlot->graph(1)->setData(abscissa_q_, baseline_q);
            ui->spectrumPlot->graph(2)->setPen(QColor("red"));
            ui->spectrumPlot->graph(2)->setData(abscissa_q_, corrected_q);
        }
        catch(exception e){
            workspace_->main_window()->DisplayExceptionWarning(e);
        }
    }
    else if (method == "Vancouver Raman Algorithm (IModPoly)"){
        int poly_order = ui->param_0SpinBox->value();
        int max_it = ui->param_1SpinBox->value();
        double threshold = ui->param_2DoubleSpinBox->value();
        vec baseline, corrected;
        double err;
        try{
            corrected = Vespucci::Math::LinLeastSq::IModPoly(spectrum_, abscissa_,
                                                             baseline, corrected,
                                                             err, poly_order,
                                                             max_it, threshold);
            qvec baseline_q = qvec::fromStdVector(conv_to<stdvec>::from(baseline));
            qvec corrected_q = qvec::fromStdVector(conv_to<stdvec>::from(corrected));
            ui->spectrumPlot->graph(1)->setPen(QColor("black"));
            ui->spectrumPlot->graph(1)->setData(abscissa_q_, baseline_q);
            ui->spectrumPlot->graph(2)->setPen(QColor("red"));
            ui->spectrumPlot->graph(2)->setData(abscissa_q_, corrected_q);
        }catch(exception e){
            workspace_->main_window()->DisplayExceptionWarning(e);
        }
    }
    else if (method == "CWT"){
        //not implemented yet

        /*
        int lambda = ui->param_0SpinBox->value();
        int penalty_order = ui->param_1SpinBox->value();
        double SNR_threshold = ui->param_2DoubleSpinBox->value();
        double peak_shape_threshold = ui->param_3DoubleSpinBox->value();
        try{
            dataset_->CWTBaseline(lambda, penalty_order, SNR_threshold, peak_shape_threshold);
        }catch(exception e){
            workspace_->main_window()->DisplayExceptionWarning(e);
        }
        */
    }
    else{
        QMessageBox::warning(this, "Error", "An Error Occurred in BaselineDialog");
    }
    ui->spectrumPlot->replot();
}
