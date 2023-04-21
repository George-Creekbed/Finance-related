#ifndef OPTION_H_INCLUDED
#define OPTION_H_INCLUDED

class Option {
public:
    Option(const double s, const double k, const double r, const double d, const double v, const double T)
        : spot{s}, strike{k}, interest_rate{r}, dividend_rate{d}, vol{v}, maturity{T} {}
    virtual ~Option() = default;

    virtual double computeBlackScholesValue() const = 0;
    virtual double computeDelta() const = 0;
    virtual double computeGamma() const = 0;
    virtual double computeVega() const = 0;
protected:
    double spot;
    double strike;
    double interest_rate;
    double dividend_rate;
    double vol;
    double maturity;
};

class Call: public Option {
public:
    Call(const double s, const double k, const double r, const double d, const double v, const double T)
        : Option(s, k, r, d, v, T) {}
    double computeBlackScholesValue() const override;
    double computeDelta() const override;
    double computeGamma() const override;
    double computeVega() const override;

    double deltaNoDividends() const;
    double probabilityInTheMoney() const;
};

class Put: public Option {
public:
    Put(const double s, const double k, const double r, const double d, const double v, const double T)
        : Option(s, k, r, d, v, T) {}
    double computeBlackScholesValue() const override;
    double computeDelta() const override;
    double computeGamma() const override;
    double computeVega() const override;

    double deltaNoDividends() const;
    double probabilityInTheMoney() const;
};

#endif // OPTION_H_INCLUDED
