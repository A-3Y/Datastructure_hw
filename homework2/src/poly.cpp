#include "Polynomial.h"
#include <cmath>
#include <algorithm>

Polynomial::Polynomial() : termArray(nullptr), capacity(4), terms(0) {
    termArray = new Term[capacity];
}

Polynomial::Polynomial(const Polynomial& other)
    : capacity(other.capacity), terms(other.terms)
{
    termArray = new Term[capacity];
    for (int i = 0; i < terms; ++i)
        termArray[i] = other.termArray[i];
}

Polynomial::~Polynomial() {
    delete[] termArray;
}

Polynomial& Polynomial::operator=(const Polynomial& other) {
    if (this == &other) return *this;
    delete[] termArray;
    capacity = other.capacity;
    terms = other.terms;
    termArray = new Term[capacity];
    for (int i = 0; i < terms; ++i)
        termArray[i] = other.termArray[i];
    return *this;
}

void Polynomial::grow() {
    int newCap = (capacity == 0) ? 4 : capacity * 2;
    Term* newArr = new Term[newCap];
    for (int i = 0; i < terms; ++i)
        newArr[i] = termArray[i];
    delete[] termArray;
    termArray = newArr;
    capacity = newCap;
}

void Polynomial::pushTerm(float coef, int exp) {
    if (terms == capacity) grow();
    termArray[terms++] = Term(coef, exp);
}

Polynomial Polynomial::Add(const Polynomial& poly) {
    Polynomial result;
    int i = 0, j = 0;
    while (i < this->terms && j < poly.terms) {
        if (this->termArray[i].getExp() > poly.termArray[j].getExp()) {
            result.pushTerm(this->termArray[i].getCoef(), this->termArray[i].getExp());
            ++i;
        }
        else if (this->termArray[i].getExp() < poly.termArray[j].getExp()) {
            result.pushTerm(poly.termArray[j].getCoef(), poly.termArray[j].getExp());
            ++j;
        }
        else {
            float sumCoef = this->termArray[i].getCoef() + poly.termArray[j].getCoef();
            if (fabs(sumCoef) > 1e-6) {
                result.pushTerm(sumCoef, this->termArray[i].getExp());
            }
            ++i; ++j;
        }
    }
    while (i < this->terms) {
        result.pushTerm(this->termArray[i].getCoef(), this->termArray[i].getExp());
        ++i;
    }
    while (j < poly.terms) {
        result.pushTerm(poly.termArray[j].getCoef(), poly.termArray[j].getExp());
        ++j;
    }
    return result;
}

Polynomial Polynomial::Mult(const Polynomial& poly) {
    Polynomial result;
    for (int i = 0; i < this->terms; ++i) {
        for (int j = 0; j < poly.terms; ++j) {
            float c = this->termArray[i].getCoef() * poly.termArray[j].getCoef();
            int e = this->termArray[i].getExp() + poly.termArray[j].getExp();
            bool found = false;
            for (int k = 0; k < result.terms; ++k) {
                if (result.termArray[k].getExp() == e) {
                    result.termArray[k].coef += c;
                    found = true;
                    break;
                }
            }
            if (!found && fabs(c) > 1e-6) {
                result.pushTerm(c, e);
            }
        }
    }
    // 移除係數為0的項
    int idx = 0;
    for (int i = 0; i < result.terms; ++i) {
        if (fabs(result.termArray[i].getCoef()) > 1e-6)
            result.termArray[idx++] = result.termArray[i];
    }
    result.terms = idx;
    // 依指數由大到小排序
    std::sort(result.termArray, result.termArray + result.terms, [](const Term& a, const Term& b) {
        return a.getExp() > b.getExp();
        });
    return result;
}

float Polynomial::Eval(float f) {
    float sum = 0;
    for (int i = 0; i < terms; ++i)
        sum += termArray[i].getCoef() * pow(f, termArray[i].getExp());
    return sum;
}

istream& operator>>(istream& is, Polynomial& poly) {
    cout << "Enter number of terms: ";
    int n;
    is >> n;
    if (n <= 0) {
        poly.terms = 0;
        return is;
    }
    if (poly.termArray) delete[] poly.termArray;
    poly.capacity = std::max(4, n);
    poly.termArray = new Term[poly.capacity];
    poly.terms = 0;
    for (int i = 0; i < n; ++i) {
        float c; int e;
        cout << "Enter coefficient and exponent for term #" << (i + 1) << ": ";
        is >> c >> e;
        if (fabs(c) > 1e-6) poly.pushTerm(c, e);
    }
    // 依指數由大到小排序
    std::sort(poly.termArray, poly.termArray + poly.terms, [](const Term& a, const Term& b) {
        return a.getExp() > b.getExp();
        });
    return is;
}

ostream& operator<<(ostream& os, const Polynomial& poly) {
    if (poly.terms == 0) {
        os << "0";
        return os;
    }
    for (int i = 0; i < poly.terms; ++i) {
        float coef = poly.termArray[i].getCoef();
        int exp = poly.termArray[i].getExp();
        if (i > 0 && coef > 0) os << " + ";
        if (exp == 0)
            os << coef;
        else if (exp == 1)
            os << coef << "x";
        else
            os << coef << "x^" << exp;
    }
    return os;
}