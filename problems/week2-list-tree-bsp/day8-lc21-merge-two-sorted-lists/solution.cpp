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
// 题目示例：
// 示例 1：
//   Input: list1 = [1,2,4], list2 = [1,3,4]
//   Output: [1,1,2,3,4,4]
//
// 示例 2：
//   Input: list1 = [], list2 = []
//   Output: []
//
// 示例 3：
//   Input: list1 = [], list2 = [0]
//   Output: [0]
//
// 约束与要求：
//   - 两个链表的节点数目范围是 [0, 50]
//   - -100 <= Node.val <= 100
//   - l1 和 l2 均按非递减顺序排列
//
// 复杂度目标：O(n+m) 时间，O(1) 空间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜哑节点 + 双路归并
//
// 1. 图像直觉
//   两个链表各自已经有序，所以真正有资格成为“下一个结果节点”的只有两个当前表头：
//
//   list1: 1 -> 2 -> 4
//          ^
//   list2: 1 -> 3 -> 4
//          ^
//
//   result: dummy -> ... -> tail
//
//   若两个表头是 2 和 3，就不用看后面的 4：因为各链内部有序，2 一定是所有剩余节点中的最小候选。接走 2 后，只推进 list1。
//
// 2. 一句话核心
//   结果链每次只在两个剩余链表的最小元素之间二选一，把较小表头接到 tail 后。
//
// 3. 公式 / 不变量
//   循环不变量：
//   1. dummy.next ... tail 已经是全局最小的已处理节点，并保持非递减；
//   2. list1 和 list2 分别指向各自尚未处理部分的最小节点。
//
//   next(result) = min(list1->val, list2->val)。
//
// 4. 执行步骤
//   1. 建立 dummy 和 tail，避免单独处理结果链第一个节点。
//   2. 两链都非空时比较当前表头，把较小节点直接接到 tail->next。
//   3. 只推进被选中的来源指针，再把 tail 移到新接入节点。
//   4. 一条链耗尽后，另一条剩余后缀已经整体有序且不小于当前 tail，可一次性接上。
//   记忆：只比较两个队头；谁小接谁，谁被接走谁前进。
//
// 5. 为什么不会漏 / 不会重
//   因为每条剩余链表内部有序，各自表头就是该链的最小值；两表头中更小者因此也是所有剩余节点的全局最小值。
//   每轮恰好消耗一个节点，不会重复；当一链为空时，另一链所有节点都尚未处理且顺序已经正确，整体接上不会漏节点。
//
// 6. 边界与易错点
//   题目要求拼接原链表节点，不需要复制节点值；相等时从任一边取都能保持非递减。使用 dummy 后最终返回 dummy.next，而不是 tail。
//
// 7. 举一反三
//   这是归并排序 merge 阶段的链表版本。以后 LC-23 合并 K 个升序链表，只是把“两个当前最小头二选一”扩展成“用小根堆从 K 个当前表头中取最小”。
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
        // dummy 只提供稳定的结果链起点；tail 永远指向“已经合并完成部分”的最后一个节点。
        // 因而首节点和后续节点可以使用完全相同的 tail->next 连接逻辑。
        ListNode dummy;
        ListNode* tail = &dummy;

        while (list1 && list2) {
            // 两条剩余链各自有序，所以它们的表头分别是各自剩余部分的最小值；
            // 下一结果节点只需在这两个表头之间二选一，不需要查看更后面的节点。
            //
            // chosen 的类型是 ListNode*&：它不是普通指针副本，而是对 list1 或 list2
            // 这个“头指针变量”的引用。所以下面的 chosen=chosen->next 会直接推进被选中的来源指针。
            ListNode*& chosen = (list1->val <= list2->val) ? list1 : list2;

            // 先把当前全局最小头节点接到结果尾部；节点本身被复用，没有复制 val 或 new 新节点。
            tail->next = chosen;

            // 来源链跳过刚被消费的节点；因为 chosen 是引用，这一步等价于推进 list1 或 list2。
            chosen = chosen->next;

            // tail 再移动到刚接入的节点，保持它始终指向结果链最后一个节点。
            tail = tail->next;
        }

        // 一条链耗尽后，另一条剩余后缀已经整体有序，且其头就是所有未处理节点中的唯一候选；
        // 无需再逐节点比较，可以把整段一次接到 tail 后。
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
