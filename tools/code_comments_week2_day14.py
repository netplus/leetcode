"""Reviewed C++ key-code comments for Week 2 / Day 14.

Entries are added only after an independent per-problem review.
"""


CODE_COMMENTS_WEEK2_DAY14 = {
    124: r'''// ---------- Solution ----------
class Solution {
    // 路径至少包含一个真实节点，所以全局答案不能从 0 开始；
    // 全负树中最佳答案可能仍是负数，INT_MIN 才不会把“空路径”误当候选。
    int best = INT_MIN;

    // gain(node) 返回“必须从 node 出发、向下最多选择一侧”时可继续交给父节点的最大单臂路径和。
    int gain(TreeNode* node) {
        if (!node) return 0;

        // 若某侧向下贡献为负，把它接入当前路径只会变差；
        // max(0, ...) 表示当前节点可以选择完全不进入这条负收益分支。
        const int left = max(0, gain(node->left));
        const int right = max(0, gain(node->right));

        // 当前节点作为路径最高汇合点时，可以同时使用左右两条正贡献，形成一条完整候选路径。
        best = max(best, node->val + left + right);

        // 但向父节点继续延伸时不能带两侧，否则路径会在当前节点分叉；只能返回较好的单臂。
        return node->val + max(left, right);
    }

public:
    int maxPathSum(TreeNode* root) {
        // 成员状态每次公开调用前重置，避免复用 Solution 时保留上一次树的答案。
        best = INT_MIN;
        gain(root);
        return best;
    }
};''',
}
