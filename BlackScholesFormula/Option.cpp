#include <utility>
#include "Option.h"
#include "BlackScholesFormula.h"

double Call::computeBlackScholesValue() const {
    return BlackScholesFormula(spot, strike, interest_rate, dividend_rate, vol, maturity).first;
}

double Call::computeDelta() const {
    return Delta(spot, strike, interest_rate, dividend_rate, vol, maturity).first;
}

double Call::computeGamma() const {
    return Gamma(spot, strike, interest_rate, dividend_rate, vol, maturity).first;
}

double Call::computeVega() const {
    return Vega(spot, strike, interest_rate, dividend_rate, vol, maturity).first;
}

double Call::deltaNoDividends() const {
    double d1 = d1and2(spot, strike, interest_rate, dividend_rate, vol, maturity).first;

    return getStandardDistr(d1).first;
}

double Call::probabilityInTheMoney() const {
    double d2 = d1and2(spot, strike, interest_rate, dividend_rate, vol, maturity).second;

    return getStandardDistr(d2).first;
}

double Put::computeBlackScholesValue() const {
    return BlackScholesFormula(spot, strike, interest_rate, dividend_rate, vol, maturity).second;
}

double Put::computeDelta() const {
    return Delta(spot, strike, interest_rate, dividend_rate, vol, maturity).second;
}

double Put::computeGamma() const {
    return Gamma(spot, strike, interest_rate, dividend_rate, vol, maturity).second;
}

double Put::computeVega() const {
    return Vega(spot, strike, interest_rate, dividend_rate, vol, maturity).second;
}

double Put::deltaNoDividends() const {
    double d1 = d1and2(spot, strike, interest_rate, dividend_rate, vol, maturity).first;

    return -getStandardDistr(d1).second;
}

double Put::probabilityInTheMoney() const {
    double d2 = d1and2(spot, strike, interest_rate, dividend_rate, vol, maturity).second;

    return getStandardDistr(d2).second;
}
