#include "plsdata.h"

PLSData::PLSData(mat X_loadings, mat Y_loadings,
                 mat X_scores, mat Y_scores,
                 mat coefficients, mat percent_variance)
{
    cout << "PLSData::PLSData" << endl;
    X_loadings_ = X_loadings;
    Y_loadings_ = Y_loadings;
    X_scores_ = X_scores;
    Y_scores_ = Y_scores;
    coefficients_ = coefficients;
    percent_variance_ = percent_variance;
}

mat *PLSData::X_loadings()
{
    return &X_loadings_;
}

mat *PLSData::Y_loadings()
{
    return &Y_loadings_;
}

mat *PLSData::X_scores()
{
    return &X_scores_;
}

mat* PLSData::Y_scores()
{
    return &Y_scores_;
}

mat* PLSData::coefficients()
{
    return &coefficients_;
}

mat *PLSData::percent_variance()
{
    return &percent_variance_;
}

int PLSData::NumberComponents()
{
    return coefficients_.n_cols;
}

colvec PLSData::Results(int i, bool &valid)
{
    cout << "PLSData::Results" << endl;
    cout << "i = " << i << endl;
    cout << "coefficients_.n_cols = " << coefficients_.n_cols << endl;
    if (coefficients_.n_cols < i){
        valid = false;
        return X_loadings_.col(coefficients_.n_cols - 1);
    }

    else{
        valid = true;
        return X_loadings_.col(i);
    }

}
