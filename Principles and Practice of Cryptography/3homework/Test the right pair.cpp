/*x′ 控制明文对的差分；

过滤组 用于保证筛选出的明密文对满足某些条件（例如，密文的某几组部分保持一致）；

攻击的密钥部分编号 用于指定针对哪两个部分进行差分统计；

预期差分值 则是实际验证差分链中特定部分应出现的预期输出差分。
*/

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstdlib>
using namespace std;

// ===================== 基本函数（来自 filtration.cpp） =========================

// S盒替换（4-bit S盒），将 16 位二进制字符串按 4 位一组替换
void LH_s(string b, string &a) {
    a = b;
    for (int i = 0; i < 16; i += 4) {
        string str = a.substr(i, 4);
        int t = (str[0]-'0')*8 + (str[1]-'0')*4 + (str[2]-'0')*2 + (str[3]-'0');
        switch(t){
            case 0:  a.replace(i,4,"1110"); break;
            case 1:  a.replace(i,4,"0100"); break;
            case 2:  a.replace(i,4,"1101"); break;
            case 3:  a.replace(i,4,"0001"); break;
            case 4:  a.replace(i,4,"0010"); break;
            case 5:  a.replace(i,4,"1111"); break;
            case 6:  a.replace(i,4,"1011"); break;
            case 7:  a.replace(i,4,"1000"); break;
            case 8:  a.replace(i,4,"0011"); break;
            case 9:  a.replace(i,4,"1010"); break;
            case 10: a.replace(i,4,"0110"); break;
            case 11: a.replace(i,4,"1100"); break;
            case 12: a.replace(i,4,"0101"); break;
            case 13: a.replace(i,4,"1001"); break;
            case 14: a.replace(i,4,"0000"); break;
            case 15: a.replace(i,4,"0111"); break;
        }
    }
}

// P盒置换：对16位二进制字符串进行置换，结果保存在a中
void LH_p(string c, string &a) {
    if(c.length() != 16) { a = string(16, '0'); return; }
    a = c;
    a[1] = c[4]; a[2] = c[8]; a[3] = c[12]; a[4] = c[1];
    a[6] = c[9]; a[7] = c[13]; a[8] = c[2]; a[9] = c[6];
    a[11] = c[14]; a[12] = c[3]; a[13] = c[7]; a[14] = c[11];
}

// 异或操作：对两个二进制字符串a和b进行异或，结果存入c
void yh(string a, string b, string &c) {
    c.resize(a.length());
    for (int i = 0; i < a.length(); i++) {
        c[i] = (a[i] != b[i]) ? '1' : '0';
    }
}

// 将整数转换为16位二进制字符串
string int_to_bin16(int n) {
    string s(16, '0');
    for (int i = 15; i >= 0; i--) {
        s[15-i] = ((n >> i) & 1) ? '1' : '0';
    }
    return s;
}

// SPN加密函数：输入16位明文x和子密钥向量k（5个16位字符串），返回最后一轮部分异或结果y
string encrypt_spn(const string &x, const vector<string> &k) {
    const int Nr = 4;
    string u[4], w[4], v[4];
    string y;
    w[0] = x;
    for (int i = 0; i < Nr; i++) {
        yh(w[i], k[i], u[i]);
        LH_s(u[i], v[i]);
        if (i != Nr-1) LH_p(v[i], w[i+1]);
    }
    yh(v[3], k[4], y);
    return y;
}

// 过滤函数：检查给定密文y与y*在指定的组（1~4，对应子字符串位置）是否相同
bool check_condition(const string &y, const string &y_star, const vector<int> &groups) {
    for (int group : groups) {
        int start = (group - 1) * 4; // 用户输入的组号假设从1开始
        if (y.substr(start, 4) != y_star.substr(start, 4))
            return false;
    }
    return true;
}

// ===================== 部分解密工具 =====================

// 逆S盒：4位S盒的逆运算（已知S盒映射） 
// S盒映射（题中定义）：
// 0->E, 1->4, 2->D, 3->1, 4->2, 5->F, 6->B, 7->8, 8->3, 9->A, 10->6, 11->C, 12->5, 13->9, 14->0, 15->7
int invSBox(int nibble) {
    // 根据上面的映射计算逆S盒
    int invSBoxArr[16] = {14, 3, 4, 8, 1, 12, 10, 15, 7, 13, 9, 6, 11, 2, 0, 5};
    // 注意：invSBoxArr[SBox[x]] = x, 这里 nibble 为 S盒输出，返回对应输入
    return invSBoxArr[nibble];
}

// 将4位二进制字符串转换为整数（0~15）
int bin4_to_int(const string &s) {
    int val = 0;
    for (char c : s) {
        val = (val << 1) | (c - '0');
    }
    return val;
}

// ===================== 主程序：统计256对候选密钥出现频率 =====================
int main(){
    // 固定密钥 K (32位二进制字符串)，与filtration.cpp一致
    string K = "00111010100101001101011000111111";
    vector<string> k(5);
    for (int i = 0; i < 5; i++) {
        k[i] = K.substr(i * 4, 16);  // 分离出5个16位子密钥
    }
    
    // 读取用户输入
    string x_diff_str;  // 输入差分 x'，16位二进制字符串
    cout << "Enter input differential x' (16-bit binary): ";
    cin >> x_diff_str;
    if(x_diff_str.length() != 16) {
        cout << "Error: x' must be 16 bits.\n";
        return 1;
    }
    // 将 x_diff_str 转换为整数
    int x_diff = stoi(x_diff_str, nullptr, 2);
    
    // 过滤条件：需要检查哪些组的明文对加密结果必须相同（例如：选择1组即可）
    int n_filter;
    cout << "Enter number of groups to filter (1-4): ";
    cin >> n_filter;
    vector<int> filterGroups;
    cout << "Enter " << n_filter << " distinct group numbers (1 to 4): ";
    for (int i = 0; i < n_filter; i++){
        int group;
        cin >> group;
        if(group < 1 || group > 4) {
            cout << "Error: group must be 1 to 4.\n";
            return 1;
        }
        filterGroups.push_back(group);
    }
    
    // 选择待破解的密钥部分（两个部分，共256种候选组合）
    int attackPart1, attackPart2;
    cout << "Enter first attacked key part (0-3): ";
    cin >> attackPart1;
    cout << "Enter second attacked key part (0-3): ";
    cin >> attackPart2;
    if(attackPart1 < 0 || attackPart1 > 3 || attackPart2 < 0 || attackPart2 > 3) {
        cout << "Error: attacked key parts must be between 0 and 3.\n";
        return 1;
    }
    
    // 预期差分值（以十六进制输入，0~F）—— 对应部分解密后的 u 值差分
    char expDiffChar1, expDiffChar2;
    cout << "Enter expected differential for part " << attackPart1 << " (hex 0-F): ";
    cin >> expDiffChar1;
    cout << "Enter expected differential for part " << attackPart2 << " (hex 0-F): ";
    cin >> expDiffChar2;
    int expectedDiff1 = (isdigit(expDiffChar1)) ? (expDiffChar1 - '0') : (toupper(expDiffChar1) - 'A' + 10);
    int expectedDiff2 = (isdigit(expDiffChar2)) ? (expDiffChar2 - '0') : (toupper(expDiffChar2) - 'A' + 10);
    
    // 频率统计：16x16矩阵，候选组合 (L_a, L_b)
    int freqMatrix[16][16] = {0};
    int totalQuadruples = 0;
    
    // 枚举所有16位明文 x
    for (int i = 0; i < 65536; i++){
        string x = int_to_bin16(i);
        // 计算 x* = x XOR x_diff
        string x_star;
        yh(x, int_to_bin16(x_diff), x_star);
        
        // 计算密文 y 与 y*（利用SPN加密）
        string y = encrypt_spn(x, k);
        string y_star = encrypt_spn(x_star, k);
        
        // 过滤：检查过滤组要求（例如，要求这些组的密文部分相同）
        if (!check_condition(y, y_star, filterGroups)) continue;
        
        totalQuadruples++;
        
        // 对待破解的两个部分进行部分解密验证
        // 例如：每个部分占4位，索引位置为: part i -> bits [i*4, i*4+3]
        int pos1 = attackPart1 * 4;
        int pos2 = attackPart2 * 4;
        string y_part1 = y.substr(pos1, 4);
        string y_star_part1 = y_star.substr(pos1, 4);
        string y_part2 = y.substr(pos2, 4);
        string y_star_part2 = y_star.substr(pos2, 4);
        
        int y_part1_val = bin4_to_int(y_part1);
        int y_star_part1_val = bin4_to_int(y_star_part1);
        int y_part2_val = bin4_to_int(y_part2);
        int y_star_part2_val = bin4_to_int(y_star_part2);
        
        // 遍历所有候选密钥 nibble组合
        for (int cand1 = 0; cand1 < 16; cand1++){
            // 计算部分解密：v = candidate XOR (ciphertext nibble)
            int v1 = cand1 ^ y_part1_val;
            int v1_star = cand1 ^ y_star_part1_val;
            // 逆S盒
            int u1 = invSBox(v1);
            int u1_star = invSBox(v1_star);
            int delta_u1 = u1 ^ u1_star;
            for (int cand2 = 0; cand2 < 16; cand2++){
                int v2 = cand2 ^ y_part2_val;
                int v2_star = cand2 ^ y_star_part2_val;
                int u2 = invSBox(v2);
                int u2_star = invSBox(v2_star);
                int delta_u2 = u2 ^ u2_star;
                
                // 如果两部分的差分均等于预期，则计数
                if(delta_u1 == expectedDiff1 && delta_u2 == expectedDiff2) {
                    freqMatrix[cand1][cand2]++;
                }
            } // for cand2
        } // for cand1
        
    } // for all x
    
    // 打印结果表格
    cout << "\n统计候选密钥组合出现频率（共" << totalQuadruples << "个符合过滤条件的四元组）:\n";
    cout << "     ";
    for (int j = 0; j < 16; j++) {
        cout << " " << uppercase << setw(3) << std::hex << j;
    }
    cout << "\n     " << string(16*4, '-') << "\n";
    for (int i = 0; i < 16; i++){
        cout << " " << uppercase << setw(2) << std::hex << i << " :";
        for (int j = 0; j < 16; j++){
            cout << setw(4) << dec << freqMatrix[i][j];
        }
        cout << "\n";
    }
    
    return 0;
}
