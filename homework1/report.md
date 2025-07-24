# 41143245

作業一

## 解題說明

本題要求實現計算阿克曼函數(遞迴和非遞迴)

### 解題策略

1. 使用遞迴函式將問題拆解為更小的子問題： 


Ackermann 函數 A(m, n) 根據 m 和 n 的值進行拆解，其遞迴方式如下： 


A(m, n) = ? 


若 m = 0，則返回 n + 1 


若 n = 0，則返回 A(m - 1, 1) 


否則返回 A(m - 1, A(m, n - 1)) 


2. 當 m = 0 或 n = 0 時，作為遞迴的結束條件。 


m = 0：直接返回 n + 1 


n = 0：將 m-1, 1 作為新的子問題，遞迴呼叫 


3. 主程式呼叫遞迴函式，並輸出計算結果。 


主程式接收輸入的 m 和 n，呼叫 Ackermann 遞迴函式，最後輸出結果。

## 程式實作
首先是遞迴的:
```cpp
#include <iostream>
using namespace std;

unsigned int ackermann_recursive(unsigned int m, unsigned int n) {
    if (m == 0) {
        return n + 1;
    }
    else if (n == 0) {
        return ackermann_recursive(m - 1, 1);
    }
    else {
        return ackermann_recursive(m - 1, ackermann_recursive(m, n - 1));
    }
}

int main() {
    unsigned int m, n;
    cout << "請輸入 m: ";
    cin >> m;
    cout << "請輸入 n: ";
    cin >> n;
    cout << "Ackermann(" << m << ", " << n << ") = " << ackermann_recursive(m, n) << endl;
    return 0;
}
```
再來是非遞迴的:

```cpp
#include <iostream>
#include <cstdlib>
using namespace std;

struct Stack {
    unsigned int m[1024] = { 0 };
    unsigned int n[1024] = { 0 };
    int top = -1;
    Stack() {} 
    void push(unsigned int x, unsigned int y) {
        top++;
        m[top] = x;
        n[top] = y;
    }
    void pop() {
        top--;
    }
    bool empty() {
        return top < 0;
    }
    unsigned int get_m() { return m[top]; }
    unsigned int get_n() { return n[top]; }
    void set_n(unsigned int val) { n[top] = val; }
};

unsigned int ackermann_iterative(unsigned int m, unsigned int n) {
    Stack stk;
    stk.push(m, n);
    unsigned int result = 0;
    while (!stk.empty()) {
        unsigned int cur_m = stk.get_m();
        unsigned int cur_n = stk.get_n();
        stk.pop();
        if (cur_m == 0) {
            result = cur_n + 1;
            if (!stk.empty()) {
                stk.set_n(result);
            }
        }
        else if (cur_n == 0) {
            stk.push(cur_m - 1, 1);
        }
        else {
            stk.push(cur_m - 1, 0); 
            stk.push(cur_m, cur_n - 1);
        }
    }
    return result;
}

int main() {
    unsigned int m, n;
    cout << "請輸入 m: ";
    cin >> m;
    cout << "請輸入 n: ";
    cin >> n;
    cout << "Ackermann(" << m << ", " << n << ") = " << ackermann_iterative(m, n) << endl;
    return 0;
}
```
## 效能分析

Ackermann 函數是一個成長極快、遞迴深度極高的數學函數，其時間與空間複雜度都非常大。
因此，程式的時間複雜度僅用 Ackermann 本身表示：

時間複雜度 = O(A(m, n))
 
空間複雜度 = O(A(m, n))
## 測試與驗證
### 測試案例
| 測試案例 | 輸入參數 $n$ | 預期輸出 | 實際輸出 |
|----------|--------------|----------|----------|
| 測試一   | $m = 0$, $n = 1$      | 2        | 2        |
| 測試二   | $m = 3$, $n = 1$     | 13        | 13        |
| 測試三   | $m = 2$, $n = 5$      | 13        | 13        |
| 測試四   | $m = 0$, $n = 99$      | 100       | 100       |
| 測試五   | $m = 5$, $n = 0$     | 異常拋出 | 異常拋出 |



### 編譯與執行指令
```shell
$ g++ -std=c++17 -o Ackman.cpp
$ ./Ackman
0
1
```

```shell
$ g++ -std=c++17 -o Ackman.cpp
$ ./Ackman
3
15
```

```shell
$ g++ -std=c++17 -o Ackman.cpp
$ ./Ackman
2
5
```

```shell
$ g++ -std=c++17 -o Ackman.cpp
$ ./Ackman
0
99
```

```shell
$ g++ -std=c++17 -o Ackman.cpp
$ ./Ackman
5
0
```

### 結論

阿克曼函數作為一個經典遞迴題目，展現了數學遞迴在計算機科學中的極端增長。透過本次 C++ 實作，不僅鞏固了遞迴函式的撰寫技巧，也對遞迴終止條件的重要性有更深刻的體會。經過測試，遞迴版雖然直觀，但受限於系統堆疊深度；非遞迴（以陣列模擬堆疊）則能處理較大參數，但仍須留意記憶體消耗。整體而言，阿克曼函數程式驗證了遞迴演算法的高效及其潛在限制。

## 申論及開發報告

遞迴的整體語意較為直觀且不複雜，非遞迴的方式相對而言較為複雜。雖然是同一程式，但遞迴相對初學者而言淺顯的展示出這個函式的作用(先判斷m、n的是否為0)再進行下去。
