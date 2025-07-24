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

### 編譯與執行指令

### 結論

## 申論及開發報告
