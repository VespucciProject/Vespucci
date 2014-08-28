/************************************************************************************
    Copyright (C) 2014 Daniel P. Foose - All Rights Reserved

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
***************************************************************************************/
#include "spectrumviewer.h"
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
                               SpecMap *dataset,
                               QSize widget_size,
                               QString directory) :
    QDialog(parent),
    ui(new Ui::SpectrumViewer)
{
    ui->setupUi(this);
    linked_to_map_ = true;
    map_data_ = map_data;
    spectrum_plot_ = this->findChild<QCustomPlot *>("spectrum");
    spectrum_plot_->addGraph();
    spectrum_plot_->xAxis->setLabel(x_axis_description);
    spectrum_plot_->yAxis->setLabel(y_axis_description);

    spectrum_plot_->replot();

    QVector<double> plot_data = dataset->PointSpectrum(0);
    QVector<double> wavelength = dataset->WavelengthQVector();

    coordinate_label_ = this->findChild<QLabel *>("coordinateLabel");
    value_label_ = this->findChild<QLabel *>("valueLabel");

    spectrum_plot_->graph(0)->addData(wavelength, plot_data);


    spectrum_plot_->xAxis->setRange(dataset->WavelengthRange());
    spectrum_plot_->yAxis->setRange(dataset->PointSpectrumRange(0));
    dataset_ = dataset;
    widget_size_ = widget_size;
    directory_ = directory;

    spectrum_plot_->setInteraction(QCP::iRangeDrag, true);
    spectrum_plot_->setInteraction(QCP::iRangeZoom, true);

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
                               SpecMap *dataset,
                               int endmember,
                               QString directory,
                               QString type) :
    QDialog(parent),
    ui(new Ui::SpectrumViewer)
{
    ui->setupUi(this);
    spectrum_plot_ = this->findChild<QCustomPlot *>("spectrum");
    spectrum_plot_->addGraph();
    spectrum_plot_->xAxis->setLabel(dataset->x_axis_description());
    spectrum_plot_->yAxis->setLabel(dataset->y_axis_description());
    QVector<double> plot_data;
    spectrum_plot_->replot();
    cout << "If" << endl;
    if (type == "VCA"){
        plot_data = dataset->vertex_components_data()->Endmember(endmember);
    }
    QVector<double> wavelength = dataset->WavelengthQVector();
    coordinate_label_ = this->findChild<QLabel *>("coordinateLabel");
    value_label_ = this->findChild<QLabel *>("valueLabel");
    spectrum_plot_->graph(0)->addData(wavelength, plot_data);
    dataset_ = dataset;
    directory_ = directory;

    spectrum_plot_->setInteraction(QCP::iRangeDrag, true);
    spectrum_plot_->setInteraction(QCP::iRangeZoom, true);
    spectrum_plot_->rescaleAxes();
}


SpectrumViewer::~SpectrumViewer()
{
    delete ui;
}

void SpectrumViewer::SetPlot(QVector<double> wavelength,
                             QVector<double> intensity)
{
    spectrum_plot_->graph(0)->setData(wavelength, intensity);
    spectrum_plot_->rescaleAxes();
    spectrum_plot_->replot();
}

void SpectrumViewer::SetSecondPlot(QVector<double> first_abcissa, QVector<double> second_abcissa,
                                   QVector<double> first_intensities, QVector<double> second_intensities)
{
    if (spectrum_plot_->graphCount() <= 2){
        spectrum_plot_->addGraph(spectrum_plot_->graph(0)->keyAxis(), spectrum_plot_->graph(0)->valueAxis());
        spectrum_plot_->graph(1)->addData(first_abcissa, first_intensities);
        spectrum_plot_->addGraph(spectrum_plot_->graph(0)->keyAxis(), spectrum_plot_->graph(0)->valueAxis());
        spectrum_plot_->graph(2)->addData(second_abcissa, second_intensities);
    }

    spectrum_plot_->graph(1)->setData(first_abcissa, first_intensities);
    spectrum_plot_->graph(1)->setPen(QPen(QColor("red")));
    spectrum_plot_->graph(1)->setVisible(true);

    spectrum_plot_->graph(2)->setData(second_abcissa, second_intensities);
    spectrum_plot_->graph(2)->setPen(QPen(QColor("red")));
    spectrum_plot_->graph(2)->setVisible(true);
    spectrum_plot_->replot();
}

void SpectrumViewer::SetSecondPlot(QVector<double> abcissa, QVector<double> intensities)
{
    if (spectrum_plot_->graphCount() <= 1){
        spectrum_plot_->addGraph(spectrum_plot_->graph(0)->keyAxis(), spectrum_plot_->graph(0)->valueAxis());
        spectrum_plot_->graph(1)->addData(abcissa, intensities);
    }

    spectrum_plot_->graph(1)->setData(abcissa, intensities);
    spectrum_plot_->graph(1)->setPen(QPen(QColor("red")));
    spectrum_plot_->graph(1)->setVisible(true);
    spectrum_plot_->replot();
}

void SpectrumViewer::MapClicked(QCPAbstractPlottable *plottable, QMouseEvent *event)
{
    unsigned int i;
    double x = (double) event->x();
    double y = (double) widget_size_.height() - (double) event->y();

    x /= widget_size_.width();
    y /= widget_size_.height();

    QCPRange key_range = dataset_->KeyRange();
    QCPRange value_range = dataset_->ValueRange();
    double key_span = key_range.upper - key_range.lower;
    double value_span = value_range.upper - value_range.lower;

    double x_position = x * key_span + key_range.lower;
    double y_position = y * value_span + value_range.lower;


    //find nearest x value
    colvec x_values = dataset_->x();
    colvec y_values = dataset_->y();

    double x_diff = x_position - x_values(0);
    double y_diff = y_position - y_values(0);
    double x_diff_buf, y_diff_buf, x_value, y_value;

    for (i = 0; i < x_values.n_rows; ++i){
        x_diff_buf = x_position - x_values(i);
        if ((x_diff < 0 && x_diff_buf >= 0) || (x_diff >= 0 && x_diff_buf < 0))
            break;
    }

    if (fabs(x_diff_buf) > fabs(x_position - x_values(i-1)))
        x_value = x_values(i-1);
    else
        x_value = x_values(i);

    for (i = 0; i < y_values.n_rows; ++i){
        y_diff_buf = y_position - y_values(i);
        if ((y_diff < 0 && y_diff_buf >= 0) || (y_diff >= 0 && y_diff_buf < 0))
            break;
    }

    if (fabs(y_diff_buf) > fabs(y_position - y_values(i-1)))
        y_value = y_values(i-1);
    else
        y_value = y_values(i);

    QVector<int> x_indices;
    for (i = 0; i < x_values.n_elem; ++i){
        if (x_values(i) == x_value)
            x_indices.append(i);
    }
    for (i = 0; i < (unsigned int) x_indices.size(); ++i){
        if (y_values(x_indices[i]) == y_value)
            break;
    }
    current_index_ = x_indices[i];

    QVector<double> wavelength = dataset_->WavelengthQVector();
    QVector<double> intensities = dataset_->PointSpectrum(current_index_);

    current_x_ = x_value;
    current_y_ = y_value;

    coordinate_label_->setText("(" +
                               QString::number(current_x_) +
                               ", " +
                               QString::number(current_y_) +
                               ")");
    value_label_->setText(QString::number(map_data_->results_at_position(current_x_, current_y_)));
    SetPlot(wavelength, intensities);
    if (map_data_->univariate_area())
        SetSecondPlot(map_data_->first_abcissa(), map_data_->first_baseline(current_index_));
    if (map_data_->univariate_bandwidth()){
        SetSecondPlot(map_data_->first_abcissa(), map_data_->first_baseline(current_index_));
        QCPItemLine *mid_line;
        QVector<double> mid_line_vec;
        if (spectrum_plot_->itemCount() == 0){
            QCPItemLine *mid_line = new QCPItemLine(spectrum_plot_);
            QVector<double> mid_line_vec = map_data_->mid_line(current_index_);
            mid_line->start->setCoords(mid_line_vec[0], mid_line_vec[1]);
            mid_line->end->setCoords(mid_line_vec[2], mid_line_vec[3]);
            mid_line->setPen(QPen(QColor("red")));
            spectrum_plot_->addItem(mid_line);
        }

    }
    if (map_data_->band_ratio_area()){
        SetSecondPlot(map_data_->first_abcissa(),
                      map_data_->second_abcissa(),
                      map_data_->first_baseline(current_index_),
                      map_data_->second_baseline(current_index_));
    }


}

void SpectrumViewer::on_pushButton_clicked()
{
    if (!linked_to_map_){
        //only images can be saved in this view
        QString filename =
                QFileDialog::getSaveFileName(this,
                                             "Save As...",
                                             directory_,
                                             tr("TIFF (*.tif);;"
                                                "JPEG (*.jpg);;"
                                                "PNG (*.png);;"
                                                "BMP (*.bmp);;"));
        bool success;
        QFileInfo file_info(filename);
        QString extension = file_info.suffix();
        if (extension == "bmp")
            success = spectrum_plot_->saveBmp(filename);
        else if (extension == "jpg")
            success = spectrum_plot_->saveJpg(filename);
        else
            success = spectrum_plot_->savePng(filename);
        //PNG is default because everyone can open them
        return; //don't do any of the other stuff
    }





    QString filename =
            QFileDialog::getSaveFileName(this,
                                         "Save As...",
                                         directory_,
                                         tr("Tab-delimited text (*.txt);;"
                                            "PDF (*.pdf);;"
                                            "TIFF (*.tif);;"
                                            "JPEG (*.jpg);;"
                                            "PNG (*.png);;"
                                            "BMP (*.bmp);;"
                                            "CSV (*.csv);;"
                                            "Armadillo binary (*.arma);;"));
    bool success;
    QFileInfo file_info(filename);
    QString extension = file_info.suffix();

    int width = spectrum_plot_->width();
    int height = spectrum_plot_->height();
    double scale = 1.0;

    rowvec spectrum = dataset_->spectra().row(current_index_);
    rowvec wavelength = dataset_->wavelength();
    mat results;
    results.insert_rows(0, wavelength);
    results.insert_rows(1, spectrum);
    results = results.t();
    QString description =
            map_data_->name()
            +" ("
            +QString::number(current_x_)
            +", "
            +QString::number(current_y_)
            +")";

    if (extension== "pdf")
        success = spectrum_plot_->savePdf(filename,
                                          true, width, height,
                                          "Vespucci", description);
    else if (extension == "tif")
        success = spectrum_plot_->saveRastered(filename, width, height, scale, "TIF", 1);
    else if (extension == "bmp")
        success = spectrum_plot_->saveBmp(filename);
    else if (extension == "jpg")
        success = spectrum_plot_->saveJpg(filename);
    else if (extension == "png")
        success = spectrum_plot_->savePng(filename);
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
