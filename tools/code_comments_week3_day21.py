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
        if (r < 0 || r >= rows || c < 0 || c >= cols || board[r][c] != word[index]) return false;
        if (index + 1 == static_cast<int>(word.size())) return true;
        const char saved = board[r][c];
        board[r][c] = '\0';
        bool found = search(board, word, r + 1, c, index + 1)
                  || search(board, word, r - 1, c, index + 1)
                  || search(board, word, r, c + 1, index + 1)
                  || search(board, word, r, c - 1, index + 1);
        board[r][c] = saved;
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
};''',

    139: r'''// ---------- Solution ----------
class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        // 字典只承担“候选子串是否是一条合法边”的查询，因此用哈希集合避免每次线性扫描 wordDict。
        unordered_set<string> words(wordDict.begin(), wordDict.end());
        int maxLength = 0;
        for (const string& word : wordDict) maxLength = max(maxLength, static_cast<int>(word.size()));
        // reachable[i] 表示前缀 s[0:i) 能否被字典词完整切分；空前缀是合法起点。
        vector<char> reachable(s.size() + 1, false);
        reachable[0] = true;
        for (int start = 0; start < static_cast<int>(s.size()); ++start) {
            if (!reachable[start]) continue;
            // 只从可达切分点扩展，并用 maxLength 裁掉不可能命中的过长候选。
            for (int length = 1; length <= maxLength && start + length <= static_cast<int>(s.size()); ++length) {
                if (words.count(s.substr(start, length))) reachable[start + length] = true;
            }
        }
        return reachable[s.size()];
    }
};''',

    152: r'''// ---------- Solution ----------
class Solution {
public:
    int maxProduct(vector<int>& nums) {
        // 三个状态都从 nums[0] 开始，保证单元素或全负数组不会被虚假的 0 初值覆盖。
        // currentMax/currentMin 分别是“必须以当前位置结尾”的最大/最小连续子数组乘积；
        // best 才是所有已处理终点中的历史全局最大值。
        int currentMax = nums[0];
        int currentMin = nums[0];
        int best = nums[0];

        for (int i = 1; i < static_cast<int>(nums.size()); ++i) {
            const int value = nums[i];

            // 乘负数会把大小顺序完全翻转：旧最小值可能成为新最大值的最佳来源，旧最大值反之。
            // 先交换两者后，后续就仍可用“最大延续最大、最小延续最小”的统一写法。
            if (value < 0) swap(currentMax, currentMin);

            // 所有以 i 结尾的候选只有两类：从 value 单独重开，或把 value 接到旧后缀后。
            // swap 已经处理了负数时的来源互换，因此这里只需分别维护新的两个极值。
            currentMax = max(value, currentMax * value);
            currentMin = min(value, currentMin * value);

            // currentMax 只覆盖“当前终点”，全局答案必须另外保留历史 best。
            best = max(best, currentMax);
        }
        return best;
    }
};''',
}
