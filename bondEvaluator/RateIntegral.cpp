#include <functional>
#include <valarray>
#include <variant>
#include "RateIntegral.h"

using std::function;    using std::valarray;
using std::variant;     using std::get;

template<typename T>
T KnownRateIntegral(const T& times, const T& rates) {
    return times * rates;
}

template<typename T>
T ZeroRateIntegral(const T& times, const function<T (T)>& rate) {
    return times * rate(times);
}

template<typename T>
T InstantRateIntegral(const T& times, const double tol, const function<T (T)>& rate, const Formula& quadrature) {
    function<T (const T&, const T&, int, function<T (T)>)>
        my_method = createQuadrature<T>(quadrature);
    const size_t array_size = times.size();
    T tol_array(tol, array_size);
    tol_array[array_size-1] *= 1E-2;
    T inf_lim(0.0, array_size);
    int intervals{4};

    return convergeToTol(quadrature, inf_lim, times, intervals, tol_array, rate, my_method);
}

template<typename T, template<typename ...> typename container>
container<T> GivenYieldIntegral(const T yield, const container<T>& times) {
    return yield * times;
}

template<typename T>
T makeRateIntegral(const InterestRates& choice,
                   const Formula& quadrature,
                   const T& times,
                   const variant<double, T>& int_rate,
                   const double tol,
                   const function<T (T)>& rate_law)
{
    T integrated_rate(times.size());
    switch(choice) {
        case InterestRates::known_rate:
            integrated_rate = KnownRateIntegral(times, get<T>(int_rate) );
            break;
        case InterestRates::zero_rate:
            integrated_rate = ZeroRateIntegral(times, rate_law);
            break;
        case InterestRates::instantaneous:
            integrated_rate = InstantRateIntegral(times, tol, rate_law, quadrature);
            break;
        case InterestRates::given_yield:
            integrated_rate = GivenYieldIntegral(get<double>(int_rate), times);
            break;
        default:
            throw("Error in the integrated rate selection");
    }

    return integrated_rate;
}
