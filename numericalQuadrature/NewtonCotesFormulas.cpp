#include <iostream>
#include <iomanip>
#include "NewtonCotesFormulas.h"

using namespace std;

NewtonCotesFormulas::NewtonCotesFormulas(double a, double b, double inp_tol, string&& name):
    inf(a), sup(b), tol(inp_tol), method_name(name) {}

double NewtonCotesFormulas::convergeToTol(unsigned intervals, double (*func)(double)) {
    double old_integral = computeIntegral(intervals, func);
    intervals *= 2;
    double new_integral = computeIntegral(intervals, func);
    double difference = abs(new_integral - old_integral);

    while(difference > tol) {
        cout << setprecision(8) << string(9 - to_string(intervals).size(), ' ') <<
                intervals << string(3,' ') << fixed << new_integral << string(3,' ') <<
                scientific << difference << endl;
        old_integral = new_integral;
        intervals *= 2;
        new_integral = computeIntegral(intervals, func);
        difference = abs(new_integral - old_integral);
    }
    cout << setprecision(8) << fixed << string(9 - to_string(intervals).size(), ' ') <<
            intervals << string(3,' ') << new_integral << string(3,' ') <<
            scientific << difference << endl;

    return new_integral;
}

double NewtonCotesFormulas::printConvergenceValues(unsigned intervals, double (*func)(double)) {
    cout << method_name << " method, with tolerance " << tol << "\n";
    if (intervals == 0)
        intervals = 1;
    cout << "intervals" << string(5,' ') << "integral " << string(9,' ') << "tol\n";
    cout << setprecision(8) << fixed << string(9 - to_string(intervals).size(), ' ') <<
            intervals << string(3,' ') << computeIntegral(intervals, func) <<
            string(3,' ') << 0.0 << endl;
    double result = convergeToTol(intervals, func);

    return result;
}

Midpoint::Midpoint(double a, double b, double inp_tol):
    NewtonCotesFormulas(a, b, inp_tol, move("Midpoint")){}  //move("Midpoint")

Trapezoidal::Trapezoidal(double a, double b, double inp_tol):
    NewtonCotesFormulas(a, b, inp_tol, move("Trapezoidal")){}

Simpsons::Simpsons(double a, double b, double inp_tol):
    NewtonCotesFormulas(a, b, inp_tol, move("Simpson's")){}

double Midpoint::computeIntegral(unsigned intervals, double (*func)(double)) {
    double interval_width = (sup - inf) / intervals;
    double result = 0;
    for (unsigned i = 1; i <= intervals; ++i)
        result += func(inf + (i - 0.5) * interval_width);

    return result * interval_width;
}

double Trapezoidal::computeIntegral(unsigned intervals, double (*func)(double)) {
    double interval_width = (sup - inf) / intervals;
    double result = (func(inf) + func(sup)) / 2.;
    for (unsigned i = 1; i <= intervals - 1; ++i)
        result += func(inf + i * interval_width);

    return result * interval_width;
}

double Simpsons::computeIntegral(unsigned intervals, double (*func)(double)) {
    double interval_width = (sup - inf) / intervals;
    double result = (func(inf) + func(sup)) / 6;
    for (unsigned i = 1; i <= intervals - 1; ++i)
        result += func(inf + i * interval_width) / 3 + 2 * func(inf + (i - 0.5) * interval_width) / 3;
    result += 2 * func(inf + (intervals - 0.5) * interval_width) / 3;

    return result * interval_width;
}
