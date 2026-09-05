"""Reviewed C++ key-code comments for Week 3 / Day 21.

Entries are added only after an independent per-problem review.
"""


CODE_COMMENTS_WEEK3_DAY21 = {
    79: r'''// ---------- Solution ----------
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
};''',

    139: r'''// ---------- Solution ----------
class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        // 字典只承担“候选子串是否是一条合法边”的查询，因此用哈希集合避免每次线性扫描 wordDict。
        unordered_set<string> words(wordDict.begin(), wordDict.end());

        // 从某个切分点出发，任何长度超过最长字典词的子串都不可能命中；
        // maxLength 直接裁掉这些确定无用的候选结束位置。
        int maxLength = 0;
        for (const string& word : wordDict) maxLength = max(maxLength, static_cast<int>(word.size()));

        // reachable[i] 表示前缀 s[0:i) 能否被字典词完整切分。
        // 空前缀天然是一个合法起点，因此 reachable[0]=true；没有它，任何后续位置都无法被首次到达。
        vector<char> reachable(s.size() + 1, false);
        reachable[0] = true;

        for (int start = 0; start < static_cast<int>(s.size()); ++start) {
            // 不可达位置没有任何合法前缀能走到这里，即使后面恰好存在字典词也不能作为完整切分使用。
            if (!reachable[start]) continue;

            // 只从真正可达的切分点向前连边；同一个字典词可以在不同 start 被重复使用，
            // 因为题目限制的是字符串拼接关系，而不是“每个字典词只能消费一次”。
            for (int length = 1; length <= maxLength && start + length <= static_cast<int>(s.size()); ++length) {
                if (words.count(s.substr(start, length))) reachable[start + length] = true;
            }
        }

        // 位置 n 可达，等价于完整前缀 s[0:n) 可以被若干字典词首尾相接地覆盖。
        return reachable[s.size()];
    }
};''',
}
