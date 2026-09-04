// ============================================================================
// LC-142：环形链表 II
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 2 周 / 第 9 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给定一个链表的头节点 head，返回链表开始入环的第一个节点。
// 如果链表无环，则返回 null。
// 如果链表中有某个节点，可以通过连续跟踪 next 指针再次到达，则链表中存在环。
// 为了表示给定链表中的环，评测系统内部使用整数 pos 来表示链表尾连接到链表中的位置（索引从 0 开始）。
// 如果 pos 是 -1，则在该链表中没有环。
// 注意：pos 不作为参数进行传递，仅仅是为了标识链表的实际情况。
// 不允许修改链表。
//
// 约束与要求：
//   - 链表中节点的数目范围在范围 [0, 10^4] 内
//   - -10^5 <= Node.val <= 10^5
//   - pos 的值为 -1 或者链表中的一个有效索引
//
// 复杂度目标：O(n) 时间，O(1) 空间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜Floyd 两阶段：从环内相遇点反推出入口
//
// 0. 优化是怎么来的
//   最自然的入口定位方法，是沿链表扫描并把访问过的节点地址放进 unordered_set；第一次再次遇到的节点就是环入口。它清晰且 O(n) 时间，但需要 O(n) 额外空间。
//
//   要做到 O(1) 空间，就不能保存完整访问历史。LC-141 已经告诉我们：快慢指针的速度差可以在不记 visited 的情况下制造一个环内相遇点。关键是这个相遇点不仅证明“有环”，它还携带一个路程同余关系。
//
//   把第一次相遇时 fast 比 slow 多走的整圈写成等式，可推出：head 到入口的距离，等于相遇点到入口的距离再加若干整圈。因此一个指针回到 head、另一个留在相遇点并改为同速，二者会恰好在入口会合。
//
//   优化类型：显式保存访问历史 -> 用周期中的路程关系编码历史信息；空间从 O(n) 降为 O(1)。
//
// 1. 图像直觉
//   先沿用 LC-141 找到环内相遇点 M：
//
//   head -- a步 --> ENTRY -- b步 --> M
//                    ^               |
//                    |               |
//                    +---- c步 <-----+
//
//   环长 L = b + c。
//
//   第一阶段看起来只告诉我们“有环”，但相遇发生的路程其实已经藏着入口信息。
//   相遇后把 slow 放回 head，fast 留在 M，然后两者都改成每次走 1 步：
//
//   slow: head  -------- a步 --------> ENTRY
//   fast: M ---- c步 + 若干整圈 ------> ENTRY
//
//   两段路程恰好等长，所以它们下一次相遇的位置就是 ENTRY。
//
// 2. 一句话核心
//   第一次相遇负责制造一个路程等式；把一个指针重置到 head 后，两指针同速前进，就把这个等式直接走成环入口。
//
// 3. 公式 / 不变量
//   设：
//   a = head 到入口距离
//   b = 入口到第一次相遇点 M 的距离
//   c = M 沿环回到入口的距离
//   L = b + c
//
//   第一次相遇时 slow 走 a+b，fast 走 2(a+b)。
//   fast 比 slow 多走了整数圈 kL：
//
//   2(a+b) - (a+b) = kL
//   a + b = kL
//   a = kL - b
//     = (k-1)L + c
//
//   所以：
//   head 到入口的 a 步 = M 到入口的 c 步 + 若干完整环。
//
// 4. 执行步骤
//   1. 先执行 LC-141 的快慢指针阶段；若 fast 或 fast->next 为空，直接返回 nullptr。
//   2. 第一次 slow==fast 后，把 slow 重置为 head，fast 留在相遇点。
//   3. 第二阶段两者都只走一步，不再保持 1:2 速度。
//   4. 当 slow==fast 时返回该节点，它就是入环入口。
//   记忆：第一阶段用速度差找相遇，第二阶段取消速度差：head 和相遇点一起走，入口处会合。
//
// 5. 为什么不会漏 / 不会重
//   第一阶段由 LC-141 保证：有环一定能得到一个环内相遇点，无环一定提前返回 nullptr。
//   由 a=(k-1)L+c，从 head 出发走 a 步必到入口；从 M 出发走同样 a 步，则先走 c 到入口，再绕若干整圈，最终仍停在入口。
//   因此第二阶段同速前进时，两者最迟在入口相遇；在入口之前 slow 还处于非环前缀，而 fast 始终在环内，不可能提前指向同一节点，所以这个相遇点恰好是第一个入环节点。
//
// 6. 边界与易错点
//   最容易混淆的是第二阶段仍让 fast 走两步——那会破坏 a=(k-1)L+c 的同路程对应。不要试图通过修改 next 做标记，题目明确要求不修改链表。空链表、自环、入口就是 head 都由同一逻辑覆盖。
//
// 7. 举一反三
//   这是 LC-141 的直接升级：先用“相对速度”发现周期，再用“路程同余”定位周期入口。数组中把 nums[i] 看成 next 指针的重复数问题，也可以原样迁移这个模型。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n pos。
//   第 2 行：n 个节点值。
//   输出：入环节点值；无环时输出 -1。
// test.in 的预期输出：2
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
    ListNode* detectCycle(ListNode* head) {
        ListNode* slow = head;
        ListNode* fast = head;
        do {
            if (!fast || !fast->next) return nullptr;
            slow = slow->next;
            fast = fast->next->next;
        } while (slow != fast);

        slow = head;
        while (slow != fast) {
            slow = slow->next;
            fast = fast->next;
        }
        return slow;
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n, pos; if (!(cin >> n >> pos)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    ListNode* head = buildList(a);
    if (pos >= 0) {
        ListNode* tail = head; while (tail->next) tail = tail->next;
        ListNode* p = head; for (int i = 0; i < pos; ++i) p = p->next;
        tail->next = p;
    }
    ListNode* ans = sol.detectCycle(head);
    cout << (ans ? ans->val : -1) << "\n";
    return 0;
}
