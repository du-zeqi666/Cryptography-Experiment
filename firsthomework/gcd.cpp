#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
using namespace std;
int gcd(int a,int b){
    if (b == 0) return a;  // 终止条件：b 为 0，则 a 就是 GCD
    return gcd(b, a % b);  // 递归调用，交换 a 和 b
}

int main() {
    int n;
    cout << "Enter the size of the array (n): ";
    cin >> n;

    vector<int> arr(n);
    cout << "Enter " << n << " integers: ";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    unordered_map<int, int> gcd_count; // 统计每个 GCD 出现的次数
    int max_freq = 0;
    int most_frequent_gcd = 0;

    // 计算所有两两组合 (组合数 C(n,2))
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) { // j 从 i+1 开始，确保每对组合只计算一次
            int gcd_value =gcd(arr[i], arr[j]); // 使用 std::gcd
            gcd_count[gcd_value]++;

            // 记录出现频率最高的 GCD
            if (gcd_count[gcd_value] > max_freq) {
                max_freq = gcd_count[gcd_value];
                most_frequent_gcd = gcd_value;
            }
        }
    }

    // 输出所有 GCD 及其出现次数
    cout << "GCD occurrences:\n";
    for (const auto& entry : gcd_count) {
        cout << "GCD " << entry.first << ": " << entry.second << " times\n";
    }

    cout << "Most frequent GCD: " << most_frequent_gcd << " (occurred " << max_freq << " times)\n";
    system("pause");
    return 0;
}
