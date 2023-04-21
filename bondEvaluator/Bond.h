#ifndef BOND_H_INCLUDED
#define BOND_H_INCLUDED

#include <iostream>
#include <memory>
#include <functional>
#include <valarray>
#include <variant>
#include "NewtonCotesFormulas.h"

// bond types
enum class InterestRates {
    known_rate = 1,
    zero_rate,
    instantaneous,
    given_yield
};

// BASE CLASS
class Bond {
public:
    Bond(const std::valarray<double>& inp_times, const double inp_coupon,
         const std::valarray<double> inp_integrated_rates)
         : cash_times{inp_times}, coupon_rate{inp_coupon},
           integrated_rates{inp_integrated_rates}, discount_factors( cash_times.size() )
         {}
    virtual ~Bond() = default;

    void setConstants(size_t&, double&);

    //virtual
    void computeDiscountFactors();
    void computeValues(const double, const double = 100, const double = 1E-6);

    std::valarray<double> getCashTimes() const;
    double getCouponRate() const;
    std::valarray<double> getIntegratedRates() const;
    std::valarray<double> getDiscountFactors() const;
    double getValue() const;
    double getDuration() const;
    double getConvexity() const;

    virtual double getGivenYield() const {return 0;}
    double getYield() const;
    double getParYieldCoupon() const;
    double NewtonMethodForYield(const double, const double, std::function<double (double)>&,
                                std::function<double (double)>&);

    void printData(std::ostream&, const InterestRates&);

protected:
    // acquired on input
    std::valarray<double> cash_times;
    double coupon_rate;
    // computed after initialization
    std::valarray<double> integrated_rates;
    std::valarray<double> discount_factors;
    double bond_value{0};
    double duration{0};
    double convexity{0};
    double yield{0};
    double par_yield_coupon{0};
};

// DERIVED CLASSES
class KnownRateBond : public Bond {
public:
    KnownRateBond(const std::valarray<double>&, const double, const std::valarray<double>&,
                  const std::valarray<double>&, const double = 100);
private:
    std::valarray<double> known_rates;
};

template<typename T>
class ZeroRateBond : public Bond {
public:
    ZeroRateBond(const std::valarray<double>&, const double, const std::valarray<double>&,
                 const std::function<T (T)>&, const double = 100);
private:
    std::function<T (T)> interest_rate;
};

template<typename T>
class InstantRateBond : public Bond {
public:
    InstantRateBond(const std::valarray<double>&, const double, const std::valarray<double>&,
                    const std::function<T (T)>&, const double, const double = 1E-6);
private:
    std::function<T (T)> interest_rate;
};

class BondGivenYield : public Bond {
public:
    BondGivenYield(const std::valarray<double>&, const double, const std::valarray<double>&,
                   const double, const double);
    double getGivenYield() const override;
private:
    double given_yield;
};

// BOND FACTORY FUNCTION
template<typename T>
std::unique_ptr<Bond> makeBond(const InterestRates&,
                               const std::valarray<double>&,
                               const double,
                               const std::valarray<double>&,
                               const std::function<T (T)>&,
                               const std::variant<double, std::valarray<double> >&,
                               const double = 100,
                               const double = 1E-6);

#include "Bond.cpp"
#endif // BOND_H_INCLUDED
