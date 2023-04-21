#ifndef NONLINEARSOLVERS_H_INCLUDED
#define NONLINEARSOLVERS_H_INCLUDED

#include <functional>

double bisectionMethod (const double, const double, std::function<double (double)>&,
                        const double = 1E-6, const double = 1E-9);
double newtonsMethod (const double, std::function<double (double)>&, std::function<double (double)>&,
                      const double = 1E-6, const double = 1E-9);
double secantMethod (const double, const double, std::function<double (double)>&,
                     const double = 1E-6, const double = 1E-9);

#endif // NONLINEARSOLVERS_H_INCLUDED
