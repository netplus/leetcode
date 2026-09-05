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

    92: r'''// ---------- Solution ----------
class Solution {
public:
    ListNode* reverseBetween(ListNode* head, int left, int right) {
        // dummy 让 left==1 时仍然存在统一的“区间前驱”；
        // 返回 dummy.next 也会自动得到可能发生变化的新头节点。
        ListNode dummy(0, head);
        ListNode* before = &dummy;

        // before 最终固定在第 left 个节点之前，之后整个反转过程都不再移动它。
        for (int pos = 1; pos < left; ++pos) before = before->next;

        // first 是原区间的第一个节点。
        // 头插过程中它的位置不断向右退，但指针本身不动，并始终充当“已反转区间的尾”。
        ListNode* first = before->next;

        // 区间长度为 right-left+1；第一个节点 first 不需要搬，
        // 只需把它后面的 right-left 个节点依次提到 before 后面。
        for (int i = 0; i < right - left; ++i) {
            ListNode* moved = first->next;

            // 必须先让 first 跳过 moved，保持“反转段尾 -> 未处理后缀”的连接。
            // 如果先改 moved->next 而没有摘干净 moved，原链关系可能形成环或丢失剩余入口。
            first->next = moved->next;

            // moved 插到当前反转段最前面：先让它指向旧头，再让 before 改指 moved。
            // before 始终不动，因此每次都是同一个插入口。
            moved->next = before->next;
            before->next = moved;
        }

        // 区间外前缀由 before 保持连接，区间外后缀由 first->next 保持连接；
        // dummy.next 因此始终是整个结果链表的统一入口。
        return dummy.next;
    }
};''',

    21: r'''// ---------- Solution ----------
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
        // dummy 只提供稳定的结果链起点；tail 永远指向“已经合并完成部分”的最后一个节点。
        // 因而首节点和后续节点可以使用完全相同的 tail->next 连接逻辑。
        ListNode dummy;
        ListNode* tail = &dummy;

        while (list1 && list2) {
            // 两条剩余链各自有序，所以它们的表头分别是各自剩余部分的最小值；
            // 下一结果节点只需在这两个表头之间二选一，不需要查看更后面的节点。
            //
            // chosen 的类型是 ListNode*&：它不是普通指针副本，而是对 list1 或 list2
            // 这个“头指针变量”的引用。所以下面的 chosen=chosen->next 会直接推进被选中的来源指针。
            ListNode*& chosen = (list1->val <= list2->val) ? list1 : list2;

            // 先把当前全局最小头节点接到结果尾部；节点本身被复用，没有复制 val 或 new 新节点。
            tail->next = chosen;

            // 来源链跳过刚被消费的节点；因为 chosen 是引用，这一步等价于推进 list1 或 list2。
            chosen = chosen->next;

            // tail 再移动到刚接入的节点，保持它始终指向结果链最后一个节点。
            tail = tail->next;
        }

        // 一条链耗尽后，另一条剩余后缀已经整体有序，且其头就是所有未处理节点中的唯一候选；
        // 无需再逐节点比较，可以把整段一次接到 tail 后。
        tail->next = list1 ? list1 : list2;

        return dummy.next;
    }
};''',
}
