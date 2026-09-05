"""Reviewed C++ key-code comments for Week 3 / Day 16.

Entries are added only after an independent per-problem review.
"""


CODE_COMMENTS_WEEK3_DAY16 = {
    127: r'''// ---------- Solution ----------
class Solution {
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        // unused 同时承担两项职责：
        // 1) 字典：候选字符串只有仍存在于 unused 中，才是图里的合法单词节点；
        // 2) visited：一个单词第一次入队后立刻从 unused 删除，之后不会被等长/更长路径重复搜索。
        unordered_set<string> unused(wordList.begin(), wordList.end());

        // 题目要求转换序列最后必须是 wordList 中的 endWord；若它根本不在字典中，不存在合法答案。
        if (!unused.count(endWord)) return 0;

        // beginWord 本身不要求出现在 wordList；若恰好存在，先删除它，避免后续某次字符替换又绕回起点。
        unused.erase(beginWord);

        // BFS 状态的 distance 统计“序列中包含多少个单词”，不是已经做了多少次变换；
        // 因此起点 beginWord 本身已经占 1 个单词，初始距离必须是 1。
        queue<pair<string, int>> pending;
        pending.push({beginWord, 1});

        while (!pending.empty()) {
            auto [word, distance] = pending.front();
            pending.pop();

            // 无权图 BFS 按距离非降序出队；endWord 第一次出队时，这个 distance 就是最短序列长度。
            if (word == endWord) return distance;

            // 图没有预先显式建边：当前 word 的所有真实邻居，都由“只改一个字符”这个规则现场生成。
            for (int i = 0; i < static_cast<int>(word.size()); ++i) {
                const char original = word[i];

                for (char c = 'a'; c <= 'z'; ++c) {
                    if (c == original) continue;
                    word[i] = c;

                    // find 是纯查询：只判断这个一位替换后的字符串是否仍是未访问的合法字典节点。
                    // 若找到，第一次发现已经是最短距离，所以先入队，再 erase(it) 立即完成 visited 标记。
                    auto it = unused.find(word);
                    if (it != unused.end()) {
                        pending.push({word, distance + 1});
                        unused.erase(it);
                    }
                }

                // 内层枚举会原地改写 word[i]；处理完这一位后必须恢复原字符，
                // 否则下一位置会在“已经改过一位”的错误字符串上继续生成候选，等价于一次改多个字符。
                word[i] = original;
            }
        }

        // 所有从 beginWord 可达的未访问单词都已处理仍未到终点，说明不存在转换序列。
        return 0;
    }
};''',
}
