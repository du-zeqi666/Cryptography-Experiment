#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

int gcd(int a,int b){
    if (b == 0) return a;  // 终止条件：b 为 0，则 a 就是 GCD
    return gcd(b, a % b);  // 递归调用，交换 a 和 b
}
// 计算 a, b 使得 ax + b ≡ y (mod 26) ，已知两组 (x1, y1) 和 (x2, y2)
void solve_coefficients(int x1, int y1, int x2, int y2, int &count) {
    int a, b;
    int det = (x1 - x2 + 26) % 26;
    if (det == 0) {
        return; // 无唯一解，跳过
    }
    
    // 计算 a = (y1 - y2) * det^(-1) mod 26
    bool found = false;
    for (int i = 1; i < 26; i++) {
        if ((det * i) % 26 == 1) {
            a = ((y1 - y2 + 26) * i) % 26;
            found = true;
            break;
        }
    }
    if (!found) return;
    
    // 确保 a 与 26 互素
    if (gcd(a, 26) != 1) return;
    
    // 计算 b = (y1 - a * x1) mod 26

    b = (y1 - a * x1) % 26;
    if (b < 0) b += 26;
    
    cout << "(" << a << ", " << b << ")" << endl;
    count++;
}

// 将字符转换为 0-25 的整数
vector<int> convert_to_numbers(const string &input) {
    vector<int> numbers;
    for (char c : input) {
        if (c >= 'A' && c <= 'Z') {
            numbers.push_back(c - 'A');
        }
    }
    return numbers;
}

int main() {
    int n;
    cout << "Enter the length of character arrays (n): ";
    cin >> n;
    
    string input1, input2;
    cout << "Enter first sequence of uppercase letters: ";
    cin >> input1;
    cout << "Enter second sequence of uppercase letters: ";
    cin >> input2;
    
    vector<int> arr1 = convert_to_numbers(input1);
    vector<int> arr2 = convert_to_numbers(input2);
    
    if (arr1.size() < n || arr2.size() < n) {
        cout << "Input sequences must each have at least " << n << " characters." << endl;
        return 1;
    }
    
    // 生成所有可能的组合并计算 a, b，避免出现 A-B, A-C 这种配对情况
    int count = 0;
    cout << "All possible (a, b) solutions:" << endl;
    for (size_t i = 0; i < arr1.size(); i++) {
        for (size_t j = 0; j < arr1.size(); j++) {
            for (size_t t = 0; t < arr1.size(); t++) {
                for (size_t s = 0; s < arr1.size(); s++) {
            if (arr1[i] == arr1[j]) continue; // 避免 A-B, A-C 的配对情况
            if (arr2[s] == arr2[t]) continue;
            solve_coefficients(arr1[i], arr2[s], arr1[j], arr2[t], count);
                }
            }
        }
    }
    
    cout << "Total valid (a, b) pairs: " << count << endl;
    system("pause");
    return 0;
}
