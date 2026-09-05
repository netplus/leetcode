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
}
