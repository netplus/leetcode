// ============================================================================
// LC-142：环形链表 II
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 2 周 / 第 9 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给定一个链表的头节点 head，返回链表开始入环的第一个节点。
// 如果链表无环，则返回 null。
// 如果链表中有某个节点，可以通过连续跟踪 next 指针再次到达，则链表中存在环。
// 为了表示给定链表中的环，评测系统内部使用整数 pos 来表示链表尾连接到链表中的位置（索引从 0 开始）。
// 如果 pos 是 -1，则在该链表中没有环。
// 注意：pos 不作为参数进行传递，仅仅是为了标识链表的实际情况。
// 不允许修改链表。
//
// 约束与要求：
//   - 链表中节点的数目范围在范围 [0, 10^4] 内
//   - -10^5 <= Node.val <= 10^5
//   - pos 的值为 -1 或者链表中的一个有效索引
//
// 复杂度目标：O(n) 时间，O(1) 空间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜Floyd 两阶段：判环后定位入口
// - 核心要点：
//   1. 思路起点：快慢指针相遇后，将一个指针放回头部；两者同速前进时的下一次相遇点就是环入口。
//   2. 执行逻辑：1. 先以 1/2 倍速度寻找环内相遇点；2. 若 fast 到空则无环；3. slow 回到 head，与 fast 同速前进直到相等。
//   3. 为什么这样做：设头到入口 a、入口到相遇点 b、环剩余 c，首次相遇满足 2(a+b)=a+b+k(b+c)，可得 a 与 c 后若干整环等长，故同速相遇于入口。
// - 边界与易错点：第二阶段不能继续让 fast 走两步；不能修改节点；无环必须返回 nullptr。
// - 举一反三：Floyd 数学关系也适用于数组映射中的重复数、函数迭代周期与伪随机序列检测。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n pos。
//   第 2 行：n 个节点值。
//   输出：入环节点值；无环时输出 -1。
// test.in 的预期输出：2
// ============================================================================
#include <bits/stdc++.h>
using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

[[maybe_unused]] static ListNode* buildList(const vector<int>& a) {
    ListNode dummy; ListNode* tail = &dummy;
    for (int v : a) { tail->next = new ListNode(v); tail = tail->next; }
    return dummy.next;
}
[[maybe_unused]] static void printList(ListNode* h) {
    bool first = true;
    for (ListNode* c = h; c; c = c->next) { if (!first) cout << " "; cout << c->val; first = false; }
    cout << "\n";
}

// ---------- 题解实现 ----------
class Solution {
public:
    ListNode* detectCycle(ListNode* head) {
        ListNode* slow = head;
        ListNode* fast = head;
        do {
            if (!fast || !fast->next) return nullptr;
            slow = slow->next;
            fast = fast->next->next;
        } while (slow != fast);

        slow = head;
        while (slow != fast) {
            slow = slow->next;
            fast = fast->next;
        }
        return slow;
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n, pos; if (!(cin >> n >> pos)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    ListNode* head = buildList(a);
    if (pos >= 0) {
        ListNode* tail = head; while (tail->next) tail = tail->next;
        ListNode* p = head; for (int i = 0; i < pos; ++i) p = p->next;
        tail->next = p;
    }
    ListNode* ans = sol.detectCycle(head);
    cout << (ans ? ans->val : -1) << "\n";
    return 0;
}

