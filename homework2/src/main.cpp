#include "Polynomial.h"
#include <iostream>
using namespace std;

int main() {
    Polynomial p1, p2;
    cout << "Input first polynomial:\n";
    cin >> p1;
    cout << "Input second polynomial:\n";
    cin >> p2;

    cout << "\nP1(x) = " << p1 << endl;
    cout << "P2(x) = " << p2 << endl;

    Polynomial sum = p1.Add(p2);
    Polynomial prod = p1.Mult(p2);

    cout << "\nP1(x) + P2(x) = " << sum << endl;
    cout << "P1(x) * P2(x) = " << prod << endl;

    float x;
    cout << "\nEvaluate polynomials at x = ";
    cin >> x;
    cout << "P1(" << x << ") = " << p1.Eval(x) << endl;
    cout << "P2(" << x << ") = " << p2.Eval(x) << endl;

    return 0;
}