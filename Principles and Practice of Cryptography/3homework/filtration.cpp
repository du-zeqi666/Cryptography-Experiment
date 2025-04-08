#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

// S盒替换
void LH_s(string b, string& a) {
    a = b;
    for (int i = 0; i < 16; i += 4) {
        string str = a.substr(i, 4);
        int t = (str[0] - '0') * 8 + (str[1] - '0') * 4 + (str[2] - '0') * 2 + (str[3] - '0');
        switch (t) {
            case 0: a.replace(i, 4, "1110"); break;
            case 1: a.replace(i, 4, "0100"); break;
            case 2: a.replace(i, 4, "1101"); break;
            case 3: a.replace(i, 4, "0001"); break;
            case 4: a.replace(i, 4, "0010"); break;
            case 5: a.replace(i, 4, "1111"); break;
            case 6: a.replace(i, 4, "1011"); break;
            case 7: a.replace(i, 4, "1000"); break;
            case 8: a.replace(i, 4, "0011"); break;
            case 9: a.replace(i, 4, "1010"); break;
            case 10: a.replace(i, 4, "0110"); break;
            case 11: a.replace(i, 4, "1100"); break;
            case 12: a.replace(i, 4, "0101"); break;
            case 13: a.replace(i, 4, "1001"); break;
            case 14: a.replace(i, 4, "0000"); break;
            case 15: a.replace(i, 4, "0111"); break;
        }
    }
}

// P盒置换
void LH_p(string c, string& a) {
    if (c.length() != 16) {
        a = string(16, '0');
        return;
    }
    a = c;
    a[1] = c[4]; a[2] = c[8]; a[3] = c[12]; a[4] = c[1];
    a[6] = c[9]; a[7] = c[13]; a[8] = c[2]; a[9] = c[6];
    a[11] = c[14]; a[12] = c[3]; a[13] = c[7]; a[14] = c[11];
}

// 异或操作
void yh(string a, string b, string& c) {
    c.resize(a.length());
    for (int i = 0; i < a.length(); i++) {
        c[i] = (a[i] != b[i]) ? '1' : '0';
    }
}

// 将整数转换为 16 位二进制字符串
string int_to_bin16(int n) {
    string s(16, '0');
    for (int i = 15; i >= 0; i--) {
        s[15 - i] = ((n >> i) & 1) ? '1' : '0';
    }
    return s;
}

// SPN 加密函数
string encrypt_spn(const string& x, const vector<string>& k) {
    if (x.length() != 16) {
        cerr << "Error: Plaintext must be 16 bits.\n";
        return string(16, '0');
    }
    const int Nr = 4;
    string u[4], w[4], v[4];
    string y;

    w[0] = x;
    for (int i = 0; i < Nr; i++) {
        yh(w[i], k[i], u[i]);
        LH_s(u[i], v[i]);
        if (i != Nr - 1) LH_p(v[i], w[i + 1]);
    }
    yh(v[3], k[4], y);
    return y;
}

// 检查 y 和 y* 在指定组上是否满足条件
bool check_condition(const string& y, const string& y_star, const vector<int>& groups) {
    for (int group : groups) {
        int start = (group - 1) * 4; // 组号从 1 开始，转换为 0-based 索引
        string y_group = y.substr(start, 4);
        string y_star_group = y_star.substr(start, 4);
        if (y_group != y_star_group) {
            return false;
        }
    }
    return true;
}

int main() {
    // 固定密钥 K (32 位)
    string K = "00111010100101001101011000111111";
    vector<string> k(5);
    for (int i = 0; i < 5; i++) {
        k[i] = K.substr(i * 4, 16); // 提取 5 个 16 位子密钥
    }

    string x_prime;
    int n;
    vector<int> groups;

    // 获取用户输入
    cout << "Enter input differential x' (16 bits): ";
    cin >> x_prime;
    cout << "Enter the number of groups to check (n, 1-4): ";
    cin >> n;

    // 验证 n 的范围
    if (n < 1 || n > 4) {
        cout << "Error: n must be between 1 and 4.\n";
        return 1;
    }

    cout << "Enter " << n << " distinct group numbers (1-4): ";
    for (int i = 0; i < n; i++) {
        int group;
        cin >> group;
        if (group < 1 || group > 4) {
            cout << "Error: Group numbers must be between 1 and 4.\n";
            return 1;
        }
        groups.push_back(group);
    }

    // 验证 x' 的合法性
    if (x_prime.length() != 16) {
        cout << "Error: x' must be a 16-bit binary string.\n";
        return 1;
    }
    for (char c : x_prime) {
        if (c != '0' && c != '1') {
            cout << "Error: x' must contain only 0s and 1s.\n";
            return 1;
        }
    }

    // 输出筛选条件
    cout << "\nScreening quadruples (x, x*, y, y*) satisfying y_<i> = y*_<i> for i in {";
    for (size_t i = 0; i < groups.size(); i++) {
        cout << groups[i];
        if (i < groups.size() - 1) cout << ", ";
    }
    cout << "}:\n";

    int count = 0;

    // 遍历所有可能的 x 值
    for (int i = 0; i < 65536; i++) {
        string x = int_to_bin16(i);
        string x_star;
        yh(x, x_prime, x_star); // x* = x XOR x'

        string y = encrypt_spn(x, k);
        string y_star = encrypt_spn(x_star, k);

        if (check_condition(y, y_star, groups)) {
            count++;
            cout << "Quadruple " << count << ":\n";
            cout << "  x:     " << x << "\n";
            cout << "  x*:    " << x_star << "\n";
            cout << "  y:     " << y << "\n";
            cout << "  y*:    " << y_star << "\n\n";
        }
    }

    cout << "Total quadruples found: " << count << "\n";
    return 0;
}