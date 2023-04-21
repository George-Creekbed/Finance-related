#include <iostream>
#include <vector>
#include <valarray>
#include <functional>
#include <cmath>
#include <algorithm>
#include <iterator>
#include "NewtonRaphsonMultiDim.h"

using namespace std;

int main()
{
    function<double (const valarray<double>)>
        f1 = [](const valarray<double> x) {
            return 0.125 * x[5] * x[1] - 0.03125 * x[5] * x[2] + 0.0375 * x[5] * x[3] + x[6] + 0.08;
        };
    function<double (const valarray<double>)>
        f2 = [](const valarray<double> x) {
            return -0.03125 * x[5] * x[1] + 0.125 * x[5] * x[2] - 0.0375 * x[5] * x[3] + x[6] + 0.12;
        };
    function<double (const valarray<double>)>
        f3 = [](const valarray<double> x) {
            return 0.0375 * x[5] * x[1] - 0.0375 * x[5] * x[2] + 0.18 * x[5] * x[3] + x[6] + 0.16;
        };
    function<double (const valarray<double>)>
        f4 = [](const valarray<double> x) {
            return 0.08 * x[4] * x[5] + x[6] + 0.05;
        };
    function<double (const valarray<double>)>
        f5 = [](const valarray<double> x) {
            return 0.0625 * pow(x[1], 2) + 0.0625 * pow(x[2], 2) + 0.09 * pow(x[3], 2) + 0.04 * pow(x[4], 2) -
                   0.03125 * x[1] * x[2] - 0.0375 * x[2] * x[3] + 0.0375 * x[1] * x[3] - 0.0576;
        };
    function<double (const valarray<double>)>
        f6 = [](const valarray<double> x) {
            return x[1] + x[2] + x[3] + x[4] - 1;
        };
    valarray<function<double (const valarray<double>)> > F{f1, f2, f3, f4, f5, f6};

    function<double (const valarray<double>)>
        df11 = [](const valarray<double> x) {
            return 0.125 * x[5];
        };
    function<double (const valarray<double>)>
        df12 = [](const valarray<double> x) {
            return -0.03125 * x[5];
        };
    function<double (const valarray<double>)>
        df13 = [](const valarray<double> x) {
            return 0.0375 * x[5];
        };
    function<double (const valarray<double>)>
        df15 = [](const valarray<double> x) {
            return 0.125 * x[1] - 0.03125 * x[2] + 0.0375 * x[3];
        };
    function<double (const valarray<double>)>
        df22 = [](const valarray<double> x) {
            return 0.125 * x[5];
        };
    function<double (const valarray<double>)>
        df23 = [](const valarray<double> x) {
            return -0.0375 * x[5];
        };
    function<double (const valarray<double>)>
        df25 = [](const valarray<double> x) {
            return -0.03125 * x[1] + 0.125 * x[2] - 0.0375 * x[3];
        };
    function<double (const valarray<double>)>
        df33 = [](const valarray<double> x) {
            return 0.18 * x[5];
        };
    function<double (const valarray<double>)>
        df35 = [](const valarray<double> x) {
            return 0.0375 * x[1] - 0.0375 * x[2] + 0.18 * x[3];
        };
    function<double (const valarray<double>)>
        df44 = [](const valarray<double> x) {
            return 0.08;
        };
    function<double (const valarray<double>)>
        df45 = [](const valarray<double> x) {
            return 0.08 * x[4];
        };
    function<double (const valarray<double>)>
        zero = [](const valarray<double> x) {
            return 0.0;
        };
    function<double (const valarray<double>)>
        one = [](const valarray<double> x) {
            return 1.0;
        };
    valarray< valarray<function<double (const valarray<double>)> > >
        DF{{df11, df12, df13, zero, df15, one},
           {df12, df22, df23, zero, df25, one},
           {df13, df23, df33, zero, df35, one},
           {zero, zero, zero, df44, df45, one},
           {df15, df25, df35, df45, zero, zero},
           {one, one, one, one, zero, zero}};

    valarray<double> X_0 = {0., 0., 0., 0., 0., 0.};

    valarray<double> sol = NewtonRaphsonNDim(F, DF, X_0);

    cout << "Solution: " << endl;
    copy(begin(sol), end(sol), ostream_iterator<double>(cout, " "));

    return 0;
}
