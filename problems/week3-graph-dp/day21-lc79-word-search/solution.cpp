// ============================================================================
// LC-79：单词搜索
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 3 周 / 第 21 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给定一个 m x n 二维字符网格 board 和一个字符串单词 word。
// 如果 word 存在于网格中，返回 true；否则，返回 false。
// 单词必须按照字母顺序，通过相邻的单元格内的字母构成，其中“相邻”单元格是那些水平相邻或垂直相邻的单元格。
// 同一个单元格内的字母不允许被重复使用。
//
// 约束与要求：
//   - m == board.length
//   - n = board[i].length
//   - 1 <= m, n <= 6
//   - 1 <= word.length <= 15
//   - board 和 word 仅由大小写英文字母组成
//
// 复杂度目标：O(m*n*4^L) 时间（回溯）。
//
// ----------------------------------------------------------------------------
// 解法精讲｜网格回溯：路径内标记与恢复
// - 核心要点：
//   1. 思路起点：搜索状态由当前位置和 word 下标组成；同一条路径内格子不能复用，可临时改写字符作为 visited，回退后恢复。
//   2. 执行逻辑：1. 从每个格尝试匹配 word[0]；2. 字符匹配后若已到末尾则成功，否则标记并搜索四邻；3. 无论成功失败都恢复当前字符。
//   3. 为什么这样做：递归只沿相邻且未在路径中的匹配字符前进，所以找到的路径必合法；任意合法路径会从其首格开始按方向被枚举。
// - 边界与易错点：标记只应在当前递归路径生效，必须恢复；同格不能二次使用；大小写都可能出现，哨兵选输入不会含的字符。
// - 举一反三：棋盘单词、迷宫简单路径和拼图搜索都需要区分全局 visited 与仅当前路径 visited。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：m n。
//   接下来 m 行：每行是由 n 个字符组成的字符串。
//   一行：待查找单词。
//   输出：单词存在时输出 1，否则输出 0。
// test.in 的预期输出：1
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
    int rows = 0, cols = 0;

    bool search(vector<vector<char>>& board, const string& word, int r, int c, int index) {
        if (r < 0 || r >= rows || c < 0 || c >= cols || board[r][c] != word[index]) {
            return false;
        }
        if (index + 1 == static_cast<int>(word.size())) return true;

        const char saved = board[r][c];
        board[r][c] = '\0';
        bool found = search(board, word, r + 1, c, index + 1)
                  || search(board, word, r - 1, c, index + 1)
                  || search(board, word, r, c + 1, index + 1)
                  || search(board, word, r, c - 1, index + 1);
        board[r][c] = saved;  // 即使 found=true 也恢复，保持输入不变
        return found;
    }

public:
    bool exist(vector<vector<char>>& board, string word) {
        rows = static_cast<int>(board.size());
        cols = static_cast<int>(board[0].size());
        for (int r = 0; r < rows; ++r)
            for (int c = 0; c < cols; ++c)
                if (search(board, word, r, c, 0)) return true;
        return false;
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int m, n; if (!(cin >> m >> n)) return 0;
    vector<vector<char>> b(m, vector<char>(n));
    for (int i = 0; i < m; ++i) { string row; cin >> row; for (int j = 0; j < n; ++j) b[i][j] = row[j]; }
    string word; cin >> word;
    cout << (sol.exist(b, word) ? 1 : 0) << "\n";
    return 0;
}

