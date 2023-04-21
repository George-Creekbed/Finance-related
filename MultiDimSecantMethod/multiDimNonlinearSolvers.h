#ifndef MULTIDIMNONLINEARSOLVERS_H_INCLUDED
#define MULTIDIMNONLINEARSOLVERS_H_INCLUDED

#include <valarray>
#include <functional>
#include <Eigen/Dense>

Eigen::Matrix<double, 3, 3> computeMatrixDeriv(const std::valarray<double>&,
                                               const std::valarray< std::function<double (std::valarray<double>)> >&,
                                               const double);

std::valarray<double> multiDimSecantMethod(const std::valarray<double>&,
                                           const std::valarray< std::function<double (std::valarray<double>)> >&,
                                           const double = 1E-6, const double = 1E-6, const double = 1E-9);

#endif // MULTIDIMNONLINEARSOLVERS_H_INCLUDED
