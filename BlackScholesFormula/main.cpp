#include <iostream>
#include<iomanip>
#include "BlackScholesFormula.h"
#include "Option.h"

using namespace std;

int main()
{
    constexpr double one_twentyfour = 1./ 24;
    const double T[6] = {0.5, 0.25, 0.5, 0.25, 0.5, 0.25}; //{3, 5, 10, 20}; //{one_twentyfour, 0.25, 1};
    const double K[6] = {45, 45, 48, 48, 51, 51};
    Call my_call{30.625, 40, 0.05, 0, 0.3, 0.5};//spot, strike, interest_rate, dividend_rate, vol, maturity //{102., 100., 0.05, 0., 0.3, 0.5};//{92., 90., 0.05, 0., 0.2, 0.25};//{50., 50., 0., 0., 0.3, T[2]}; //{50., 45., 0.06, 0.02, 0.2, 0.5};
    Put my_put{40, K[2], 0.05, 0., 0.3, T[2]};//{102., 100., 0.05, 0., 0.3, 0.5};//{92., 90., 0.05, 0., 0.2, 0.25};//{50., 50., 0., 0., 0.3, T[2]}; //(50., 45., 0.06, 0.02, 0.2, 0.5);

    cout.precision(10);
    cout << "Results:\n";
    cout << "European Option:\n";
    cout << "Value: Call = " << my_call.computeBlackScholesValue()
         << " ; Put = " << my_put.computeBlackScholesValue() << '\n';
    cout << "Delta: Call = " << my_call.computeDelta()
         << " ; Put = " << my_put.computeDelta() << '\n';
    cout << "Gamma: Call = " << my_call.computeGamma()
         << " ; Put = " << my_put.computeGamma() << '\n';
    cout << "Vega: Call = " << my_call.computeVega()
         << " ; Put = " << my_put.computeVega() << '\n';
    cout << "Delta, no dividends: Call = " << my_call.deltaNoDividends()
         << " ; Put = " << my_put.deltaNoDividends() << '\n';
    cout << "Probability to be in-the-money: Call = " << my_call.probabilityInTheMoney()
         << " ; Put = " << my_put.probabilityInTheMoney() << '\n';

//    double x = 0.125;
//    cout << "N(x): " << getStandardDistr(x).first << '\n';

    return 0;
}
