# 41143245

作業二

## 解題說明

**實作流程**

1. 先設計 Term 和 Polynomial 的資料成員。
2. 完成建構式、解構式、grow、pushTerm 等基本功能。
3. 實作 operator>>、operator<<。
4. 實作 Add、Mult（可用 pushTerm 保證安全）。
5. 完成 Eval。
6. 測試各功能。

實作一個多項式（Polynomial）類別，具備以下功能：

建構多項式（Polynomial）： 預設建構式，建立一個零多項式。

多項式加法（Add）： 計算兩個多項式的和。

多項式乘法（Mult）： 計算兩個多項式的積。

多項式加總（Eval）： 代入某個變數值計算多項式數值。

多項式的輸入/輸出： 透過運算子多載（<<, >>）完成。

並限定多項式以**動態陣列（Term termArray）儲存所有非零項*，每項使用 Term 類別（包含係數 coef 與指數 exp）。

### 解題策略
Term類別：存放單一多項式項（係數+指數），可設為 Polynomial 的 friend。

Polynomial類別：動態陣列 termArray 存放所有非零項，記錄容量 (capacity) 與實際項數 (terms)。

**多項式加法:**

指數相同則係數相加，否則指數較大的先加入結果。

每次新增前要檢查是否需要擴充 termArray。

**多項式乘法**

兩層迴圈將每一項分別相乘，指數相加，係數相乘。

結果合併同指數項（可以在結果陣列查找/合併）。

最後移除係數為0的項，並排序。

**多項式加總**

用迴圈將每項係數×(f的指數次方)加總。

**輸入輸出運算子多載**

operator>>：先輸入項數，再依序輸入每項係數與指數，存入 termArray。

operator<<：依序輸出各項。

## 程式實作

**Polynomial.h**
```Polynomial.h
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
```
**poly.cpp**

```poly.cpp
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
    int idx = 0;
    for (int i = 0; i < result.terms; ++i) {
        if (fabs(result.termArray[i].getCoef()) > 1e-6)
            result.termArray[idx++] = result.termArray[i];
    }
    result.terms = idx;
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
```

**主程式(main)  main.cpp**

```main.cpp
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
```
## 效能分析

| 操作 | 時間複雜度 | 空間複雜度 
|----------|--------------|----------|
| 輸入/排序   | O(nlogn)     | O(n)         |
| 加法 Add   | O(n+m)      | O(n+m)         |
| 乘法 Mult  | O(n*m)     | O(n*m)         |
| 加總 Eval   | O(n)     | O(1)        |
| 輸出   | O(n)      | O(1)  |

## 測試與驗證
### 測試案例

|  測試案例  | 輸入參數  |    預期輸出      |      實際輸出      |
|----------|--------------|--------------------|-------------------|
| 測試一   | $數量(terms)皆=0、 P1(x) = 0、 P2(x) = 0、  代入x=3$    | $P1(x) + P2(x) = 0、 P1(x) * P2(x) = 0、 P1(3) = 0、P2(3) = 0$      | $P1(x) + P2(x) = 0、P1(x) * P2(x) = 0、P1(3) = 0、P2(3) = 0$      |
| 測試二   | $數量(terms)皆=1、 P1(x) = 2x^3、 P2(x) = 12x^3、  代入x=3$      | $P1(x) + P2(x) = 14x^3、 P1(x) * P2(x) = 24x^6、 P1(3) = 54、P2(3) = 324$        | $P1(x) + P2(x) = 14x^3、P1(x) * P2(x) = 24x^6、P1(3) = 54、P2(3) = 324$        |
| 測試三   | $數量(terms)皆=2、 P1(x) = 2x^3 + 3x^2、 P2(x) = 12x^3 + 5x^2、  代入x=5$      | $P1(x) + P2(x) = 14x^3 + 8x^2、 P1(x) * P2(x) = 24x^6 + 46x^5 + 15x^4、 P1(5) = 325、P2(5) = 1625$        | $P1(x) + P2(x) = 14x^3 + 8x^2、 P1(x) * P2(x) = 24x^6 + 46x^5 + 15x^4、 P1(5) = 325、P2(5) = 1625$        |
| 測試四   | $數量(terms)皆=5、 P1(x) = 12x^4 + 10x^3 + 5x^2 + 1x + 9、 P2(x) = 1x^4 + 23x^3 + 12x^2 + 25x + 35、  代入x=5$      | $P1(x) + P2(x) = 13x^4 + 33x^3 + 17x^2 + 26x + 44、 P1(x) * P2(x) = 12x^8 + 286x^7 + 379x^6 + 536x^5 + 762x^4 + 694x^3 + 308x^2 + 260x + 315、 P1(5) = 8889、P2(5) = 3960$       | $P1(x) + P2(x) = 13x^4 + 33x^3 + 17x^2 + 26x + 44、 P1(x) * P2(x) = 12x^8 + 286x^7 + 379x^6 + 536x^5 + 762x^4 + 694x^3 + 308x^2 + 260x + 315、 P1(5) = 8889、P2(5) = 3960$       |
### 編譯與執行指令

以測試一為例:

```shell
g++ -std=c++17 -o poly main.cpp Polynomial.cpp
./poly
0
0
3

```

### 結論

程式能正確進行多項式的加法、乘法與加總等運算，計算結果正確。

1. 多項式的每一項均以結構化物件（Term）儲存，並動態管理記憶體空間，確保不會緩衝區溢位。
2. 針對多項式項數、輸入格式、零多項式等多種情境進行測試，程式在各種情況下均能正確運作並輸出預期的結果。
3. 經過效能分析，主要加法和乘法操作在一般情形下效能合理，能滿足大多數時候的應用需求。

## 申論及開發報告

### 題目分析與設計理念

題目要求設計一個能夠進行多項式運算（加法、乘法、加總等）的 C++ 類別，並以動態陣列方式儲存多項式的每一個非零項。此設計主要考慮到多項式項數不定、運算過程中項數會動態變化（例如加法、乘法後可能新增或合併項），因此必須採用能隨需擴充的資料結構。
最核心的挑戰在於**正確管理動態記憶體**與**設計高彈性、可擴充的物件導向架構**。透過實作本多項式類別，不僅加深了對 C++ 物件導向、記憶體配置、運算子多載等觀念的理解，也體會到資料結構選擇對效能與安全性的影響。  
此外，經由單元測試與效能分析，驗證此程式在正確性、效率與可維護性上都達到預期目標。未來若需支援更大規模的多項式運算，亦可考慮引入tree等進階資料結構進行優化。
