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
}
