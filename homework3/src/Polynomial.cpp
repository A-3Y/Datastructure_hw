#include "Polynomial.h"

// Initialize static member
PolyNode* Polynomial::avail = nullptr;

// Get a node from avail or new
PolyNode* Polynomial::getNode(int c, int e, PolyNode* l) {
    PolyNode* node;
    if (avail) {
        node = avail;
        avail = avail->link;
    }
    else {
        node = new PolyNode;
    }
    node->coef = c;
    node->exp = e;
    node->link = l;
    return node;
}

// Return node to available list
void Polynomial::retNode(PolyNode* node) {
    node->link = avail;
    avail = node;
}

// Default constructor
Polynomial::Polynomial() {
    header = getNode();
    header->link = header;
}

// Copy constructor
Polynomial::Polynomial(const Polynomial& a) {
    header = getNode();
    header->link = header;
    copyFrom(a);
}

// Destructor
Polynomial::~Polynomial() {
    clear();
    retNode(header);
    header = nullptr;
}

// Assignment operator
const Polynomial& Polynomial::operator=(const Polynomial& a) {
    if (this != &a) {
        clear();
        copyFrom(a);
    }
    return *this;
}

// Clear all nodes (except header)
void Polynomial::clear() {
    PolyNode* curr = header->link;
    while (curr != header) {
        PolyNode* temp = curr;
        curr = curr->link;
        retNode(temp);
    }
    header->link = header;
}

// Deep copy from another polynomial
void Polynomial::copyFrom(const Polynomial& a) {
    PolyNode* tail = header;
    for (PolyNode* curr = a.header->link; curr != a.header; curr = curr->link) {
        PolyNode* node = getNode(curr->coef, curr->exp, header);
        tail->link = node;
        tail = node;
    }
    tail->link = header;
}

// Input operator
istream& operator>>(istream& is, Polynomial& x) {
    int n, c, e;
    x.clear();
    is >> n;
    PolyNode* tail = x.header;
    for (int i = 0; i < n; ++i) {
        is >> c >> e;
        PolyNode* node = x.getNode(c, e, x.header);
        tail->link = node;
        tail = node;
    }
    tail->link = x.header;
    return is;
}

// Output operator
ostream& operator<<(ostream& os, const Polynomial& x) {
    int count = 0;
    for (PolyNode* curr = x.header->link; curr != x.header; curr = curr->link)
        ++count;
    os << count;
    for (PolyNode* curr = x.header->link; curr != x.header; curr = curr->link)
        os << " " << curr->coef << " " << curr->exp;
    return os;
}

// Addition
Polynomial Polynomial::operator+(const Polynomial& b) const {
    Polynomial result;
    PolyNode* ta = header->link;
    PolyNode* tb = b.header->link;
    PolyNode* tail = result.header;

    while (ta != header && tb != b.header) {
        if (ta->exp > tb->exp) {
            tail->link = result.getNode(ta->coef, ta->exp, result.header);
            ta = ta->link;
        }
        else if (ta->exp < tb->exp) {
            tail->link = result.getNode(tb->coef, tb->exp, result.header);
            tb = tb->link;
        }
        else {
            int sum = ta->coef + tb->coef;
            if (sum != 0)
                tail->link = result.getNode(sum, ta->exp, result.header);
            else {
                ta = ta->link; tb = tb->link;
                continue;
            }
            ta = ta->link;
            tb = tb->link;
        }
        tail = tail->link;
    }
    while (ta != header) {
        tail->link = result.getNode(ta->coef, ta->exp, result.header);
        tail = tail->link;
        ta = ta->link;
    }
    while (tb != b.header) {
        tail->link = result.getNode(tb->coef, tb->exp, result.header);
        tail = tail->link;
        tb = tb->link;
    }
    tail->link = result.header;
    return result;
}

// Subtraction
Polynomial Polynomial::operator-(const Polynomial& b) const {
    Polynomial result;
    PolyNode* ta = header->link;
    PolyNode* tb = b.header->link;
    PolyNode* tail = result.header;

    while (ta != header && tb != b.header) {
        if (ta->exp > tb->exp) {
            tail->link = result.getNode(ta->coef, ta->exp, result.header);
            ta = ta->link;
        }
        else if (ta->exp < tb->exp) {
            tail->link = result.getNode(-tb->coef, tb->exp, result.header);
            tb = tb->link;
        }
        else {
            int diff = ta->coef - tb->coef;
            if (diff != 0)
                tail->link = result.getNode(diff, ta->exp, result.header);
            else {
                ta = ta->link; tb = tb->link;
                continue;
            }
            ta = ta->link;
            tb = tb->link;
        }
        tail = tail->link;
    }
    while (ta != header) {
        tail->link = result.getNode(ta->coef, ta->exp, result.header);
        tail = tail->link;
        ta = ta->link;
    }
    while (tb != b.header) {
        tail->link = result.getNode(-tb->coef, tb->exp, result.header);
        tail = tail->link;
        tb = tb->link;
    }
    tail->link = result.header;
    return result;
}

// Multiplication
Polynomial Polynomial::operator*(const Polynomial& b) const {
    Polynomial result;
    for (PolyNode* ta = header->link; ta != header; ta = ta->link) {
        Polynomial temp;
        PolyNode* tail = temp.header;
        for (PolyNode* tb = b.header->link; tb != b.header; tb = tb->link) {
            int c = ta->coef * tb->coef;
            int e = ta->exp + tb->exp;
            tail->link = temp.getNode(c, e, temp.header);
            tail = tail->link;
        }
        tail->link = temp.header;
        result = result + temp;
    }
    return result;
}

// Evaluate
float Polynomial::Evaluate(float x) const {
    float sum = 0.0f;
    for (PolyNode* curr = header->link; curr != header; curr = curr->link) {
        float term = curr->coef;
        for (int i = 0; i < curr->exp; ++i)
            term *= x;
        sum += term;
    }
    return sum;
}