#ifndef NEWTONCOTESFORMULAS_H_INCLUDED
#define NEWTONCOTESFORMULAS_H_INCLUDED

#include <string>

class NewtonCotesFormulas {
public:
    NewtonCotesFormulas(double, double, double, std::string&&);
    virtual ~NewtonCotesFormulas() = default;
    virtual double computeIntegral(unsigned, double (*func)(double)) = 0;
    double convergeToTol(unsigned, double (*func)(double));
    double printConvergenceValues(unsigned, double (*func)(double));

protected:
    double inf, sup;
    double tol;
    std::string method_name;
};

class Midpoint: public NewtonCotesFormulas {
public:
    Midpoint(double, double, double);
    double computeIntegral(unsigned, double (*func)(double)) override;
};

class Trapezoidal: public NewtonCotesFormulas {
public:
    Trapezoidal(double, double, double);
    double computeIntegral(unsigned, double (*func)(double)) override;
};

class Simpsons: public NewtonCotesFormulas {
public:
    Simpsons(double, double, double);
    double computeIntegral(unsigned, double (*func)(double)) override;
};

//#include "NewtonCotesFormulas.cpp"
#endif // NEWTONCOTESFORMULAS_H_INCLUDED
