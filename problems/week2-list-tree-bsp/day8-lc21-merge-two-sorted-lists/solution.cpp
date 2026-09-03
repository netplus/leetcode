// ============================================================================
// LC-21：合并两个有序链表
// 难度：简单
// 优先级：P0（必做）
// 学习进度：第 2 周 / 第 8 天
// ----------------------------------------------------------------------------
// 题目描述：
// 将两个升序链表合并为一个新的升序链表并返回。
// 新链表是通过拼接给定的两个链表的所有节点组成的。
//
// 约束与要求：
//   - 两个链表的节点数目范围是 [0, 50]
//   - -100 <= Node.val <= 100
//   - l1 和 l2 均按非递减顺序排列
//
// 复杂度目标：O(n+m) 时间，O(1) 空间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜哑节点归并两个有序链表
// - 核心要点：
//   1. 思路起点：tail 指向结果链表末尾，每次从两个链表当前头中选择较小者接入，等价于归并排序的 merge 步骤。
//   2. 执行逻辑：1. 建立 dummy/tail；2. 两表均非空时接入较小节点并推进来源指针；3. 把剩余非空后缀一次接到 tail。
//   3. 为什么这样做：两表头是各自剩余部分的最小值，选两者较小者就是全局下一个节点；归纳可知结果始终有序且不遗漏。
// - 边界与易错点：应复用原节点而非只复制值；相等时选任一侧都正确；哑节点避免单独处理结果头。
// - 举一反三：多个有序链表、外部排序与归并排序都复用相同的“取当前最小头”不变量。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n (list1 的长度)。
//   第 2 行：n 个 list1 节点值。
//   第 3 行：m (list2 的长度)。
//   第 4 行：m 个 list2 节点值。
//   输出：合并后的链表节点值，以空格分隔。
// test.in 的预期输出：1 1 2 3 4 4
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
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
        ListNode dummy;
        ListNode* tail = &dummy;
        while (list1 && list2) {
            ListNode*& chosen = (list1->val <= list2->val) ? list1 : list2;
            tail->next = chosen;
            chosen = chosen->next;
            tail = tail->next;
        }
        tail->next = list1 ? list1 : list2;
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
    int m; cin >> m;
    vector<int> b(m);
    for (int i = 0; i < m; ++i) cin >> b[i];
    printList(sol.mergeTwoLists(buildList(a), buildList(b)));
    return 0;
}

