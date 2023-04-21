/** Computes the important quantities for a bond from input
*   All quantities are acquired as class data members,
*   useful to print all on file or on excel
*   TODO: - add parser to type interest time-law directly on input
*         - add xlw interface
*         DONE- fix virtual computeDiscountFactors() function not overridden by InstantRateBond class
*   ****EDIT: Modified version that adds an object that evaluates the discount factors apart from the Bond objects,
*             Then plugs in the resulting valarray<double> to them. Now Bond hierarchy is unnecessary.****
**/
#include <iostream>
#include <initializer_list>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <valarray>
#include <functional>
#include <variant>
#include "Bond.h"
#include "RateIntegral.h"

using std::initializer_list;    using std::valarray;
using std::cout;                using std::cin;
using std::function;            using std::copy;
using std::begin;               using std::unique_ptr;
using std::variant;             using std::get;
using std::move;

// helps with input
void flushCin() {
    cin.ignore();
    cin.clear();
}

// processes inputs
void inputFromUser(valarray<double>& times,
                double& coupon,
                variant<double, valarray<double> >& inp_rate_value,
                function<valarray<double> (valarray<double>)>& inp_rate_law,
                InterestRates& type,
                Formula& quadrature,
                double& principal,
                double& tol)
{
//    cout << "Please input coupon dates as fraction (decimal number) of the year,"
//         << " and press Enter key when done: \n";
//    constexpr size_t MAX_SIZE = 1000;
//    double input_times[MAX_SIZE];
//    int counter;
//    while (cin.peek() != '\n')  // while next input is not Enter key
//        cin >> input_times[counter++];
//    flushCin();
//    times.resize(counter);
//    copy(input_times, input_times+counter, begin(times));
    // or briefly, while testing
    //times = {0.1667, 0.6667, 1.1667, 1.6667};
    times = {0.5, 1., 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5};

    cout << "\nPlease enter the coupon rate in fraction of year (decimal): \n";
    cin >> coupon;

    cout << "\nPlease select (1, 2, 3 or 4) how the interest rate law above should be computed: \n";
    cout << "Known rates at given dates ---> (" << static_cast<int>(InterestRates::known_rate) << ")\n"; //(1)
    cout << "Zero rate --------------------> (" << static_cast<int>(InterestRates::zero_rate) << ")\n";  //(2)
    cout << "Instantaneous rate -----------> (" << static_cast<int>(InterestRates::instantaneous) << ")\n"; //(3)
    cout << "Yield rate -------------------> (" << static_cast<int>(InterestRates::given_yield) << ")\n";   //(4)
    int input_type;
    do {
        cin >> input_type;
        switch (input_type) {
            case 1:
                type = InterestRates::known_rate;
                break;
            case 2:
                type = InterestRates::zero_rate;
                break;
            case 3:
                type = InterestRates::instantaneous;
                break;
            case 4:
                type = InterestRates::given_yield;
                break;
            default:
                cout << "Select 1, 2, 3 or 4 only\n";
                flushCin();
                break;
        }
    } while (input_type < 1 || input_type > 4);
    flushCin();

    cout << "\nPlease digit a value for the principal, or press Enter to leave it at 100 : \n";
    while (cin.peek() != '\n')
        cin >> principal;
    flushCin();

    if (type == InterestRates::known_rate) {
//        inp_known_rates = {0.05, 0.0525, 0.0535, 0.055};  // debug
        cout << "Please input known interest rate at coupon dates, separated by space,"
             << " and press Enter key when done: \n";
        inp_rate_value = move( valarray<double>(0., times.size()) );
        while (cin.peek() != '\n') {  // while next input is not Enter key
            for (double& rate : get<valarray<double> >(inp_rate_value) )
                cin >> rate;
        }
        flushCin();
    } else if (type == InterestRates::instantaneous) {
        cout << "Choose the numerical integration method for the interest rate law :\n"
             << "Midpoint numeric quadrature -----> (1)\n"
             << "Trapezoidal numeric quadrature --> (2)\n"
             << "Simpson's numeric quadrature ----> (3)\n\n";
        int input_quadrature;
        do {
            cin >> input_quadrature;
            switch (input_quadrature) {
                case 1:
                    quadrature = Formula::Midpoint;
                    break;
                case 2:
                    quadrature = Formula::Trapezoidal;
                    break;
                case 3:
                    quadrature = Formula::Simpsons;
                    break;
                default:
                    cout << "Select 1, 2 or 3 only\n";
                    flushCin();
                    break;
            }
        } while (input_quadrature < 1 || input_quadrature > 3);
        flushCin();

        cout << "\nPlease enter the tolerance value sufficient for convergence of integration above,";
        cout << " or press Enter to leave it at 1E-6 : \n";
        while (cin.peek() != '\n')
            cin >> tol;
        flushCin();

    } else if (type == InterestRates::given_yield) {
        cout << "Please input the bond yield and press Enter key when done: \n";
        get<double>(inp_rate_value) = 0;
        while (cin.peek() != '\n') // while next input is not Enter key
            cin >> get<double>(inp_rate_value);
    }
}

// functor for interest rate law if any
// would be nice to input this directly through a parser
template<typename T>
function<T (T)> interestRateLaw() {
    return [](const T t)->T {
               //return 0.0525 + 1 / (100 * (1 + exp(- t * t)));
               //return 0.0525 + log(1 + 2.*t) / 200;  // another law
               //return 0.05 + 0.005 * log(1. + t);
               return 0.05 + 0.01 * log(1. + t / 2.);
    };
}


int main()
{
    // declare variables
    valarray<double> times;
    double coupon, principal{100}, tol{1E-6};
    InterestRates bond_type;
    Formula quadrature;
    variant<double, valarray<double> > inp_rate_value;
    // acquire input
    function<valarray<double> (valarray<double>)> inp_rate_law = interestRateLaw<valarray<double> >();
    inputFromUser(times, coupon, inp_rate_value, inp_rate_law,
                  bond_type, quadrature, principal, tol);

    // calculates bond data and print it on output
    valarray<double> integrated_rate = makeRateIntegral(bond_type, quadrature, times,
                                                        inp_rate_value, tol, inp_rate_law);
    unique_ptr<Bond> my_bond = makeBond(bond_type, times, coupon, integrated_rate,
                                        inp_rate_law, inp_rate_value, principal, tol);
                                        // inp_rate_law, inp_rate_value can be omitted here in makeBond

    my_bond->printData(cout, bond_type);

    return 0;
}
