#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <iostream>
using namespace std;

struct PolyNode {
    int coef;
    int exp;
    PolyNode* link;
};

class Polynomial {
private:
    PolyNode* header; // header node for circular linked list

    // Available space list for memory management
    static PolyNode* avail;

    // Utility functions
    PolyNode* getNode(int c = 0, int e = 0, PolyNode* l = nullptr);
    void retNode(PolyNode* node);
    void clear();
    void copyFrom(const Polynomial& a);

public:
    Polynomial();                                   // Default constructor
    Polynomial(const Polynomial& a);                // Copy constructor
    ~Polynomial();                                  // Destructor
    const Polynomial& operator=(const Polynomial& a); // Assignment operator

    friend istream& operator>>(istream& is, Polynomial& x);
    friend ostream& operator<<(ostream& os, const Polynomial& x);

    Polynomial operator+(const Polynomial& b) const;
    Polynomial operator-(const Polynomial& b) const;
    Polynomial operator*(const Polynomial& b) const;

    float Evaluate(float x) const;
};

#endif
