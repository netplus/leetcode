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

    94: r'''// ---------- Solution ----------
class Solution {
public:
    vector<int> inorderTraversal(TreeNode* root) {
        vector<int> order;

        // ancestors 不是普通“待访问节点”集合，而是一路向左递归时被暂停的调用帧：
        // 栈中节点都还没访问自己，因为它们的左子树尚未完整返回。
        stack<TreeNode*> ancestors;
        TreeNode* current = root;

        // current 表示当前正在进入的子树；ancestors 非空表示仍有暂停祖先等待恢复。
        // 两者都为空时，整棵树才真正处理完。
        while (current || !ancestors.empty()) {
            // 模拟递归的 inorder(node->left)：一路向左下潜，
            // 同时把“左子树回来后还要访问根、再去右边”的祖先现场保存到栈里。
            while (current) {
                ancestors.push(current);
                current = current->left;
            }

            // current==nullptr 说明最近栈顶节点的左子树已经完整结束；
            // 此刻正好处于递归中的 Left 与 Right 之间，因此现在才访问这个根。
            current = ancestors.top();
            ancestors.pop();
            order.push_back(current->val);

            // 根访问完后开始 inorder(root->right)；右子树内部又会先一路向左下潜。
            current = current->right;
        }

        return order;
    }
};''',

    145: r'''// ---------- Solution ----------
class Solution {
public:
    vector<int> postorderTraversal(TreeNode* root) {
        vector<int> order;
        if (!root) return order;

        // 第一阶段并不直接生成后序，而是生成更容易用单栈维护的 Root -> Right -> Left。
        stack<TreeNode*> pending;
        pending.push(root);

        while (!pending.empty()) {
            TreeNode* node = pending.top();
            pending.pop();

            // 节点一弹出就记录，所以当前序列的“根”一定先出现。
            order.push_back(node->val);

            // 想让下一次先处理 right，就必须让 right 后入栈：
            // 因此与 LC-144 相反，先压 left、再压 right，最终得到 Root -> Right -> Left。
            if (node->left) pending.push(node->left);
            if (node->right) pending.push(node->right);
        }

        // 对整棵树的 Root -> Right -> Left 序列整体反转，
        // 每个局部相对顺序同步变成 Left -> Right -> Root，即真正后序。
        reverse(order.begin(), order.end());
        return order;
    }
};''',
}
