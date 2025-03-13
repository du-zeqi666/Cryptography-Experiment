//求解密钥字
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <cctype>
#include <iomanip>

using namespace std;

// 英文字母出现频率表（A-Z）
const vector<double> ENGLISH_LETTER_FREQUENCIES = {
    0.082, 0.015, 0.028, 0.043, 0.127, 0.022, 0.020, 0.061, 0.070, 0.002,
    0.008, 0.040, 0.024, 0.067, 0.075, 0.019, 0.001, 0.060, 0.063, 0.091,
    0.028, 0.010, 0.023, 0.001, 0.020, 0.001
};

// 计算单个组在特定移位下的 M_g 值
double calculate_mg_for_group(const string& group, int shift) {
    vector<int> freq(26, 0);
    int total = 0;

    for (char c : group) {
        if (isalpha(c)) {
            freq[(toupper(c) - 'A' - shift + 26) % 26]++;
            total++;
        }
    }

    double mg = 0.0;
    for (int i = 0; i < 26; i++) {
        mg += (freq[i] / (double)total) * ENGLISH_LETTER_FREQUENCIES[i];
    }
    
    return mg;
}

// 计算密文的所有分组和所有移位方案的 M_g 值
vector<pair<int, double>> calculate_mg_values(const string& cipher_text, int group_size) {
    vector<string> groups(group_size);
    vector<pair<int, double>> key(group_size);  // 存储 (移位值, M_g 值)

    // 按照分组大小将密文拆分
    int index = 0;
    for (char c : cipher_text) {
        if (isalpha(c)) {
            groups[index % group_size] += toupper(c);
            index++;
        }
    }

    // 计算每组的最佳移位
    for (int i = 0; i < group_size; i++) {
        double max_mg = 0.0;
        int best_shift = 0;

        cout << "分组 " << i + 1 << " 的 M_g 值:" << endl;
        for (int shift = 0; shift < 26; shift++) {
            double mg = calculate_mg_for_group(groups[i], shift);
            cout << "移位 " << setw(2) << shift << " (" << char('A' + shift) << "): " 
                 << fixed << setprecision(6) << mg << endl;
            if (mg > max_mg) {
                max_mg = mg;
                best_shift = shift;
            }
        }

        key[i] = {best_shift, max_mg};
        cout << "最佳移位: " << best_shift << " (" << char('A' + best_shift) << ")，M_g 值: " 
             << fixed << setprecision(6) << max_mg << "\n" << endl;
    }

    return key;
}

// 使用维吉尼亚密码解密密文
string decrypt_vigenere(const string& cipher_text, const vector<int>& key) {
    string decrypted_text;
    int key_len = key.size();
    int index = 0;

    for (char c : cipher_text) {
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            decrypted_text += (c - base - key[index % key_len] + 26) % 26 + base;
            index++;
        } else {
            decrypted_text += c;
        }
    }

    return decrypted_text;
}

int main() {
    string cipher_text;
    int group_size;

    cout << "请输入密文: ";
    getline(cin, cipher_text);
    
    cout << "请输入密钥长度: ";
    cin >> group_size;

    vector<pair<int, double>> key_with_mg = calculate_mg_values(cipher_text, group_size);

    // 输出密钥及其 M_g 值
    cout << "推测出的密钥: ";
    for (auto [shift, mg] : key_with_mg) {
        cout << char('A' + shift);
    }
    cout << endl;

    // 仅获取密钥的移位值
    vector<int> key;
    for (auto [shift, mg] : key_with_mg) {
        key.push_back(shift);
    }

    // 解密密文
    string decrypted_text = decrypt_vigenere(cipher_text, key);
    cout << "解密文本: " << decrypted_text << endl;
    system("pause");
    return 0;
}

