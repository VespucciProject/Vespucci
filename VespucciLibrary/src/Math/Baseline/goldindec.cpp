#include "mconvert.h"
#include "goldindec.h"
///
/// \brief Spec::Smoothing::Goldindec
/// \param vec X Input arma::vec
/// \param vec Y Input arma::vec
/// \param p poly_order Polynomail order
/// \param peak_ratio
/// \param eps
/// \return vec baseline
/// 
// References: Goldindec: A Novel Algorithm for Raman Spectrum Baseline Correction
vec Spec::Baseline::GoldindecBaseline(vec X, vec Y, uword p, double peak_ratio, double eps)
{
  double OLD_s, OPT_s, s, udr, up_down_rate, a, b ;
  uword t;
  vec OPT_z, n, y, z ;
  uvec i;
  y = Y ;
  n = X ;
  a = 0.0 ;
  b = 1.0 ;
  udr = T_rate(peak_ratio) ;
  s = a+0.382*(b-a) ;
  z = LEGEND_C(y, n, p, s) ;
  up_down_rate = m2cpp::length(uvec(find(y>=z)))*1.0/m2cpp::length(uvec(find(y<z))) ;
  t = 0 ;
  while (std::abs(up_down_rate-udr)>eps)
  {
    OLD_s = s ;
    if (up_down_rate-udr>eps)
    {
      a = s ;
      s = a+0.382*(b-a) ;
      z = LEGEND_C(y, n, p, s) ;
      up_down_rate = m2cpp::length(uvec(find(y>=z)))*1.0/m2cpp::length(uvec(find(y<z))) ;
    }
    else
    {
      b = s ;
      s = a+0.382*(b-a) ;
      z = LEGEND_C(y, n, p, s) ;
      up_down_rate = m2cpp::length(uvec(find(y>=z)))*1.0/m2cpp::length(uvec(find(y<z))) ;
    }
    if (std::abs(OLD_s-s)<0.00001)
      break ;
    t = t+1 ;
  }
  OPT_s = s ;
  OPT_z = LEGEND_C(y, n, p, OPT_s) ;
  return OPT_z ;
}

vec Spec::Baseline::LEGEND_C(vec y, vec n, uword ord, double s)
{
  double alpha, dely, maxy ;
  mat T, Tinv ;
  uword N, num, it;
  vec a, d, res, z, zp ;
  rowvec p;
  uvec i, j;
  N = m2cpp::length(n) ;
  n = sort(n) ;
  i = sort_index(n) ;
  y = y(i) ;
  maxy = arma::max(y) ;
  dely = (maxy-arma::min(y))/2.0 ;
  n = 2*(n-n(N-1))/(n(N-1)-n(0))+1 ;
  y = (y-maxy)/dely+1 ;
  p = m2cpp::fspan(0, 1, ord);
  mat n_temp = repmat(n, 1, ord+1);
  for(num=0;num<ord+1;num++)
      T.insert_cols(num, arma::pow(n_temp.col(num), p[num]));
  //T = arma::pow(repmat(n, 1, ord+1), repmat(p, N, 1)) ;
  Tinv = pinv(arma::trans(T)*T)*arma::trans(T) ;
  a = Tinv*y ;
  z = T*a ;
  alpha = 0.99*1/2.0 ;
  it = 0 ;
  zp = arma::ones<vec>(N) ;
  d = arma::zeros<vec>(N) ;
  while (arma::as_scalar(arma::sum(arma::square((z-zp))))/arma::as_scalar(arma::sum(arma::square(zp)))>1e-9)
  {
    it = it+1 ;
    zp = z ;
    res = y-z ;
    for (num=0; num<N; num++)
    {
      if (res[num]<s)
        d[num] = res[num]*(2*alpha-1) ;
      else
        d[num] = -res[num]-alpha*(pow(s, 3))/(2*res[num]*res[num]) ;
    }
    a = Tinv*(y+d) ;
    z = T*a ;
  }
  j = sort_index(i) ;
  z = (z(j)-1)*dely+maxy ;
  return z ;
}

double Spec::Baseline::T_rate(double x)
{
  double y ;
  y = 0.7679+11.2358*x-39.7064*pow(x, 2)+92.3583*pow(x, 3) ;
  return y ;
}
