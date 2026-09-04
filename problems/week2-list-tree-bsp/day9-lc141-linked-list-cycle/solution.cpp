// ============================================================================
// LC-141：环形链表
// 难度：简单
// 优先级：P0（必做）
// 学习进度：第 2 周 / 第 9 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给你一个链表的头节点 head，判断链表中是否有环。
// 如果链表中有某个节点，可以通过连续跟踪 next 指针再次到达，则链表中存在环。
// 为了表示给定链表中的环，评测系统内部使用整数 pos 来表示链表尾连接到链表中的位置（索引从 0 开始）。
// 注意：pos 不作为参数进行传递。
// 仅仅是为了标识链表的实际情况。
// 如果链表中存在环，则返回 true。
// 否则，返回 false。
//
// 约束与要求：
//   - 链表中节点的数目范围是 [0, 10^4]
//   - -10^5 <= Node.val <= 10^5
//   - pos 为 -1 或者链表中的一个有效索引。
//
// 复杂度目标：O(n) 时间，O(1) 空间（fast/slow）。
//
// ----------------------------------------------------------------------------
// 解法精讲｜Floyd 快慢指针：用速度差发现周期
//
// 1. 图像直觉
//   把有环链表想成“直道 + 环形跑道”：
//
//   head -> a -> b -> c -> d
//                    ^         \
//                    |          e
//                    +---- g <- f
//
//   slow 每轮走 1 步，fast 每轮走 2 步。
//   在直道上，两者只是前后移动；一旦 slow 也进入环，就等价于两个人在圆形跑道上追逐。
//
//   此时关键不再是“fast 的绝对位置”，而是 fast 相对 slow 每轮多走 1 格：
//
//   相对距离：0, 1, 2, 3, ...  (mod 环长)
//
//   所以只要存在环，相对距离迟早会再次变成 0，也就是两指针相遇。
//   无环则完全不同：fast 只会一路冲到 nullptr。
//
// 2. 一句话核心
//   让 fast 比 slow 每轮多走一步；无环时 fast 会掉出链表，有环时这个相对位移会在有限环上必然追成 0。
//
// 3. 公式 / 不变量
//   设环长为 L。slow 和 fast 都进入环后，每轮：
//
//   relative = (fast_position - slow_position) mod L
//   relative_next = (relative + 1) mod L
//
//   因为 relative 只有 0..L-1 共 L 种状态，连续 +1 mod L 必然到达 0。
//
//   循环不变量：
//   - slow 每轮前进 1 个 next；
//   - fast 每轮前进 2 个 next；
//   - 只要 fast 和 fast->next 存在，这两个推进都是合法的。
//
// 4. 执行步骤
//   1. slow 和 fast 都从 head 出发。
//   2. 只在 fast!=nullptr 且 fast->next!=nullptr 时推进：slow 一步，fast 两步。
//   3. 推进后若 slow==fast，说明两个指针在同一个节点相遇，链表必有环。
//   4. 若循环因为 fast 或 fast->next 为空而结束，说明存在尾部，链表无环。
//   记忆：无环看 fast 会不会掉出去；有环看 fast 能不能在跑道上追到 slow。
//
// 5. 为什么不会漏 / 不会重
//   无环链表中 next 最终指向 nullptr，fast 每轮走两步，因此必先到达尾部，绝不可能无限运行。
//   有环时，slow 最终也会进入环；从那以后两者都不会离开环。fast 相对 slow 每轮恰好多走 1 个节点，在长度 L 的有限圆环上相对距离按 +1 mod L 演化，所以至多 L 轮就会成为 0。
//   相遇比较的是节点地址，因此不会把“值相同的不同节点”误判成环。
//
// 6. 边界与易错点
//   循环条件必须按 fast && fast->next 检查后才能执行 fast->next->next；判断相遇要比较节点指针而不是 val。题面中的 pos 只是评测系统构造环的辅助信息，不是 hasCycle 的参数。
//
// 7. 举一反三
//   LC-142 完全复用这一阶段来证明“环存在”，再利用相遇时累积的路程关系定位入口。更一般地，只要状态是确定性地指向下一个状态，Floyd 都可以用 O(1) 空间检测是否进入周期。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n pos。
//   第 2 行：n 个节点值。
//   pos 表示链表尾连回的下标，-1 表示无环。
//   输出：链表有环时输出 1，否则输出 0。
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
public:
    bool hasCycle(ListNode* head) {
        ListNode* slow = head;
        ListNode* fast = head;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
            if (slow == fast) return true;
        }
        return false;
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
    cout << (sol.hasCycle(head) ? 1 : 0) << "\n";
    return 0;
}
