#include <cmath>
#include <iostream>
#include "nonlinearSolvers.h"

using std::function;      using std::max;
using std::abs;

double bisectionMethod (const double left, const double right, function<double (double)>& f,
                        const double tol_interval, const double tol_approx)
{
    double a(left), b(right), x_half(0);

    int counter{1};
    while (max(f(a), f(b)) > tol_approx || (b - a) > tol_interval) {
        x_half = (a + b) / 2.;
        if (f(a) * f(x_half) < 0)
            b = x_half;
        else
            a = x_half;
        std::cout << "Iteration " << counter++ << ": x_half " << x_half << " | max(f(a), f(b)) "
                  << max(f(a), f(b)) << " | (b - a) " << b - a << '\n';
    }

    return x_half;
}

double newtonsMethod (const double x_0, function<double (double)>& f, function<double (double)>& deriv_f,
                      const double tol_consec, const double tol_approx)
{
    double x_new(x_0);
    double x_old = x_0 - 1;

    int counter{1};
    while (abs(f(x_new)) > tol_approx || abs(x_new - x_old) > tol_consec) {
        x_old = x_new;
        x_new = x_old - f(x_old) / deriv_f(x_old);
        std::cout << "Iteration " << counter++ << ": x_new " << x_new << " | f(new) "
                  << f(x_new) << " | (x_new - x_old) " << x_new - x_old << '\n';
    }

    return x_new;
}

double secantMethod (const double x_0, const double x_1, function<double (double)>& f,
                     const double tol_consec, const double tol_approx)
{
    double x_new(x_1), x_old(x_0);
    double x_oldold = x_0 - 1.;
    double num_deriv_f(1.);

    int counter{1};
    while (abs(f(x_new)) > tol_approx || abs(x_new - x_old) > tol_consec) {
        x_oldold = x_old;
        x_old = x_new;
        num_deriv_f = (f(x_old) - f(x_oldold)) / (x_old - x_oldold);
        x_new = x_old - f(x_old) / num_deriv_f;
        std::cout << "Iteration " << counter++ << ": x_new " << x_new << " | f(new) " << f(x_new)
                  << " | (x_new - x_old) " << x_new - x_old << '\n';
    }

    return x_new;
}

//function<double
//pickMethod(const ImplicitMethod& choice) {
//    if (choice == ImplicitMethod::bisection)
//        return bisectionMethod;
//    else if (choice == ImplicitMethod::Newton)
//        return NewtonMethod;
//    else if (choice == ImplicitMethod::secant)
//        return secantMethod;
//    else
//        throw runtime_error("Pick between choice = 1, 2, or 3");
