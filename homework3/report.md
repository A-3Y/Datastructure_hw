# 41143245

作業三

## 解題說明

**實作流程**

1. 先設計 PolyNode 結構與 Polynomial 的資料成員。
2. 完成建構式、解構式、節點管理（getNode, retNode）、清空等基本功能。
3. 實作 operator>>、operator<<。
4. 實作 operator+、operator-、operator*（可用 getNode 保證安全）。
5. 完成 Evaluate。
6. 測試各功能。

本作業實作一個多項式（Polynomial）類別，具備以下功能：

- 建構多項式（Polynomial）：預設建構式，建立一個零多項式（僅有 header 節點）。
- 多項式加法（operator+）：計算兩個多項式的和。
- 多項式減法（operator-）：計算兩個多項式的差。
- 多項式乘法（operator*）：計算兩個多項式的積。
- 多項式函數值（Evaluate）：代入變數值計算多項式數值。
- 多項式的輸入/輸出：透過運算子多載（<<, >>）完成。

多項式以**循環鏈結串列（含 header node）**儲存，每項使用 PolyNode 結構（包含 coef, exp, link）。

### 解題策略

**PolyNode 結構**：存放單一多項式項（係數 coef、指數 exp、連結 link）。

**Polynomial 類別**：  
- 以 circular linked list 儲存所有項（含 header node）。
- 靜態 avail 節點池管理回收節點記憶體。
- 支援copy建構、賦值、加減乘、清空、函數值等操作。

**多項式加法:**
- 兩串列皆降冪排序，逐項比對指數，相同則係數相加，不同則指數較大者先插入結果。
- 新增節點時用 getNode，節點管理安全。

**多項式減法:**
- 與加法類似，係數相減。

**多項式乘法:**
- 兩層迴圈，每項分別相乘，產生新項插入暫存多項式（再進行項合併）。

**多項式函數值:**
- 走訪串列，每項計算 coef × (x^exp) 累加。

**輸入輸出運算子多載:**
- operator>>：先輸入項數，再依序輸入每項係數與指數，插入串列。
- operator<<：依序走訪串列，輸出所有項。

---

## 程式實作

**Polynomial.h**
```cpp
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
    PolyNode* header;
    static PolyNode* avail;

    PolyNode* getNode(int c = 0, int e = 0, PolyNode* l = nullptr);
    void retNode(PolyNode* node);
    void clear();
    void copyFrom(const Polynomial& a);

public:
    Polynomial();
    Polynomial(const Polynomial& a);
    ~Polynomial();
    const Polynomial& operator=(const Polynomial& a);

    friend istream& operator>>(istream& is, Polynomial& x);
    friend ostream& operator<<(ostream& os, const Polynomial& x);

    Polynomial operator+(const Polynomial& b) const;
    Polynomial operator-(const Polynomial& b) const;
    Polynomial operator*(const Polynomial& b) const;

    float Evaluate(float x) const;
};

#endif
```

**Polynomial.cpp**
```cpp
#include "Polynomial.h"

PolyNode* Polynomial::avail = nullptr;

PolyNode* Polynomial::getNode(int c, int e, PolyNode* l) {
    PolyNode* node;
    if (avail) {
        node = avail;
        avail = avail->link;
    } else {
        node = new PolyNode;
    }
    node->coef = c;
    node->exp = e;
    node->link = l;
    return node;
}

void Polynomial::retNode(PolyNode* node) {
    node->link = avail;
    avail = node;
}

Polynomial::Polynomial() {
    header = getNode();
    header->link = header;
}

Polynomial::Polynomial(const Polynomial& a) {
    header = getNode();
    header->link = header;
    copyFrom(a);
}

Polynomial::~Polynomial() {
    clear();
    retNode(header);
    header = nullptr;
}

const Polynomial& Polynomial::operator=(const Polynomial& a) {
    if (this != &a) {
        clear();
        copyFrom(a);
    }
    return *this;
}

void Polynomial::clear() {
    PolyNode* curr = header->link;
    while (curr != header) {
        PolyNode* temp = curr;
        curr = curr->link;
        retNode(temp);
    }
    header->link = header;
}

void Polynomial::copyFrom(const Polynomial& a) {
    PolyNode* tail = header;
    for (PolyNode* curr = a.header->link; curr != a.header; curr = curr->link) {
        PolyNode* node = getNode(curr->coef, curr->exp, header);
        tail->link = node;
        tail = node;
    }
    tail->link = header;
}

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

ostream& operator<<(ostream& os, const Polynomial& x) {
    int count = 0;
    for (PolyNode* curr = x.header->link; curr != x.header; curr = curr->link)
        ++count;
    os << count;
    for (PolyNode* curr = x.header->link; curr != x.header; curr = curr->link)
        os << " " << curr->coef << " " << curr->exp;
    return os;
}

Polynomial Polynomial::operator+(const Polynomial& b) const {
    Polynomial result;
    PolyNode* ta = header->link;
    PolyNode* tb = b.header->link;
    PolyNode* tail = result.header;

    while (ta != header && tb != b.header) {
        if (ta->exp > tb->exp) {
            tail->link = result.getNode(ta->coef, ta->exp, result.header);
            ta = ta->link;
        } else if (ta->exp < tb->exp) {
            tail->link = result.getNode(tb->coef, tb->exp, result.header);
            tb = tb->link;
        } else {
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

Polynomial Polynomial::operator-(const Polynomial& b) const {
    Polynomial result;
    PolyNode* ta = header->link;
    PolyNode* tb = b.header->link;
    PolyNode* tail = result.header;

    while (ta != header && tb != b.header) {
        if (ta->exp > tb->exp) {
            tail->link = result.getNode(ta->coef, ta->exp, result.header);
            ta = ta->link;
        } else if (ta->exp < tb->exp) {
            tail->link = result.getNode(-tb->coef, tb->exp, result.header);
            tb = tb->link;
        } else {
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
```

**main.cpp**
```cpp
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
    cout << "Enter value to evaluate p(x), q(x): ";
    cin >> x;
    cout << "p(" << x << ") = " << p.Evaluate(x) << endl;
    cout << "q(" << x << ") = " << q.Evaluate(x) << endl;

    return 0;
}
```

---

## 效能分析

| 操作         | 時間複雜度 | 空間複雜度 |
|--------------|------------|------------|
| 輸入/輸出    | O(n)       | O(n)       |
| 加法         | O(n+m)     | O(n+m)     |
| 減法         | O(n+m)     | O(n+m)     |
| 乘法         | O(n*m)     | O(n+m)     |
| 函數值         | O(n)       | O(1)       |

---

## 測試與驗證

### 測試案例

|  測試案例  | 輸入參數  |    預期輸出      |      實際輸出      |
|----------|--------------|--------------------|-------------------|
| 測試一   | p: 0<br>q: 0<br>x=3 | p+q=0，p-q=0，p*q=0，p(3)=0，q(3)=0 | p+q=0，p-q=0，p*q=0，p(3)=0，q(3)=0 |
| 測試二   | p: 1 2 3<br>q: 1 12 3<br>x=3 | p+q=1 14 3<br>p-q=1 -10 3<br>p*q=1 24 6<br>p(3)=54<br>q(3)=324 | p+q=1 14 3<br>p-q=1 -10 3<br>p*q=1 24 6<br>p(3)=54<br>q(3)=324 |
| 測試三   | p: 2 2 3 3 2<br>q: 2 12 3 5 2<br>x=5 | p+q=2 14 3 8 2<br>p-q=2 -10 3 -2 2<br>p*q=3 24 6 46 5 15 4<br>p(5)=325<br>q(5)=1625 | p+q=2 14 3 8 2<br>p-q=2 -10 3 -2 2<br>p*q=3 24 6 46 5 15 4<br>p(5)=325<br>q(5)=1625 |
| 測試四   | p: 5 12 4 10 3 5 2 1 1 9 0<br>q: 5 1 4 23 3 12 2 25 1 35 0<br>x=5 | p+q=5 13 4 33 3 17 2 26 1 44 0<br>p-q=5 11 4 -13 3 -7 2 -24 1 -26 0<br>p*q=9 12 8 286 7 379 6 536 5 762 4 694 3 308 2 260 1 315 0<br>p(5)=8889<br>q(5)=3960 | p+q=5 13 4 33 3 17 2 26 1 44 0<br>p-q=5 11 4 -13 3 -7 2 -24 1 -26 0<br>p*q=9 12 8 286 7 379 6 536 5 762 4 694 3 308 2 260 1 315 0<br>p(5)=8889<br>q(5)=3960 |
| 測試五   | p: 3 2 2 3 1 4 0<br>q: 2 4 2 5 0<br>x=2 | p+q=3 6 2 3 1 9 0<br>p-q=3 -2 2 3 1 -1 0<br>p*q=5 8 4 12 3 26 2 15 1 20 0<br>p(2)=18 | p+q=3 6 2 3 1 9 0<br>p-q=3 -2 2 3 1 -1 0<br>p*q=5 8 4 12 3 26 2 15 1 20 0<br>p(2)=18 |

### 編譯與執行指令

以測試一為例:

```shell
g++ -std=c++17 -o polynomial main.cpp Polynomial.cpp
./polynomial
0
0
3
```

### 結論

程式能正確進行多項式的加法、乘法與加總函數值等運算，計算結果正確。
本次作業透過循環鏈結串列及節點池管理，成功實現了一元多項式的高效運算與記憶體管理。所有運算皆能正確處理零多項式、重複次方及邊界情形，且釋放節點後能重複利用，有效提升資源利用率。加減乘法各自以串列操作設計，函數值則直接對每一項計算，均達到預期效能。經多組測試案例驗證，結果皆與手算一致，證明本設計正確且穩定。未來若需支援浮點係數、多變數或進階功能，亦可在本架構下擴充。

## 申論及開發報告

### 題目分析與設計理念
題目要求設計一個能夠進行多項式運算（加法、減法、乘法、函數值等）的 C++ 類別，並以**循環鏈結串列（circular linked list）**搭配**header node**來儲存多項式的每一個項。題目特別強調每個多項式節點需記錄係數、指數並以 link 連結下一節點，同時要求實作**可用節點池（available-space list）**來管理記憶體，提升刪除與重複利用節點的效率。這使得設計重點不僅在於運算正確性，更在於如何有效率且安全地管理記憶體與指標，並維持正確的多項式運算結果。


程式的設計運用部分物件導向方法，將多項式表示與運算封裝於 Polynomial 類別內，並以 PolyNode 結構表示每一項。利用 header node 方便串列操作及判斷終點，並設計靜態 avail 節點池管理所有釋放節點，以減少 new/delete 次數、提升效能。所有運算（加、減、乘）皆以串列走訪、比較指數方式合併新項，保證項次降冪、合併同次方且節點管理安全。以運算子多載實現直觀的輸入/輸出與函數賦值操作，並嚴格釋放資源防止記憶體洩漏。經過多組測試案例驗證，無論在正確性、效率上都有達到原本預期。
