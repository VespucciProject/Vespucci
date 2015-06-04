#include "cwtridge.h"
#include "Math/VespucciMath.h"
Vespucci::Math::CWTRidge::CWTRidge(int id)
{
    id_ = id;
}

///
/// \brief Vespucci::Math::CWTRidge::InsertPoint
/// \param row
/// \param column
/// \param value
/// Add a pair to the list of ridge positions
void Vespucci::Math::CWTRidge::InsertPoint(arma::uword row, arma::uword column, double value)
{

    arma::uvec new_row(2);
    new_row(0) = row;
    new_row(1) = column;
    try{
        points_.insert_rows(points_.n_rows, new_row.t());
        coefs_.resize(points_.n_rows);
        coefs_(coefs_.n_rows - 1) = value;
    }catch(std::exception e){
        std::cerr << "error in Vespucci::Math::CWTRidge::InsertPoint" << std::endl;
        throw e;
    }
}

void Vespucci::Math::CWTRidge::Merge(Vespucci::Math::CWTRidge &new_ridge)
{
    try{
        points_.insert_rows(points_.n_rows, new_ridge.points());
        coefs_.insert_rows(coefs_.n_rows, new_ridge.coefs());
    }catch(std::exception e){
        std::cerr << "Error in Vespucci::Math::CWTRidge::Merge" << std::endl;
    }
}


arma::umat Vespucci::Math::CWTRidge::points() const
{
    return points_;
}


///
/// \brief Vespucci::Math::CWTRidge::RemoveLastPoint
/// remove the last added point (in case gap is too high)
void Vespucci::Math::CWTRidge::RemoveLastPoint()
{
    if (points_.n_rows > 0)
        points_.shed_row(points_.n_rows - 1);
}

///
/// \brief Vespucci::Math::CWTRidge::HasPoint
/// \param row
/// \param column
/// \return Whether or not a point with coordinates given exists
///
bool Vespucci::Math::CWTRidge::HasPoint(arma::uword row, arma::uword column) const
{
    arma::uvec row_ind = arma::find(points_.col(0) == row && points_.col(1) == column);
    return (row_ind.n_elem > 0);
}

///
/// \brief Vespucci::Math::CWTRidge::value
/// \param row
/// \param column
/// \return The CWT coefficient at a particular row/column pair
///
double Vespucci::Math::CWTRidge::value(arma::uword row, arma::uword column) const
{
    arma::uvec ind = arma::find(points_.col(0) == row && points_.col(1) == column);
    return (ind.n_elem > 0 ? coefs_(ind(0)) : std::nan(""));
}

double Vespucci::Math::CWTRidge::max_value() const
{
    arma::uvec max_indices = arma::find(coefs_ == coefs_.max());
    return coefs_(max_indices(0));
}

///
/// \brief Vespucci::Math::CWTRidge::coefs
/// \return A vector consisting of the CWT coefficients of this ridge
///
arma::vec Vespucci::Math::CWTRidge::coefs() const
{
    return coefs_;
}

///
/// \brief Vespucci::Math::CWTRidge::LastPosition
/// \return The position most recently added (if checking for gap)
///
arma::uword Vespucci::Math::CWTRidge::LastPosition() const
{
    if (points_.n_rows == 0)
        return 0;
    else
        return points_(points_.n_rows-1);
}

///
/// \brief Vespucci::Math::CWTRidge::length
/// \return The ridge length (number of points in the ridge)
///
arma::uword Vespucci::Math::CWTRidge::length() const
{
    return coefs_.n_rows;
}

///
/// \brief Vespucci::Math::CWTRidge::PeakCenter
/// \return The row number of the center of the peak (point with highest coefficient)
///
arma::uword Vespucci::Math::CWTRidge::PeakCenter() const
{
    arma::uvec ind = arma::find(coefs_ == coefs_.max());
    //ind(0) is where to look
    arma::Row<arma::uword> point = points_.row(ind(0));
    //The row of the peak center corresponds to the wavenumber
    return point(0);
}

double Vespucci::Math::CWTRidge::EstimateSNR(std::string method, arma::uword window_size, const arma::vec &noise)
{
    double signal = coefs_.max();
    arma::uword start, end, center;
    try{
        center = this->PeakCenter();
    }
    catch(std::exception e){
        std::cerr << "SNR(): error finding peak center" << std::endl;
        throw e;
    }
    start = (center > window_size ? center - window_size : 0);
    end = (center + window_size >= coefs_.n_elem ? noise.n_elem - 1 : center + window_size);
    arma::vec abs_noise;
    try{
        abs_noise = arma::abs(noise.rows(start, end)); //first scale must be sent
    }
    catch(std::exception e){
        std::cerr << "SNR(): error calculating noise" << std::endl;
        std::cerr << "center = " << center << std::endl;
        std::cerr << "start = " << start << "\t end = " << end << std::endl;
        std::cerr << "noise has " << noise.n_rows << " rows" << std::endl;
        throw e;
    }

    double noise_level;
    if (method == "quantile"){
        noise_level = Vespucci::Math::quantile(abs_noise, 0.95);
    }
    else if (method == "mean"){
        noise_level = arma::mean(abs_noise);
    }
    else if (method == "stddev"){
        noise_level = arma::stddev(abs_noise);
    }
    else{
        throw std::invalid_argument("CWTRidge::SNR: " + method + " not a valid SNR method");
    }
    calculated_snr_ = std::abs(signal / noise_level);
    return calculated_snr_;
}

///
/// \brief Vespucci::Math::CWTRidge::SNR
/// \param method How to estimate noise (quantile, mean, stddev)
/// \param window_size Area of "support region"
/// \param noise A vector representing CWT at scale = 1;
/// \return
///
double Vespucci::Math::CWTRidge::SNR() const
{
    return calculated_snr_;
}

double Vespucci::Math::CWTRidge::EstimateWidth(const arma::vec &spectra, const arma::vec &first_haar_coefs, const arma::vec &second_haar_coefs, const arma::vec &abscissa)
{
    std::cout << "CWTRidge::EstimateWidth" << std::endl;
    arma::uword first = PeakCenter() - 3*scale();
    arma::uword last =  (PeakCenter() + 3*scale() < spectra.n_rows ? PeakCenter() + 3*scale() : spectra.n_rows - 1);
    arma::vec X = spectra.rows(first, last);//search
    arma::vec dX = first_haar_coefs.rows(first, last);
    arma::vec d2X = second_haar_coefs.rows(first, last);
    arma::sp_vec local_minima = Vespucci::Math::LocalMinima(X, dX, d2X);

    arma::vec q = scale() * arma::ones(local_minima.n_rows) - local_minima;

    arma::uvec negatives = arma::find(q > 0);
    arma::uvec positives = arma::find(q < 0);


    if ((negatives.n_elem > 0)){
        left_minimum_ = negatives.max();
    }
    else{
        arma::vec window = spectra.rows(first, PeakCenter());
        arma::uvec q1 = arma::find(window == window.min());
        left_minimum_ = PeakCenter() - q1.max(); //pick closest if more than one found
    }

    if (positives.n_elem > 0){
        right_minimum_ = positives.min();
    }

    else{
        arma::vec window = spectra.rows(PeakCenter(), last);
        arma::uvec q1 = arma::find(window == window.min());
        right_minimum_ = PeakCenter() - q1.min(); //pick closest if more than one found
    }
    width_ = std::abs(abscissa(left_minimum_) - abscissa(right_minimum_));
    return width_;
}

///
/// \brief Vespucci::Math::CWTRidge::EstimateWidth
/// \param spectra
/// \param abscissa
/// \param max_width should be the even number before a power of two
/// \return
///
double Vespucci::Math::CWTRidge::EstimateWidth(const arma::vec &spectra, const arma::vec &abscissa)
{
    arma::uword center = PeakCenter();
    arma::uword scl = scale();
    //for some reason, it didn't round down, so I'm using conditional
    arma::uword first = ((3*scl >= center) ? 0 : center - 3*scl);
    arma::uword last = ((center + 3*scl) < spectra.n_rows ? center + 3*scl : spectra.n_rows - 1);
    arma::vec X;
    try{
        X = spectra.rows(first, last);//search
    }catch(std::exception e){
        std::cerr << "center = " << center << std::endl;
        std::cerr << "scl = " << scl << std::endl;
        std::cerr << "first = " << first << std::endl;
        std::cerr << "last = " << last << std::endl;
        std::cerr << "X.n_rows = " << X.n_rows << std::endl;
        std::cerr << "error resizing X" <<std::endl;
        throw e;
    }

    arma::uvec scales = {scl};
    arma::mat dX, d2X;
    try{
        dX = Vespucci::Math::Transform::cwt(X, "haar", scales);
    }catch(std::exception e){
        std::cerr << "error calculating dX!" <<std::endl;
        throw e;
    }
    try{
        d2X = Vespucci::Math::Transform::cwt(dX.col(0), "haar", scales);
    }catch(std::exception e){
        std::cerr << "error calculating d2X!" << std::endl;
        throw e;
    }
    arma::sp_vec local_minima;
    try{
        local_minima = Vespucci::Math::LocalMinima(X, dX, d2X);
    }catch(std::exception e){
        std::cerr << "error calculating local_minima!" << std::endl;
        throw e;
    }

    //find the local minima larger than and smaller than the center index
    arma::uvec non_zero_indices = arma::find( (arma::vec) local_minima);
    arma::uvec lefts = non_zero_indices(arma::find(non_zero_indices < center));
    arma::uvec rights = non_zero_indices(arma::find(non_zero_indices > center));


    try{
        if ((lefts.n_elem > 0)){
            left_minimum_ = lefts.max();
        }
        else{
            arma::vec window = spectra.rows(first, center);
            arma::uvec q1 = arma::find(window == window.min());
            left_minimum_ = center - q1.max(); //pick closest if more than one found
        }

        if (rights.n_elem > 0){
            right_minimum_ = rights.min();
        }

        else{
            arma::vec window = spectra.rows(center, last);
            arma::uvec q1 = arma::find(window == window.min());
            right_minimum_ = center + q1.min(); //pick closest if more than one found
        }
        width_ = std::abs(abscissa(left_minimum_) - abscissa(right_minimum_));
    }catch(std::exception e){
        std::cerr << "error in rest of function!" << std::endl;
        std::cerr << "left_minimum_ = " << left_minimum_ << std::endl;
        std::cerr << "right_minimum_ = " << right_minimum_ << std::endl;
        std::cerr << "abscissa.n_rows = " << abscissa.n_rows << std::endl;
        throw e;
    }

    return width_;

}

double Vespucci::Math::CWTRidge::width() const
{
    return width_;
}

int Vespucci::Math::CWTRidge::id() const
{
    return id_;
}

///
/// \brief Vespucci::Math::CWTRidge::scale
/// \return The ideal scale (proportional to peak width) of this ridge
///
arma::uword Vespucci::Math::CWTRidge::scale() const
{
    //find the maximum value of coefs_
    arma::uvec max_indices = arma::find(coefs_ == coefs_.max());
    arma::Row<arma::uword> indices = points_.row(max_indices(0));
    return indices(1) + 1; //column zero === scale = 1.
}

///
/// \brief Vespucci::Math::CWTRidge::gap
/// \return The maximum difference between two adjacent points on the ridge
///
arma::uword Vespucci::Math::CWTRidge::gap() const
{
    if (points_.n_rows < 2){
        return 0;
    }
    else{
        arma::uvec gaps = Vespucci::Math::diff((arma::uvec) points_.col(0));
        return gaps.max(); //maximum difference between two rows
    }
}

///
/// \brief Vespucci::Math::CWTRidge::operator >
/// \param c Another CWTRidge to compare peak centers
/// \return
///
bool Vespucci::Math::CWTRidge::operator >(const Vespucci::Math::CWTRidge &c) const
{
    return (c.PeakCenter() > PeakCenter());
}

///
/// \brief Vespucci::Math::CWTRidge::operator >
/// \param center A value to compare to the peak center
/// \return
///
bool Vespucci::Math::CWTRidge::operator >(const arma::uword &c) const
{
    return (c > PeakCenter());
}

///
/// \brief Vespucci::Math::CWTRidge::operator >=
/// \param c Another CWTRidge to compare peak centers
/// \return
///
bool Vespucci::Math::CWTRidge::operator >=(const Vespucci::Math::CWTRidge &c) const
{
    return (c.PeakCenter() >= PeakCenter());
}

///
/// \brief Vespucci::Math::CWTRidge::operator >=
/// \param center A value to compare to the peak center
/// \return
///
bool Vespucci::Math::CWTRidge::operator >=(const arma::uword &c) const
{
    return (c >= PeakCenter());
}

///
/// \brief Vespucci::Math::CWTRidge::operator ==
/// \param c Another CWTRidge to compare peak centers
/// \return
///
bool Vespucci::Math::CWTRidge::operator ==(const Vespucci::Math::CWTRidge &c) const
{
    return (c.PeakCenter() == PeakCenter());
}

///
/// \brief Vespucci::Math::CWTRidge::operator ==
/// \param center A value to compare to the peak center
/// \return
///
bool Vespucci::Math::CWTRidge::operator ==(const arma::uword &c) const
{
    return (c == PeakCenter());
}

///
/// \brief Vespucci::Math::CWTRidge::operator <=
/// \param c Another CWTRidge to compare peak centers
/// \return
///
bool Vespucci::Math::CWTRidge::operator <=(const Vespucci::Math::CWTRidge &c) const
{
    return (c.PeakCenter() <= PeakCenter());
}

///
/// \brief Vespucci::Math::CWTRidge::operator <=
/// \param center A value to compare to the peak center
/// \return
///
bool Vespucci::Math::CWTRidge::operator <=(const arma::uword &c) const
{
    return (c <= PeakCenter());
}

///
/// \brief Vespucci::Math::CWTRidge::operator <
/// \param c Another CWTRidge to compare peak centers
/// \return
bool Vespucci::Math::CWTRidge::operator <(const Vespucci::Math::CWTRidge &c) const
{
    return (c.PeakCenter() < PeakCenter());
}

///
/// \brief Vespucci::Math::CWTRidge::operator <
/// \param center A value to compare to the peak center
/// \return
bool Vespucci::Math::CWTRidge::operator <(const arma::uword &c) const
{
    return (c < PeakCenter());
}

///
/// \brief Vespucci::Math::CWTRidge::operator !=
/// \param c Another CWTRidge to compare peak centers
/// \return
bool Vespucci::Math::CWTRidge::operator !=(const Vespucci::Math::CWTRidge &c) const
{
    return (c.PeakCenter() != PeakCenter());
}

///
/// \brief Vespucci::Math::CWTRidge::operator !=
/// \param center A value to compare to the peak center
/// \return
bool Vespucci::Math::CWTRidge::operator !=(const arma::uword &c) const
{
    return (c != PeakCenter());
}
