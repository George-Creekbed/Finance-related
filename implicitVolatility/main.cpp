#define _USE_MATH_DEFINES

#include <iostream>
#include <iomanip>
#include <cmath>
#include <functional>
#include "nonlinearSolvers.h"
#include "StandardDistribution.h"

using namespace std;

// helps with input
void flushCin() {
    cin.ignore();
    cin.clear();
}

int main()
{
    cout << "Input time to maturity T:\n";
    double T;
    cin >> T;
    cout << "Input strike price K:\n";
    double K;
    cin >> K;
    cout << "Input spot price S:\n";
    double S;
    cin >> S;
    cout << "Input call option price C:\n";
    double C;
    cin >> C;
    cout << "Input risk-free interest rate r:\n";
    double r;
    cin >> r;
    cout << "Input dividend rate d:\n";
    double d;
    cin >> d;

    StandardNormal distr;
    function<double (StandardNormal&, const double&)> cdf = StandardNormal::cumulativeDistributionFunction;
    function<double (double)> BSCall =
        [&] (double x) {
            double d1 = (log(S/K) + (r - d + 0.5 * x * x) * T) / (x * sqrt(T));
            double d2 = d1 - x * sqrt(T);
            return S * exp(- d * T) * cdf(distr, d1) - K * exp(- r * T) * cdf(distr, d2) - C;
        };
    function<double (double)> VegaCall =
        [&] (double x) {
            double d1 = (log(S/K) + (r - d + 0.5 * x * x) * T) / (x * sqrt(T));
            return S * exp(- d * T - 0.5 * d1 * d1) * sqrt(T / (2 * M_PI));
        };

    cout << "Select a nonlinear solver: \n"
         << "Bisection method:  1\n"
         << "Newton's method:   2\n"
         << "Secant method:     3\n";
    int choice;
    cin >> choice;

    double vol;
    switch(choice) {
        case 1:
            {double left(1E-4), right(1.0);
            vol = bisectionMethod (left, right, BSCall);}
            break;
        case 2:
            {double x0{0.5};
            vol = newtonsMethod (x0, BSCall, VegaCall);}
            break;
        case 3:
            {double x1{0.5}, x0{-0.5};
            vol = secantMethod (x0, x1, BSCall);}
            break;
        default:
            cout << "Select 1, 2 or 3 only\n";
            flushCin();
            break;
    }

    cout << "The implied volatility is: " << setprecision(7) << vol << "\n\n";

    return 0;
}
