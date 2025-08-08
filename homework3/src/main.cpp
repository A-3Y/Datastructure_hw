#include <iostream>
#include "Polynomial.h"
using namespace std;

int main() {
    Polynomial p, q, r;
    cout << "Enter polynomial p (n c1 e1 c2 e2 ...): ";
    cin >> p;
    cout << "Enter polynomial q (n c1 e1 c2 e2 ...): ";
    cin >> q;

    cout << "p = " << p << endl;
    cout << "q = " << q << endl;

    r = p + q;
    cout << "p + q = " << r << endl;

    r = p - q;
    cout << "p - q = " << r << endl;

    r = p * q;
    cout << "p * q = " << r << endl;

    float x;
    cout << "Enter value to evaluate p(x): ";
    cin >> x;
    cout << "p(" << x << ") = " << p.Evaluate(x) << endl;

    return 0;
}