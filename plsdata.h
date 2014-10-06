#ifndef PLSDATA_H
#define PLSDATA_H
#include "vespuccidataset.h"
using namespace arma;

///
/// \brief The PLSData class
/// A class for performing and storing data related to partial least squares
/// determinant analysis
class PLSData
{
public:
    PLSData(QSharedPointer<VespucciDataset> parent,
            QString *directory);

    bool Apply(mat spectra, rowvec wavelength, int components);

    mat *X_loadings();
    mat *Y_loadings();
    mat *X_scores();
    mat *Y_scores();
    mat *coefficients();
    mat *percent_variance();  

    int NumberComponents();
    colvec Results(const uword i, bool &valid);
private:
    ///
    /// \brief Y_
    /// In this case, "dummy" data.
    mat Y_;

    ///
    /// \brief X_loadings_
    /// Output
    mat X_loadings_;

    ///
    /// \brief Y_loadings_
    /// Output
    mat Y_loadings_;
    ///
    /// \brief X_scores_
    /// Output
    mat X_scores_;

    ///
    /// \brief Y_scores_
    /// Output
    mat Y_scores_;

    ///
    /// \brief coefficients_
    /// Output
    mat coefficients_;

    ///
    /// \brief percent_variance_
    /// Output. This determines how much each determinant
    mat percent_variance_;

    ///
    /// \brief fitted_
    /// Fitted data, pretty much useless for our purposes
    mat fitted_;
    ///
    /// \brief parent_
    /// The dataset that starts this instance
    QSharedPointer<VespucciDataset> parent_;

    ///
    /// \brief directory_
    /// Global working directory
    QString *directory_;
};

#endif // PLSDATA_H
