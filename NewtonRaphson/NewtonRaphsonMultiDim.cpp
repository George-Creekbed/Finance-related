#include <iostream>
#include <cmath>
#include <numeric>
#include "NewtonRaphsonMultiDim.h"

using std::valarray;        using std::vector;
using std::function;        using std::abs;
using std::cout;            using std::endl;
using std::accumulate;      //using std::pow;
using std::sqrt;

// dgeev_ is a symbol in the LAPACK library files
extern "C" {
    extern int dposv_(const char*, const int*, const int*, valarray< valarray<double> >*,
                      const int*, valarray<double>*, const int*, int*);
    //extern int dgeev_(char*,char*,int*,double*,int*,double*, double*, double*, int*, double*, int*, double*, int*, int*);
}

double computeNorm(const valarray<double>& input) {
    valarray<double> input_squared = pow(input, 2.);
    double sum = accumulate(begin(input_squared), end(input_squared), 0.0);

    return sqrt(sum);
}


valarray<double> NewtonRaphsonNDim(const valarray<function<double (const valarray<double>)> >& sys,
                                   const valarray< valarray<function<double (const valarray<double>)> > >& deriv_sys,
                                   const valarray<double>& x_0, const double tol_approx, const double tol_consec)
{
    const char UPLO = 'L';
    const int N = sys.size();
    const int NRHS = 1;

    valarray< valarray<double> > A(valarray<double>(N), N);
    for (int i = 0; i != N; ++i) {
        for (int j = 0; j != N; ++j)
            A[i][j] = deriv_sys[i][j](x_0);
    }

    const int LDA(N), LDB(N);
    valarray<double> B(N);
    int INFO(0);

    valarray<double> F_current(N);
    for (int i = 0; i != N; ++i)
        F_current[i] = sys[i](x_0);

    double norm_F_current = computeNorm(F_current);
    valarray<double> x_new(x_0), x_old(6);
    x_old = x_0 - 1.0;
    valarray<double> x_diff = x_new - x_old;
    double norm_x_diff = computeNorm(x_diff);
    while(norm_F_current > tol_approx || norm_x_diff > tol_consec) {
        x_old = x_new;

        for (int i = 0; i != N; ++i) {
            for (int j = 0; j != N; ++j)
                A[i][j] = deriv_sys[i][j](x_old);
        }

//        dgeev_(&Nchar,&Nchar,&n,data,&n,eigReal,eigImag,
//               vl,&one,vr,&one, work,&lwork,&info);
        dposv_(&UPLO, &N, &NRHS, &A, &LDA, &B, &LDB, &INFO);

        // check for errors
        if (INFO != 0) {
            cout << "Error: dposv returned error code " << INFO << endl; // throw("Error: ...") would be better
        }

        x_new = x_old - B;

        for (int i = 0; i != N; ++i)
            B[i] = sys[i](x_new);
        norm_F_current = computeNorm(B);
        x_diff = x_new - x_old;
        norm_x_diff = computeNorm(x_diff);
    }

    return x_new;
}
