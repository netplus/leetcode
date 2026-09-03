// ============================================================================
// LC-139：单词拆分
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 3 周 / 第 21 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给你一个字符串 s 和一个字符串列表 wordDict 作为字典。
// 如果可以利用字典中出现的一个或多个单词拼接出 s 则返回 true。
// 注意：不要求字典中出现的单词全部都使用，并且字典中的单词可以重复使用。
//
// 约束与要求：
//   - 1 <= s.length <= 300
//   - 1 <= wordDict.length <= 1000
//   - 1 <= wordDict[i].length <= 20
//   - s 和 wordDict[i] 仅由小写英文字母组成
//   - wordDict 中的所有字符串互不相同
//
// 复杂度目标：O(n^2) 或 O(n*L) 时间（DP）。
//
// ----------------------------------------------------------------------------
// 解法精讲｜前缀可达 DP
// - 核心要点：
//   1. 思路起点：dp[i] 表示前 i 个字符能否被字典词完整切分；从每个可达位置 i 尝试接一个字典词，把终点标为可达。
//   2. 执行逻辑：1. 字典放入哈希集合并求最大词长；2. dp[0]=true；3. 对可达 i 枚举长度并检查 s.substr(i,len)，命中则置 dp[i+len]。
//   3. 为什么这样做：任一合法切分的最后一个词从某个可达前缀开始，转移会发现它；反之每次转移都在合法前缀后追加字典词，因此新状态也合法。
// - 边界与易错点：同一字典词允许重复使用；空前缀必须设为 true；最大词长可限制无意义枚举。
// - 举一反三：字符串分段、解码和路径可达 DP 的核心都是“前缀状态 + 最后一段/下一段选择”。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：s。
//   第 2 行：L (单词数量)。
//   第 3 行：L 个以空格分隔的单词。
//   输出：s 可被拆分时输出 1，否则输出 0。
// test.in 的预期输出：1
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        unordered_set<string> words(wordDict.begin(), wordDict.end());
        int maxLength = 0;
        for (const string& word : wordDict) maxLength = max(maxLength, static_cast<int>(word.size()));
        vector<char> reachable(s.size() + 1, false);
        reachable[0] = true;

        for (int start = 0; start < static_cast<int>(s.size()); ++start) {
            if (!reachable[start]) continue;
            for (int length = 1; length <= maxLength && start + length <= static_cast<int>(s.size()); ++length) {
                if (words.count(s.substr(start, length))) reachable[start + length] = true;
            }
        }
        return reachable[s.size()];
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    string s; if (!(cin >> s)) return 0;
    int L; cin >> L;
    vector<string> w(L);
    for (int i = 0; i < L; ++i) cin >> w[i];
    cout << (sol.wordBreak(s, w) ? 1 : 0) << "\n";
    return 0;
}

