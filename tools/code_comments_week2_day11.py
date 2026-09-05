"""Reviewed C++ key-code comments for Week 2 / Day 11.

Entries are added only after an independent per-problem review.
"""


CODE_COMMENTS_WEEK2_DAY11 = {
    226: r'''// ---------- Solution ----------
class Solution {
public:
    TreeNode* invertTree(TreeNode* root) {
        // 递归契约：invertTree(node) 返回“以 node 为根、已经完成左右镜像的子树根”。
        // 空子树本身就是镜像完成状态，因此直接返回 nullptr。
        if (!root) return nullptr;

        // 先让原左右子树各自独立完成全部翻转，再拿到两个已经正确的子问题结果。
        // 先保存返回值可以避免直接覆盖 root->left/root->right 后丢失原始子树入口。
        TreeNode* left = invertTree(root->left);
        TreeNode* right = invertTree(root->right);

        // 当前节点只负责组合：原右子树的镜像挂到 left，原左子树的镜像挂到 right。
        root->left = right;
        root->right = left;

        // 此时以 root 为根的整棵子树已经完成镜像，返回给父节点继续组合。
        return root;
    }

    // 非递归替代实现：把“递归栈里等待处理的节点”显式换成 BFS queue。
    // 这不是另一套翻转规则：每个节点最终仍然只做一次左右子指针交换。
    TreeNode* invertTreeBFS(TreeNode* root) {
        if (!root) return nullptr;

        queue<TreeNode*> pending;
        pending.push(root);

        // 不变量：pending 中保存“已经发现，但还没有执行左右交换”的节点。
        while (!pending.empty()) {
            TreeNode* node = pending.front();
            pending.pop();

            // 当前节点的镜像操作完全局部：只交换它自己的两个孩子入口。
            swap(node->left, node->right);

            // 这里在 swap 之后把新的 left/right 入队是安全的：
            // 它们仍然就是当前节点原来的两个孩子，只是左右位置已经互换。
            // 每个非空孩子只由自己的父节点入队一次，因此最终每个节点恰好处理一次。
            if (node->left) pending.push(node->left);
            if (node->right) pending.push(node->right);
        }

        // 所有节点都完成一次局部交换后，整棵树自然成为原树的镜像。
        return root;
    }
};''',

    236: r'''// ---------- Solution ----------
class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        // 返回值不是普通“搜索结果”，而是当前子树向父节点上报的一份有效证据：
        // nullptr 表示这棵子树没有目标；p/q 表示当前命中目标；其它非空节点表示子树内已形成汇合点。
        // 当前节点若就是 p/q，可直接上报自己；若另一目标在它的后代中，按定义它本身就应是 LCA。
        if (!root || root == p || root == q) return root;

        // 后序收集左右子树证据；每棵子树内部会先完成自己的判断再向当前节点汇报。
        TreeNode* left = lowestCommonAncestor(root->left, p, q);
        TreeNode* right = lowestCommonAncestor(root->right, p, q);

        // 左右同时非空，说明两份目标证据第一次在当前节点的两侧汇合；
        // 更低的单个子树不可能同时包含两边证据，因此当前 root 就是最低公共祖先。
        if (left && right) return root;

        // 只有一边有证据时，当前节点还不是汇合点，原样把唯一证据继续向父节点传递；
        // 两边都空时这个表达式也自然返回 nullptr。
        return left ? left : right;
    }
};''',

    543: r'''// ---------- Solution ----------
class Solution {
    // 全局记录“已经在某个节点闭合完成”的最佳双臂路径；按边数计。
    int diameter = 0;

    // height(node) 的返回契约只允许一条可继续向父节点延伸的单臂高度。
    // 这里高度按节点数计：空子树为 0，叶子节点会返回 1。
    int height(TreeNode* node) {
        if (!node) return 0;

        // 后序先拿到左右子树已经计算好的单臂高度。
        const int leftHeight = height(node->left);
        const int rightHeight = height(node->right);

        // 当前节点可以把左右两臂同时闭合成一条完整路径。
        // 因 leftHeight/rightHeight 是从孩子向下的节点数，站在当前 node 看，
        // 它们恰好等于向左/向右可走的边数，所以候选直径直接是两者之和，不再 +1。
        diameter = max(diameter, leftHeight + rightHeight);

        // 但返回父节点时路径不能在当前 node 分叉，只能选择更长的一侧继续向上。
        return 1 + max(leftHeight, rightHeight);
    }

public:
    int diameterOfBinaryTree(TreeNode* root) {
        // 成员状态每次公开调用前重置，避免复用同一个 Solution 对象时保留旧答案。
        diameter = 0;
        height(root);
        return diameter;
    }
};''',

    98: r'''// ---------- Solution ----------
class Solution {
    // validate(node, lower, upper) 的契约：当前 node 必须落在所有祖先共同留下的严格开区间中，
    // 且整棵子树也要继续满足由这个上下文递推出来的 BST 约束。
    bool validate(TreeNode* node, long long lower, long long upper) {
        // 空子树没有任何节点违反约束，因此天然合法。
        if (!node) return true;

        // BST 使用严格不等号；等于任一边界也非法，所以重复值会被拒绝。
        // 这里检查的是祖先累计出的全局区间，而不是只和直接父节点比较。
        if (node->val <= lower || node->val >= upper) return false;

        // 左子树继承旧 lower，并把当前值收紧为新的严格 upper；
        // 右子树继承旧 upper，并把当前值收紧为新的严格 lower。
        return validate(node->left, lower, node->val)
            && validate(node->right, node->val, upper);
    }

public:
    bool isValidBST(TreeNode* root) {
        // 节点值本身允许等于 INT_MIN/INT_MAX，不能用 int 极值当开区间哨兵；
        // 用 long long 的更宽边界才能把所有合法 int 值完整包含进去。
        return validate(root, LLONG_MIN, LLONG_MAX);
    }
};''',
}
