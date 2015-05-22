#ifndef CWTDATA_H
#define CWTDATA_H
#include "qcustomplot.h"
#include "Math/VespucciMath.h"
#include "Data/Dataset/vespuccidataset.h"
#include "Math/Transform/cwt.h"
#include "Math/PeakFinding/cwtridge.h"
#include "Math/PeakFinding/peakfinding.h"
using namespace arma;
class CWTData
{
public:
    CWTData(QSharedPointer<VespucciDataset> dataset);
    void Apply(std::string wavelet = "mexh",
               uword max_scale = 70,
               uword gap_threshold = 3,
               uword min_ridge_length = 5,
               uword search_width = 5,
               double noise_threshold = 3.0,
               std::string noise_method = "quantile",
               uword noise_window_size = 500,
               bool save_coefs = false,
               bool save_coef_plots = false,
               bool save_ridge_plots = false,
               bool save_ridge_plot_data = false,
               bool estimate_width = false,
               QString save_directory = "CWT Data",
               QString image_format = "pdf",
               QCPColorGradient gradient = QCPColorGradient::cbGreys);

    mat *centers();
private:
    QSharedPointer<VespucciDataset> dataset_;

    ///
    /// \brief peak_data_
    /// Contains, for each column (spectra) a n x 4 (where n = no. of found peaks)
    /// matrix where the first column is peak position in spectral abscissa units,
    /// second column is width in spectral abscissa units, third column is CWT
    ///  ridge length and fourth column is signal-to-noise ratio from CWT coefs.
    field<mat> peak_data_;

    ///
    /// \brief centers_
    /// The first column consists of all found centers in spectral abscissa units.
    /// The second column is the number of times a center at that position was
    /// found. The third column is the average signal-to-noise. The fourth_column
    /// is the average length.
    mat centers_;


};

#endif // CWTDATA_H
