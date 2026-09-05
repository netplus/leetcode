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

    142: r'''// ---------- Solution ----------
class Solution {
public:
    ListNode* detectCycle(ListNode* head) {
        ListNode* slow = head;
        ListNode* fast = head;

        // slow/fast 初始都等于 head，因此不能在“尚未走一步”时把 slow==fast 当作环内相遇。
        // do...while 强制先推进，再检查是否真正相遇。
        do {
            // fast 下一轮要走两步；只要 fast 或 fast->next 为空，就说明存在真实链尾、没有环。
            if (!fast || !fast->next) return nullptr;
            slow = slow->next;
            fast = fast->next->next;
        } while (slow != fast);

        // 第一阶段相遇点 M 已经携带路程关系：
        // 若 head->entry 距离为 a，entry->M 为 b，环长为 L，第一次相遇满足 a+b=kL，
        // 因而 a=(k-1)L+(L-b)。也就是“从 head 到入口”与“从 M 到入口再绕整圈”同长。
        slow = head;

        // 第二阶段必须取消 1:2 速度差，让两者都每轮走 1 步。
        // slow 从 head 走 a 步到入口；fast 从 M 走同样 a 步，也恰好落在入口。
        // 在入口之前 slow 位于非环前缀而 fast 始终在环内，所以不会提前相遇。
        while (slow != fast) {
            slow = slow->next;
            fast = fast->next;
        }

        // 两者第一次重新会合的节点就是环入口。
        return slow;
    }
};''',
}
