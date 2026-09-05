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
}
