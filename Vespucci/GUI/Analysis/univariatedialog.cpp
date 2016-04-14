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

///
/// \brief UnivariateDialog::UnivariateDialog
/// \param parent See QDialog
/// \param ws Current workspace
/// \param row Selected row in the dataset list widget
///
UnivariateDialog::UnivariateDialog(QWidget *parent, VespucciWorkspace *ws, const QString &dataset_key) :
    QDialog(parent),
    ui(new Ui::UnivariateDialog)
{
    ui->setupUi(this);
    workspace = ws;
    dataset_ = workspace->GetDataset(dataset_key);

    min_line_edit_ = findChild<QLineEdit *>("minLineEdit");
    max_line_edit_ = findChild<QLineEdit *>("maxLineEdit");
    name_line_edit_ = findChild<QLineEdit *>("nameLineEdit");
    spectrum_plot_ = findChild<QCustomPlot *>("spectrumPlot");
    value_method_combo_box_ = findChild<QComboBox *>("peakComboBox");
    range_label_ = findChild<QLabel *>("rangeLabel");
    map_check_box_ = findChild<QCheckBox *>("mapCheckBox");
    search_window_spin_box_ = findChild<QSpinBox *>("searchWindowSpinBox");

    min_line_ = new QCPItemStraightLine(spectrum_plot_);
    min_line_->point1->setCoords(0, 0);
    min_line_->point2->setCoords(0, 1);
    max_line_ = new QCPItemStraightLine(spectrum_plot_);
    max_line_->point1->setCoords(0, 0);
    max_line_->point2->setCoords(0, 1);
    double min, max;
    try{
        min = dataset_->wavelength_ptr()->min();
        max = dataset_->wavelength_ptr()->max();
    }
    catch(exception e){
        cerr << e.what();
        workspace->main_window()->DisplayExceptionWarning(e);
        min = 0;
        max = 0;
    }

    QString label_text = QString::number(min) + "–" + QString::number(max);
    range_label_->setText(label_text);

    min_line_edit_->setValidator(new QDoubleValidator(min, max, 2, this));
    max_line_edit_->setValidator(new QDoubleValidator(min, max, 2, this));

    uword origin = dataset_->FindOrigin();
    QVector<double> plot_data, wavelength;

    try{
        plot_data = dataset_->PointSpectrum(origin);
        wavelength = dataset_->WavelengthQVector();
    }
    catch(exception e){}
    if (plot_data.isEmpty()){plot_data = dataset_->PointSpectrum(0);}
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
    close();
    if (min_line_edit_->text().isEmpty() || max_line_edit_->text().isEmpty()){
        QMessageBox::warning(this, "Invalid Input!", "You must enter numbers for left and right bounds.");
        return;
    }
    double entered_min = min_line_edit_->text().toDouble();
    double entered_max = max_line_edit_->text().toDouble();
    uint bound_window = search_window_spin_box_->value();
    QString name = name_line_edit_->text();
    if(!name.size()){
        name = "Univariate " + QString::number(dataset_->UnivariateCount());
    }
    QString value_method = value_method_combo_box_->currentText();
    if (value_method == "Riemann Sum"){
        try{
            dataset_->Univariate(name, entered_min, entered_max, bound_window);
        }catch(exception e){
            workspace->main_window()->DisplayExceptionWarning(e);
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
