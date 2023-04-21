#include <iostream>
#include <cmath>
#include <functional>
#include <stdexcept>
#include "NewtonCotesFormulas.h"

using std::cout;            using std::abs;
using std::function;        using std::runtime_error;

template<typename Quadrature, typename T>  // also prints convergence table
T convergeToTol(const Formula& choice, const T& inf, const T& sup, int intervals,
                     const T& tol, const function<T (T)>& func, const Quadrature& my_method)
{
    // set up variables
    T old_integral = my_method(inf, sup, intervals, func);
    auto array_size = tol.size();
    printConvergenceValues(cout, 0, choice, intervals, tol[array_size-1],
                           old_integral[array_size-1], old_integral[array_size-1]);
    intervals *= 2;
    T new_integral = my_method(inf, sup, intervals, func);
    T difference = abs(new_integral - old_integral);

    // iterate until convergence
    while((difference > tol).max()) {
        printConvergenceValues(cout, 1, choice, intervals, tol[array_size-1],
                               new_integral[array_size-1], difference[array_size-1]);
        old_integral = new_integral;
        intervals *= 2;
        new_integral = my_method(inf, sup, intervals, func);
        difference = abs(new_integral - old_integral);
    }

    printConvergenceValues(cout, 2, choice, intervals, tol[array_size-1],
                           new_integral[array_size-1], difference[array_size-1]);

    return new_integral;
}

template <typename T>
T Midpoint(const T& inf, const T& sup, const int intervals, const function<T (T) >& func) {
    T interval_width = (sup - inf) / intervals;
    T result(0., inf.size());
    for (int i = 1; i <= intervals; ++i)
        result += func(inf + (i - 0.5) * interval_width);

    return result * interval_width;
}

template <typename T>
T Trapezoidal(const T& inf, const T& sup, const int intervals, const function<T (T)>& func) {
    T interval_width = (sup - inf) / intervals;
    T result = (func(inf) + func(sup)) / 2.;
    for (auto i = 1.; i <= intervals - 1; ++i)
        result += func(inf + i * interval_width);

    return result * interval_width;
}

template <typename T>
T Simpsons(const T& inf, const T& sup, const int intervals, const function<T (T)>& func) {
    T interval_width = (sup - inf) / intervals;
    T result = (func(inf) + func(sup)) / 6;
    for (auto i = 1.; i <= intervals - 1; ++i)
        result += func(inf + i * interval_width) / 3. + 2. * func(inf + (i - 0.5) * interval_width) / 3.;
    result += 2. * func(inf + (intervals - 0.5) * interval_width) / 3.;

    return result * interval_width;
}

// factory function integration methods
template<typename T> function<T (const T&, const T&, const int, const function<T (T)>&) >
createQuadrature(const Formula& choice) {
    if (choice == Formula::Midpoint)
        return Midpoint<T>;
    else if (choice == Formula::Trapezoidal)
        return Trapezoidal<T>;
    else if (choice == Formula::Simpsons)
        return Simpsons<T>;
    else
        throw runtime_error("Pick between choice = 1, 2, or 3");

    return 0;  // never gets here
}
