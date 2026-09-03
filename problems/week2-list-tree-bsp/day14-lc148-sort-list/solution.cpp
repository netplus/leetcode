// ============================================================================
// LC-148：排序链表
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 2 周 / 第 14 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给你链表的头结点 head，请将其按升序排列并返回排序后的链表。
//
// 约束与要求：
//   - 链表中节点的数目在范围 [0, 5 * 10^4] 内
//   - -10^5 <= Node.val <= 10^5
//
// 复杂度目标：O(n log n) 时间，O(1) 空间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜自底向上链表归并排序
// - 核心要点：
//   1. 思路起点：归并排序适合链表，因为拆分和合并只改指针；自底向上依次合并长度 1、2、4... 的相邻段，可把额外空间降到 O(1)。
//   2. 执行逻辑：1. 先统计链表长度；2. 每轮按 runSize 切出 left/right 两段；3. 归并后接回已排序前缀，并将 runSize 翻倍。
//   3. 为什么这样做：第 k 轮开始前每个长度 runSize 的段已排序；两两归并后得到长度 2*runSize 的有序段，最终段长覆盖 n 即全局有序。
// - 边界与易错点：split 必须断开链；merge 要返回合并段尾以便继续拼接；链表归并不需要随机访问。
// - 举一反三：外部排序、大文件归并和迭代归并排序都使用“固定段长翻倍”的阶段不变量。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n。
//   第 2 行：n 个节点值。
//   输出：排序后的链表节点值，以空格分隔。
// test.in 的预期输出：1 2 3 4
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
    static ListNode* split(ListNode* head, int size) {
        while (--size > 0 && head) head = head->next;
        if (!head) return nullptr;
        ListNode* second = head->next;
        head->next = nullptr;
        return second;
    }

    static pair<ListNode*, ListNode*> mergeRuns(ListNode* a, ListNode* b) {
        ListNode dummy;
        ListNode* tail = &dummy;
        while (a && b) {
            ListNode*& chosen = (a->val <= b->val) ? a : b;
            tail->next = chosen;
            chosen = chosen->next;
            tail = tail->next;
        }
        tail->next = a ? a : b;
        while (tail->next) tail = tail->next;
        return {dummy.next, tail};
    }

public:
    ListNode* sortList(ListNode* head) {
        int length = 0;
        for (ListNode* node = head; node; node = node->next) ++length;
        ListNode dummy(0, head);

        for (int runSize = 1; runSize < length; runSize *= 2) {
            ListNode* current = dummy.next;
            ListNode* sortedTail = &dummy;
            while (current) {
                ListNode* left = current;
                ListNode* right = split(left, runSize);
                current = split(right, runSize);
                auto [mergedHead, mergedTail] = mergeRuns(left, right);
                sortedTail->next = mergedHead;
                sortedTail = mergedTail;
            }
        }
        return dummy.next;
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
    printList(sol.sortList(buildList(a)));
    return 0;
}

