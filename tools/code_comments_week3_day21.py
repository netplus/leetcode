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

    210: r'''// ---------- Solution ----------
class Solution {
public:
    vector<int> findOrder(int numCourses, vector<vector<int>>& prerequisites) {
        // [a,b] 表示“学 a 前必须先学 b”，所以图边必须是 b -> a；
        // indegree[a] 统计 a 当前还剩多少个尚未完成的直接先修课程。
        vector<vector<int>> next(numCourses);
        vector<int> indegree(numCourses, 0);
        for (const auto& relation : prerequisites) {
            next[relation[1]].push_back(relation[0]);
            ++indegree[relation[0]];
        }

        // 初始入度为 0 的课程没有任何未完成先修，可以立即作为拓扑序的候选起点。
        queue<int> ready;
        for (int course = 0; course < numCourses; ++course)
            if (indegree[course] == 0) ready.push(course);

        vector<int> order;
        while (!ready.empty()) {
            int course = ready.front();
            ready.pop();

            // 一个课程只有在入度已为 0 时才会进入 ready，因此此刻把它写入 order 一定满足全部先修约束。
            order.push_back(course);

            // “完成 course”相当于删除它的所有出边；某后继只有在最后一条未完成先修边被删除、
            // 入度恰好降到 0 时才第一次入队，因此每门课不会重复进入 ready。
            for (int dependent : next[course])
                if (--indegree[dependent] == 0) ready.push(dependent);
        }

        // 若输出了全部课程，order 就是一条合法拓扑序；若数量不足，剩余节点始终无法产生零入度点，说明被有向环卡住。
        return static_cast<int>(order.size()) == numCourses ? order : vector<int>{};
    }
};''',
}
