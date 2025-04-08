#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <random>
#include <iomanip>

using namespace std;

// S-box substitution
void LH_s(string b, string& a) {
    a = b;
    for (int i = 0; i < a.length(); i += 4) {
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

// P-box permutation
void LH_p(string c, string& a) {
    if (c.length() != 16) {
        a = string(16, '0');
        return;
    }
    a = c;
    a[1] = c[4];
    a[2] = c[8];
    a[3] = c[12];
    a[4] = c[1];
    a[6] = c[9];
    a[7] = c[13];
    a[8] = c[2];
    a[9] = c[6];
    a[11] = c[14];
    a[12] = c[3];
    a[13] = c[7];
    a[14] = c[11];
}

// XOR operation
void yh(string a, string b, string& c) {
    c.resize(a.length());
    for (int i = 0; i < a.length(); i++) {
        c[i] = (a[i] != b[i]) ? '1' : '0';
    }
}

// Convert integer to 16-bit binary string
string int_to_bin16(int n) {
    string s(16, '0');
    for (int i = 15; i >= 0; i--) {
        s[15 - i] = ((n >> i) & 1) ? '1' : '0';
    }
    return s;
}

// SPN encryption function
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

int main() {
    // Fixed key K (32 bits)
    string K = "00111010100101001101011000111111";
    if (K.length() < 32) {
        cerr << "Error: Key must be at least 32 bits.\n";
        return 1;
    }

    // Generate subkeys using sliding window
    vector<string> k(5);
    for (int i = 1; i <= 5; i++) {
        k[i - 1] = K.substr(4 * i - 4, 16);
    }

    int n;
    cout << "Enter the number of plaintext-ciphertext pairs to generate (n): ";
    cin >> n;

    if (cin.fail() || n < 0 || n > 65536) {
        cout << "Invalid input. n should be an integer between 0 and 65536.\n";
        return 1;
    }

    // Random number generation
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 65535); // 16-bit range: 0 to 2^16 - 1

    set<string> used_plaintexts;
    vector<pair<string, string>> pairs;

    // Generate n unique plaintext-ciphertext pairs
    while (pairs.size() < static_cast<size_t>(n)) {
        int rand_num = dis(gen);
        string plaintext = int_to_bin16(rand_num);
        if (used_plaintexts.insert(plaintext).second) {
            string ciphertext = encrypt_spn(plaintext, k);
            pairs.push_back({plaintext, ciphertext});
        }
    }

    // Output the pairs
    cout << "\nGenerated Plaintext-Ciphertext Pairs:\n";
    for (size_t i = 0; i < pairs.size(); i++) {
        cout << "Pair " << i + 1 << ":\n";
        cout << "  Plaintext:  " << pairs[i].first << "\n";
        cout << "  Ciphertext: " << pairs[i].second << "\n\n";
    }

    return 0;
}