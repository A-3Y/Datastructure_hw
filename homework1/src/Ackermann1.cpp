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