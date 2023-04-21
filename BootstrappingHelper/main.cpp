#include <iostream>
#include <utility>
#include <functional>
#include <Eigen/Dense>
#include "nonlinearSolvers.h"

using namespace std;
using namespace Eigen;
using vec = Matrix<double, Dynamic, 1>;

pair<double, double> linearInterpolator(const double t_0, const double t_1, const double t, const double r_0) {
    double a = (t_1 - t) * r_0 / (t_1 - t_0);
    double b = (t - t_0) / (t_1 - t_0);

    return make_pair(a, b);
}
int main()
{
    cout << "You want to: " << endl;
    cout << "* Interpolate a rate --------------------> (1)\n";
    cout << "* Solve for rate with Newton's method ---> (2)\n";
    int choice;
    cin >> choice;
    if (choice == 1) {
        cout << "\n Insert t0, t1, t, and r0: \n";
        double t_0, t_1, t, r_0;
        cin >> t_0 >> t_1 >> t >> r_0;
        pair<double, double> coefficients = linearInterpolator(t_0, t_1, t, r_0);
        cout << "Coefficients are (a + b * r_1): \n";
        cout << "a = " << get<0>(coefficients) << " | b = " << get<1>(coefficients) << '\n';
    } else if (choice == 2) {
        double n = 2., P = 100.;
        double B = 103., C = 0.04, T = 5.;
        vec t {{0.5, 1., 1.5, 2., 2.5, 3.}};
        vec rates {{0.0506356, 0.0493696, 0.0520786, 0.0514504, 0.0491534, 0.0468564}};
        vec discounts = ( exp(-t.array() * rates.array()) ).matrix();

        vec a{{0.0407931, 0.0271954, 0.0135977}};
        vec b{{0.100002, 0.400001, 0.700001}};
        vec t_x{{3.5, 4., 4.5}};
        function<double (double)> bond =
            [&](double x) {
                vec discounts_x = ( exp (-t_x.array() * (a.array() + b.array() * x)) ).matrix();
                if (t_x.maxCoeff() == 0.0)
                    discounts_x *= 0.0;
                return (C * P / n) * ( discounts.sum() + discounts_x.sum() )  +
                       (P * (1 + C / n)) * exp(-T * x) - B;
            };
        function<double (double)> deriv_bond =
            [&](double x) {
                vec discounts_x = ( exp (-t_x.array() * (a.array() + b.array() * x)) ).matrix();
                return -(C * P / n) * ( t_x.array() * b.array() * discounts_x.array() ).matrix().sum() -
                       (P * (1 + C / n)) * T * exp(-T * x);
            };

        double x_rate = newtonsMethod(0.1, bond, deriv_bond);
        //double x_rate = bisectionMethod(-0.1, 0.1, bond);
        cout << "The rate requested is " << x_rate << '\n';
    } else
        throw("Pick either 1 or 2\n");

    return 0;
}
