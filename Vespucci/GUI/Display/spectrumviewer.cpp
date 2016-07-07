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
#include "GUI/Display/spectrumviewer.h"
#include "Global/vespucci.h"
#include "Global/global.h"
#include "ui_spectrumviewer.h"

///
/// \brief SpectrumViewer::SpectrumViewer
/// The constructor for when a spectrum viewer is linked to a MapViewer
/// \param parent
/// \param map_data
/// \param x_axis_description
/// \param y_axis_description
/// \param dataset
/// \param widget_size
/// \param directory
///
SpectrumViewer::SpectrumViewer(MapViewer *parent,
                               MapData *map_data,
                               const QString x_axis_description,
                               const QString y_axis_description,
                               QSharedPointer<VespucciDataset> dataset,
                               QSize widget_size,
                               QString directory) :
    QDialog(parent),
    ui(new Ui::SpectrumViewer)
{
    ui->setupUi(this);
    linked_to_map_ = true;
    map_data_ = map_data;
    ui->spectrum->addGraph();
    ui->spectrum->xAxis->setLabel(x_axis_description);
    ui->spectrum->yAxis->setLabel(y_axis_description);

    ui->spectrum->replot();

    QVector<double> plot_data = dataset->PointSpectrum(0);
    QVector<double> wavelength = dataset->WavelengthQVector();

    ui->spectrum->graph(0)->addData(wavelength, plot_data);


    ui->spectrum->xAxis->setRange(dataset->WavelengthRange());
    ui->spectrum->yAxis->setRange(dataset->PointSpectrumRange(0));
    dataset_ = dataset;
    widget_size_ = widget_size;
    directory_ = directory;

    ui->spectrum->setInteraction(QCP::iRangeDrag, true);
    ui->spectrum->setInteraction(QCP::iRangeZoom, true);
}

///
/// \brief SpectrumViewer::SpectrumViewer
/// A constructor for using a spectrum viewer to view VCA endmembers
/// \param parent
/// \param dataset
/// \param endmember
/// \param directory
///
SpectrumViewer::SpectrumViewer(DataViewer *parent,
                               QSharedPointer<VespucciDataset> dataset,
                               int endmember,
                               QString directory,
                               QString type) :
    QDialog(parent),
    ui(new Ui::SpectrumViewer)
{
    ui->setupUi(this);
    dataset_ = dataset;
    ui->coordinateLabel->setVisible(false);
    ui->valueLabel->setVisible(false);
    ui->spectrum->addGraph();
    ui->spectrum->xAxis->setLabel(dataset->x_axis_description());
    ui->spectrum->yAxis->setLabel(dataset->y_axis_description());
    QVector<double> plot_data;
    ui->spectrum->replot();

    QVector<double> wavelength = dataset->WavelengthQVector();
    ui->spectrum->graph(0)->addData(wavelength, plot_data);
    directory_ = directory;

    ui->spectrum->setInteraction(QCP::iRangeDrag, true);
    ui->spectrum->setInteraction(QCP::iRangeZoom, true);
    ui->spectrum->rescaleAxes();
}

SpectrumViewer::SpectrumViewer(SpectrumSelectionDialog *parent,
                               QSharedPointer<VespucciDataset> dataset):
    QDialog(parent),
    ui(new Ui::SpectrumViewer)
{
    ui->setupUi(this);
    dataset_ = dataset;

    ui->coordinateLabel->setVisible(false);
    ui->valueLabel->setVisible(false);


    ui->spectrum->addGraph();
    ui->spectrum->xAxis->setLabel(dataset->x_axis_description());
    ui->spectrum->yAxis->setLabel(dataset->y_axis_description());

    cout << "set data" << endl;
    QVector<double> wavelength = dataset->WavelengthQVector();
    QVector<double> plot_data = dataset->PointSpectrum(0);
    ui->spectrum->graph(0)->addData(wavelength, plot_data);
    dataset_ = dataset;

    ui->spectrum->setInteraction(QCP::iRangeDrag, true);
    ui->spectrum->setInteraction(QCP::iRangeZoom, true);
    ui->spectrum->rescaleAxes();
    cout << "end of constructor" << endl;
}

SpectrumViewer::SpectrumViewer(DataViewer *parent,
                               QSharedPointer<VespucciDataset> dataset,
                               vec y, QString ordinate_label):
    QDialog(parent),
    ui(new Ui::SpectrumViewer)
{
    ui->setupUi(this);
    dataset_ = dataset;

    ui->coordinateLabel->setVisible(false);
    ui->valueLabel->setVisible(false);

    ui->spectrum->addGraph();
    ui->spectrum->xAxis->setLabel(dataset->x_axis_description());
    ui->spectrum->yAxis->setLabel(ordinate_label);

    QVector<double> wavelength = dataset->WavelengthQVector();
    QVector<double> plot_data =
            QVector<double>::fromStdVector(conv_to<std::vector<double> >::from(y));
    ui->spectrum->graph(0)->addData(wavelength, plot_data);
    dataset_ = dataset;

    ui->spectrum->setInteraction(QCP::iRangeDrag, true);
    ui->spectrum->setInteraction(QCP::iRangeZoom, true);
    ui->spectrum->rescaleAxes();
}

\



SpectrumViewer::~SpectrumViewer()
{
    delete ui;
}

///
/// \brief SpectrumViewer::SetPlot
/// \param wavelength
/// \param intensity
/// Set the plot of the spectrum viewer
void SpectrumViewer::SetPlot(QVector<double> wavelength,
                             QVector<double> intensity)
{
    ui->spectrum->graph(0)->setData(wavelength, intensity);
    ui->spectrum->rescaleAxes();
    ui->spectrum->replot();
}

///
/// \brief SpectrumViewer::SetSecondPlot
/// \param first_abscissa
/// \param second_abscissa
/// \param first_intensities
/// \param second_intensities
/// Set other plots (baselines)
void SpectrumViewer::SetSecondPlot(QVector<double> first_abscissa, QVector<double> second_abscissa,
                                   QVector<double> first_intensities, QVector<double> second_intensities)
{
    if (ui->spectrum->graphCount() <= 2){
        ui->spectrum->addGraph(ui->spectrum->graph(0)->keyAxis(), ui->spectrum->graph(0)->valueAxis());
        ui->spectrum->graph(1)->addData(first_abscissa, first_intensities);
        ui->spectrum->addGraph(ui->spectrum->graph(0)->keyAxis(), ui->spectrum->graph(0)->valueAxis());
        ui->spectrum->graph(2)->addData(second_abscissa, second_intensities);
    }

    ui->spectrum->graph(1)->setData(first_abscissa, first_intensities);
    ui->spectrum->graph(1)->setPen(QPen(QColor("red")));
    ui->spectrum->graph(1)->setVisible(true);

    ui->spectrum->graph(2)->setData(second_abscissa, second_intensities);
    ui->spectrum->graph(2)->setPen(QPen(QColor("red")));
    ui->spectrum->graph(2)->setVisible(true);
    ui->spectrum->replot();
}

///
/// \brief SpectrumViewer::SetSecondPlot
/// \param abscissa
/// \param intensities
/// Set the second plot.
void SpectrumViewer::SetSecondPlot(QVector<double> abscissa, QVector<double> intensities)
{
    if (ui->spectrum->graphCount() <= 1){
        ui->spectrum->addGraph(ui->spectrum->graph(0)->keyAxis(), ui->spectrum->graph(0)->valueAxis());
        ui->spectrum->graph(1)->addData(abscissa, intensities);
    }

    ui->spectrum->graph(1)->setData(abscissa, intensities);
    ui->spectrum->graph(1)->setPen(QPen(QColor("red")));
    ui->spectrum->graph(1)->setVisible(true);
    ui->spectrum->replot();
}

///
/// \brief SpectrumViewer::MapClicked
/// \param plottable The QCPColorMap
/// \param event The QMouseEvent
/// A slot which receives the locations
void SpectrumViewer::MapClicked(QCPAbstractPlottable *plottable, QMouseEvent *event)
{
    QCPColorMap *color_map = qobject_cast<QCPColorMap*>(plottable);
    current_x_ = color_map->keyAxis()->pixelToCoord(event->x());
    current_y_ = color_map->valueAxis()->pixelToCoord(event->y());
    current_z_ = color_map->data()->data(current_x_, current_y_);
    arma::uvec row = arma::find(map_data_->results_ == current_z_);
    if (row.n_elem == 0)
        return;
    else
        current_index_ = row(0);

    QVector<double> wavelength = dataset_->WavelengthQVector();
    QVector<double> intensities = dataset_->PointSpectrum(current_index_);
    double x_value = dataset_->x(current_index_);
    double y_value = dataset_->y(current_index_);

    ui->coordinateLabel->setText("(" +
                               QString::number(x_value) +
                               ", " +
                               QString::number(y_value) +
                               ")");
    ui->valueLabel->setText(QString::number(current_z_));
    SetPlot(wavelength, intensities);
    if (map_data_->univariate_area())
        SetSecondPlot(map_data_->first_abscissa(), map_data_->first_baseline(current_index_));
    if (map_data_->univariate_bandwidth()){
        SetSecondPlot(map_data_->first_abscissa(), map_data_->first_baseline(current_index_));
        if (ui->spectrum->itemCount() == 0){
            QCPItemLine *mid_line = new QCPItemLine(ui->spectrum);
            QVector<double> mid_line_vec = map_data_->mid_line(current_index_);
            mid_line->start->setCoords(mid_line_vec[0], mid_line_vec[1]);
            mid_line->end->setCoords(mid_line_vec[2], mid_line_vec[3]);
            mid_line->setPen(QPen(QColor("red")));
            ui->spectrum->addItem(mid_line);
        }

    }
    if (map_data_->band_ratio_area()){
        SetSecondPlot(map_data_->first_abscissa(),
                      map_data_->second_abscissa(),
                      map_data_->first_baseline(current_index_),
                      map_data_->second_baseline(current_index_));
    }
    if (map_data_->band_ratio_derivative()){
        SetSecondPlot(map_data_->first_abscissa(current_index_),
                      map_data_->second_abscissa(current_index_),
                      map_data_->first_baseline(current_index_),
                      map_data_->second_baseline(current_index_));
    }
    if (map_data_->univariate_derivative()){
        SetSecondPlot(map_data_->first_abscissa(current_index_),
                      map_data_->first_baseline(current_index_));
    }


}

///
/// \brief SpectrumViewer::on_pushButton_clicked
/// Exports the data or a picture of the plot
void SpectrumViewer::on_pushButton_clicked()
{
    if (!linked_to_map_){
        //only images can be saved in this view
        QString filename =
                QFileDialog::getSaveFileName(this,
                                             tr("Save File"),
                                             directory_,
                                             tr("Scalable Vector Graphics (*.svg);; "
                                                "Enhanced Windows Metafile (*.emf);; "
                                                "Portable Document Format (*.pdf);; "
                                                "Tagged Image File Format (*.tif);; "
                                                "Windows Bitmap (*.bmp);; "
                                                "Portable Network Graphics (*.png);; "
                                                "JPEG (*.jpg)"));
        bool success = Vespucci::SavePlot(ui->spectrum, filename);
        if(success)
            QMessageBox::information(this, "Success!", "File " + filename + " written successfully");
        else
            QMessageBox::warning(this, "File Save Failed", "File " + filename + " was not written successfully");
        return; //don't do any of the other stuff
    }





    QString filename =
            QFileDialog::getSaveFileName(this,
                                         "Save As...",
                                         directory_,
                                         tr("Tab-delimited text (*.txt);;"
                                            "Comma-separated Values (*.csv);;"
                                            "Scalable Vector Graphics (*.svg);; "
                                            "Enhanced Windows Metafile (*.emf);; "
                                            "Portable Document Format (*.pdf);; "
                                            "Tagged Image File Format (*.tif);; "
                                            "Windows Bitmap (*.bmp);; "
                                            "Portable Network Graphics (*.png);; "
                                            "JPEG (*.jpg)"
                                            ));
    bool success;
    QFileInfo file_info(filename);

    QString extension = file_info.suffix();

    vec spectrum = dataset_->spectra_ptr()->col(current_index_);
    vec wavelength = dataset_->wavelength();
    mat results;
    results.insert_rows(0, wavelength);
    results.insert_rows(1, spectrum);
    results = results.t();

    if ( (extension== "pdf")
       ||(extension == "tif")
       ||(extension == "bmp")
       ||(extension == "jpg")
       ||(extension == "png")
       ||(extension == "svg")
       ||(extension == "emf")){
        success = Vespucci::SavePlot(ui->spectrum, filename);
    }

    else if (extension == "csv")
        success = results.save(filename.toStdString(), csv_ascii);
    else if (extension == "arma")
        success = spectrum.save(filename.toStdString(), arma_binary);
    else
        success = results.save(filename.toStdString(), raw_ascii);

    if (success)
        QMessageBox::information(this, "Success!", "Saving " + filename + " successful");
    else
        QMessageBox::warning(this, "Failure", "Saving " + filename + " not successful");



}

void SpectrumViewer::SpectrumChanged(QVector<double> &wavelength, QVector<double> &intensity)
{
    SetPlot(wavelength, intensity);
}
