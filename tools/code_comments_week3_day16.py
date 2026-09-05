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

    417: r'''// ---------- Solution ----------
class Solution {
public:
    vector<vector<int>> pacificAtlantic(vector<vector<int>>& heights) {
        const int rows = static_cast<int>(heights.size());
        const int cols = static_cast<int>(heights[0].size());

        // 同一个格子“能否到太平洋”和“能否到大西洋”是两份独立可达状态，不能共用 visited。
        vector<vector<char>> pacific(rows, vector<char>(cols, false));
        vector<vector<char>> atlantic(rows, vector<char>(cols, false));

        // flood 在“反向水流图”里做一次多源 BFS：seen 表示当前这一个海洋能反向到达哪些内陆格。
        // starts 按值传入，是一份本次洪泛专用的源列表；调用处用 move 只是在避免额外复制。
        auto flood = [&](vector<vector<char>>& seen, vector<pair<int, int>> starts) {
            queue<pair<int, int>> pending;

            // 同一海洋的两条边会在角落产生重复源；入队前先检查 seen，确保每个源格只进入队列一次。
            for (auto [r, c] : starts) {
                if (!seen[r][c]) {
                    seen[r][c] = true;
                    pending.push({r, c});
                }
            }

            const int directions[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
            while (!pending.empty()) {
                auto [r, c] = pending.front();
                pending.pop();

                for (auto& d : directions) {
                    int nr = r + d[0], nc = c + d[1];

                    // 原水流允许 high -> low/equal；把边反过来以后，海洋向内陆只能走到 high/equal：
                    // heights[next] >= heights[current]。首次发现时立即标记，避免重复入队。
                    if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && !seen[nr][nc]
                        && heights[nr][nc] >= heights[r][c]) {
                        seen[nr][nc] = true;
                        pending.push({nr, nc});
                    }
                }
            }
        };

        vector<pair<int, int>> pStarts, aStarts;

        // 太平洋接触左边界，大西洋接触右边界。
        for (int r = 0; r < rows; ++r) {
            pStarts.push_back({r, 0});
            aStarts.push_back({r, cols - 1});
        }

        // 太平洋接触上边界，大西洋接触下边界；四个角的重复会由 flood 的 seen 检查消掉。
        for (int c = 0; c < cols; ++c) {
            pStarts.push_back({0, c});
            aStarts.push_back({rows - 1, c});
        }

        // move 只是把临时源列表的存储交给 lambda 参数，减少一次 vector 拷贝；不改变多源 BFS 语义。
        flood(pacific, move(pStarts));
        flood(atlantic, move(aStarts));

        vector<vector<int>> answer;
        // 一个格子原方向能同时流向两海洋，当且仅当它属于两次反向可达集合的交集。
        for (int r = 0; r < rows; ++r)
            for (int c = 0; c < cols; ++c)
                if (pacific[r][c] && atlantic[r][c]) answer.push_back({r, c});
        return answer;
    }
};''',
}
