#include <iostream>
#include <cmath>
//#include <numeric>
#include "multiDimNonlinearSolvers.h"

using std::cout;        using std::function;
using std::valarray;    using std::abs;
using std::begin;       using std::end;
using namespace Eigen;

typedef Matrix<double, 3, 1> Eigvect;
typedef Matrix<double, 3, 3> Eigmat;

//double computeNorm(const valarray<double>& input) {
//    valarray<double> input_squared = pow(input, 2.);
//    double sum = accumulate(begin(input_squared), end(input_squared), 0.0);
//
//    return sqrt(sum);
//}

Eigmat computeMatrixDeriv(const valarray<double>& x,
                          const valarray< function<double (valarray<double>)> >& F,
                          const double h)
{
    valarray<double> h1{h, 0., 0.}, h2{0., h, 0.}, h3{0., 0., h};
    Eigvect x_vect{x[0], x[1], x[2]};
    double a11 = 0.5 * (F[0](x + h1) - F[0](x - h1)) / h;
    double a12 = 0.5 * (F[0](x + h2) - F[0](x - h2)) / h;
    double a13 = 0.5 * (F[0](x + h3) - F[0](x - h3)) / h;
    double a21 = 0.5 * (F[1](x + h1) - F[1](x - h1)) / h;
    double a22 = 0.5 * (F[1](x + h2) - F[1](x - h2)) / h;
    double a23 = 0.5 * (F[1](x + h3) - F[1](x - h3)) / h;
    double a31 = 0.5 * (F[2](x + h1) - F[2](x - h1)) / h;
    double a32 = 0.5 * (F[2](x + h2) - F[2](x - h2)) / h;
    double a33 = 0.5 * (F[2](x + h3) - F[2](x - h3)) / h;
    Eigmat A;
    A << a11, a12, a13,
         a21, a22, a23,
         a31, a32, a33;

    return A;
}

valarray<double> multiDimSecantMethod(const valarray<double>& x_0,
                                      const valarray< function<double (valarray<double>)> >& F,
                                      const double h, const double tol_consec, const double tol_approx)
{
    Eigvect x_new {x_0[0], x_0[1], x_0[2]};
    Eigvect x_old {x_new[0] - 1., x_new[1] - 1., x_new[2] - 1.};
    valarray<double> F_new{F[0](x_0), F[1](x_0), F[2](x_0)};
    valarray<double> x_new_val{x_0}, x_old_val{x_0};
    Eigvect B {F_new[0], F_new[1], F_new[2]};
    int counter{1};
    while ((B.norm() > tol_approx) || ((x_new - x_old).norm() > tol_consec)) {
        x_old = x_new;
        x_old_val = x_new_val;

        Eigmat A = computeMatrixDeriv(x_old_val, F, h);
        Eigvect linsys = A.partialPivLu().solve(B);
        x_new = x_old - linsys;

        x_new_val = {x_new[0], x_new[1], x_new[2]};
        F_new = {F[0](x_new_val), F[1](x_new_val), F[2](x_new_val)};
        B = {F_new[0], F_new[1], F_new[2]};
        cout << "Iteration: " << counter++ << "\n x_new: \n" << x_new << "\n max(f(a), f(b)): "
             << B.norm() << " | (b - a) " << (x_new - x_old).norm() << '\n';
    }

    return x_new_val;
}
