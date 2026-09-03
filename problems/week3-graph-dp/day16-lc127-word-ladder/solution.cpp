// ============================================================================
// LC-127：单词接龙
// 难度：困难
// 优先级：P0（必做）
// 学习进度：第 3 周 / 第 16 天
// ----------------------------------------------------------------------------
// 题目描述：
// 字典 wordList 中从单词 beginWord 到 endWord 的转换序列是一个按下述规格形成的序列 beginWord -> s_1 -> s_2 -> ... ->
// s_k：每一对相邻的单词只差一个字母。
// 对于 1 <= i <= k 时，每个 s_i 都在 wordList 中。
// 注意，beginWord 不需要在 wordList 中。
// s_k == endWord 给你两个单词 beginWord 和 endWord 和一个字典 wordList，返回从 beginWord 到 endWord 的最短转换序列中的单词数目。
// 如果不存在这样的转换序列，返回 0。
//
// 约束与要求：
//   - 1 <= beginWord.length <= 10
//   - endWord.length == beginWord.length
//   - 1 <= wordList.length <= 5000
//   - wordList[i].length == beginWord.length
//   - beginWord、endWord 和 wordList[i] 由小写英文字母组成
//   - beginWord != endWord
//   - wordList 中的所有字符串互不相同
//
// 复杂度目标：O(L^2 * N) 使用 BFS。
//
// ----------------------------------------------------------------------------
// 解法精讲｜单词隐式图上的 BFS
// - 核心要点：
//   1. 思路起点：每个单词是节点，相差一个字符的单词间有边；无需显式 O(N^2) 建图，可逐位替换 26 个字母并在字典集合中查邻居。
//   2. 执行逻辑：1. 若 endWord 不在字典返回 0；2. beginWord 以距离 1 入队；3. 逐层枚举每个位置的替换词，未访问词删除集合后入队。
//   3. 为什么这样做：BFS 按边数递增访问节点，首次生成 endWord 的路径必最短；从集合删除等价于标记 visited，不会影响任何更短路径。
// - 边界与易错点：序列长度包含起点和终点；beginWord 无需在 wordList；替换后要恢复原字符。
// - 举一反三：状态可局部变换且每步代价相同时，可把问题视为隐式无权图，用 BFS 找最少操作数。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：beginWord endWord。
//   第 2 行：L (wordList 中的单词数量)。
//   第 3 行：L 个以空格分隔的单词。
//   输出：最短转换序列的长度（单词数）。
// test.in 的预期输出：5
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        unordered_set<string> unused(wordList.begin(), wordList.end());
        if (!unused.count(endWord)) return 0;
        unused.erase(beginWord);
        queue<pair<string, int>> pending;
        pending.push({beginWord, 1});

        while (!pending.empty()) {
            auto [word, distance] = pending.front();
            pending.pop();
            if (word == endWord) return distance;
            for (int i = 0; i < static_cast<int>(word.size()); ++i) {
                const char original = word[i];
                for (char c = 'a'; c <= 'z'; ++c) {
                    if (c == original) continue;
                    word[i] = c;
                    auto it = unused.find(word);
                    if (it != unused.end()) {
                        pending.push({word, distance + 1});
                        unused.erase(it);  // 入队即标记，避免重复搜索
                    }
                }
                word[i] = original;
            }
        }
        return 0;
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    string b, e; if (!(cin >> b >> e)) return 0;
    int L; cin >> L;
    vector<string> wl(L);
    for (int i = 0; i < L; ++i) cin >> wl[i];
    cout << sol.ladderLength(b, e, wl) << "\n";
    return 0;
}

