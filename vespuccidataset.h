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

#ifndef VespucciDataset_H
#define VespucciDataset_H
#include "arma_ext.h"
#include <QTextStream>
#include <iostream>
#include <QVector>
#include <vector>
#include <qcustomplot.h>
#include "univariatemap.h"
#include "principalcomponentsdata.h"
#include "plsdata.h"
#include "vcadata.h"
#include "vespucciworkspace.h"




class MapData;
class PrincipalComponentsData;
class PLSData;
class VCAData;
class SpectrumViewer;
class MainWindow;
class VespucciWorkspace;

using namespace std;
using namespace arma;
///
/// \brief The VespucciDataset class
/// This is the main class for dealing with hyperspectral data. This handles the
///  import and export of spectra, and the creation of maps. Images are handled
/// by the MapData class.
class VespucciDataset
{
public:
    VespucciDataset();
    VespucciDataset(QString text_filename,
                    MainWindow *main_window,
                    QString *directory,
                    QFile *log_file,
                    QString name,
                    QString x_axis_description,
                    QString y_axis_description,
                    bool swap_spatial);


    VespucciDataset(QString binary_filename,
                    MainWindow *main_window,
                    QString *directory,
                    QString name,
                    QFile *log_file);


    VespucciDataset(QString name,
                    MainWindow *main_window,
                    QString *directory,
                    QFile *log_file,
                    QSharedPointer<VespucciDataset> original,
                    uvec indices);

    ~VespucciDataset();
    // PRE-PROCESSING FUNCTIONS //
    //general
    void Undo(QString operation);

    // map editing
    void CropSpectra(double x_min, double x_max, double y_min, double y_max);

    // Normalization functions

    mat ZScoreNormCopy();
    void MinMaxNormalize();
    void ZScoreNormalize();
    void UnitAreaNormalize();

    //Spectral Pre-processing

    void SubtractBackground(mat background, QString filename);
    void MedianFilter(int window_size);
    void LinearMovingAverage(int window_size);
    void Derivatize(unsigned int derivative_order,
                    unsigned int polynomial_order,
                    unsigned int window_size);
    void SingularValue(int singular_values);
    mat spdiags(mat B, QVector<int> d, int m, int n);

    void Baseline(QString method, int window_size);


    // HELPER FUNCTIONS //

    // Grabs one line of spectra_, at indices specified, recasts as QVector for QCustomPlot //
    QVector<double> PointSpectrum(const uword index);

    // Converts wavelength_ to QVector //
    QVector<double> WavelengthQVector();

    // Finds the index of spectra_ with x and y values closest to the input //
    int FindIndex(int x_value, int y_value);

    // Returns vector containing start and end index within a range
    uvec FindRange(double start, double end);

    // These QCustomPlot sizes apply to all maps created from dataset //
    int KeySize();
    int ValueSize();
    QCPRange KeyRange();
    QCPRange ValueRange();



    // IMAGING FUNCTIONS //

    void Univariate(uword min,
                    uword max,
                    QString name,
                    QString value_method,
                    QString integration_method,
                    uword gradient_index);

    void BandRatio(uword first_min,
                   uword first_max,
                   uword second_min,
                   uword second_max,
                   QString name,
                   QString value_method, QString integration_method,
                   unsigned int gradient_index);

    void PartialLeastSquares(uword components, uword image_component,
                             QString name,
                             unsigned int gradient_index, bool recalculate);

    void PrincipalComponents(int component,
                            QString name,
                            int gradient_index,
                             bool recalculate);

    void VertexComponents(uword endmembers, uword image_component,
                          QString name, unsigned gradient_index, bool recalculate);

    void KMeans(size_t clusters,
               QString name);

    void IndependentComponents(int image_component,
                               QString name,
                               int gradient_index,
                               bool recalculate);

    void Agglomerative(int clusters,
                      QString distance_metric,
                      QString name);

    //MEMBER ACCESS FUNCTIONS:
    rowvec wavelength();
    colvec x();
    colvec y();
    colvec x(uvec indices);
    colvec y(uvec indices);
    double x(uword index);
    double y(uword index);
    rowvec wavelength(uvec indices);
    mat spectra(uvec indices);
    mat spectra();
    const QString name();

    bool principal_components_calculated();
    bool vertex_components_calculated();
    bool partial_least_squares_calculated();
    bool k_means_calculated();
    bool radical_calculated();

    PrincipalComponentsData *principal_components_data();
    VCAData *vertex_components_data();
    PLSData *partial_least_squares_data();
    mat *k_means_data();
    const QString x_axis_description();
    const QString y_axis_description();

    void SetName(QString new_name);

    //MAP HANDLING FUNCTIONS
    QStringList map_names();

    //adds or removes dataset or map to relevant lists
    void AddDataset(VespucciDataset dataset);
    void RemoveDataset(QString name);

    void AddMap(QSharedPointer<MapData> map);
    void RemoveMapAt(unsigned int i);
    int map_loading_count();

    void SetXDescription(QString description);
    void SetYDescription(QString description);

    QCPRange WavelengthRange();
    QCPRange PointSpectrumRange(int i);

    QCPColorGradient GetGradient(int gradient_number);
    QCPColorGradient GetClusterGradient(int clusters);

    bool ConstructorCancelled();
    mat AverageSpectrum(bool stats);

    mat *x_ptr();
    mat *y_ptr();
    mat *wavelength_ptr();
    mat *spectra_ptr();

    bool non_spatial();



private:
    void DestroyLogFile();

    ///
    /// \brief wavelength_
    /// The spectral abcissa of the spectra in spectra_
    rowvec wavelength_;

    ///
    /// \brief x_
    /// The spatial horizontal position
    colvec x_;

    ///
    /// \brief y_
    /// The spatial vertical position
    colvec y_;

    ///
    /// \brief spectra_
    /// The list of spectra. Every index corresponds to a value for x, a value
    /// for y, and a spectrum.
    mat spectra_;

    ///
    /// \brief spectra_old_
    /// A copy of spectra_, which is not modified by pre-processing functions
    /// until another pre-processing function is called. This allows for undoing
    /// pre-processing functions.
    mat spectra_old_;

    ///
    /// \brief name_
    /// The name chosen by the user for this dataset.
    QString name_;

    ///
    /// \brief log_file_ Pointer to the QFile associated with the log stream
    /// This is allocated with "new" outside the VespucciDataset object, then
    /// deallocated with delete inside the VespucciDataset destructor
    QFile *log_file_;


    ///
    /// \brief log_stream_ A log of all UI-induced functions called on this dataset
    ///
    QTextStream log_stream_;

    QString last_operation_;

    ///
    /// \brief map_list_widget_
    /// A pointer back to the list widget containing created maps in the main
    /// window.
    QListWidget *map_list_widget_;

    ///
    /// \brief maps_
    /// A list of the maps created from this data set, managed by VespucciDataset class
    QList<QSharedPointer<MapData> > maps_;

    ///
    /// \brief principal_components_data_
    /// A pointer to a container holding all PCA data and stats.
    PrincipalComponentsData *principal_components_data_;

    ///
    /// \brief partial_least_squares_data_
    /// A pointer to a container holding all PLS data and stats
    PLSData *partial_least_squares_data_;

    ///
    /// \brief vertex_components_data_
    /// A pointer to a container (on the heap) holding all VCA data and stats.
    VCAData *vertex_components_data_;

    ///
    /// \brief k_means_data_
    /// A matrix containing the K-means cluster assignments. A container object
    /// may be used later.
    mat k_means_data_;

    ///
    /// \brief radical_components_
    /// A matrix consisting of the independent components calculated by RADICAL
    /// Y=WX where X is spectra_
    mat radical_components_;

    ///
    /// \brief radical_unmixing_
    /// An unmixing matrix from RADICAL Y=WX W is the matrix.
    ///
    mat radical_unmixing_;

    ///
    /// \brief map_names_
    /// A list of map names
    QStringList map_names_;

    ///
    /// \brief map_loading_count_
    /// How many maps have been loaded, used mostly for default names.
    int map_loading_count_;

    ///
    /// \brief x_axis_description_
    /// Spectral abcissa legend
    QString x_axis_description_; //abcissa legend

    ///
    /// \brief y_axis_description_
    /// Spectral ordinate legend
    QString y_axis_description_; //ordinate legend

    ///
    /// \brief directory_
    /// Pointer back to the string in the workspace containing the working path
    QString *directory_;

    ///
    /// \brief z_scores_calculated_
    /// Whether or not Z-scores have been calculated (triggers determination of
    /// "negative" peaks (finding minimums are areas where spectrum is lower than
    /// average)
    bool z_scores_calculated_;

    ///
    /// \brief principal_components_calculated_
    /// Whether or not PCA has been performed
    bool principal_components_calculated_;

    ///
    /// \brief partial_least_squares_calculated_
    /// Whether or not PLS has been performed
    bool partial_least_squares_calculated_;

    ///
    /// \brief vertex_components_calculated_
    /// Whether or not VCA has been performed.
    bool vertex_components_calculated_;

    ///
    /// \brief k_means_calculated_
    /// Whether or not k means has been performed.
    bool k_means_calculated_;

    ///
    /// \brief radical_calcualted_
    /// Whether or not RADICAL has been performed.
    bool radical_calcualted_;

    ///
    /// \brief constructor_canceled_
    /// Whether or not cancel button is clicked.
    bool constructor_canceled_;

    ///
    /// \brief flipped_
    /// Whether or not original data set had x in the second column
    bool flipped_;

    ///
    /// \brief main_window_
    /// Pointer back to the main window of the app.
    MainWindow *main_window_;

    ///
    /// \brief non_spatial_
    /// True if the dataset has empty x_ and y_
    bool non_spatial_;
};

#endif // VespucciDataset_H
