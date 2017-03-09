/*******************************************************************************
    Copyright (C) 2014-2017 Wright State University - All Rights Reserved
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
#include "Data/Analysis/multianalyzer.h"

BandRatioDialog::BandRatioDialog(QWidget *parent,
                                 QSharedPointer<VespucciWorkspace> ws,
                                 QSharedPointer<AbstractDataAnalyzer> analyzer) :
    QDialog(parent),
    ui(new Ui::BandRatioDialog),
    workspace_(ws),
    analyzer_(analyzer)
{
    ui->setupUi(this);

    first_min_line_ = new QCPItemStraightLine(ui->spectrumCustomPlot);
    first_max_line_ = new QCPItemStraightLine(ui->spectrumCustomPlot);
    second_min_line_ = new QCPItemStraightLine(ui->spectrumCustomPlot);
    second_max_line_ = new QCPItemStraightLine(ui->spectrumCustomPlot);

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

    double min = analyzer_->AbscissaMin();
    double max = analyzer_->AbscissaMax();

    QDoubleValidator *validator = new QDoubleValidator(min, max, 2, this);


    QString label_text = QString::number(min) + "–" + QString::number(max);
    ui->rangeLabel->setText(label_text);
    ui->indexSpinBox->setRange(0, analyzer_->columns() - 1);

    uword middle = analyzer_->columns() / 2;
    ui->indexSpinBox->setValue(middle);

    QVector<double> plot_data = Vespucci::FromArmaVec(analyzer_->PointSpectrum(middle));
    QVector<double> wavelength = Vespucci::FromArmaVec(analyzer_->abscissa());
    if (plot_data.isEmpty()){plot_data = Vespucci::FromArmaVec(analyzer_->PointSpectrum(0));}

    ui->firstMinLineEdit->setValidator(validator);
    ui->firstMaxLineEdit->setValidator(validator);
    ui->secondMinLineEdit->setValidator(validator);
    ui->secondMaxLineEdit->setValidator(validator);
    ui->spectrumCustomPlot->addGraph();
    ui->spectrumCustomPlot->graph(0)->addData(wavelength, plot_data);
    ui->spectrumCustomPlot->rescaleAxes();
    ui->spectrumCustomPlot->setInteraction(QCP::iRangeDrag, true);
    ui->spectrumCustomPlot->setInteraction(QCP::iRangeZoom, true);
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
    bool ok = !ui->firstMinLineEdit->text().isEmpty()
              && !ui->firstMaxLineEdit->text().isEmpty()
              && !ui->secondMinLineEdit->text().isEmpty()
              && !ui->secondMaxLineEdit->text().isEmpty();

    if (ok){
        double first_entered_min = ui->firstMinLineEdit->text().toDouble();
        double second_entered_min = ui->secondMinLineEdit->text().toDouble();
        double first_entered_max = ui->firstMaxLineEdit->text().toDouble();
        double second_entered_max = ui->secondMaxLineEdit->text().toDouble();

        if (first_entered_min < analyzer_->AbscissaMin()
                || second_entered_min < analyzer_->AbscissaMin()){
            QMessageBox::warning(this,
                                 "Invalid Input!",
                                 "You have entered a left bound that is smaller "
                                 "than the smallest number on the spectral abscissa");
            return;
        }

        if (first_entered_max > analyzer_->AbscissaMax()
                || second_entered_max > analyzer_->AbscissaMax()){
            QMessageBox::warning(this,
                                 "Invalid Input!",
                                 "You have entered a right bound that is larger "
                                 "than the largest number on the spectral abscissa");
            return;
        }
        uint bound_window = ui->searchWindowSpinBox->value();


        QString name = ui->nameLineEdit->text();
        QString value_method = ui->methodComboBox->currentText();

        if (value_method == "Empirical"){
            try{
                analyzer_->BandRatio(name, first_entered_min, first_entered_max,
                                 second_entered_min, second_entered_max,
                                 bound_window);
            }catch(exception e){
                workspace_->main_window()->DisplayExceptionWarning(e);
            }
        }
        else if (value_method == "Gaussian Fit"){
            try{

            }catch(exception e){
                workspace_->main_window()->DisplayExceptionWarning(e);
            }
        }
        else{
            QMessageBox::warning(this, "Error Occurred",
                                 "A non-fatal error occurred: invalid input from ui->methodComboBox");
        }
    }
    else{
        QMessageBox::warning(this,
                             "Invalid Input!",
                             "You must enter numbers for left and right bounds.");
    }
    analyzer_.clear();
    close();
}


///
/// \brief BandRatioDialog::on_buttonBox_rejected
/// The window closes when the user clicks "Cancel"
void BandRatioDialog::on_buttonBox_rejected()
{
    close();
    analyzer_.clear();
}

void BandRatioDialog::on_firstMinLineEdit_textChanged(const QString &arg1)
{
    bool ok;
    double value = arg1.toDouble(&ok);
    if (!ok)
        return;
    first_min_line_->point1->setCoords(value, 0);
    first_min_line_->point2->setCoords(value, 1);
    if (!ui->spectrumCustomPlot->hasItem(first_min_line_))
        ui->spectrumCustomPlot->addItem(first_min_line_);
    ui->spectrumCustomPlot->repaint();
}

void BandRatioDialog::on_firstMaxLineEdit_textChanged(const QString &arg1)
{
    bool ok;
    double value = arg1.toDouble(&ok);
    if (!ok)
        return;
    first_max_line_->point1->setCoords(value, 0);
    first_max_line_->point2->setCoords(value, 1);
    if (!ui->spectrumCustomPlot->hasItem(first_max_line_))
        ui->spectrumCustomPlot->addItem(first_max_line_);
    ui->spectrumCustomPlot->repaint();
}

void BandRatioDialog::on_secondMinLineEdit_textChanged(const QString &arg1)
{
    bool ok;
    double value = arg1.toDouble(&ok);
    if (!ok)
        return;
    second_min_line_->point1->setCoords(value, 0);
    second_min_line_->point2->setCoords(value, 1);
    if (!ui->spectrumCustomPlot->hasItem(second_min_line_))
        ui->spectrumCustomPlot->addItem(second_min_line_);
}

void BandRatioDialog::on_secondMaxLineEdit_textChanged(const QString &arg1)
{
    bool ok;
    double value = arg1.toDouble(&ok);
    if (!ok)
        return;
    second_max_line_->point1->setCoords(value, 0);
    second_max_line_->point2->setCoords(value, 1);
    if (!ui->spectrumCustomPlot->hasItem(second_max_line_))
        ui->spectrumCustomPlot->addItem(second_max_line_);
}


void BandRatioDialog::on_indexSpinBox_editingFinished()
{
    size_t index = ui->indexSpinBox->value();
    QVector<double> plot_data = Vespucci::FromArmaVec(analyzer_->PointSpectrum(index));
    QVector<double> abscissa = Vespucci::FromArmaVec(analyzer_->abscissa());
    ui->spectrumCustomPlot->graph(0)->clearData();
    ui->spectrumCustomPlot->graph(0)->addData(abscissa, plot_data);
    ui->spectrumCustomPlot->replot(QCustomPlot::rpImmediate);
}
