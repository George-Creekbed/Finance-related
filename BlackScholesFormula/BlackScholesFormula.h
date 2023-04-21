#ifndef BLACKSCHOLESFORMULA_H_INCLUDED
#define BLACKSCHOLESFORMULA_H_INCLUDED

#include <utility>
#include <initializer_list>

std::pair<double,double> d1and2(const double&, const double&, const double&, const double&,
                           const double&, const double&, const double& = 0);
std::pair<double,double> getStandardDistr(const double&);

std::pair<double,double> BlackScholesFormula(const double&, const double&, const double&, const double&,
                           const double&, const double&, const double& = 0);
std::pair<double,double> BlackScholesFormula(const std::initializer_list<double>&);

std::pair<double,double> Delta(const double&, const double&, const double&, const double&,
                           const double&, const double&, const double& = 0);
std::pair<double,double> Gamma(const double&, const double&, const double&, const double&,
                           const double&, const double&, const double& = 0);
std::pair<double,double> Vega(const double&, const double&, const double&, const double&,
                           const double&, const double&, const double& = 0);

#endif // BLACKSCHOLESFORMULA_H_INCLUDED
