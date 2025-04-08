#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

// S-box
int SBox[16] = {0xE, 0x4, 0xD, 0x1, 0x2, 0xF, 0xB, 0x8,
                0x3, 0xA, 0x6, 0xC, 0x5, 0x9, 0x0, 0x7};

// P-box inverse permutation
int PBox_inv[16] = {0, 4, 8, 12, 1, 5, 9, 13,
                    2, 6, 10, 14, 3, 7, 11, 15};

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

// Apply P-box permutation to a 16-bit differential
string apply_P(const string& diff) {
    string new_diff(16, '0');
    for (int j = 0; j < 16; j++) {
        new_diff[j] = diff[PBox_inv[j]];
    }
    return new_diff;
}

// Get the most probable output dy and its probability
pair<int, double> get_most_probable_dy(int dx, const vector<vector<int>>& DDT) {
    if (dx == 0) return {0, 1.0};
    int max_count = 0, best_dy = 0;
    for (int dy = 0; dy < 16; dy++) {
        if (DDT[dx][dy] > max_count) {
            max_count = DDT[dx][dy];
            best_dy = dy;
        }
    }
    return {best_dy, static_cast<double>(max_count) / 16.0};
}

// Compute differential chain for a given Δx
pair<string, double> compute_chain(const string& delta_x, const vector<vector<int>>& DDT) {
    vector<int> delta_u = bin_to_ints(delta_x);
    double prob = 1.0;

    for (int r = 0; r < 3; r++) {
        vector<int> delta_v(4);
        for (int i = 0; i < 4; i++) {
            auto [dy, p] = get_most_probable_dy(delta_u[i], DDT);
            delta_v[i] = dy;
            prob *= p;
        }
        string delta_v_str;
        for (int dy : delta_v) delta_v_str += int_to_bin(dy);
        delta_u = bin_to_ints(apply_P(delta_v_str));
    }

    string delta_u4_str;
    for (int d : delta_u) delta_u4_str += int_to_bin(d);
    return {delta_u4_str, prob};
}

// Check if Δu⁴ has at most 2 groups not all zero
bool is_valid_u4(const string& delta_u4) {
    int non_zero_groups = 0;
    for (int i = 0; i < 16; i += 4) {
        bool all_zero = true;
        for (int j = 0; j < 4; j++) {
            if (delta_u4[i + j] != '0') {
                all_zero = false;
                break;
            }
        }
        if (!all_zero) non_zero_groups++;
    }
    return non_zero_groups <= 2;
}

// Entry point
int main() {
    vector<vector<int>> DDT = build_DDT();
    vector<tuple<string, string, double>> valid_chains;

    for (int group = 0; group < 4; group++) {
        for (int dx = 1; dx < 16; dx++) {
            string delta_x(16, '0');
            string dx_bin = int_to_bin(dx);
            for (int i = 0; i < 4; i++)
                delta_x[group * 4 + i] = dx_bin[i];

            auto [delta_u4, prob] = compute_chain(delta_x, DDT);
            if (is_valid_u4(delta_u4)) {
                valid_chains.emplace_back(delta_x, delta_u4, prob);
            }
        }
    }

    // Sort by probability descending
    sort(valid_chains.begin(), valid_chains.end(), [](auto& a, auto& b) {
        return get<2>(a) > get<2>(b);
    });

    // Output
    cout << fixed << setprecision(10);
    int index = 1;
    for (auto& [dx, u4, prob] : valid_chains) {
        cout << "Rank " << index++ << ":\n";
        cout << "  Δx       : " << dx << "\n";
        cout << "  Prob     : " << prob << "\n";
        cout << "  Δu⁴      : " << u4 << "\n\n";
    }

    cout << "Total valid chains: " << valid_chains.size() << "\n";
    return 0;
}
