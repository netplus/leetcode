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

    143: r'''// ---------- Solution ----------
class Solution {
public:
    void reorderList(ListNode* head) {
        if (!head || !head->next) return;

        // fast 从 head->next 出发，使切分结果满足：偶数长度两半等长，奇数长度前半段多一个节点。
        // 循环结束时 slow 正好停在前半段最后一个节点，也就是待断链的位置。
        ListNode* slow = head;
        ListNode* fast = head->next;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }

        // second 保存后半段入口后必须立即断开 first/second 两段；
        // 否则后续反转并交织时旧的 slow->next 仍存在，可能让节点形成环。
        ListNode* second = slow->next;
        slow->next = nullptr;

        // 完全复用 LC-206：把原后半段从 ...->Ln-1->Ln 改成 Ln->Ln-1->...，
        // 这样“从尾向前读取”被变成可以沿 next 正向读取。
        ListNode* reversed = nullptr;
        while (second) {
            ListNode* next = second->next;
            second->next = reversed;
            reversed = second;
            second = next;
        }

        // 此时 first 依次给出 L0,L1,...；second 依次给出 Ln,Ln-1,...。
        // 每轮从两条正向链各取一个节点，拉链式生成目标顺序。
        ListNode* first = head;
        second = reversed;
        while (second) {
            // 改 next 前先保住两条链各自尚未处理的入口，否则交织后无法继续原链。
            ListNode* nextFirst = first->next;
            ListNode* nextSecond = second->next;

            first->next = second;
            second->next = nextFirst;

            first = nextFirst;
            second = nextSecond;
        }

        // 后半段长度不会超过前半段；second 用尽时，若有奇数长度中点，它已自然留在结果末尾。
    }
};''',
}
