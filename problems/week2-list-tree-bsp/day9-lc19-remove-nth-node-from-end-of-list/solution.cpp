// ============================================================================
// LC-19：删除链表的倒数第 N 个结点
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 2 周 / 第 9 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给你一个链表，删除链表的倒数第 n 个结点，并且返回链表的头结点。
//
// 约束与要求：
//   - 链表中结点的数目为 sz
//   - 1 <= sz <= 30
//   - 0 <= Node.val <= 100
//   - 1 <= n <= sz
//
// 复杂度目标：O(n) 时间，O(1) 空间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜哑节点 + 固定间距快慢指针
// - 核心要点：
//   1. 思路起点：让 fast 比 slow 领先 n 个节点；fast 到末尾时，slow 正好位于倒数第 n 个节点的前驱。
//   2. 执行逻辑：1. dummy 指向 head，两个指针从 dummy 出发；2. fast 先走 n+1 步；3. 随后同步前进到 fast 为空，再删除 slow->next。
//   3. 为什么这样做：同步阶段始终保持 n+1 条边的间距，因此 fast 越过尾部时 slow 位于目标前一节点；哑节点使删除原头也遵循同一规则。
// - 边界与易错点：领先 n 还是 n+1 取决于指针起点，本实现从 dummy 出发走 n+1；删除后不要再访问被删节点。
// - 举一反三：固定窗口间距可找倒数第 k 个节点、中点以及流式序列中的延迟位置。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n (长度) k (要删除的倒数第 k 个节点)。
//   第 2 行：n 个节点值。
//   输出：操作后的链表节点值，以空格分隔。
// test.in 的预期输出：1 2 3 5
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
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        ListNode dummy(0, head);
        ListNode* fast = &dummy;
        ListNode* slow = &dummy;
        for (int i = 0; i <= n; ++i) fast = fast->next;
        while (fast) {
            fast = fast->next;
            slow = slow->next;
        }
        ListNode* removed = slow->next;
        slow->next = removed->next;
        delete removed;
        return dummy.next;
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n, k; if (!(cin >> n >> k)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    printList(sol.removeNthFromEnd(buildList(a), k));
    return 0;
}

