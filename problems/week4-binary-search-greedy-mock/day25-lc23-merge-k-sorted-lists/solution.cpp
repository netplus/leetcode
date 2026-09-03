// ============================================================================
// LC-23：合并 K 个升序链表
// 难度：困难
// 优先级：P1（进阶）
// 学习进度：第 4 周 / 第 25 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给你一个链表数组，每个链表都已经按升序排列。
// 请你将所有链表合并到一个升序链表中，返回合并后的链表。
//
// 约束与要求：
//   - k == lists.length
//   - 0 <= k <= 10^4
//   - 0 <= lists[i].length <= 500
//   - -10^4 <= lists[i][j] <= 10^4
//   - lists[i] 按升序排列
//   - lists[i].length 的总和不超过 10^4
//
// 复杂度目标：O(N log k) 时间（小根堆）。
//
// ----------------------------------------------------------------------------
// 解法精讲｜多路归并：最小堆保存每条链当前头
// - 核心要点：
//   1. 思路起点：每条链表内部有序，全局下一个最小节点一定在某条链的当前头；最小堆在 k 个候选头间选择。
//   2. 执行逻辑：1. 所有非空链表头入堆；2. 弹出最小节点接到结果尾；3. 把该节点的后继作为同链新候选入堆。
//   3. 为什么这样做：堆始终包含每条尚未耗尽链的最小未输出节点，因此堆顶是所有剩余节点的全局最小值，归纳得到完整有序序列。
// - 边界与易错点：堆比较器要比较 node->val；应复用节点且最后令 tail 正确连接；空链表数组返回 nullptr。
// - 举一反三：k 路归并用于外部排序、日志时间线合并和归并多个有序迭代器，复杂度通常是 O(N log k)。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：k。
//   随后读取 k 个数据块：每块先用一行给出 li（链表长度），再用一行给出 li 个节点值。
//   输出：合并后的链表节点值，以空格分隔。
// test.in 的预期输出：1 1 2 3 4 4 5 6
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
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        auto greaterNode = [](ListNode* a, ListNode* b) { return a->val > b->val; };
        priority_queue<ListNode*, vector<ListNode*>, decltype(greaterNode)> smallest(greaterNode);
        for (ListNode* head : lists) if (head) smallest.push(head);

        ListNode dummy;
        ListNode* tail = &dummy;
        while (!smallest.empty()) {
            ListNode* node = smallest.top();
            smallest.pop();
            if (node->next) smallest.push(node->next);
            tail->next = node;
            tail = node;
        }
        return dummy.next;
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int k; if (!(cin >> k)) return 0;
    vector<ListNode*> lists;
    for (int i = 0; i < k; ++i) {
        int li; cin >> li;
        vector<int> v(li);
        for (int j = 0; j < li; ++j) cin >> v[j];
        lists.push_back(buildList(v));
    }
    printList(sol.mergeKLists(lists));
    return 0;
}

