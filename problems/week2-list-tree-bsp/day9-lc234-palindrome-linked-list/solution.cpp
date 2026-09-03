// ============================================================================
// LC-234：回文链表
// 难度：简单
// 优先级：P1（进阶）
// 学习进度：第 2 周 / 第 9 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给你一个单链表的头节点 head，请你判断该链表是否为回文链表。
// 如果是，返回 true；否则，返回 false。
//
// 约束与要求：
//   - 链表中节点数目在范围 [1, 10^5] 内
//   - 0 <= Node.val <= 9
//
// 复杂度目标：O(n) 时间，O(1) 空间（进阶要求）。
//
// ----------------------------------------------------------------------------
// 解法精讲｜找中点 + 反转后半段 + 双向比较
// - 核心要点：
//   1. 思路起点：回文链表的前半段应与反转后的后半段逐项相等；用快慢指针找到后半段起点即可 O(1) 额外空间完成。
//   2. 执行逻辑：1. fast 两步、slow 一步找到中点；2. 反转 slow 开始的后半段；3. 比较两段，并再次反转以恢复原链表。
//   3. 为什么这样做：后半段长度不大于前半段；反转后从两端向中心的对应节点变成同向排列，逐项相等当且仅当原序列回文。
// - 边界与易错点：奇数长度时 slow 指向中间节点，多比较一次不影响；为保持调用者可观察结构，比较后恢复链表。
// - 举一反三：“破坏性变换—使用—恢复”能在常量空间解决回文、重排等问题，但工程代码要明确恢复契约。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n。
//   第 2 行：n 个节点值。
//   输出：是回文链表时输出 1，否则输出 0。
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
    static ListNode* reverse(ListNode* head) {
        ListNode* previous = nullptr;
        while (head) {
            ListNode* next = head->next;
            head->next = previous;
            previous = head;
            head = next;
        }
        return previous;
    }

public:
    bool isPalindrome(ListNode* head) {
        if (!head || !head->next) return true;
        ListNode* slow = head;
        ListNode* fast = head;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }

        ListNode* reversed = reverse(slow);
        ListNode* right = reversed;
        ListNode* left = head;
        bool equal = true;
        while (right) {
            if (left->val != right->val) equal = false;
            left = left->next;
            right = right->next;
        }
        reverse(reversed);  // 恢复原链表，避免隐藏副作用
        return equal;
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
    cout << (sol.isPalindrome(buildList(a)) ? 1 : 0) << "\n";
    return 0;
}

