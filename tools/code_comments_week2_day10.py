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

    102: r'''// ---------- Solution ----------
class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> levels;
        if (!root) return levels;

        // 每一轮 while 开始时，pending 中恰好按从左到右顺序保存“当前层”的全部节点。
        // 本轮处理这些节点时，新发现的孩子只追加到队尾，成为下一层 frontier。
        queue<TreeNode*> pending;
        pending.push(root);

        while (!pending.empty()) {
            // 必须在任何孩子入队之前冻结当前 queue 大小。
            // width 只描述这一层；若把循环上界写成动态 pending.size()，
            // size 会随着孩子入队增长，下一层节点会被错误地混进当前层。
            const int width = static_cast<int>(pending.size());
            vector<int> level;
            level.reserve(width);

            for (int i = 0; i < width; ++i) {
                TreeNode* node = pending.front();
                pending.pop();
                level.push_back(node->val);

                // 当前层按左到右出队；每个父节点又按 left、right 顺序入队孩子，
                // 因而下一轮 pending 仍自然保持下一层的从左到右顺序。
                if (node->left) pending.push(node->left);
                if (node->right) pending.push(node->right);
            }

            // 恰好消费 width 个旧节点后，本层已经完整结束；
            // 此刻 pending 只剩下一层，因此现在才把这批 level 作为一个整体提交到结果。
            // move 只是避免复制这个临时 vector，不参与 BFS 的正确性。
            levels.push_back(move(level));
        }

        return levels;
    }
};''',

    104: r'''// ---------- Solution ----------
class Solution {
public:
    int maxDepth(TreeNode* root) {
        // 函数契约：maxDepth(node) 返回“以 node 为根的子树最大深度”，按路径上的节点数计。
        // 空子树不贡献任何节点，因此深度为 0；这个边界也让叶子节点自然得到 1。
        if (!root) return 0;

        // 从当前节点到最远叶子的路径只能选择左、右子树中的一条继续向下；
        // 两个递归调用分别完整计算左右子树深度，父节点取更深的一边，再把自己这一层 +1。
        // 这里不能写成 left+right+1：左右相加是在拼一条经过当前节点的双臂路径，那是 LC-543 直径的模型。
        return 1 + max(maxDepth(root->left), maxDepth(root->right));
    }
};''',
}
