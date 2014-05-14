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

#ifndef SPECMAP_H
#define SPECMAP_H
#include <armadillo>
#include <QTextStream>
#include <iostream>
#include <QVector>
#include <vector>
#include "qcustomplot.h"
#include "univariatemap.h"
//#include "vespucciworkspace.h"

using namespace std;
using namespace arma;

class SpecMap
{
public:
    SpecMap();
    SpecMap(QTextStream& inputstream, QMainWindow *main_window);



    // PRE-PROCESSING FUNCTIONS //

    int PreProcess(QString normalization_method,
                   int polynomial_order,
                   int derivative_order,
                   int filter_window_size);


    // HELPER FUNCTIONS //

    // Grabs one line of spectra_, at indices specified, recasts as QVector for QCustomPlot //
    QVector<double> PointSpectrum(int index);

    // Converts wavelength_ to QVector //
    QVector<double> WavelengthQVector();

    // Finds the index of spectra_ with x and y values closest to the input //
    int FindIndex(int x_value, int y_value);

    // Returns vector containing start and end index within a range
    vector<int> FindRange(double start, double end);

    // These QCustomPlot sizes apply to all maps created from dataset //
    int KeySize();
    int ValueSize();
    QCPRange KeyRange();
    QCPRange ValueRange();



    // IMAGING FUNCTIONS //

    void Univariate(int min,
                    int max,
                    QString name,
                    QString value_method,
                    int gradient_index);

    void BandRatio(int first_min,
                   int first_max,
                   int second_min,
                   int second_max,
                   QString name,
                   QString value_method);

    mat PartialLeastSquares(int components,
                            bool include_negative_scores);

    mat PrincipalComponents(int components,
                            bool include_negative_scores);

    mat VectorComponents(int endmembers,
                         bool include_negative_scores);

    mat KMeans(int clusters);

    mat Agglomerative(int clusters,
                      QString distance_metric);

    //MEMBER ACCESS FUNCTIONS:
    rowvec wavelength();
    colvec x();
    colvec y();
    mat spectra();
    const QString name();

    void SetName(QString new_name);

    //MAP HANDLING FUNCTIONS
    QStringList map_names();

    //adds or removes dataset or map to relevant lists
    void AddDataset(SpecMap dataset);
    void RemoveDataset(QString name);

    void AddMap(MapData *map);
    void RemoveMap(QString name);
    void RemoveMapAt(int i);
    int map_loading_count();

    void SetXDescription(QString description);
    void SetYDescription(QString description);

    QCPRange WavelengthRange();
    QCPRange PointSpectrumRange(int i);

    QCPColorGradient GetGradient(int gradient_number);

private:
    rowvec wavelength_;
    colvec x_;
    colvec y_;
    mat spectra_;

    QString name_;


    QListWidget *map_list_widget_;
    QList<MapData *> maps_;
    QStringList map_names_;
    int map_loading_count_;

    QString x_axis_description_; //abcissa legend
    QString y_axis_description_; //ordinate legend
};

#endif // SPECMAP_H
