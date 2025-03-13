#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

// 计算所有长度为3的子串的索引差距
unordered_map<string, vector<int>> find_trigram_distances(const string &text) {
    unordered_map<string, vector<int>> distances;
    unordered_map<string, vector<int>> seen_positions;
    
    // 遍历所有长度为3的子串
    for (size_t i = 0; i < text.size() - 2; i++) {
        string trigram = text.substr(i, 3);
        seen_positions[trigram].push_back(i);
    }
    
    // 计算每个子串的索引差距
    for (const auto &entry : seen_positions) {
        const string &trigram = entry.first;
        const vector<int> &positions = entry.second;
        if (positions.size() > 1) {
            for (size_t j = 1; j < positions.size(); j++) {
                distances[trigram].push_back(positions[j] - positions[j - 1]);
            }
        }
    }
    
    return distances;
}

int main() {
    string text = "BNVSNSIHQCEELSSKKYERIFJKXUMBGYKAMQLJTYAVFBKVT "
                  "DVBPVVRJYYLAOKYMPQSCGDLFSRLLPROYGESEBUUALRWXM "
                  "MASAZLGLEDFJBZAVVPXWICGJXASCBYEHOSNMULKCEAHTQ "
                  "OKMFLEBKFXLRRFDTZXCIWBJSICBGAWDVYDHAVFJXZIBKC "
                  "GJIWEAHTTOEWTUHKRQVVRGZBXYIREMMASCSPBNLHJMBLR "
                  "FFJELHWEYLWISTFVVYFJCMHYUYRUFSFMGESIGRLWALSWM "
                  "NUHSIMYYITCCQPZSICEHBCCMZFEGVJYOCDEMMPGHVAAUM "
                  "ELCMOEHVLTIPSUYILVGFLMVWDVYDBTHFRAYISYSGKVSUUHYHGGCKTMBLRX";
    
    // 移除空格
    text.erase(remove(text.begin(), text.end(), ' '), text.end());
    text.shrink_to_fit(); // 确保字符串容量调整
    
    unordered_map<string, vector<int>> distances = find_trigram_distances(text);
    
    // 输出结果
    for (const auto &entry : distances) {
        cout << "Trigram '" << entry.first << "': distances [";
        for (size_t i = 0; i < entry.second.size(); i++) {
            cout << entry.second[i];
            if (i < entry.second.size() - 1) cout << ", ";
        }
        cout << "]" << endl;
    }
    system("pause");
    return 0;
}
