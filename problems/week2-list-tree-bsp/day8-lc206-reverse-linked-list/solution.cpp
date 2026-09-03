// ============================================================================
// LC-206：反转链表
// 难度：简单
// 优先级：P0（必做）
// 学习进度：第 2 周 / 第 8 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给你单链表的头节点 head，请你反转链表，并返回反转后的链表。
//
// 约束与要求：
//   - 链表中节点的数目范围是 [0, 5000]
//   - -5000 <= Node.val <= 5000
//
// 复杂度目标：O(n) 时间，O(1) 空间（迭代）。
//
// ----------------------------------------------------------------------------
// 解法精讲｜链表原地反转：三指针
// - 核心要点：
//   1. 思路起点：遍历时将当前节点的 next 指向已反转前缀；prev 是新链表头，current 是尚未处理部分的头。
//   2. 执行逻辑：1. 先保存 current->next；2. 把 current->next 改为 prev；3. prev/current 同步前进。
//   3. 为什么这样做：每轮后 prev 链包含原链表前缀且方向完全反转，current 指向未处理后缀；循环结束时前缀覆盖全部节点。
// - 边界与易错点：改指针前必须保存 next，否则会丢失后缀；空链表和单节点无需特判；返回 prev 而非原 head。
// - 举一反三：链表反转是区间反转、回文链表、重排链表和 K 组翻转的基础操作。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n。
//   第 2 行：n 个以空格分隔的节点值。
//   输出：反转后的链表节点值，以空格分隔。
// test.in 的预期输出：5 4 3 2 1
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
    ListNode* reverseList(ListNode* head) {
        ListNode* previous = nullptr;
        ListNode* current = head;
        while (current) {
            ListNode* next = current->next;  // 改链前保存尚未处理的后缀
            current->next = previous;
            previous = current;
            current = next;
        }
        return previous;
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    ListNode* head = buildList(a);
    printList(sol.reverseList(head));
    return 0;
}

