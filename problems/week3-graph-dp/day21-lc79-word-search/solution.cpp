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
// 题目示例：
// 示例 1：
//   Input: board = [["A","B","C","E"],["S","F","C","S"],["A","D","E","E"]], word = "ABCCED"
//   Output: true
//
// 示例 2：
//   Input: board = [["A","B","C","E"],["S","F","C","S"],["A","D","E","E"]], word = "SEE"
//   Output: true
//
// 示例 3：
//   Input: board = [["A","B","C","E"],["S","F","C","S"],["A","D","E","E"]], word = "ABCB"
//   Output: false
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
// 解法精讲｜路径回溯：把 Day 13 的选择-递归-撤销迁移到二维网格
//
// 1. 图像直觉
//   以 word=ABCCED 为例：
//
//   A B C E
//   S F C S
//   A D E E
//   ^
//   index=0
//
//   选中 A 后，只能从它的上下左右寻找 B；找到 B 后继续寻找 C。
//   每深入一层，都把当前格暂时封住：
//
//   A* -> B* -> C* -> C* -> E* -> D
//
//   星号表示“当前路径已经用过”。若某一步走不通，就撤销最后一个星号，再尝试上一层的其他方向。
//
//   这和 LC-46 全排列的 used 完全同构：
//   LC-46 的候选是数组下标；本题的候选是四邻格子。
//   LC-46 回退时 used[i]=false；本题回退时恢复 board[r][c]。
//
// 2. 一句话核心
//   从每个可能的首字母出发，沿四邻递归匹配下一个字符；当前路径使用过的格子临时封锁，返回上一层时恢复。
//
// 3. 公式 / 不变量
//   search(r,c,index) 的含义：
//   是否存在一条从 (r,c) 开始的合法简单路径，恰好匹配 word[index..]。
//
//   进入状态的必要条件：
//   0 <= r < rows
//   0 <= c < cols
//   board[r][c] == word[index]
//
//   若 index+1 == word.size()：当前字符已经完成最后一个匹配，返回 true。
//   否则临时标记当前格，再枚举四个后继：
//   (r+1,c), (r-1,c), (r,c+1), (r,c-1)。
//
//   路径不变量：递归栈中已经匹配的每个格子互不重复，并且字符序列严格等于 word[0..index]。
//
// 4. 执行步骤
//   1. 外层枚举每个格子作为 word[0] 的候选起点；search 自己负责字符不匹配时立即失败
//   2. 进入 search 后先检查边界和字符；若当前 index 已是最后一个字符，直接成功
//   3. 保存 board[r][c]，再把它改成哨兵，表示这个格子只在当前递归路径中暂时不可用
//   4. 依次尝试四个相邻方向去匹配 index+1；任一方向成功即可令 found=true
//   5. 无论 found 是否为 true，都恢复原字符后再返回，保证输入结构和兄弟搜索分支不受污染
//   记忆：路径内占用，回退时恢复；visited 的生命周期就是递归栈。
//
// 5. 为什么不会漏 / 不会重
//   任意被算法接受的递归链，每一层都只从上一个格子的四邻进入，字符又经过 word[index] 检查；路径内已使用格被哨兵阻止再次进入，因此它一定满足题目的相邻、字符顺序和不重复约束。反过来，任意合法路径都有一个首格，外层枚举一定会尝试它；在路径的每个位置，四方向枚举一定包含合法路径的下一格，而路径中的格子此前未重复，因此不会被哨兵错误阻止，所以整条合法路径必能被枚举到。
//
// 6. 边界与易错点
//   不要使用永久的全局 visited：某个格子在路径 A 中失败后，仍可能在路径 B 中合法使用。恢复动作不能因为短路成功而省略，否则 exist 返回前会改变调用者的 board；当前实现先保存 found、再统一恢复。哨兵必须选择题面不会出现的字符，'\0' 满足当前英文字母约束。
//
// 7. 举一反三
//   直接复用 Day 13 的回溯框架：状态选择 -> 做选择 -> 递归 -> 撤销选择。它也和 Day 15 的网格 BFS 形成对比：连通分量问题的 visited 是全局事实，而简单路径搜索的 visited 只是当前路径约束。数独、迷宫简单路径、棋盘单词搜索都应先问 visited 究竟是全局还是 path-local。
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

    // search(r,c,index) 只回答一件事：能否从当前格开始匹配 word[index..]。
    // 递归栈中已经走过的格子会被临时封锁，因此每条搜索路径天然保持“同一格不能重复使用”。
    bool search(vector<vector<char>>& board, const string& word, int r, int c, int index) {
        // 越界或当前字符不匹配时，这个方向不可能继续；哨兵 '\0' 也会在这里自然被判为不匹配。
        if (r < 0 || r >= rows || c < 0 || c >= cols || board[r][c] != word[index]) {
            return false;
        }

        // 当前格已经匹配到 word 的最后一个字符时，不需要再寻找后继，整条路径成功。
        if (index + 1 == static_cast<int>(word.size())) return true;

        // visited 只属于“当前递归路径”，不能永久标记：同一格在另一条候选路径中仍可能合法使用。
        // 用题面不会出现的 '\0' 临时覆盖当前格，等价于 path-local used[r][c]=true。
        const char saved = board[r][c];
        board[r][c] = '\0';

        // 下一字符必须来自上下左右四邻之一；短路 OR 只影响是否继续尝试其它方向，不影响后面的统一恢复。
        bool found = search(board, word, r + 1, c, index + 1)
                  || search(board, word, r - 1, c, index + 1)
                  || search(board, word, r, c + 1, index + 1)
                  || search(board, word, r, c - 1, index + 1);

        // 无论成功还是失败都必须撤销当前路径的选择；否则兄弟分支或调用者会看到被污染的 board。
        board[r][c] = saved;
        return found;
    }

public:
    bool exist(vector<vector<char>>& board, string word) {
        rows = static_cast<int>(board.size());
        cols = static_cast<int>(board[0].size());

        // 合法答案的首字符可能位于任意格；每次 search 都拥有自己独立的 path-local visited 生命周期。
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
