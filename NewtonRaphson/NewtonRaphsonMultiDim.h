#ifndef NEWTONRAPHSONMULTIDIM_H_INCLUDED
#define NEWTONRAPHSONMULTIDIM_H_INCLUDED

#include <valarray>
#include <vector>
#include <functional>

double computeNorm(const std::valarray<double>&);

std::valarray<double> NewtonRaphsonNDim(const std::valarray<std::function<double (const std::valarray<double>)> >&,
                                        const std::valarray<std::valarray<std::function<double (const std::valarray<double>)>>>&,
                                        const std::valarray<double>&,
                                        const double = 1E-6,
                                        const double = 1E-9);

#endif // NEWTONRAPHSONMULTIDIM_H_INCLUDED
