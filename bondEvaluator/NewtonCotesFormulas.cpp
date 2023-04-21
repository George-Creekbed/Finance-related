#include <iostream>
#include <iomanip>
#include <string>
#include "NewtonCotesFormulas.h"

using std::string;          using std::cout;
using std::setprecision;    using std::fixed;
using std::scientific;      using std::ostream;
using std::to_string;

// this just prints the formula name on cout
ostream& operator<< (ostream& os, const Formula& method) {
    if (method == Formula::Midpoint)
        os << "Midpoint";
    else if (method == Formula::Trapezoidal)
        os << "Trapezoidal";
    else if (method == Formula::Simpsons)
        os << "Simpsons";

    return os;
}

void printConvergenceValues(ostream& os, const int flag, const Formula& choice, const int intervals,
                            const double tol, const double integral, double difference)
{
    string pad_to_size = string(9 - to_string(intervals).size(), ' ');
    switch(flag) {
        case 0:
            os << "Numerical integration, convergence table\n";
            os << choice << " method, with tolerance " << setprecision(1) << tol << "\n";
            os << "intervals" << string(5,' ') << "integral " << string(9,' ') << "tol\n";
            os << setprecision(8) << fixed << pad_to_size << intervals << string(3,' ')
                 << integral << string(3,' ') << scientific << difference << '\n';
            break;
        case 1:
            //cout.setf(ios_base::scientific);
            //cout.precision(8);
            os << pad_to_size << intervals << string(3,' ') << fixed << integral
                 << string(3,' ') << scientific << difference << '\n';
            break;
        case 2:
            os << fixed << pad_to_size << intervals << string(3,' ') << integral
                 << string(3,' ') << scientific << difference << "\n\n";
            break;
    }
}
