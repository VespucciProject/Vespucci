#ifndef GOLDINDEC_H
#define GOLDINDEC_H
using namespace arma ;
namespace Spec{
    namespace Baseline{
        vec GoldindecBaseline(vec X,
                              vec Y,
                              uword p,
                              double peak_ratio,
                              double eps);
        vec LEGEND_C(vec y, vec n, uword ord, double s);
        double T_rate(double x);
    }
}
#endif // GOLDINDEC_H
