
// #include <iostream>
// #include <string>
// #include <vector>
// #include <iomanip>

// using namespace std;

// // S-box (πₛ)
// int SBox[16] = {0xE, 0x4, 0xD, 0x1, 0x2, 0xF, 0xB, 0x8, 0x3, 0xA, 0x6, 0xC, 0x5, 0x9, 0x0, 0x7};

// // P-box inverse permutation (πₚ⁻¹, 0-based indices)
// int PBox_inv[16] = {0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15};

// // Build Differential Distribution Table (DDT)
// vector<vector<int>> build_DDT() {
//     vector<vector<int>> DDT(16, vector<int>(16, 0));
//     for (int dx = 0; dx < 16; dx++) {
//         for (int x = 0; x < 16; x++) {
//             int x2 = x ^ dx;
//             int dy = SBox[x] ^ SBox[x2];
//             DDT[dx][dy]++;
//         }
//     }
//     return DDT;
// }

// // Apply P-box permutation to a 16-bit differential
// string apply_P(const string& diff) {
//     string new_diff(16, '0');
//     for (int j = 0; j < 16; j++) {
//         new_diff[j] = diff[PBox_inv[j]];
//     }
//     return new_diff;
// }

// // Get the most probable output differential (dy) and its probability for a given dx
// pair<int, double> get_most_probable_dy(int dx, const vector<vector<int>>& DDT) {
//     if (dx == 0) return {0, 1.0};
//     int max_count = 0;
//     int best_dy = 0;
//     for (int dy = 0; dy < 16; dy++) {
//         if (DDT[dx][dy] > max_count) {
//             max_count = DDT[dx][dy];
//             best_dy = dy;
//         }
//     }
//     return {best_dy, static_cast<double>(max_count) / 16.0};
// }

// // Convert 4-bit integer to binary string
// string int_to_bin(int n) {
//     string s;
//     for (int i = 3; i >= 0; i--) {
//         s += ((n >> i) & 1) ? '1' : '0';
//     }
//     return s;
// }

// // Convert 16-bit binary string to vector of 4-bit integers
// vector<int> bin_to_ints(const string& s) {
//     vector<int> ints(4);
//     for (int i = 0; i < 16; i += 4) {
//         string group = s.substr(i, 4);
//         ints[i / 4] = stoi(group, nullptr, 2);
//     }
//     return ints;
// }

// // Compute differential chain for a given input differential
// pair<string, double> compute_differential_chain(const string& delta_x, const vector<vector<int>>& DDT) {
//     vector<int> delta_u = bin_to_ints(delta_x);
//     double prob = 1.0;

//     // Rounds 1 to 3
//     for (int r = 1; r <= 3; r++) {
//         vector<int> delta_v(4);
//         for (int i = 0; i < 4; i++) {
//             auto [dy, p] = get_most_probable_dy(delta_u[i], DDT);
//             delta_v[i] = dy;
//             prob *= p;
//         }
//         string delta_v_str;
//         for (int dy : delta_v) {
//             delta_v_str += int_to_bin(dy);
//         }
//         string delta_w_str = apply_P(delta_v_str);
//         delta_u = bin_to_ints(delta_w_str);
//     }

//     // Δu⁴ (last round S-box input differential)
//     string delta_u4_str;
//     for (int d : delta_u) {
//         delta_u4_str += int_to_bin(d);
//     }
//     return {delta_u4_str, prob};
// }

// int main() {
//     vector<vector<int>> DDT = build_DDT();
//     int chain_count = 0;

//     cout << fixed << setprecision(10);
//     for (int group = 0; group < 4; group++) {
//         for (int dx = 1; dx < 16; dx++) {
//             string delta_x(16, '0');
//             string dx_bin = int_to_bin(dx);
//             for (int i = 0; i < 4; i++) {
//                 delta_x[group * 4 + i] = dx_bin[i];
//             }

//             auto [delta_u4_str, prob] = compute_differential_chain(delta_x, DDT);
//             cout << "Chain " << ++chain_count << ":\n";
//             cout << "  Input differential (Δx): " << delta_x << "\n";
//             cout << "  Diffusion rate (Prob): " << prob << "\n";
//             cout << "  Last round S-box input differential (Δu⁴): " << delta_u4_str << "\n\n";
//         }
//     }

//     // Check total chains
//     cout << "Total differential chains computed: " << chain_count << "\n";
//     return 0;
// }
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

// S-box (πₛ)
int SBox[16] = {0xE, 0x4, 0xD, 0x1, 0x2, 0xF, 0xB, 0x8, 0x3, 0xA, 0x6, 0xC, 0x5, 0x9, 0x0, 0x7};

// P-box inverse permutation (πₚ⁻¹, 0-based indices)
int PBox_inv[16] = {0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15};

// Build Differential Distribution Table (DDT)
vector<vector<int>> build_DDT() {
    vector<vector<int>> DDT(16, vector<int>(16, 0));
    for (int dx = 0; dx < 16; dx++) {
        for (int x = 0; x < 16; x++) {
            int x2 = x ^ dx;
            int dy = SBox[x] ^ SBox[x2];
            DDT[dx][dy]++;
        }
    }
    return DDT;
}

// Apply P-box permutation to a 16-bit differential
string apply_P(const string& diff) {
    string new_diff(16, '0');
    for (int j = 0; j < 16; j++) {
        new_diff[j] = diff[PBox_inv[j]];
    }
    return new_diff;
}

// Get the most probable output differential (dy) and its probability for a given dx
pair<int, double> get_most_probable_dy(int dx, const vector<vector<int>>& DDT) {
    if (dx == 0) return make_pair(0, 1.0);
    int max_count = 0;
    int best_dy = 0;
    for (int dy = 0; dy < 16; dy++) {
        if (DDT[dx][dy] > max_count) {
            max_count = DDT[dx][dy];
            best_dy = dy;
        }
    }
    return make_pair(best_dy, static_cast<double>(max_count) / 16.0);
}

// Convert 4-bit integer to binary string
string int_to_bin(int n) {
    string s;
    for (int i = 3; i >= 0; i--) {
        s += ((n >> i) & 1) ? '1' : '0';
    }
    return s;
}

// Convert 16-bit binary string to vector of 4-bit integers
vector<int> bin_to_ints(const string& s) {
    vector<int> ints(4);
    for (int i = 0; i < 16; i += 4) {
        string group = s.substr(i, 4);
        ints[i / 4] = stoi(group, nullptr, 2);
    }
    return ints;
}

// Compute differential chain for a given input differential
pair<string, double> compute_differential_chain(const string& delta_x, const vector<vector<int>>& DDT) {
    vector<int> delta_u = bin_to_ints(delta_x);
    double prob = 1.0;

    // Rounds 1 to 3
    for (int r = 1; r <= 3; r++) {
        vector<int> delta_v(4);
        for (int i = 0; i < 4; i++) {
            auto result = get_most_probable_dy(delta_u[i], DDT);
            int dy = result.first;
            double p = result.second;
            delta_v[i] = dy;
            prob *= p;
        }
        string delta_v_str;
        for (int dy : delta_v) {
            delta_v_str += int_to_bin(dy);
        }
        string delta_w_str = apply_P(delta_v_str);
        delta_u = bin_to_ints(delta_w_str);
    }

    // Δu⁴ (last round S-box input differential)
    string delta_u4_str;
    for (int d : delta_u) {
        delta_u4_str += int_to_bin(d);
    }
    return make_pair(delta_u4_str, prob);
}

int main() {
    vector<vector<int>> DDT = build_DDT();
    int chain_count = 0;

    cout << fixed << setprecision(10);
    for (int group = 0; group < 4; group++) {
        for (int dx = 1; dx < 16; dx++) {
            string delta_x(16, '0');
            string dx_bin = int_to_bin(dx);
            for (int i = 0; i < 4; i++) {
                delta_x[group * 4 + i] = dx_bin[i];
            }

            auto result = compute_differential_chain(delta_x, DDT);
            string delta_u4_str = result.first;
            double prob = result.second;

            cout << "Chain " << ++chain_count << ":\n";
            cout << "  Input differential (Δx): " << delta_x << "\n";
            cout << "  Diffusion rate (Prob): " << prob << "\n";
            cout << "  Last round S-box input differential (Δu⁴): " << delta_u4_str << "\n\n";
        }
    }

    // Check total chains
    cout << "Total differential chains computed: " << chain_count << "\n";
    return 0;
}