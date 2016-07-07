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
#ifndef SPECTRUMVIEWER_H
#define SPECTRUMVIEWER_H

#include <QDialog>
#include "GUI/Display/mapviewer.h"
#include "GUI/Display/dataviewer.h"
#include "GUI/Display/spectrumselectiondialog.h"
class SpectrumSelectionDialog;
class MapViewer;
class MapData;
class VespucciDataset;
class DataViewer;

namespace Ui {
class SpectrumViewer;
}

///
/// \brief The SpectrumViewer class
/// A class for displaying spectra
class SpectrumViewer : public QDialog
{
    Q_OBJECT

public:
    SpectrumViewer(MapViewer *parent,
                   MapData *map_data,
                   const QString x_axis_description,
                   const QString y_axis_description, QSharedPointer<VespucciDataset> dataset, QSize widget_size, QString directory);


    SpectrumViewer(DataViewer *parent,
                   QSharedPointer <VespucciDataset> dataset,
                   int endmember,
                   QString directory,
                   QString type);

    SpectrumViewer(SpectrumSelectionDialog *parent,
                   QSharedPointer<VespucciDataset> dataset);

    SpectrumViewer(DataViewer *parent,
                   QSharedPointer<VespucciDataset> dataset,
                   vec y, QString ordinate_label);

    ~SpectrumViewer();
    void SetPlot(QVector<double> wavelength,
                 QVector<double> intensity);

    void SetSecondPlot(QVector<double> abscissa, QVector<double> intensities);
    void SetSecondPlot(QVector<double> first_abscissa, QVector<double> second_abscissa,
                       QVector<double> first_intensities, QVector<double> second_intensities);
private slots:
    void MapClicked(QCPAbstractPlottable *plottable, QMouseEvent *event);
    void on_pushButton_clicked();
    void SpectrumChanged(QVector<double> &wavelength, QVector<double> &intensity);

private:
    Ui::SpectrumViewer *ui;

    ///
    /// \brief spectrum_plot_
    /// The widget containing the plot
    QCustomPlot *spectrum_plot_;

    ///
    /// \brief parent_
    /// The window displaying an associated map (if one exists)
    MapViewer *parent_;

    /// The MapData object containing the image data (if one exists)
    MapData *map_data_;

    ///
    /// \brief dataset_
    /// The current dataset
    QSharedPointer<VespucciDataset> dataset_;

    ///
    /// \brief widget_size_
    /// This size of the current widget
    QSize widget_size_;

    ///
    /// \brief directory_
    /// The working directory
    QString directory_;

    ///
    /// \brief current_index_
    /// The current index (in the dataset) of the displayed spectrum
    int current_index_;

    ///
    /// \brief current_x_
    /// Current x position on the image
    double current_x_;

    ///
    /// \brief current_y_
    /// Current y position on the image
    double current_y_;

    ///
    /// \brief current_z_
    /// Current value at the image
    double current_z_;

    ///
    /// \brief linked_to_map_
    /// Whether or not there are associated MapData and MapViewer objects
    bool linked_to_map_;
};

#endif // SPECTRUMVIEWER_H
