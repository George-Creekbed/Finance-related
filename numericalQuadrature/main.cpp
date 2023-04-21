#include <iostream>
#include <cmath>
#include <chrono>
#include "NewtonCotesFormulas.h"
#include "NewtonCotesFactory.h"
//#include "func.cpp"

using namespace std;

// timer
unsigned int stopwatch()
{
    static auto start_time = chrono::steady_clock::now();

    auto end_time = chrono::steady_clock::now();
    auto delta    = chrono::duration_cast<chrono::microseconds>(end_time - start_time);

    start_time = end_time;

    return delta.count();
}

//struct funcToIntegrate;
//struct funcToIntegrate {
//    double operator()(double x) {
//        return sqrt(x) + exp(-x);
//    }
//};

double func(double x) {
   return sqrt(x) * exp(-x);
   //return pow(x, 2.5) / (1 + x * x);
}

int main()
{
    cout << "Choose method:\n"
         << "Midpoint numeric quadrature -----> (1)\n"
         << "Trapezoidal numeric quadrature --> (2)\n"
         << "Simpson's numeric quadrature ----> (3)\n" << endl;
//    int choice;
//    cin >> choice;
//    while (choice < 1 || choice > 3) {
//        cout << "Pick again a number from 1 to 3\n";
//        cin >> choice;
//    }
    stopwatch();
    NewtonCotesFactory creator;
    for (int choice : {1,2,3}) {
        NewtonCotesFormulas* rule = creator.createQuadrature(choice, 1, 3, 0.000001);
        //NewtonCotesFormulas* rule = creator.createQuadrature(choice, 0, 1, 0.00000000001);
        double result = rule->printConvergenceValues(4, &func);
        cout << "Final result within tolerance: " << result << endl << endl;
        delete rule;
    }

    cout << "Performance time: " << stopwatch() << " microseconds.\n\n";

    return 0;
}
