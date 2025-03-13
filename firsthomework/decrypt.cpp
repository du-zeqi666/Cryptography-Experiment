//已知密钥字与密文转换
#include <iostream>
#include <string>
#include <vector>

// 函数：生成密钥
std::string generateKey(const std::string& str, const std::string& key) {
    std::string newKey = key;
    while (newKey.size() < str.size()) {
        newKey += key;
    }
    return newKey.substr(0, str.size());
}

// 函数：解密
std::string decrypt(const std::string& cipher_text, const std::string& key) {
    std::string orig_text;
    for (size_t i = 0; i < cipher_text.size(); ++i) {
        char x = (cipher_text[i] - key[i] + 26) % 26 + 'A';
        orig_text += x;
    }
    return orig_text;
}

int main() {
    std::string cipher_text;
    std::string key;

    std::cout << "请输入密文（大写字母）：";
    std::cin >> cipher_text;
    std::cout << "请输入密钥（大写字母）：";
    std::cin >> key;

    // 生成与密文等长的密钥
    std::string newKey = generateKey(cipher_text, key);

    // 解密
    std::string decrypted_text = decrypt(cipher_text, newKey);

    std::cout << "解密后的文本是：" << decrypted_text << std::endl;
    system("pause");
    return 0;
}