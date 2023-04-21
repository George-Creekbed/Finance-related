#ifndef STANDARDDISTRIBUTION_H_INCLUDED
#define STANDARDDISTRIBUTION_H_INCLUDED

#include <random>

class StandardNormal: public std::normal_distribution<double> {
public:
    // normal_distribution is set to (0, 1) by default, so no need for constructor

    double probabilityDensityFunction(const double&);
    double cumulativeDistributionFunction(const double&);

    double var() const {return std::normal_distribution<double>::stddev() * std::normal_distribution<double>::stddev();}
};

#endif // STANDARDDISTRIBUTION_H_INCLUDED
