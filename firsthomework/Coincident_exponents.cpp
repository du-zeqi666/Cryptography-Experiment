/*重合指数法
写一个维吉尼亚密码中的重合指数法，c++程序，
输入的为英文文本，以及n，密钥字长度从1到n,
输出每个值对应的重合指数值与平均重合指数值*/
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

// 计算某个分组的重合指数（IC）
double calculateIC(const string& text) {
    vector<int> freq(26, 0);
    int length = text.size();

    if (length < 2) return 0.0;

    // 统计字母出现频率
    for (char c : text) {
        if (isalpha(c)) {
            freq[toupper(c) - 'A']++;
        }
    }

    // 计算 IC = (Σ f_i * (f_i - 1)) / (N * (N - 1))
    double ic = 0.0;
    for (int f : freq) {
        ic += f * (f - 1);
    }
    
    return ic / (length * (length - 1));
}

// 计算密钥长度从 1 到 n 时的 IC 值
void analyzeIC(const string& text, int maxKeyLength) {
    cout << fixed << setprecision(6);  // 设置小数精度
    cout << "Key Length\tIC Values\t\tAverage IC\n";

    for (int keyLen = 1; keyLen <= maxKeyLength; keyLen++) {
        vector<string> groups(keyLen);
        int count = 0;

        // 按密钥长度分组
        for (size_t i = 0; i < text.size(); i++) {
            if (isalpha(text[i])) {
                groups[i % keyLen] += toupper(text[i]); // 只保留字母
                count++;
            }
        }

        // 计算所有分组的 IC 详细值
        vector<double> icValues;
        double sumIC = 0.0;
        for (const string& group : groups) {
            double ic = calculateIC(group);
            icValues.push_back(ic);
            sumIC += ic;
        }
        double avgIC = sumIC / keyLen;

        // 输出详细 IC 值
        cout << keyLen << "\t\t";
        for (double ic : icValues) {
            cout << ic << " ";
        }
        cout << "\t" << avgIC << endl;
    }
}

int main() {
    string inputText;
    int maxKeyLen;

    cout << "Enter the English text: ";
    getline(cin, inputText);
    
    cout << "Enter the maximum key length (n): ";
    cin >> maxKeyLen;

    analyzeIC(inputText, maxKeyLen);
    system("pause");
    return 0;
}


