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
UnivariateDialog::UnivariateDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws, QSharedPointer<AbstractDataAnalyzer> analyzer) :
    QDialog(parent),
    ui(new Ui::UnivariateDialog),
    workspace_(ws),
    analyzer_(analyzer)
{
    ui->setupUi(this);
    min_line_ = new QCPItemStraightLine(ui->spectrumPlot);
    min_line_->point1->setCoords(0, 0);
    min_line_->point2->setCoords(0, 1);
    max_line_ = new QCPItemStraightLine(ui->spectrumPlot);
    max_line_->point1->setCoords(0, 0);
    max_line_->point2->setCoords(0, 1);
    double min, max;
    try{
        min = analyzer_->AbscissaMin();
        max = analyzer_->AbscissaMax();
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

    uword middle = analyzer_->columns() / 2;
    ui->indexSpinBox->setRange(0, analyzer_->columns() - 1);
    ui->indexSpinBox->setValue(middle);
    QVector<double> plot_data, wavelength;

    try{
        plot_data = Vespucci::FromArmaVec(analyzer_->PointSpectrum(middle));
        wavelength = Vespucci::FromArmaVec(analyzer_->abscissa());
    }
    catch(exception e){}
    if (plot_data.isEmpty())
        plot_data = Vespucci::FromArmaVec(analyzer_->PointSpectrum(0));
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
        name = "Univariate";
    }
    QString value_method = ui->methodComboBox->currentText();
    if (!analyzer_.isNull()){
        if (value_method == "Empirical"){
            try{
                analyzer_->Univariate(name, entered_min, entered_max, bound_window);
            }catch(exception e){
                workspace_->main_window()->DisplayExceptionWarning(e);
            }
        }
        else{
            try{
                analyzer_->FitPeak(name, value_method, entered_min, entered_max);
            }catch(exception e){

            }
        }
    }
    analyzer_.clear();
    close();
}


///
/// \brief UnivariateDialog::on_buttonBox_rejected
/// Close window when "Cancel" clicked.
void UnivariateDialog::on_buttonBox_rejected()
{
    analyzer_.clear();
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

void UnivariateDialog::on_indexSpinBox_editingFinished()
{
    size_t index = ui->indexSpinBox->value();
    QVector<double> plot_data = Vespucci::FromArmaVec(analyzer_->PointSpectrum(index));
    QVector<double> abscissa = Vespucci::FromArmaVec(analyzer_->abscissa());
    ui->spectrumPlot->graph(0)->clearData();
    ui->spectrumPlot->graph(0)->addData(abscissa, plot_data);
    ui->spectrumPlot->replot(QCustomPlot::rpImmediate);
}
