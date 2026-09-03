// ============================================================================
// LC-160：相交链表
// 难度：简单
// 优先级：P1（进阶）
// 学习进度：第 2 周 / 第 8 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给你两个单链表的头节点 headA 和 headB，请你找出并返回两个单链表相交的起始节点。
// 如果两个链表不存在相交节点，返回 null。
// 图示两个链表在节点 c1 开始相交：题目数据保证整个链式结构中不存在环。
// 注意，函数返回结果后，链表必须保持其原始结构。
// 自定义评测：评测系统的输入如下（你设计的程序不适用此输入）：intersectVal - 相交的起始节点的值。
// 如果不存在相交节点，这一值为 0 listA - 第一个链表 listB - 第二个链表 skipA - 在 listA 中（从头节点开始）跳到交叉节点的节点数 skipB - 在 listB
// 中（从头节点开始）跳到交叉节点的节点数评测系统将根据这些输入创建链式数据结构，并将两个头节点 headA 和 headB 传递给你的程序。
// 如果程序能够正确返回相交节点，那么你的解决方案将被视作正确答案。
//
// 约束与要求：
//   - listA 中节点数目为 m
//   - listB 中节点数目为 n
//   - 1 <= m, n <= 3 * 10^4
//   - 1 <= Node.val <= 10^5
//   - 0 <= skipA <= m
//   - 0 <= skipB <= n
//   - 如果 listA 和 listB 没有交点，intersectVal 为 0
//   - 如果 listA 和 listB 有交点，intersectVal == listA[skipA] == listB[skipB]
//
// 复杂度目标：O(n+m) 时间，O(1) 空间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜双指针切换头部：对齐路径长度
// - 核心要点：
//   1. 思路起点：pA 走完 A 后转到 B，pB 走完 B 后转到 A；两者都走 a+b 步，长度差被第二段自动抵消。
//   2. 执行逻辑：1. 两个指针分别从 headA/headB 出发；2. 到 null 时切换到另一条链表头；3. 首次相同节点即交点；无交点时共同为 null。
//   3. 为什么这样做：若有公共尾长 c，两指针在走完各自独有段与对方独有段后同时到达公共头；若无交点，则同时走到 null。
// - 边界与易错点：比较的是节点地址不是值；不得修改链表；循环条件 pA!=pB 已覆盖无交点情形。
// - 举一反三：“走完自己的路再走对方的路”是消除长度差的技巧，也可用于比较两条路径的公共后缀。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：na（listA 长度）、nb（listB 长度）；本地适配器忽略 skipA 和 skipB。
//   第 2 行：listA 的 na 个节点值，末尾包含共享后缀。
//   第 3 行：listB 的 nb 个节点值，末尾使用相同的共享后缀。
//   第 4 行：interLen（共享节点数）。
//   构造时令最后 interLen 个节点共享；输出相交节点的值，不相交则输出 -1。
// test.in 的预期输出：8
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
    ListNode* getIntersectionNode(ListNode* headA, ListNode* headB) {
        ListNode* a = headA;
        ListNode* b = headB;
        while (a != b) {
            a = a ? a->next : headB;
            b = b ? b->next : headA;
        }
        return a;  // 交点，或两者共同到达的 nullptr
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int na, nb; if (!(cin >> na >> nb)) return 0;
    vector<int> a(na), b(nb);
    for (int i = 0; i < na; ++i) cin >> a[i];
    for (int i = 0; i < nb; ++i) cin >> b[i];
    int interLen; cin >> interLen;
    // Build lists; last interLen nodes are shared.
    ListNode* headA = buildList(a);
    ListNode* headB = buildList(b);
    if (interLen > 0) {
        // walk A to the start of the shared suffix and attach a shared tail from B
        int skipA = na - interLen, skipB = nb - interLen;
        // rebuild with shared tail
        vector<int> shared(a.begin() + skipA, a.end());
        ListNode dA, *tA = &dA;
        for (int i = 0; i < skipA; ++i) { tA->next = new ListNode(a[i]); tA = tA->next; }
        ListNode* sharedHead = buildList(shared);
        tA->next = sharedHead;
        ListNode dB, *tB = &dB;
        for (int i = 0; i < skipB; ++i) { tB->next = new ListNode(b[i]); tB = tB->next; }
        tB->next = sharedHead;
        headA = dA.next; headB = dB.next;
    }
    ListNode* ans = sol.getIntersectionNode(headA, headB);
    cout << (ans ? ans->val : -1) << "\n";
    return 0;
}

