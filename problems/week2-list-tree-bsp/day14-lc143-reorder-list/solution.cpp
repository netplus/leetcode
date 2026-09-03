// ============================================================================
// LC-143：重排链表
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 2 周 / 第 14 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给定一个单链表 L 的头节点 head，单链表 L 表示为：L_0 → L_1 → … → L_n - 1 → L_n 请将其重新排列后变为：L_0 → L_n → L_1 → L_n - 1 → L_2
// → L_n - 2 → … 不能只是单纯的改变节点内部的值，而是需要实际的进行节点交换。
//
// 约束与要求：
//   - 链表的长度范围为 [1, 5 * 10^4]
//   - 1 <= node.val <= 1000
//
// 复杂度目标：O(n) 时间，O(1) 空间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜中点 + 反转后半段 + 交替合并
// - 核心要点：
//   1. 思路起点：目标顺序交替取原链表头、尾、次头、次尾；先把链表一分为二并反转后半段，尾部顺序就变成可顺向读取。
//   2. 执行逻辑：1. 快慢指针找前半段尾并断链；2. 原地反转后半段；3. 从两段轮流摘一个节点交织。
//   3. 为什么这样做：第一段保持 L0,L1...，反转第二段得到 Ln,Ln-1...；交替合并恰好产生题目顺序，且每个原节点使用一次。
// - 边界与易错点：必须先断开两段以免成环；奇数长度让前半段多一个节点；函数原地修改且无返回值。
// - 举一反三：拆分、局部变换、再归并是链表题常见三段式，例如回文判断、奇偶链表和拉链合并。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n。
//   第 2 行：n 个节点值。
//   输出：重排后的链表节点值，以空格分隔。
// test.in 的预期输出：1 4 2 3
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
    void reorderList(ListNode* head) {
        if (!head || !head->next) return;
        ListNode* slow = head;
        ListNode* fast = head->next;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }

        ListNode* second = slow->next;
        slow->next = nullptr;  // 先断链，避免交织时产生环
        ListNode* reversed = nullptr;
        while (second) {
            ListNode* next = second->next;
            second->next = reversed;
            reversed = second;
            second = next;
        }

        ListNode* first = head;
        second = reversed;
        while (second) {
            ListNode* nextFirst = first->next;
            ListNode* nextSecond = second->next;
            first->next = second;
            second->next = nextFirst;
            first = nextFirst;
            second = nextSecond;
        }
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
    sol.reorderList(head);
    printList(head);
    return 0;
}

