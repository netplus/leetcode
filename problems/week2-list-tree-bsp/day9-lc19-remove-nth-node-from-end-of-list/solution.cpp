// ============================================================================
// LC-19：删除链表的倒数第 N 个结点
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 2 周 / 第 9 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给你一个链表，删除链表的倒数第 n 个结点，并且返回链表的头结点。
//
// 题目示例：
// 示例 1：
//   Input: head = [1,2,3,4,5], n = 2
//   Output: [1,2,3,5]
//
// 示例 2：
//   Input: head = [1], n = 1
//   Output: []
//
// 示例 3：
//   Input: head = [1,2], n = 1
//   Output: [1]
//
// 约束与要求：
//   - 链表中结点的数目为 sz
//   - 1 <= sz <= 30
//   - 0 <= Node.val <= 100
//   - 1 <= n <= sz
//
// 复杂度目标：O(n) 时间，O(1) 空间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜哑节点 + 固定间距双指针定位倒数位置
//
// 1. 图像直觉
//   倒数第 n 个难在：单链表不能从尾巴往前走。
//   解决办法不是回头，而是让两个指针保持固定距离。
//
//   例如删除倒数第 2 个：
//
//   dummy -> 1 -> 2 -> 3 -> 4 -> 5 -> null
//     slow              fast
//
//   先让 fast 从 dummy 走 n+1=3 条边：
//
//   dummy -> 1 -> 2 -> 3 -> 4 -> 5 -> null
//     slow              fast
//           <--- 3 edges --->
//
//   之后两者同步走。fast 一旦到 null，slow 就恰好停在目标节点 4 的前驱 3：
//
//                     slow -> [4] -> 5 -> null
//                              ^
//                            删除
//
//   我们真正要找的不是“倒数第 n 个”，而是它的前驱，因为删除动作需要改 predecessor->next。
//
// 2. 一句话核心
//   让 fast 始终比 slow 多走 n+1 条边；fast 越过尾部时，slow 就被这段固定距离精确定位到待删节点前驱。
//
// 3. 公式 / 不变量
//   使用 dummy 后，把 null 也看成链表尾部之后的一个边界位置。
//
//   初始化：fast 比 slow 领先 n+1 条边。
//   同步阶段不变量：
//   distance(slow, fast) = n+1 条边。
//
//   当 fast == nullptr 时：
//   slow->next 恰好是倒数第 n 个节点。
//
//   为什么是 n+1 而不是 n？
//   因为 slow 要停在目标的“前一个节点”，而不是目标本身。
//
// 4. 执行步骤
//   1. 建立 dummy->head，让删除原 head 也拥有统一的前驱节点。
//   2. fast 和 slow 都从 dummy 出发，先让 fast 走 n+1 步。
//   3. 随后 fast/slow 同步每次走一步，直到 fast==nullptr。
//   4. 此时 removed=slow->next，令 slow->next=removed->next，再释放 removed。
//   记忆：从尾部不好倒着数，就在前面拉开固定尺子；要删目标，就让慢指针对准它的前驱。
//
// 5. 为什么不会漏 / 不会重
//   fast 先领先 n+1 条边，之后两个指针每轮同时走一步，所以间距始终不变。
//   当 fast 到达链表末端之后的 nullptr 边界时，从 slow 到 nullptr 仍正好有 n+1 条边：第一条到 slow->next，剩余 n 条覆盖从目标到尾后的距离，因此 slow->next 正是倒数第 n 个节点。
//   整个过程中 fast/slow 都只向前，节点不会被重复扫描；删除只跳过一个确定节点，不会漏删或多删。
//
// 6. 边界与易错点
//   “领先 n 还是 n+1”不能死记，要先明确 slow 最终要停在目标还是目标前驱。本实现两指针从 dummy 出发并需要前驱，所以是 n+1。dummy 还消除了删除头节点的特殊分支。
//
// 7. 举一反三
//   它和 LC-141 的快慢指针外形相似，但目的不同：LC-141 利用速度差，LC-19 维护固定距离。以后找倒数第 k 个节点只需让 slow 对准目标本身；找链表中点则把固定距离换成 1:2 速度。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n (长度) k (要删除的倒数第 k 个节点)。
//   第 2 行：n 个节点值。
//   输出：操作后的链表节点值，以空格分隔。
// test.in 的预期输出：1 2 3 5
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
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        ListNode dummy(0, head);
        ListNode* fast = &dummy;
        ListNode* slow = &dummy;
        for (int i = 0; i <= n; ++i) fast = fast->next;
        while (fast) {
            fast = fast->next;
            slow = slow->next;
        }
        ListNode* removed = slow->next;
        slow->next = removed->next;
        delete removed;
        return dummy.next;
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n, k; if (!(cin >> n >> k)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    printList(sol.removeNthFromEnd(buildList(a), k));
    return 0;
}
