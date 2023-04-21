#define _USE_MATH_DEFINES

#include <cmath>
#include "StandardDistribution.h"

using std::abs;     using std::exp;     using std::sqrt;

double StandardNormal::probabilityDensityFunction(const double& x) {
    return (1.0 / sqrt(2.0 * M_PI)) * exp(- 0.5 * x * x);
}

double StandardNormal::cumulativeDistributionFunction(const double& t) {
    double z = abs(t);
    double y = 1. / (1. + 0.2316419 * z);
    constexpr double a1 = 0.319381530;
    constexpr double a2 = -0.356563782;
    constexpr double a3 = 1.781477937;
    constexpr double a4 = -1.821255978;
    constexpr double a5 = 1.330274429;
    double m = 1 - exp(- 0.5 * t * t) * y * (a1 + y * (a2 + y * (a3 + y * (a4 + y * a5)))) / sqrt(2 * M_PI);

    return t > 0 ? m : 1.0 - m;
}
