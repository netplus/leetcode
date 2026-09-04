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
// 题目示例：
// 示例 1：
//   Input: intersectVal = 8, listA = [4,1,8,4,5], listB = [5,6,1,8,4,5], skipA = 2, skipB = 3
//   Output: Intersected at '8'
//   Explanation: The intersected node's value is 8 (note that this must not be 0 if the two lists intersect).
//   From the head of A, it reads as [4,1,8,4,5]. From the head of B, it reads as [5,6,1,8,4,5]. There are 2 nodes before the intersected node in A; There are 3 nodes before the intersected node in B.
//   - Note that the intersected node's value is not 1 because the nodes with value 1 in A and B (2^nd node in A and 3^rd node in B) are different node references. In other words, they point to two different locations in memory, while the nodes with value 8 in A and B (3^rd node in A and 4^th node in B) point to the same location in memory.
//
// 示例 2：
//   Input: intersectVal = 2, listA = [1,9,1,2,4], listB = [3,2,4], skipA = 3, skipB = 1
//   Output: Intersected at '2'
//   Explanation: The intersected node's value is 2 (note that this must not be 0 if the two lists intersect).
//   From the head of A, it reads as [1,9,1,2,4]. From the head of B, it reads as [3,2,4]. There are 3 nodes before the intersected node in A; There are 1 node before the intersected node in B.
//
// 示例 3：
//   Input: intersectVal = 0, listA = [2,6,4], listB = [1,5], skipA = 3, skipB = 2
//   Output: No intersection
//   Explanation: From the head of A, it reads as [2,6,4]. From the head of B, it reads as [1,5]. Since the two lists do not intersect, intersectVal must be 0, while skipA and skipB can be arbitrary values.
//   Explanation: The two lists do not intersect, so return null.
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
// 解法精讲｜双指针拼接两条虚拟路径以消除长度差
//
// 0. 优化是怎么来的
//   最自然的线性解法之一，是先把链表 A 的所有节点地址放进 unordered_set，再扫描 B，遇到第一个已存在地址就是交点；时间 O(m+n)，但需要 O(m) 额外空间。
//
//   若要 O(1) 空间，真正需要消除的是两条独有前缀的长度差。让 pA 依次走 A 再走 B，让 pB 依次走 B 再走 A，两者都会走完全相同的总路程 m+n；只是把各自的“长出来的那一段”放到了不同阶段。
//
//   因此无需存 visited，也无需先显式计算两条链长度，换头本身就在重新排列路径以完成对齐。
//
//   优化类型：辅助集合/显式长度对齐 -> 两条等长虚拟路径自动抵消长度差。
//
// 1. 图像直觉
//   设两链在 C 相交：
//
//   A: a1 -> a2 ---------> c1 -> c2
//   B: b1 -> b2 -> b3 --> c1 -> c2
//
//   A 独有段短，B 独有段长，直接同步走当然对不齐。
//   让指针走两条“虚拟路径”：
//
//   pA: A独有 -> C -> B独有 -> C
//   pB: B独有 -> C -> A独有 -> C
//
//   两条路径总长度完全相同，因此前面的长度差在换头后被自动补偿；若存在公共尾，它们会在同一个 c1 对齐。
//
// 2. 一句话核心
//   让两个指针都依次走完 A+B，只是顺序相反，用相同总路程自动抵消两条链的长度差。
//
// 3. 公式 / 不变量
//   设 A 独有长度为 a，B 独有长度为 b，公共尾长度为 c。
//
//   pA 到公共入口前走：a + c + b
//   pB 到公共入口前走：b + c + a
//
//   两者相等。
//   若无公共尾，则两者最终都在走完 m+n 个节点后到 nullptr。
//
// 4. 执行步骤
//   1. a=headA、b=headB，同时向后走。
//   2. a 到 nullptr 后改从 headB 开始；b 到 nullptr 后改从 headA 开始。
//   3. 持续到 a==b：若有交点，这个地址就是第一个公共节点；若无交点，两者共同为 nullptr。
//   记忆：我走完你的路，你走完我的路；总路程相同后自然对齐。
//
// 5. 为什么不会漏 / 不会重
//   相交是“节点身份相同”，所以一旦两个指针进入同一个公共节点，之后路径完全一致。换头让两者总共经历相同的 A+B 长度，消除了各自独有前缀的长度差，因此若有公共入口必会同时到达；若没有公共节点，则唯一能相等的位置就是共同的 nullptr。
//   每个指针最多遍历两条链各一次，不需要额外 visited 集合。
//
// 6. 边界与易错点
//   必须比较节点指针地址，值相等不代表相交；不要修改 next 来做标记。写成 while (a != b) 已自然覆盖“无交点”，无需额外特判。
//
// 7. 举一反三
//   它和 Day 2 的双指针不同：这里不是利用单调性淘汰候选，而是通过重新安排路径让两个状态在同一坐标系中对齐。这个“补齐路径长度差”的思想也可用于比较两个路径的公共后缀。
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

