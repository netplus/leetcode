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
//
// 1. 图像直觉
//   例如反转 [2,4]：
//
//   dummy -> 1 -> 2 -> 3 -> 4 -> 5
//            ^    ^    ^
//          before first moved
//
//   first=2 不动。每轮把 first 后面的节点摘出来，插到 before 后：
//
//   摘 3：dummy -> 1 -> 3 -> 2 -> 4 -> 5
//   摘 4：dummy -> 1 -> 4 -> 3 -> 2 -> 5
//
//   所以不是把整个区间先切下来再反转，而是在原链上连续做 right-left 次“摘下一个，插到最前”。
//
// 2. 一句话核心
//   固定区间前驱 before 和区间原首节点 first，不断把 first 后面的节点搬到 before 后面，局部区间就会逐步逆序。
//
// 3. 公式 / 不变量
//   第 k 次头插完成后：
//   before -> [已反转的 k+1 个节点] -> first -> [尚未移动的区间节点] -> suffix
//
//   其中 first 始终是当前反转区间的尾。
//   总共需要移动的节点数 = right - left。
//
// 4. 执行步骤
//   1. 用 dummy 指向 head，使 left=1 时也有统一的区间前驱 before。
//   2. 把 before 移到第 left 个节点之前，令 first=before->next。
//   3. 重复 right-left 次：moved=first->next，把 moved 从原位置摘下。
//   4. 先修复 first->next，再令 moved->next=before->next，最后 before->next=moved 完成头插。
//   记忆：before 固定，first 固定当尾；每次把 first 后一个节点提到最前。
//
// 5. 为什么不会漏 / 不会重
//   每轮只移动一个尚未反转的区间节点 moved，并把它放到当前已反转部分最前，因此已反转部分长度严格增加 1；first 后面的未处理区间长度严格减少 1。
//   移动 right-left 次后，区间内所有节点恰好各被移动或保留一次，区间外前缀和后缀始终通过 before/first 保持连接，所以既不会漏节点也不会破坏区间外结构。
//
// 6. 边界与易错点
//   这里最容易错的是连接顺序：必须先让 first->next 跳过 moved，否则 moved 被插到前面后会形成错误链；循环次数是 right-left，不是区间长度。dummy
//   的价值是把“反转从头开始”变成普通情况。
//
// 7. 举一反三
//   直接建立在 LC-206 的“改 next 前先保住后路”之上，只是 LC-206 让 previous 向前生长，LC-92 则固定 before，用连续头插完成局部重排。K
//   组翻转和链表局部旋转都会复用这种边界节点思想。
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

