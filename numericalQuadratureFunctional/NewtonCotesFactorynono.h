#ifndef NEWTONCOTESFACTORY_H_INCLUDED
#define NEWTONCOTESFACTORY_H_INCLUDED

#include <string>
#include <stdexcept>
//#include <memory>
#include <functional>
#include "NewtonCotesFormulas.h"

enum class Formula {
    Midpoint = 1,
    Trapezoidal,
    Simpsons,
};

//class NewtonCotesFactory {      // Parametrized factory, no children
//public:
//std::unique_ptr<NewtonCotesFormulas>
std::function<double (double, double, int, std::function<double (double) >) >
createQuadrature(Formula choice, double a, double b, double inp_tol, std::function<double (double) >) {
    if (choice == Formula::Midpoint)
        //return std::make_unique<Midpoint>(a, b, inp_tol);
        return Midpoint<std::function<double (double)>(a, b, inp_tol, func, std::function<double (double) >);
    else if (choice == Formula::Trapezoidal)
        //return std::make_unique<Trapezoidal>(a, b, inp_tol);
        return Trapezoidal<std::function<double (double)>(a, b, inp_tol, func, std::function<double (double) >);
    else if (choice == Formula::Simpsons)
        //return std::make_unique<Simpsons>(a, b, inp_tol);
        return Simpsons<std::function<double (double)>(a, b, inp_tol, func, std::function<double (double) >);
    else
        throw std::runtime_error("Pick between choice = 1, 2, or 3");

    return 0;  // never gets here
}
//    virtual ~NewtonCotesFactory() = default;
//};

//class MidpointFactory: public NewtonCotesFactory {
//public:
//    NewtonCotesFormulas* createQuadrature(double a, double b, double inp_tol) const override {
//        return new Midpoint(a, b, inp_tol);
//    }
//};
//
//class TrapezoidalFactory: public NewtonCotesFactory {
//public:
//    NewtonCotesFormulas* createQuadrature(double a, double b, double inp_tol) const override {
//        return new Trapezoidal(a, b, inp_tol);
//    }
//};
//
//class SimpsonsFactory: public NewtonCotesFactory {
//public:
//    NewtonCotesFormulas* createQuadrature(double a, double b, double inp_tol) const override {
//        return new Simpsons(a, b, inp_tol);
//    }
//};

#endif // NEWTONCOTESFACTORY_H_INCLUDED
