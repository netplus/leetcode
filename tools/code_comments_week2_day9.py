"""Reviewed C++ key-code comments for Week 2 / Day 9.

Entries are added only after an independent per-problem review.
"""


CODE_COMMENTS_WEEK2_DAY9 = {
    141: r'''// ---------- Solution ----------
class Solution {
public:
    bool hasCycle(ListNode* head) {
        // slow/fast 从同一点出发；后续每轮 slow 走 1 步、fast 走 2 步。
        // 比较的是节点地址，而不是 val：值相同的不同节点并不代表形成了环。
        ListNode* slow = head;
        ListNode* fast = head;

        // fast 一轮需要访问 fast->next->next，所以必须先保证 fast 和 fast->next 都存在。
        // 若这个条件最终失败，说明链表存在真实尾部 nullptr，因此不可能有环。
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;

            // 一旦两者都进入长度为 L 的环，fast 相对 slow 每轮净前进 1 个节点；
            // 相对位置按 +1 mod L 演化，必然会到达 0，所以有环时最终一定在同一节点地址相遇。
            if (slow == fast) return true;
        }

        // fast 能走出链表只可能发生在无环结构中。
        return false;
    }
};''',
}
