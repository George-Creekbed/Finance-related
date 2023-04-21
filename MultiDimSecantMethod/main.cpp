#include <iostream>
#include <valarray>
#include <functional>
#include <cmath>
#include <algorithm>
#include <iterator>
//#include <Eigen/Dense>
#include "multiDimNonlinearSolvers.h"

using namespace std;
//using namespace Eigen;

int main()
{
    valarray<double> X{1, 2, 3};
    //valarray<double> X{2., 2., 2.};

    function<double (valarray<double>)> f1 =
        [] (const valarray<double>& x){
            return pow(x[0], 3.) + 2. * x[0] * x[1] + pow(x[2], 3.) - x[1] * x[2] + 9.;
        };
    function<double (valarray<double>)> f2 =
        [] (const valarray<double>& x){
            return 2 * pow(x[0], 2.) + 2. * x[0] * pow(x[1], 2.) + pow(x[1], 3.) * pow(x[2], 2.)
                   - pow(x[1], 2.) * x[2] - 2.;
        };
    function<double (valarray<double>)> f3 =
        [] (const valarray<double>& x){
            return x[0] * x[1] * x[2] + pow(x[0], 3.) - pow(x[2], 2.) - x[0] * pow(x[1], 2.) - 4.;
        };
    valarray< function<double (valarray<double>)> > F{f1, f2, f3};

    valarray<double> sol = multiDimSecantMethod(X, F);
    cout << "Solution is ";
    copy(begin(sol), end(sol), ostream_iterator<double>(cout, " ") );
    cout << endl;

    return 0;
}
