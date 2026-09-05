"""Reviewed C++ key-code comments for Week 2 / Day 13.

Entries are added only after an independent per-problem review.
"""


CODE_COMMENTS_WEEK2_DAY13 = {
    46: r'''// ---------- Solution ----------
class Solution {
    vector<vector<int>> answer;

    // path 是搜索树中“从根走到当前节点”的选择序列，也就是已经确定的排列前缀。
    vector<int> path;

    // used[i] 只描述当前递归路径：nums[i] 是否已经出现在 path 中。
    // 它不是全局访问标记，因此离开该分支时必须恢复。
    vector<char> used;

    void search(const vector<int>& nums) {
        // 一旦 path 已经放入 n 个互异元素，就形成一个完整排列；
        // answer 保存的是当前 path 的副本，随后 path 还会继续被回溯修改。
        if (path.size() == nums.size()) {
            answer.push_back(path);
            return;
        }

        // 当前递归层决定“排列的下一个位置选哪个尚未使用的元素”。
        for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
            if (used[i]) continue;

            // 做选择：used 与 path 必须同步更新，进入子调用时二者精确描述同一个排列前缀。
            used[i] = true;
            path.push_back(nums[i]);

            // 探索所有以当前前缀开头的完整排列。
            search(nums);

            // 撤销选择：递归返回后必须恢复到进入这个候选之前的父状态，
            // 这样下一个兄弟候选看到的 path/used 与本次尝试之前完全一致。
            path.pop_back();
            used[i] = false;
        }
    }

public:
    vector<vector<int>> permute(vector<int>& nums) {
        // 成员状态每次公开调用前重置，避免复用同一个 Solution 对象时残留旧搜索结果。
        answer.clear();
        path.clear();

        // assign(count, value) 在 C++17 中合法；这里把每个元素初始化为“当前路径尚未使用”。
        used.assign(nums.size(), false);

        search(nums);
        return answer;
    }
};''',

    78: r'''// ---------- Solution ----------
class Solution {
    vector<vector<int>> answer;

    // path 是当前已经选中的子集；与排列不同，path 的任意长度都已经构成一个合法答案。
    vector<int> path;

    // start 规定下一步只能从哪个原数组下标开始选。
    // 因此 path 中元素对应的原下标始终严格递增，为每个“无序子集”建立唯一构造顺序。
    void search(const vector<int>& nums, int start) {
        // 当前搜索节点本身就是一个合法子集；根节点 path=[] 也会在这里收集为空集。
        answer.push_back(path);

        // 只允许从 start 向右枚举下一元素，不再回头选择更小下标。
        for (int i = start; i < static_cast<int>(nums.size()); ++i) {
            // 做选择后，下一层传 i+1：当前下标不能重复使用，也不会生成 [2,1] 这种同集合的另一顺序。
            path.push_back(nums[i]);
            search(nums, i + 1);

            // 子树探索完后撤销当前元素，恢复父节点对应的子集，再尝试下一个兄弟候选。
            path.pop_back();
        }
    }

public:
    vector<vector<int>> subsets(vector<int>& nums) {
        // 成员状态在每次公开调用前重置，保证同一个 Solution 对象可重复使用。
        answer.clear();
        path.clear();

        // start=0 表示根节点还没有排除任何候选；search 会先收集空集。
        search(nums, 0);
        return answer;
    }
};''',

    39: r'''// ---------- Solution ----------
class Solution {
    vector<vector<int>> answer;
    vector<int> path;

    // start 让 path 中使用的候选下标保持不减，给“组合”规定唯一的非降序构造顺序；
    // remaining 表示当前路径距离 target 还差多少。
    void search(const vector<int>& candidates, int start, int remaining) {
        // remaining 恰好归零时，当前 path 才是一组完整答案；所有候选为正数，所以之后无需继续扩展。
        if (remaining == 0) {
            answer.push_back(path);
            return;
        }

        for (int i = start; i < static_cast<int>(candidates.size()); ++i) {
            // candidates 已升序；若当前值都超过 remaining，右侧候选只会更大，
            // 且所有数为正，不可能靠继续选择把 remaining 拉回 0，因此可直接停止本层。
            if (candidates[i] > remaining) break;

            path.push_back(candidates[i]);

            // 与 LC-78 的 i+1 不同：本题允许同一候选无限复用，所以递归仍传 i；
            // 但不会传更小下标，从而不会生成 [2,3,2] 这类同一组合的另一排列。
            search(candidates, i, remaining - candidates[i]);

            // 当前候选对应的整棵子树探索完后撤销，恢复父状态再尝试下一个候选。
            path.pop_back();
        }
    }

public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        // 排序有两个作用：建立组合的统一非降序表示，并使“过大就 break”的剪枝有单调性依据。
        // 这会原地改变调用方传入的 candidates 顺序，是当前实现可观察的副作用。
        sort(candidates.begin(), candidates.end());

        answer.clear();
        path.clear();
        search(candidates, 0, target);
        return answer;
    }
};''',

    22: r'''// ---------- Solution ----------
class Solution {
    vector<string> answer;
    string path;

    // open/close 是当前前缀已经使用的左右括号数。
    // 每次进入 search 时都保持 0 <= close <= open <= n，因此 path 始终是“仍可能完成”的合法前缀。
    void search(int n, int open, int close) {
        // 前缀始终合法且长度已达到 2*n 时，必有 open==close==n；
        // 所以当前 path 自动是一条完整合法答案，不需要再调用 LC-20 做二次验证。
        if (static_cast<int>(path.size()) == 2 * n) {
            answer.push_back(path);
            return;
        }

        // 左括号总量还没达到 n 时，可以继续开启一个尚未闭合的结构。
        if (open < n) {
            path.push_back('(');
            search(n, open + 1, close);
            path.pop_back();
        }

        // 只有当前未闭合左括号数 open-close > 0 时，才允许添加右括号；
        // 这条条件把任何 close>open 的永久非法前缀在生成之前就剪掉。
        if (close < open) {
            path.push_back(')');
            search(n, open, close + 1);
            path.pop_back();
        }
    }

public:
    vector<string> generateParenthesis(int n) {
        // 每次公开调用前清空成员状态；根搜索状态是空前缀、左右括号都尚未使用。
        answer.clear();
        path.clear();
        search(n, 0, 0);
        return answer;
    }
};''',
}
