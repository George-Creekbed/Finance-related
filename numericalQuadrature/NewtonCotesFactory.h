#ifndef NEWTONCOTESFACTORY_H_INCLUDED
#define NEWTONCOTESFACTORY_H_INCLUDED

#include <string>
#include <stdexcept>
#include "NewtonCotesFormulas.h"

class NewtonCotesFactory {      // Parametrized factory, no children
public:
    virtual inline NewtonCotesFormulas* createQuadrature(int choice, double a, double b, double inp_tol) const {
        if (choice == 1)
            return new Midpoint(a, b, inp_tol);
        else if (choice == 2)
            return new Trapezoidal(a, b, inp_tol);
        else if (choice == 3)
            return new Simpsons(a, b, inp_tol);
        else
            throw std::runtime_error("Pick either choice = 1, 2, or 3");

        return 0;  // never gets here
    }
    virtual ~NewtonCotesFactory() = default;
};

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
