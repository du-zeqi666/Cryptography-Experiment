#include <iostream>
#include <string>
using namespace std;

void yh(const string &a, const string &b, string &c) {
    c.resize(a.length());  // ✅ 确保 c 有足够空间
    for (size_t i = 0; i < a.length(); i++) {
        c[i] = a[i] ^ b[i];  // ✅ 按位异或
    }
}

int main() {
    const int l = 4, m = 4, Nr = 4;
    string k[5], u[5];
    string x, K, y;
    cin >> x >> K;

    for (int i = 1; i <= 5; i++) {
        k[i - 1] = K.substr(4 * (i - 1), 4);  
    }

    yh(x, k[0], u[0]);  // 

    cout << "u[0]: " << u[0] << endl;  

    return 0;
}
