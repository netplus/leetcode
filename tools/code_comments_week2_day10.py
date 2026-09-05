"""Reviewed C++ key-code comments for Week 2 / Day 10.

Entries are added only after an independent per-problem review.
"""


CODE_COMMENTS_WEEK2_DAY10 = {
    144: r'''// ---------- Solution ----------
class Solution {
public:
    vector<int> preorderTraversal(TreeNode* root) {
        vector<int> order;
        if (!root) return order;

        // pending 保存“以后还要进入的子树根”；栈顶始终是按前序顺序下一个该处理的节点。
        stack<TreeNode*> pending;
        pending.push(root);

        while (!pending.empty()) {
            TreeNode* node = pending.top();
            pending.pop();

            // 前序是 Root -> Left -> Right，所以节点一旦从栈顶成为当前节点，就立即访问根值。
            order.push_back(node->val);

            // 栈后进先出，而左子树必须先于右子树处理：
            // 因此反着保存未来分支——先压右，再压左，让 left 成为新的栈顶。
            if (node->right) pending.push(node->right);
            if (node->left) pending.push(node->left);
        }

        return order;
    }
};''',
}
