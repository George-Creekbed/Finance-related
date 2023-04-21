#define _USE_MATH_DEFINES

#include <cmath>
#include <functional>
#include <exception>
#include "BlackScholesFormula.h"
#include "StandardDistribution.h"

using std::pair;             using std::initializer_list;
using std::make_pair;        using std::exp;
using std::log;              using std::sqrt;
using std::runtime_error;    using std::function;
using std::get;

pair<double,double> d1and2(const double& s,
                           const double& k,
                           const double& r,
                           const double& d,
                           const double& vol,
                           const double& T,
                           const double& t)
{
    const double d1 = (log(s/k) + (r - d + 0.5 * vol * vol) * (T - t)) / (vol * sqrt(T - t));
    const double d2 = d1 - vol * sqrt(T - t);

    return make_pair(d1, d2);
}

pair<double,double> getStandardDistr(const double& d)
{
    StandardNormal distr;
    function<double (StandardNormal&, const double&)> cdf = StandardNormal::cumulativeDistributionFunction;

    return make_pair(cdf(distr, d), cdf(distr, -d));
}

pair<double,double> BlackScholesFormula(const double& s,
                                        const double& k,
                                        const double& r,
                                        const double& d,
                                        const double& vol,
                                        const double& T,
                                        const double& t)
{
//    const double d1 = (log(s/k) + (r - d + 0.5 * vol * vol) * (T - t)) / (vol * sqrt(T - t));
//    const double d2 = d1 - vol * sqrt(T - t);
    const double d1 = get<0>(d1and2(s, k, r, d, vol, T, t));
    const double d2 = get<1>(d1and2(s, k, r, d, vol, T, t));

    StandardNormal distr;
    function<double (StandardNormal&, const double&)> cdf = StandardNormal::cumulativeDistributionFunction;

    double call = s * exp(-d * (T - t)) * cdf(distr, d1) - k * exp(-r * (T - t)) * cdf(distr, d2);
    double put = k * exp(-r * (T - t)) * cdf(distr, -d2) - s * exp(-d * (T - t)) * cdf(distr, -d1);

    return make_pair(call, put);
}

pair<double,double> BlackScholesFormula(const initializer_list<double>& init) {
    if (init.size() > 7 || init.size() < 6)
        throw runtime_error("initialize input to 6 or 7 fields only.\n");

    return init.size() == 7 ? BlackScholesFormula(*(init.begin()), *(init.begin() + 1), *(init.begin() + 2),
                                                  *(init.begin() + 3), *(init.begin() + 4), *(init.begin() + 5),
                                                  *(init.begin() + 6))
                            :
                              BlackScholesFormula(*(init.begin()), *(init.begin() + 1), *(init.begin() + 2),
                                                  *(init.begin() + 3), *(init.begin() + 4), *(init.begin() + 5),
                                                  0.);
}

pair<double,double> Delta(const double& s,
                          const double& k,
                          const double& r,
                          const double& d,
                          const double& vol,
                          const double& T,
                          const double& t)
{
    const double d1 = get<0>(d1and2(s, k, r, d, vol, T, t));

    typedef double (StandardNormal::*f)(const double&);  // typedef as C-style function pointer this time
    StandardNormal distr;
    f cdf = &StandardNormal::cumulativeDistributionFunction;

    const double delta_call = exp(- d * (T - t)) * (distr.*cdf)(d1);
    const double delta_put = - exp(- d * (T - t)) * (distr.*cdf)(-d1);

    return make_pair(delta_call, delta_put);
}

pair<double,double> Gamma(const double& s,
                          const double& k,
                          const double& r,
                          const double& d,
                          const double& vol,
                          const double& T,
                          const double& t)
{
    const double d1 = get<0>(d1and2(s, k, r, d, vol, T, t));

    const double gamma = exp(- d * (T - t) - 0.5 * d1 * d1) / (s * vol * sqrt(2 * M_PI * (T - t)));

    return make_pair(gamma, gamma);
}

pair<double,double> Vega(const double& s,
                         const double& k,
                         const double& r,
                         const double& d,
                         const double& vol,
                         const double& T,
                         const double& t)
{
    const double d1 = get<0>(d1and2(s, k, r, d, vol, T, t));

    const double vega = s * exp(- d * (T - t) - 0.5 * d1 * d1) * sqrt((T - t) / (2 * M_PI));

    return make_pair(vega, vega);
}
