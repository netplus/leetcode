"""Reviewed C++ key-code comments for Week 2 / Day 8.

Entries are added only after an independent per-problem review.
"""


CODE_COMMENTS_WEEK2_DAY8 = {
    206: r'''// ---------- Solution ----------
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        // 循环开始时把链表划成两段：
        // previous = 已经完成反转的前缀头；current = 仍保持原方向的未处理后缀头。
        // 初始没有已处理节点，所以 previous=nullptr；整条原链都从 current=head 开始。
        ListNode* previous = nullptr;
        ListNode* current = head;

        while (current) {
            // 必须先保存原来的后继。下一行会改写 current->next；
            // 若不先记住它，未处理后缀的入口会从当前指针图中永久丢失。
            ListNode* next = current->next;

            // 把 current 从未处理后缀摘下，并让它指回已反转前缀的头部。
            // 这一步之后，current 已正式成为反转前缀的新头。
            current->next = previous;

            // 两个边界指针依次前进：previous 接管刚处理完的 current，
            // current 再沿刚保存的 next 进入原链剩余部分。
            previous = current;
            current = next;
        }

        // current==nullptr 说明未处理后缀已经为空；
        // previous 此时覆盖原链全部节点，并指向新的链表头。
        return previous;
    }
};''',
}
