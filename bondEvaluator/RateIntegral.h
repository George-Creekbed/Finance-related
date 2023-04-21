#ifndef RATEINTEGRAL_H_INCLUDED
#define RATEINTEGRAL_H_INCLUDED

#include <functional>
#include <valarray>
#include <variant>

template<typename T> T KnownRateIntegral(const T&, const T&);

template<typename T> T ZeroRateIntegral(const T&, const std::function<T (T)>&);

template<typename T> T InstantRateIntegral(const T&, const double, const std::function<T (T)>&, const Formula&);

template<typename T, template<typename ...> typename container = std::valarray>
container<T> GivenYieldIntegral(const T, const container<T>&);

template<typename T> T makeRateIntegral(const InterestRates&,
                                        const Formula&,
                                        const T&,
                                        const std::variant<double, T>& = 0,
                                        const double = 0,
                                        const std::function<T (T)>& = 0);

#include "RateIntegral.cpp"
#endif // RATEINTEGRAL_H_INCLUDED
