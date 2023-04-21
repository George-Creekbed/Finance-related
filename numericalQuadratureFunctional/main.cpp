#include <iostream>
#include <cmath>
#include <cctype>
#include <functional>
#include <chrono>
#include "NewtonCotesFormulas.h"

using std::cin;     using std::cout;
using std::pow;     using std::function;
using std::exp;

namespace chrono = std::chrono;

// helps with input
void flushCin() {
    cin.clear();
    cin.ignore();
}

// timer
unsigned int stopwatch()
{
    static auto start_time = chrono::steady_clock::now();

    auto end_time = chrono::steady_clock::now();
    auto delta    = chrono::duration_cast<chrono::microseconds>(end_time - start_time);

    start_time = end_time;

    return delta.count();
}

// use a functor this time
struct func {
    double operator()(double x) {
        return sqrt(x) * exp(-x);
    }
};

//double func(double x) {
//    return sqrt(x) * exp(-x);
//    //return pow(x, 2.5) / (1 + x * x);
//}

int main() {
    cout << "Choose method:\n"
         << "Midpoint numeric quadrature -----> (1)\n"
         << "Trapezoidal numeric quadrature --> (2)\n"
         << "Simpson's numeric quadrature ----> (3)\n\n";

    // user picks a quadrature method
//    int choice_int;
//    cin >> choice_int;
//    while (choice_int < 1 || choice_int > 3) {
//        flushCin();
//        cout << "Pick again a number from 1 to 3\n";
//        cin >> choice_int;
//    }
//    Formula choice = static_cast<Formula>(choice_int);

    // cycle through all quadratures
    stopwatch();
    for (Formula choice : {Formula::Midpoint, Formula::Trapezoidal, Formula::Simpsons}) {
        double result = printConvergenceValues(choice, 1, 3, 4, 0.000001, func() );
                           // above parameters:choice, inf, sup, intervals, tol, func
        cout << "Final result within tolerance: " << result << "\n\n";
    }
    cout << "Performance time: " << stopwatch() << " microseconds.\n\n";

    return 0;
}
