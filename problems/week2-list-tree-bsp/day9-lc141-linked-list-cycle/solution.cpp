// ============================================================================
// LC-141：环形链表
// 难度：简单
// 优先级：P0（必做）
// 学习进度：第 2 周 / 第 9 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给你一个链表的头节点 head，判断链表中是否有环。
// 如果链表中有某个节点，可以通过连续跟踪 next 指针再次到达，则链表中存在环。
// 为了表示给定链表中的环，评测系统内部使用整数 pos 来表示链表尾连接到链表中的位置（索引从 0 开始）。
// 注意：pos 不作为参数进行传递。
// 仅仅是为了标识链表的实际情况。
// 如果链表中存在环，则返回 true。
// 否则，返回 false。
//
// 约束与要求：
//   - 链表中节点的数目范围是 [0, 10^4]
//   - -10^5 <= Node.val <= 10^5
//   - pos 为 -1 或者链表中的一个有效索引。
//
// 复杂度目标：O(n) 时间，O(1) 空间（fast/slow）。
//
// ----------------------------------------------------------------------------
// 解法精讲｜Floyd 快慢指针判环
// - 核心要点：
//   1. 思路起点：slow 每次走一步、fast 每次走两步；无环时 fast 到达空，有环时相对速度为 1，最终必在环内追上。
//   2. 执行逻辑：1. slow/fast 都从 head 出发；2. 在 fast 与 fast->next 存在时推进；3. 若两指针相遇返回 true，否则返回 false。
//   3. 为什么这样做：进入环后可把位置看作模环长，fast 每轮相对 slow 前进一格，有限轮内必同余；无环链表则指针严格向尾部前进。
// - 边界与易错点：循环条件必须先检查 fast 再解引用 fast->next；节点值无关；题面中的 pos 只描述测试数据，不是函数参数。
// - 举一反三：速度差在周期结构中可用于找环入口、环长和重复状态，是 O(1) 空间替代 visited 集合的典型方式。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n pos。
//   第 2 行：n 个节点值。
//   pos 表示链表尾连回的下标，-1 表示无环。
//   输出：链表有环时输出 1，否则输出 0。
// test.in 的预期输出：1
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
    bool hasCycle(ListNode* head) {
        ListNode* slow = head;
        ListNode* fast = head;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
            if (slow == fast) return true;
        }
        return false;
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
    cout << (sol.hasCycle(head) ? 1 : 0) << "\n";
    return 0;
}

