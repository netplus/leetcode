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

    542: r'''// ---------- Solution ----------
class Solution {
public:
    vector<vector<int>> updateMatrix(vector<vector<int>>& mat) {
        const int rows = static_cast<int>(mat.size());
        const int cols = static_cast<int>(mat[0].size());

        // distance 同时保存答案和 visited 状态：
        // -1 表示“尚未被任何 0 的波前到达，最近距离还没确定”；非负值就是已经确定的最短距离。
        vector<vector<int>> distance(rows, vector<int>(cols, -1));
        queue<pair<int, int>> pending;

        // 所有原始 0 必须在 BFS 开始前同时成为距离 0 的源。
        // 这样队列中的不同波前是在同一张图上并行扩散，而不是逐个 0 重复跑搜索。
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (mat[r][c] == 0) {
                    distance[r][c] = 0;
                    pending.push({r, c});
                }
            }
        }

        const int directions[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        while (!pending.empty()) {
            auto [r, c] = pending.front();
            pending.pop();

            // 多源 BFS 仍按 distance 非降序扩展；当前格的距离已经是到所有 0 中最近一个的最短距离。
            for (auto& d : directions) {
                int nr = r + d[0], nc = c + d[1];

                // 只处理 distance==-1 的格子。一个格子第一次被任意源的波前到达时，
                // distance[current]+1 已经是全体 0 源中的最短值，因此以后无需再次松弛或比较。
                if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && distance[nr][nc] == -1) {
                    // 赋值发生在入队之前：从这一刻起该格已被声明 visited，其他波前不会重复入队它。
                    distance[nr][nc] = distance[r][c] + 1;
                    pending.push({nr, nc});
                }
            }
        }

        // 题目保证至少有一个 0；四邻网格连通，因此所有格最终都会从 -1 变成其最近 0 距离。
        return distance;
    }
};''',

    994: r'''// ---------- Solution ----------
class Solution {
public:
    int orangesRotting(vector<vector<int>>& grid) {
        const int rows = static_cast<int>(grid.size());
        const int cols = static_cast<int>(grid[0].size());

        // rotten 是“当前传播前沿”队列；初始化时所有原始腐烂橘子都作为时间 0 的多源起点。
        queue<pair<int, int>> rotten;

        // fresh 是尚未被任何传播波前覆盖的新鲜橘子数量，也是判断最终是否还有不可达节点的剩余工作计数器。
        int fresh = 0;
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (grid[r][c] == 2) rotten.push({r, c});
                else if (grid[r][c] == 1) ++fresh;
            }
        }

        const int directions[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        int minutes = 0;

        // 没有 fresh 时无需进入循环，minutes 自然保持 0；
        // rotten 为空但 fresh>0 时也无法继续传播，最终会返回 -1。
        while (!rotten.empty() && fresh > 0) {
            // 冻结这一分钟开始时的 frontier 大小。处理过程中新入队的橘子属于“下一分钟”，
            // 不能在本轮继续传播，否则会把多分钟链式感染错误压缩到同一分钟。
            int layer = static_cast<int>(rotten.size());
            ++minutes;

            while (layer--) {
                auto [r, c] = rotten.front();
                rotten.pop();

                for (auto& d : directions) {
                    int nr = r + d[0], nc = c + d[1];
                    if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && grid[nr][nc] == 1) {
                        // 感染时立即执行 1->2，相当于“入队即 visited”；
                        // 这样同一新鲜橘子即使同时邻接多个腐烂橘子，也只会入队一次、fresh-- 一次。
                        grid[nr][nc] = 2;
                        --fresh;
                        rotten.push({nr, nc});
                    }
                }
            }
        }

        // fresh==0 表示所有新鲜橘子都已在某一 BFS 层被覆盖；
        // 若队列耗尽后 fresh 仍大于 0，则这些橘子与所有初始腐烂源都不可达。
        return fresh == 0 ? minutes : -1;
    }
};''',
}
