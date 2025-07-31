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