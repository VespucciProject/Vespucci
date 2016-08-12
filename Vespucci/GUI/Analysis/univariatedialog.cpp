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
#include "GUI/Analysis/univariatedialog.h"
#include "ui_univariatedialog.h"
#include "Data/Analysis/multianalyzer.h"

///
/// \brief UnivariateDialog::UnivariateDialog
/// \param parent See QDialog
/// \param ws Current workspace
/// \param row Selected row in the dataset list widget
///
UnivariateDialog::UnivariateDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws, const QString &dataset_key) :
    QDialog(parent),
    ui(new Ui::UnivariateDialog)
{
    ui->setupUi(this);
    workspace_ = ws;
    dataset_ = workspace_->GetDataset(dataset_key);

    min_line_ = new QCPItemStraightLine(ui->spectrumPlot);
    min_line_->point1->setCoords(0, 0);
    min_line_->point2->setCoords(0, 1);
    max_line_ = new QCPItemStraightLine(ui->spectrumPlot);
    max_line_->point1->setCoords(0, 0);
    max_line_->point2->setCoords(0, 1);
    double min, max;
    try{
        min = dataset_->wavelength_ptr()->min();
        max = dataset_->wavelength_ptr()->max();
    }
    catch(exception e){
        cerr << e.what();
        workspace_->main_window()->DisplayExceptionWarning(e);
        min = 0;
        max = 0;
    }

    QString label_text = QString::number(min) + "–" + QString::number(max);
    ui->rangeLabel->setText(label_text);

    ui->minLineEdit->setValidator(new QDoubleValidator(min, max, 2, this));
    ui->maxLineEdit->setValidator(new QDoubleValidator(min, max, 2, this));

    uword origin = dataset_->FindOrigin();
    QVector<double> plot_data, wavelength;

    try{
        plot_data = dataset_->PointSpectrum(origin);
        wavelength = dataset_->WavelengthQVector();
    }
    catch(exception e){}
    if (plot_data.isEmpty()){plot_data = dataset_->PointSpectrum(0);}
    ui->spectrumPlot->addGraph();
    ui->spectrumPlot->graph(0)->addData(wavelength, plot_data);
    ui->spectrumPlot->rescaleAxes();
    ui->spectrumPlot->setInteraction(QCP::iRangeDrag, true);
    ui->spectrumPlot->setInteraction(QCP::iRangeZoom, true);

}

UnivariateDialog::UnivariateDialog(QSharedPointer<VespucciWorkspace> ws, const QStringList &dataset_keys)
    :QDialog(ws->main_window()),
      ui(new Ui::UnivariateDialog)
{
    ui->setupUi(this);
    workspace_ = ws;
    dataset_keys_ = dataset_keys;
    if (dataset_keys_.isEmpty()){
        close();
        return;
    }

    dataset_ = workspace_->GetDataset(dataset_keys.first());

    min_line_ = new QCPItemStraightLine(ui->spectrumPlot);
    min_line_->point1->setCoords(0, 0);
    min_line_->point2->setCoords(0, 1);
    max_line_ = new QCPItemStraightLine(ui->spectrumPlot);
    max_line_->point1->setCoords(0, 0);
    max_line_->point2->setCoords(0, 1);
    double min, max;
    try{
        min = dataset_->wavelength_ptr()->min();
        max = dataset_->wavelength_ptr()->max();
    }
    catch(exception e){
        cerr << e.what();
        workspace_->main_window()->DisplayExceptionWarning(e);
        min = 0;
        max = 0;
    }

    QString label_text = QString::number(min) + "–" + QString::number(max);
    ui->rangeLabel->setText(label_text);

    ui->minLineEdit->setValidator(new QDoubleValidator(min, max, 2, this));
    ui->maxLineEdit->setValidator(new QDoubleValidator(min, max, 2, this));

    uword origin = dataset_->FindOrigin();
    QVector<double> plot_data, wavelength;

    try{
        plot_data = dataset_->PointSpectrum(origin);
        wavelength = dataset_->WavelengthQVector();
    }
    catch(exception e){}
    if (plot_data.isEmpty()){plot_data = dataset_->PointSpectrum(0);}
    ui->spectrumPlot->addGraph();
    ui->spectrumPlot->graph(0)->addData(wavelength, plot_data);
    ui->spectrumPlot->rescaleAxes();
    ui->spectrumPlot->setInteraction(QCP::iRangeDrag, true);
    ui->spectrumPlot->setInteraction(QCP::iRangeZoom, true);
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
    close();
    if (ui->minLineEdit->text().isEmpty() || ui->maxLineEdit->text().isEmpty()){
        QMessageBox::warning(this, "Invalid Input!", "You must enter numbers for left and right bounds.");
        return;
    }
    double entered_min = ui->minLineEdit->text().toDouble();
    double entered_max = ui->maxLineEdit->text().toDouble();
    uint bound_window = ui->searchWindowSpinBox->value();
    QString name = ui->nameLineEdit->text();
    if(!name.size()){
        name = "Univariate " + QString::number(dataset_->UnivariateCount());
    }
    QString value_method = ui->methodComboBox->currentText();

    if (!dataset_keys_.isEmpty()){
        try{
            MultiAnalyzer analyzer(workspace_, dataset_keys_);
            analyzer.Univariate(name, entered_min, entered_max, bound_window);
        }catch(exception e){
            workspace_->main_window()->DisplayExceptionWarning(e);
        }
        close();
        return;
    }

    if (value_method == "Empirical"){
        try{
            dataset_->Univariate(name, entered_min, entered_max, bound_window);
        }catch(exception e){
            workspace_->main_window()->DisplayExceptionWarning(e);
        }
    }
    else if (value_method == "Gaussian Fit"){
        try{

        }catch(exception e){

        }
    }
    else{}
    dataset_.clear();
}


///
/// \brief UnivariateDialog::on_buttonBox_rejected
/// Close window when "Cancel" clicked.
void UnivariateDialog::on_buttonBox_rejected()
{
        dataset_.clear();
}

void UnivariateDialog::on_minLineEdit_textChanged(const QString &arg1)
{
    bool ok;
    double value = arg1.toDouble(&ok);
    if (!ok)
        return;

    min_line_->point1->setCoords(value, 0);
    min_line_->point2->setCoords(value, 1);
    if(!ui->spectrumPlot->hasItem(min_line_))
        ui->spectrumPlot->addItem(min_line_);
    ui->spectrumPlot->replot(QCustomPlot::rpImmediate);
}

void UnivariateDialog::on_maxLineEdit_textChanged(const QString &arg1)
{
    bool ok;
    double value = arg1.toDouble(&ok);
    if (!ok)
        return;

    max_line_->point1->setCoords(value, 0);
    max_line_->point2->setCoords(value, 1);

    if(!ui->spectrumPlot->hasItem(max_line_))
        ui->spectrumPlot->addItem(max_line_);
    ui->spectrumPlot->replot(QCustomPlot::rpImmediate);
}
