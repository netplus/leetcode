// ============================================================================
// LC-92：反转链表 II
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 2 周 / 第 8 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给你单链表的头指针 head 和两个整数 left 和 right，其中 left <= right。
// 请你反转从位置 left 到位置 right 的链表节点，返回反转后的链表。
//
// 约束与要求：
//   - 链表中节点数目为 n
//   - 1 <= n <= 500
//   - -500 <= Node.val <= 500
//   - 1 <= left <= right <= n
//
// 复杂度目标：O(n) 时间，O(1) 空间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜哑节点 + 头插法反转局部区间
// - 核心要点：
//   1. 思路起点：定位到 left 前一个节点 before；保持区间首节点 first 不动，每次把 first 后面的节点摘下并插到 before 后面。
//   2. 执行逻辑：1. 用 dummy 统一 left=1；2. 移动 before 到反转区间前；3. 执行 right-left 次摘取与头插。
//   3. 为什么这样做：第 k 次头插后，前 k+1 个区间节点已按逆序位于 before 后，first 始终是这段的尾；其余链表连接保持完整。
// - 边界与易错点：移动次数易出现 off-by-one；摘节点时要同时维护 first->next 与 moved->next；无需额外数组。
// - 举一反三：哑节点统一头部修改，头插法还适用于 K 组反转和将链表区间循环右移。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n left right。
//   第 2 行：n 个以空格分隔的节点值。
//   输出：操作后的链表节点值，以空格分隔。
// test.in 的预期输出：1 4 3 2 5
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
    ListNode* reverseBetween(ListNode* head, int left, int right) {
        ListNode dummy(0, head);
        ListNode* before = &dummy;
        for (int pos = 1; pos < left; ++pos) before = before->next;

        ListNode* first = before->next;
        for (int i = 0; i < right - left; ++i) {
            ListNode* moved = first->next;
            first->next = moved->next;   // 从区间剩余部分摘下 moved
            moved->next = before->next;
            before->next = moved;        // 插到已反转部分最前面
        }
        return dummy.next;
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n, left, right; if (!(cin >> n >> left >> right)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    ListNode* head = buildList(a);
    printList(sol.reverseBetween(head, left, right));
    return 0;
}

