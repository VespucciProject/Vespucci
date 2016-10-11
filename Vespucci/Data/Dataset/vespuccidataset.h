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
#ifndef VespucciDataset_H
#define VespucciDataset_H
#include "Math/VespucciMath.h"
#include "Data/Analysis/abstractdataanalyzer.h"
#include <QTextStream>
#include <iostream>
#include <QVector>
#include <vector>
#include <qcustomplot.h>
#include "Data/Analysis/principalcomponentsdata.h"
#include "Data/Analysis/plsdata.h"
#include "Data/Analysis/vcadata.h"
#include "Global/vespucciworkspace.h"
#include "Data/Imaging/mapdata.h"
#include <cmath>
#include <fstream>
#include "GUI/mainwindow.h"
#include "Data/Analysis/plsdata.h"
#include "Data/Import/textimport.h"
#include "Data/Import/binaryimport.h"
#include "Data/Import/textimport.h"
#include "GUI/QAbstractItemModel/maplistmodel.h"
#include "Data/Analysis/univariatedata.h"
#include "Data/Analysis/mlpackpcadata.h"
#include "Data/Analysis/analysisresults.h"
#include "GUI/QAbstractItemModel/treeitem.h"



class MapData;
class PrincipalComponentsData;
class MlpackPCAData;
class PLSData;
class VCAData;
class SpectrumViewer;
class MainWindow;
class VespucciWorkspace;
class MapListModel;
class UnivariateData;
class AnalysisResults;
class CWTData;
class TreeItem;



using namespace std;
using namespace arma;


///
/// \brief The VespucciDataset class
/// This is the main class for dealing with hyperspectral data. This handles the
///  import and export of spectra, and the creation of maps. Images are handled
/// by the MapData class. This class is intended to be allocated on the heap inside
/// of a smart pointer, there is no copy constructor.
class VespucciDataset : public AbstractDataAnalyzer
{
public:
    VespucciDataset(const QString &h5_filename,
                    MainWindow *main_window,
                    QSharedPointer<VespucciWorkspace> ws);

    VespucciDataset(QString text_filename,
                    MainWindow *main_window,
                    QString *directory,
                    QString name,
                    QString x_axis_description,
                    QString y_axis_description,
                    bool swap_spatial,
                    std::string format);

    VespucciDataset(map<pair<int,int>, string> text_filenames,
                    MainWindow *main_window,
                    QString *directory,
                    QString name,
                    QString x_axis_description,
                    QString y_axis_description,
                    int rows, int cols);

    VespucciDataset(QString name,
                    MainWindow *main_window,
                    QString *directory,
                    QSharedPointer<VespucciDataset> original,
                    uvec indices);

    VespucciDataset(QString name,
                    MainWindow *main_window,
                    QString *directory);

    ~VespucciDataset();
    // PRE-PROCESSING FUNCTIONS //
    //general
    void Undo();

    // map editing
    void CropSpectra(double x_min, double x_max, double y_min, double y_max, double wl_min, double wl_max);

    // Normalization functions

    mat ZScoreNormCopy();
    void MinMaxNormalize();
    void VectorNormalize(uword norm);
    void MeanCenter();
    void ZScoreNormalize();
    void SNVNormalize(double offset, bool center);
    void AbsoluteValue();
    void PeakIntensityNormalize(double peak_position);
    void PeakIntensityNormalize(double left_bound, double right_bound);
    void Booleanize(double min, double max, bool keep_inside, bool oneify);
    void Clamp(double min, double max);
    void ShedZeroSpectra();
    void ShedZeroWavelengths();

    //Spectral Pre-processing

    void SubtractBackground(const QStringList &data_keys);
    void MedianFilter(unsigned int window_size);
    void LinearMovingAverage(unsigned int window_size);
    void SavitzkyGolay(unsigned int derivative_order,
                    unsigned int polynomial_order,
                    unsigned int window_size);
    void SingularValue(unsigned int singular_values);
    int QUIC_SVD(double epsilon);
    void MFBaseline(int window_size, int iterations);
    void RollingBallBaseline(size_t wm, size_t ws);
    void CWTBaseline(int lambda, int penalty_order, double SNR_threshold, double peak_shape_threshold);
    void IModPolyBaseline(const uword poly_order, const uword max_it, double threshold);
    void RemoveClippedSpectra(double threshold);
    void RemoveFlatSpectra(double threshold);
    void ZeroClippedSpectra(double threshold);
    void ZeroFlatSpectra(double threshold);
    void Scale(double scaling_factor);
    void ShedSpectrum(const uword index);
    void ZeroSpectrum(const uword index);
    int HySime();
    void TransformAbscissa(QString input_units, double input_factor,
                           QString output_units, double output_factor, QString description);
    void InterpolateToNewAbscissa(const vec &new_abscissa,
                                  unsigned int poly_order,
                                  unsigned int window_size);
    void InterpolateToNewAbscissa(const vec &new_abscissa);
    void FourierTransform(int n);
    void InverseFourierTransform(int n);
    void ApplyFTWeight(QString type, double param);
    void ApplyFTWeight(double start_offset, double end_offset,
                       double power);



    // HELPER FUNCTIONS //

    // Grabs one line of spectra_, at indices specified, recasts as QVector for QCustomPlot //
    vec PointSpectrum(uword index) const;

    // Converts wavelength_ to QVector //
    QVector<double> WavelengthQVector() const;

    // Finds the index of spectra_ with x and y values closest to the input //
    int FindIndex(int x_value, int y_value) const;
    uword FindIndex(double abscissa_value) const;

    // Returns vector containing start and end index within a range
    uvec FindRange(double start, double end) const;

    uword FindOrigin() const;

    // These QCustomPlot sizes apply to all maps created from dataset //
    int KeySize() const;
    int ValueSize() const;
    QCPRange KeyRange() const;
    QCPRange ValueRange() const;

    bool Save(QString filename);
    bool Load(QString filename);
    bool SaveSpectrum(QString filename, uword column, file_type type);

    void Univariate(const QString &name, double &left_bound, double &right_bound,
                    uword bound_window);
    void FitPeak(const QString &name,
                 const QString &peak_shape,
                 double &left_bound,
                 double &right_bound);
    void BandRatio(const QString &name,
                   double &first_left_bound, double &first_right_bound,
                   double &second_left_bound, double &second_right_bound,
                   uword bound_window);
    void ClassicalLeastSquares(const QString &name, const QStringList &reference_keys);
    void PartialLeastSquares(const QString &name, uword components);
    void PLSCalibration(const QString &name, const QStringList &control_keys);
    void TrainPLSDA(const QString &name, const QStringList &label_keys);
    void CorrelationAnalysis(const QString &control_key, QString name);
    void VertexComponents(const QString &name, uword endmembers);
    void KMeans(const QString &name, const QString &metric_text, size_t clusters);
    void PrincipalComponents(const QString &name);
    void PrincipalComponents(const QString &name, bool scale_data);
    void FindPeaks(const QString &name, double sel, double threshold,
                   uword poly_order, uword window_size);
    void AgglomerativeClustering(const QString &name, const QString &linkage, const QString &metric);
    void CalculateRepresentativeSpectrum(const QString &name, QString statistic, QString metric);


    //MEMBER ACCESS FUNCTIONS:
    vec wavelength() const;
    vec abscissa() const;
    vec x() const;
    vec y() const;
    vec x(uvec indices) const;
    vec y(uvec indices) const;
    double x(uword index) const;
    double y(uword index) const;
    vec wavelength(uvec indices) const;
    mat spectra(uvec indices) const;
    mat spectra() const;
    cx_mat cx_spectra() const;
    cx_mat cx_spectra(uvec indices) const;
    const QString name() const;
    vec indices() const;
    mat *indices_ptr();
    QStringList operations();

    bool principal_components_calculated() const;
    bool mlpack_pca_calculated() const;
    bool vertex_components_calculated() const;
    bool partial_least_squares_calculated() const;

    QList<QSharedPointer<UnivariateData> > univariate_datas();
    const QString x_axis_description() const;
    const QString y_axis_description() const;

    void SetName(QString new_name);
    void SetData(const mat &spectra, const vec &wavelength, const vec &x, const vec &y);

    void SetIndices(vec indices);

    void AddMap(QSharedPointer<MapData> map);
    void RemoveMap(const QString &name);
    int map_loading_count() const;

    void SetXDescription(QString description);
    void SetYDescription(QString description);

    QCPRange WavelengthRange() const;
    QCPRange PointSpectrumRange(int i) const;

    QCPColorGradient GetGradient(int gradient_number) const;
    QCPColorGradient GetClusterGradient(int clusters) const;

    bool ConstructorCancelled() const;
    mat AverageSpectrum(bool stats) const;

    mat *x_ptr();
    mat *y_ptr();
    mat *wavelength_ptr();
    mat *abscissa_ptr();
    mat *spectra_ptr();
    mat *spectra_old_ptr(bool *ok);

    const mat &spectra_ref();
    const vec &abscissa_ref();
    const vec &x_ref();
    const vec &y_ref();

    size_t columns() const;
    double AbscissaMin() const;
    double AbscissaMax() const;

    bool non_spatial() const;
    bool meta() const;
    void SetParentDatasetIndices(mat parent_dataset_indices);
    mat *parent_dataset_indices();
    bool Undoable() const;
    uword UniqueX() const;
    uword UniqueY() const;
    int UnivariateCount() const;

    const QString last_operation() const;

    ///
    /// \brief log_text_stream_ A log of all UI-induced functions called on this dataset
    ///
    QTextStream log_text_stream_;

    MapListModel *map_list_model();

    void AddAnalysisResult(QSharedPointer<AnalysisResults> analysis_result);
    void AddAnalysisResult(QSharedPointer<AnalysisResults> analysis_result, uword start_row, uword end_row);

    QStringList AnalysisResultsKeys() const;
    QMap<QString, QStringList> AnalysisResultsTreeStructure() const;

    void ImportAuxiliaryMatrix(const QString &name, const QString &filename);
    void AddAuxiliaryMatrix(const QString &name, mat &matrix);
    void AddMatrix(const QString &name, mat &matrix);
    QStringList AuxiliaryMatrixKeys() const;
    QStringList CoreMatrixKeys() const;


    const mat & GetAnalysisResultMatrix(const QString &results_key, const QString &matrix_key) const;
    QSharedPointer<AnalysisResults> GetAnalysisResult(const QString &key);
    const mat & GetAuxiliaryMatrix(const QString &key) const;
    const mat &GetCoreMatrix(const QString &key) const;
    bool IsCoreMatrix(const QString &key) const;
    QSharedPointer<MapData> GetMapData(const QString &key);

    void CreateMap(const QString &map_name,
                   const QString &results_key,
                   const QString &matrix_key,
                   uword column,
                   QCPColorGradient gradient);
    void CreateMap(const QString &map_name,
                   const QString &matrix_key,
                   uword column,
                   QCPColorGradient gradient);

    bool ShowMapViewer(const QString &map_key, bool show);

    QStringList MapKeys() const;

    void SetOldCopies();

    bool Contains(const QString &key) const;

    bool IsValid() const;

    bool state_changed() const;

    QString filename() const;
    QString last_save_filename() const;

    bool saved() const;

    private:

    ///
    /// \brief empty_matrix_
    /// This is returned whenever a pointer to an object that does not exist is requested
    /// This matrix should never be changed.
    const mat empty_matrix_;

    ///
    /// \brief auxiliary_matrices_
    /// An AnalysisResults that users can add and remove matrices from
    QSharedPointer<AnalysisResults> auxiliary_matrices_;

    ///
    /// \brief abscissa_
    /// The spectral abscissa of the spectra in spectra_
    vec abscissa_;
    vec abscissa_old_;

    QSharedPointer<VespucciWorkspace> workspace_;

    ///
    /// \brief x_
    /// The spatial horizontal position
    vec x_;
    vec x_old_;

    ///
    /// \brief y_
    /// The spatial vertical position
    vec y_;
    vec y_old_;

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
    /// \brief spectra_imag_
    /// The imaginary part of the spectra after fft (makes ifft)
    mat spectra_imag_;


    ///
    /// \brief baselines_
    /// If spectra is baseline corrected, this matrix is the baseline of each
    /// spectrum.
    mat baselines_;

    ///
    /// \brief name_
    /// The name chosen by the user for this dataset.
    QString name_;

    ///
    /// \brief last_operation_
    /// Description of the last operation performed
    QString last_operation_;

    ///
    /// \brief maps_
    /// List of all map data types created in this dataset.
    QList<QSharedPointer<MapData> > maps_;

    ///
    /// \brief parent_dataset_indices_
    /// The index of the parent dataset, if this is a MetaDataset and this is set.
    /// This is a 3-column matrix. 0 == dataset number, 1 = x, 2 = y.
    mat parent_dataset_indices_;

    ///
    /// \brief map_loading_count_
    /// How many maps have been loaded, used mostly for default names.
    int map_loading_count_;

    ///
    /// \brief x_axis_description_
    /// Spectral abscissa legend
    QString x_axis_description_; //abscissa legend

    ///
    /// \brief y_axis_description_
    /// Spectral ordinate legend
    QString y_axis_description_; //ordinate legend

    ///
    /// \brief directory_
    /// Pointer back to the string in the workspace containing the working path
    QString *directory_;

    ///
    /// \brief indices_
    /// The indices established on dataset import.
    vec indices_;

    ///
    /// \brief z_scores_calculated_
    /// Whether or not Z-scores have been calculated (triggers determination of
    /// "negative" peaks (finding minimums are areas where spectrum is lower than
    /// average)
    bool z_scores_calculated_;

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

    ///
    /// \brief meta_
    /// Whether or not this dataset is a MetaDataset
    bool meta_;

    ///
    /// \brief analysis_results_
    /// A container for various analyses indexed by name
    ///
    ///
    QVector<QSharedPointer<AnalysisResults> > analysis_results_;

    ///
    /// \brief operations_
    /// A list of operations performed, which can also be interptreted as a macro
    /// by the MacroParser class.
    QStringList operations_;

    ///
    /// \brief last_save_filename_
    /// The filename this dataset was last saved as
    QString last_save_filename_;

    ///
    /// \brief state_changed_
    /// Whether or not the data is different since the last save
    bool state_changed_;

    ///
    /// \brief filename_
    /// Filename this dataset was imported or loaded from
    QString filename_;

    ///
    /// \brief text_filenames_
    /// If imported from multiple files, the filenames
    map<pair<int, int>, string> text_filenames_;


    bool saved_;

};
Q_DECLARE_METATYPE(QSharedPointer<VespucciDataset> *)

#endif // VespucciDataset_H
