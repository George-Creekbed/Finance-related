#include <iostream>
#include <iomanip>
#include <numeric>
#include <cmath>
#include <iterator>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <variant>
#include "Bond.h"

using std::valarray;    using std::function;
using std::begin;       using std::end;
using std::accumulate;  using std::exp;
using std::copy;        using std::setprecision;
using std::cout;        using std::move;
using std::ostream;     using std::ostream_iterator;
using std::make_unique; using std::runtime_error;
using std::unique_ptr;  using std::fixed;
using std::scientific;  using std::variant;
using std::get;

// BASE CLASS
// sets auxiliary variables that are not included in the class as data
void Bond::setConstants(size_t& index_maturity, double& coupon_frequency) {
    if (cash_times.size() == 1) {
        index_maturity = 0;
        coupon_frequency = 1 / cash_times[index_maturity];
    } else {
        index_maturity = cash_times.size() - 1;
        coupon_frequency = 1 / (cash_times[index_maturity] - cash_times[index_maturity - 1]);
    }
}

void Bond::computeDiscountFactors() {
    discount_factors = exp(- integrated_rates);
}

// compute main bond quantities
void Bond::computeValues(const double coupon_frequency, const double principal, const double tol) {
    // coupon payment
    double coupon_spot_cash_flow = principal * coupon_rate / coupon_frequency;
    // all coupons discounted to present
    valarray<double> cash_flows = coupon_spot_cash_flow * discount_factors;
    // last cashflow at expiry includes return of principal
    *(end(cash_flows)-1) += principal * *(end(discount_factors)-1);
//    copy(begin(cash_flows), end(cash_flows), ostream_iterator<double>(cout, " ")); // debug
//    cout << '\n';

    // main values
    bond_value = accumulate(begin(cash_flows), end(cash_flows), 0.0);
    valarray<double> cash_flows_t = cash_flows * cash_times;
    duration = accumulate(begin(cash_flows_t), end(cash_flows_t), 0.0) / bond_value;
    valarray<double> cash_flows_tt = cash_flows_t * cash_times;
    convexity = accumulate(begin(cash_flows_tt), end(cash_flows_tt), 0.0) / bond_value;

    // auxiliary variables will be used to compute the yield
    valarray<double> exp_cash_times = exp(- cash_times);
    function<double (double)> bond_implicit = [&](double rate_old)
    {
        valarray<double> exp_pow_rate = pow(exp_cash_times, rate_old);
        // *(end(cash_times)-1) below is time of maturity
        return bond_value - principal * exp(- rate_old * *(end(cash_times)-1)) - coupon_spot_cash_flow *
               accumulate(begin(exp_pow_rate), end(exp_pow_rate), 0.0);
    };
    function<double (double)> deriv_bond = [&](double rate_old)
    {
        valarray<double> exp_pow_rate_times_t = cash_times * pow(exp_cash_times, rate_old);
        return principal * *(end(cash_times)-1) * exp(- rate_old * *(end(cash_times)-1)) +
               coupon_spot_cash_flow * accumulate(begin(exp_pow_rate_times_t), end(exp_pow_rate_times_t), 0.0);
    };

    // compute yield and par-yield coupon
    yield = NewtonMethodForYield(tol, 0.1, bond_implicit, deriv_bond);
    par_yield_coupon = (1 - *(end(discount_factors)-1)) * coupon_frequency;
    par_yield_coupon /= accumulate(begin(discount_factors), end(discount_factors), 0.0);
}

// Newton-Raphson method to extract yield
double Bond::NewtonMethodForYield(const double tol, const double rate_init,
                                  function<double (double)>& num, function<double (double)>& denum)
{
    double rate_old = rate_init;
    double rate_new = rate_old - 1;
    while (abs(rate_new - rate_old) > tol) {
        rate_old = rate_new;
        rate_new = rate_old - num(rate_old) / denum(rate_old);
    }

    return rate_new;
}

// getters
valarray<double> Bond::getCashTimes() const {return cash_times;}
double Bond::getCouponRate() const {return coupon_rate;}
valarray<double> Bond::getIntegratedRates() const {return integrated_rates;}
valarray<double> Bond::getDiscountFactors() const {return discount_factors;}
double Bond::getValue() const {return bond_value;}
double Bond::getDuration() const {return duration;}
double Bond::getConvexity() const {return convexity;}
double Bond::getYield() const {return yield;}
double Bond::getParYieldCoupon() const {return par_yield_coupon;}

// print all bond data, e.g. on a.txt file
void Bond::printData(ostream& os, const InterestRates& choice) {
    os << "Bond current state :" << setprecision(5) << fixed;

    valarray<double> print_times = getCashTimes();
    os << "\nCoupon dates : ";
    copy(begin(print_times), end(print_times), ostream_iterator<double>(os, " "));
    os << "(per year length)\n";

    valarray<double> print_discounts = getDiscountFactors();
    os << "Discount to present value for each date : ";
    copy(begin(print_discounts), end(print_discounts), ostream_iterator<double>(os, " "));

    os << "\nCoupon rate    : " << setprecision(2) << getCouponRate() * 100 << " % \n";
    os << "Bond VALUE     : " << setprecision(5) << getValue() << '\n';
    os << "Bond duration  : " << getDuration() << '\n';
    os << "Bond convexity : " << getConvexity() << "\n\n";

    if (choice == InterestRates::given_yield)
        os << "Yield given : " << getGivenYield() << '\n';
    os << "Bond yield : " << getYield() << '\n';
    os << "Par yield coupon : " << getParYieldCoupon() << "\n\n";
}


// DERIVATE CLASSES
KnownRateBond::KnownRateBond(const valarray<double>& inp_times,
                             const double inp_coupon,
                             const valarray<double>& inp_integrated_rates,
                             const valarray<double>& inp_known_rates,
                             const double inp_principal)
                             : Bond{inp_times, inp_coupon, inp_integrated_rates}, known_rates{inp_known_rates}
{
    double principal{inp_principal}, coupon_frequency;
    size_t index_maturity;
    setConstants(index_maturity, coupon_frequency);
    computeDiscountFactors();
    computeValues(coupon_frequency, principal);
}

template<typename T>
ZeroRateBond<T>::ZeroRateBond(const valarray<double>& inp_times,
                              const double inp_coupon,
                              const valarray<double>& inp_integrated_rates,
                              const function<T (T)>& inp_rate,
                              const double inp_principal)
                              : Bond(inp_times, inp_coupon, inp_integrated_rates), interest_rate(inp_rate)
{
    double principal{inp_principal}, coupon_frequency;
    size_t index_maturity;
    setConstants(index_maturity, coupon_frequency);
    computeDiscountFactors();
    computeValues(coupon_frequency, principal);
}

template<typename T>
InstantRateBond<T>::InstantRateBond(const valarray<double>& inp_times,
                                    const double inp_coupon,
                                    const valarray<double>& inp_integrated_rates,
                                    const function<T (T)>& inp_rate,
                                    const double inp_principal,
                                    const double inp_tol)
                                    : Bond(inp_times, inp_coupon, inp_integrated_rates), interest_rate(inp_rate)
{
    double principal{inp_principal}, coupon_frequency, tol{inp_tol};
    size_t index_maturity;
    setConstants(index_maturity, coupon_frequency);
    computeDiscountFactors();
    computeValues(coupon_frequency, principal, tol);
}

BondGivenYield::BondGivenYield(const valarray<double>& inp_times,
                               const double inp_coupon,
                               const valarray<double>& inp_integrated_rates,
                               const double inp_yield,
                               const double inp_principal)
                               : Bond{inp_times, inp_coupon, inp_integrated_rates}, given_yield{inp_yield}
{
    double principal{inp_principal}, coupon_frequency;
    size_t index_maturity;
    setConstants(index_maturity, coupon_frequency);
    computeDiscountFactors();
    computeValues(coupon_frequency, principal);
}

double BondGivenYield::getGivenYield() const {return given_yield;}

// bond factory function
template<typename T>
unique_ptr<Bond> makeBond(const InterestRates& choice,
                          const valarray<double>& inp_times,
                          const double inp_coupon,
                          const valarray<double>& integrated_rate,
                          const function<T (T)>& inp_rate_law,
                          const variant<double, valarray<double> >& inp_rate_value,
                          const double principal,
                          const double tol)
{
    if (choice == InterestRates::known_rate)
        return make_unique<KnownRateBond>(inp_times, inp_coupon, integrated_rate,
                                          get<valarray<double> >(inp_rate_value), principal);
    else if (choice == InterestRates::zero_rate)
        return make_unique<ZeroRateBond<T> >(inp_times, inp_coupon, integrated_rate, inp_rate_law, principal);
    else if (choice == InterestRates::instantaneous)
        return make_unique<InstantRateBond<T> >(inp_times, inp_coupon, integrated_rate, inp_rate_law, principal, tol);
    else if (choice == InterestRates::given_yield)
        return make_unique<BondGivenYield>(inp_times, inp_coupon, integrated_rate, get<double>(inp_rate_value), principal);
    else
        throw runtime_error("Pick either choice = 1, 2, 3 or 4");

    return 0;  // never gets here
}
