#ifndef NEWTONCOTESFORMULAS_H_INCLUDED
#define NEWTONCOTESFORMULAS_H_INCLUDED

#include <iostream>
#include <functional>

enum class Formula {
    Midpoint = 1,
    Trapezoidal,
    Simpsons
};

std::ostream& operator<< (std::ostream&, const Formula&);

template<typename Quadrature, typename T>
T convergeToTol(const Formula&, const T&, const T&, int, const T&, const std::function<T (T)>&, const Quadrature&);

void printConvergenceValues(std::ostream&, const int, const Formula&, const int, const double, const double, double);

template<typename T> T Midpoint(const T&, const T&, const int, const std::function<T (T)>&);

template<typename T> T Trapezoidal(const T&, const T&, const int, const std::function<T (T)>&);

template<typename T> T Simpsons(const T&, const T&, const int, const std::function<T (T)>&);

template<typename T>
std::function<T (const T&, const T&, const int, const std::function<T (T)>&) > createQuadrature(const Formula&);

#include "NewtonCotesFormulasTemplateImplementation.cpp"
#endif // NEWTONCOTESFORMULAS_H_INCLUDED
