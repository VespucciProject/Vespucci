/*******************************************************************************
    Copyright (C) 2015 Wright State University - All Rights Reserved
    Daniel P. Foose - Author

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
#include "Math/Fitting/nonlinleastsq.h"


arma::vec Vespucci::Math::Fitting::FitBaseline(arma::vec signal, std::string function, std::vector<double> initial_params, std::vector<double> lims)
{

}


void Vespucci::Math::Fitting::Gaussian(double *p, double *x, int m, int n, void *data)
{
    //Gaussian function:
    //y = y0 + A/(w*sqrt(PI/(4*ln(2)))) * exp(-4*ln(2)*(x-xc)^2/w^2)
    //p[0] = y0
    //p[1] = xc
    //p[2] = A
    //p[3] = w



    int i;
    for (i = 0; i < n; ++i){
        x[i] = p[0] //y axis offset
                + (p[2]/(p[3]*std::sqrt(arma::datum::pi/(4*std::log(2)))) ) //"amplitude" term
                * std::exp(-4*std::log(2)*std::pow((p[1] - (double) i), 2.0) / std::pow(p[3], 2.0)); //exponential "width" term
    }
}



void Vespucci::Math::Fitting::GaussianJacobian(double *p, double *jac, int m, int n, void *data)
{
    int i, j;
    for(i=j=0; i<n; ++i){

        //with respect to y0
        jac[j++] = 1.0; //the only term for y0 is itself and a constant

        //with respect to xc
        jac[j++] = (std::pow(i, 2.0) / std::pow(p[3], 2))*-4*std::log(2) //chain rule term for xc
                   *(p[2]/(p[3]*std::sqrt(arma::datum::pi/(4*std::log(2)))) ) //"amplitude" term
                   * std::exp(-4*std::log(2)*std::pow((p[1] - (double) i), 2.0) / std::pow(p[3], 2.0)); //exponential "width" term

        //with respect to A
        jac[j++] = (1.0 /(p[3]*std::sqrt(arma::datum::pi/(4*std::log(2)))) ) //"amplitude" term without A
                   * std::exp(-4*std::log(2)*std::pow((p[1] - (double) i), 2.0) / std::pow(p[3], 2.0)); //exponential "width" term

        //with respect to w
        jac[j++] = ( (2.0 * (-4*std::log(2)*std::pow((p[1] - (double) i), 2.0) / std::pow(p[3], 2.0)) + std::pow(p[3], 2.0)) / std::pow(p[3], 4.0)) //chain rule and product rule stuff
                   * (p[2]/(std::sqrt(arma::datum::pi/(4*std::log(2)))) ) //"amplitude" term without w
                   * std::exp(-4*std::log(2)*std::pow((p[1] - (double) i), 2.0) / std::pow(p[3], 2.0)); //exponential "width" term
    }
}


///
/// \brief Vespucci::Math::Fitting::FitGaussianPeak fit a gaussian function to a vector using the Levenberg-Marquardt algorithm
/// \param signal The area the curve is to be fitted to
/// \param iterations The maximum number of iteratations to perform Levenberg-Marquardt Algorithm
/// \param reason A string describing how the algorithm ran to
/// \param params A map of pairs where the key is the name of the parameter and the value is a pair containing the value and its standard error
/// \param stats A map of double where the key is the name of a statistic and the value is the value of that statistic
/// \param residuals A
/// \return A vector containing the fitted function.
/// Perform a nonlinear least squares fitting of a gaussian peak using
/// the Levenberg-Marquardt algorithm as implemented in the levmar C library by M. I. A. Lourakis
///
/// The fitted equation is of the following form:
/// y = y0 + A/(w*sqrt(PI/(4*ln(2)))) * exp(-4*ln(2)*(x-xc)^2/w^2)
/// Params has the following keys:
/// "y0"
/// "xc"
/// "A"
/// "w"
///
/// Stats has the following keys:
/// "R squared"
/// "Adjusted R squared"
/// "Chi squared"
/// "Reduced chi squared"
/// "Points"
/// "Degrees of freedom"
/// "Residual sum of squares"
/// "Iterations"
///
arma::vec Vespucci::Math::Fitting::FitGaussianPeak(arma::vec signal,
                                                   int iterations,
                                                   std::string &reason,
                                                   std::map<std::string, std::pair<double, double> > &params,
                                                   std::map<std::string, double> &stats,
                                                   arma::vec &residuals)
{
    /*
    int i, ic, k, m, n, ntries;
    int info;
    int ma[60];
    int na[60];
    int nf[60];
    int nj[60];
    int np[60];
    int nx[60];
    double factor, fnorm1, fnorm2, tol;
    double fjac[60*40];
    double fvec[65];
    double x[40];




    int n = signal.n_rows;
    int m = 4;
    double p[m], opts[LM_OPTS_SZ], info[LM_INFO_SZ];
    int ret;

    double *x = signal.memptr(); //this is the array the levmar library will work with


    //guess some initial parameters
    //Gaussian function:
    //y = y0 + A/(w*sqrt(PI/(4*ln(2)))) * exp(-4*ln(2)*(x-xc)^2/w^2)
    //p[0] = y0
    //p[1] = xc
    //p[2] = A
    //p[3] = w


    p[0] = signal.min(); //guess y0 to be the minimum value of the input area
    p[1] = arma::mean(signal - signal.min()); //guess xc to be mean of input area
    p[2] = signal.max() - signal.min(); //guess A to be "height" of input area
    p[3] = arma::stddev(signal - signal.min()); //guess w to be standard deviation of input area














    std::cout << "Initial Parameters" << std::endl;
    std::cout << "Parameter\tValue\n";
    std::cout << "y0\t" << p[0] << "\t" << std::endl;
    std::cout << "xc\t" << p[1] << "\t" << std::endl;
    std::cout << "A\t" << p[2] << "\t"  << std::endl;
    std::cout << "w\t" << p[3] << "\t" << std::endl;

    //optimization control parameters
    //levmar options might be exposed to user later.
    opts[0] = LM_INIT_MU;
    opts[1] = 1E-15;
    opts[2] = 1E-15;
    opts[3] = 1E-20;
    opts[4] = LM_DIFF_DELTA; //not relevant because we have an analytical Jacobian

    double covar[4][4]; //[m][m] covariance matrix, needed to calculate errors in parameters

    //invoke levmar optimization function
    std::cout << "invoke levmar" << std::endl;
    ret = dlevmar_der(Vespucci::Math::Fitting::Gaussian,
                      Vespucci::Math::Fitting::GaussianJacobian,
                      p, x, m, n, iterations, opts, info,
                      NULL, &covar[0][0], NULL);

    std::cout << "Covariance matrix:" << std::endl;
    for (int i = 0; i < 4; ++i){
        for (int j = 0; j < 4; ++j){
            std::cout << covar[i][j] << "\t";
        }
        std::cout << std::endl;
    }

    std::cout << "calculate results" << std::endl;
    double fitted_a[n]; //array to hold the fitted function
    //recalculate the function with current parameters, convert to arma::vec
    Vespucci::Math::Fitting::Gaussian(p, &fitted_a[0], m, n, NULL);

    std::ofstream param_f;
    param_f.open("C:/users/dan/documents/params.csv");
    for (int i = 0; i < 4; ++i){
        param_f << p[i] << std::endl;
    }
    param_f.close();

    std::ofstream fitted_vals;
    fitted_vals.open("C:/users/dan/documents/gausfittest.csv");
    for (int i = 0; i < sizeof(fitted_a)/sizeof(fitted_a[0]); ++i){
        fitted_vals << fitted_a[i] << std::endl;
    }
    fitted_vals.close();



    arma::vec fitted(std::vector<double>(fitted_a[0], fitted_a[n-1]) );
    std::cout << "fitted.n_rows = " << fitted.n_rows << std::endl;

    std::cout << "Parameter\tValue\tErr\n";
    std::cout << "y0\t" << p[0] << "\t" << std::sqrt(covar[0][0]) << std::endl;
    std::cout << "xc\t" << p[1] << "\t" << std::sqrt(covar[1][1]) << std::endl;
    std::cout << "A\t" << p[2] << "\t" << std::sqrt(covar[2][2]) << std::endl;
    std::cout << "w\t" << p[3] << "\t" << std::sqrt(covar[3][3]) << std::endl;

    //The errors in parameters are the sqrt of the diagonal of the covariance matrix
    params["y0"] = std::pair<double, double>(p[0], std::sqrt(covar[0][0]));
    params["xc"] = std::pair<double, double>(p[1], std::sqrt(covar[1][1]));
    params["A"] = std::pair<double, double>(p[2], std::sqrt(covar[2][2]));
    params["w"] = std::pair<double, double>(p[3], std::sqrt(covar[3][3]));

    //residuals and goodness of fit measures
    try{
        residuals = signal - fitted;
    }catch(std::exception e){
        std::cerr << "residuals" <<std::endl;
        std::cerr << e.what();
    }

    double total_sum_sq, residual_sum_sq, R_squared, adj_R_squared,
            chi_squared, reduced_chi_squared;
    try{
        total_sum_sq = arma::accu(arma::pow(signal - arma::mean(signal), 2));
    }catch(std::exception e){
        std::cerr << "total_sum_sq" << std::endl;
        std::cerr << e.what();
    }
    try{
        residual_sum_sq = arma::accu(arma::pow(residuals, 2));
    }catch(std::exception e){
        std::cerr << "residual_sum_sq" << std::endl;
        std::cerr << e.what();

    }
    try{
        R_squared = 1.0 - (residual_sum_sq / total_sum_sq);
    }catch(std::exception e){
        std::cerr << "R_squared" << std::endl;
        std::cerr << e.what();

    }
    try{
        adj_R_squared = R_squared - (1.0 - R_squared)*(4 / (n - 5));
    }catch(std::exception e){
        std::cerr << "adj R_squared" << std::endl;
        std::cerr << e.what();
    }
    try{
        chi_squared = residual_sum_sq / arma::var(signal);
    }catch(std::exception e){
        std::cerr << "chi_squared" << std::endl;
        std::cerr << e.what();
    }
    try{
        reduced_chi_squared = chi_squared / (n-4.0);
    }catch(std::exception e){
        std::cerr << "reduced_chi_squared" << std::endl;
        std::cerr << e.what();
    }

    //Store relevant stats
    stats["R squared"] = R_squared;
    stats["Adjusted R squared"] = adj_R_squared;
    stats["Chi squared"] = chi_squared;
    stats["Reduced chi squared"] = reduced_chi_squared;
    stats["Points"] = n;
    stats["Degrees of freedom"] = n - 4; //four parameter fit of n will have n-4 dof
    stats["Residual sum of squares"] = residual_sum_sq;
    stats["Iterations"] = (double) info[5];


    //why regression stopped
    switch ((int) info[6]){
      case 1:
        reason = "Stopped by small gradient J^T e.";
      case 2:
        reason = "Stopped by small Dp.";
      case 3:
        reason = "Stopped because maximum number of iterations reached.";
      case 4:
        reason = "Singular matrix. Restart from current p with increased mu.";
      case 5:
        reason = "No further error reduction is possible. Restart with increased mu.";
      case 6:
        reason = "Stopped by small ||e||_2.";
      case 7:
        reason = "Stopped by invalid values. This is your fault.";
      default:
        reason = "Stopped for unknown reason";
    }


    return fitted;
    */
}
