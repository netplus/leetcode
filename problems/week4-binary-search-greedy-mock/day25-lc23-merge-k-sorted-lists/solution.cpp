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
// 解法精讲｜k 路归并：最小堆只维护每条有序链的当前 frontier
//
// 0. 优化是怎么来的
//   直接正确做法：每输出一个节点，都扫描 k 条链的当前头，找最小者后推进对应链，
//   时间 O(Nk)。重复工作是“在几乎没变化的一组链头里重新找最小值”：每次其实只有
//   一条链的候选发生变化。于是用支持取最小/删最小/插入的最小堆，把选择降为 O(log k)。
//   堆里也不必放所有剩余节点，只放每条链当前最小未输出节点。优化类型：重复查找 + frontier 压缩。
//
// 1. 图像直觉
//   L1: 1 -> 4 -> 5
//   L2: 1 -> 3 -> 4
//   L3: 2 -> 6
//   堆里只需要 {1(L1),1(L2),2(L3)}。弹出 1(L1) 后，只把 L1 的 4 补进堆；
//   L2、L3 的 frontier 没变。5、3、4、6 都被各自更小的同链节点挡在后面，无需提前入堆。
//
// 2. 一句话核心
//   每条有序链只暴露一个当前最小候选；弹出哪条链的头，就只补那条链的下一个。
//
// 3. 公式 / 不变量
//   heap 保存所有尚未耗尽链表的当前头。
//   对每条链，heap 中的该链节点就是它最小的未输出节点；因此 heap.top() 是所有剩余
//   节点的全局最小值。每个节点至多入堆一次、出堆一次，heap.size()<=k。
//   时间 O(N log k)，额外堆空间 O(k)。
//
// 4. 执行步骤
//   1. 所有非空链表首节点入最小堆。
//   2. 弹出堆顶并接到结果尾，它就是当前全局最小节点。
//   3. 若该节点有 next，只把 next 入堆，作为同链新 frontier。
//   4. 堆空时返回 dummy.next。
//   记忆：每路只露一个头；谁被取走，只补谁的下一个。
//
// 5. 为什么不会漏 / 不会重
//   每条链内部升序，所以该链当前头不大于其所有后继；全局最小剩余节点必在这些头中。
//   堆顶又是所有头中的最小值，因此每次输出正确。推进后只替换对应链 frontier，立即恢复不变量。
//   原节点只会沿自己的链被暴露一次、输出一次，不会漏也不会重。
//
// 6. 边界与易错点
//   比较器必须比较 node->val；不同链允许相同值。现有实现复用原节点，不额外 new 合并节点。
//   空 lists 或全空链自然返回 nullptr。不要把所有节点一次性入堆，否则空间退化为 O(N)。
//
// 7. 举一反三
//   这是 merge two sorted lists 的 k 路版本，也适用于外部归并排序、多路日志时间线、多个有序迭代器：
//   每个来源只暴露一个 frontier，再由堆仲裁全局下一个。
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
        // priority_queue 默认是大根堆；比较器返回 a->val>b->val 后，值更小的节点优先出队。
        auto greaterNode = [](ListNode* a, ListNode* b) { return a->val > b->val; };
        priority_queue<ListNode*, vector<ListNode*>, decltype(greaterNode)> smallest(greaterNode);

        // 每条有序链只暴露一个 frontier：当前最小的未输出节点。
        // 同一链更后的节点被这个头节点挡住，没必要提前全部入堆。
        for (ListNode* head : lists) if (head) smallest.push(head);

        ListNode dummy;
        ListNode* tail = &dummy;
        while (!smallest.empty()) {
            // 所有链各自的 frontier 中，堆顶就是全局最小的剩余节点。
            ListNode* node = smallest.top();
            smallest.pop();

            // node 被输出后，只有它所属链的 frontier 发生变化；
            // 将 node->next 补入堆即可恢复“每条未耗尽链恰暴露一个头”的不变量。
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
