#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

// 示例 S 盒（4 位输入 -> 4 位输出），可以根据需求更换
vector<int> SBox = {
    0xE, 0x4, 0xD, 0x1,
    0x2, 0xF, 0xB, 0x8,
    0x3, 0xA, 0x6, 0xC,
    0x5, 0x9, 0x0, 0x7
};

// 打印差分分布表
void printDDT(const vector<vector<int>>& ddt) {
    int size = ddt.size();

    cout << "    ";
    for (int j = 0; j < size; ++j)
        cout << setw(3) << j;
    cout << "\n   " << string(size * 3, '-') << endl;

    for (int i = 0; i < size; ++i) {
        cout << setw(2) << i << " |";
        for (int j = 0; j < size; ++j)
            cout << setw(3) << ddt[i][j];
        cout << endl;
    }
}

// 构造差分分布表
vector<vector<int>> buildDDT(const vector<int>& sbox) {
    int size = sbox.size();
    vector<vector<int>> ddt(size, vector<int>(size, 0));

    for (int dx = 0; dx < size; ++dx) {
        for (int x = 0; x < size; ++x) {
            int x2 = x ^ dx;
            int dy = sbox[x] ^ sbox[x2];
            ddt[dx][dy]++;
        }
    }

    return ddt;
}

int main() {
    auto ddt = buildDDT(SBox);

    cout << "Differential Distribution Table (DDT):\n";
    printDDT(ddt);

    return 0;
}
