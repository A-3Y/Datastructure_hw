#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <iostream>
using namespace std;

class Polynomial; // forward declaration

class Term {
    friend class Polynomial;
private:
    float coef;
    int exp;
public:
    Term(float c = 0.0, int e = 0) : coef(c), exp(e) {}
    float getCoef() const { return coef; }
    int getExp() const { return exp; }
};

class Polynomial {
private:
    Term* termArray; // array of nonzero terms
    int capacity;    // size of termArray
    int terms;       // number of nonzero terms

    void grow();
    void pushTerm(float coef, int exp);

public:
    Polynomial();
    Polynomial(const Polynomial& other);
    ~Polynomial();
    Polynomial& operator=(const Polynomial& other);

    Polynomial Add(const Polynomial& poly);
    Polynomial Mult(const Polynomial& poly);

    float Eval(float f);

    friend istream& operator>>(istream& is, Polynomial& poly);
    friend ostream& operator<<(ostream& os, const Polynomial& poly);
};

#endif