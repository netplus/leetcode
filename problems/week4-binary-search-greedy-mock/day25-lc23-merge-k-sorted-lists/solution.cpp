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
// 题目示例：
// 示例 1：
//   Input: lists = [[1,4,5],[1,3,4],[2,6]]
//   Output: [1,1,2,3,4,4,5,6]
//   Explanation: The linked-lists are:
//   [
//   1->4->5,
//   1->3->4,
//   2->6
//   ]
//   merging them into one sorted linked list:
//   1->1->2->3->4->4->5->6
//
// 示例 2：
//   Input: lists = []
//   Output: []
//
// 示例 3：
//   Input: lists = [[]]
//   Output: []
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
//   一个直接正确做法是反复扫描 k 条链表的当前头，每次找出最小头并输出，再推进那一条链。因为每条链内部有序，它不会选错；但每输出一个节点都重新线性扫描 k 个候选，总时间 O(Nk)。
//
//   重复工作恰好是‘在几乎相同的一组链头中再次找最小值’。每次输出后，只有一条链的候选发生变化，其余 k-1 个头完全没变。于是需要一个支持‘取最小 + 删除最小 + 插入一个新候选’的数据结构；最小堆把每次选择从 O(k) 降到 O(log k)。重要的是堆里不放所有剩余节点，只放每条链当前最小未输出节点。优化类型：重复查找消除 + frontier 压缩。
//
// 1. 图像直觉
//   L1: 1 -> 4 -> 5
//   L2: 1 -> 3 -> 4
//   L3: 2 -> 6
//
//   堆里只放当前头：
//           1(L1)
//          /         1(L2)   2(L3)
//
//   弹出 1(L1) 后，只有 L1 frontier 改变：
//   L1 暴露 4；L2 仍是 1；L3 仍是 2。
//
//   不需要把 5、3、4、6 提前全部放进堆，
//   因为它们前面各自还有更小的同链节点挡着。
//
// 2. 一句话核心
//   每条有序链只贡献一个当前最小候选；弹出哪条链的头，就只把那条链的下一个节点补进堆。
//
// 3. 公式 / 不变量
//   设 heap 保存所有尚未耗尽链表的当前头。
//   不变量：对每条链，heap 中若有该链节点，它就是这条链最小的未输出节点。
//
//   因此：
//   heap.top() = 所有剩余节点中的全局最小值。
//
//   每个节点至多入堆一次、出堆一次；heap 大小 <= k：
//   时间 O(N log k)，额外堆空间 O(k)。
//
// 4. 执行步骤
//   1. 把所有非空链表的首节点放入最小堆，建立每条链的 frontier
//   2. 反复弹出堆顶，把它接到结果链表尾部；此时它就是全局最小未输出节点
//   3. 若弹出节点还有 next，只把这个 next 入堆，作为该链新的 frontier
//   4. 堆为空时所有链都耗尽，返回 dummy.next
//   记忆：每路只露一个头；谁被取走，只补谁的下一个。
//
// 5. 为什么不会漏 / 不会重
//   任意时刻，对每条未耗尽链，当前头不大于该链后面的任何节点；因此所有剩余节点的全局最小值必然属于这些当前头之一，而堆顶又是所有当前头中的最小值，所以每次输出都正确。弹出后只推进对应链，立即恢复同一不变量；每个原节点只会被暴露和输出一次，不会漏也不会重。
//
// 6. 边界与易错点
//   比较器必须比较 node->val。堆中允许不同链出现相同值。现有实现复用原节点，不额外 new 合并节点；空 lists 或全空链自然返回 nullptr。不要一次把所有节点入堆，那会把空间从 O(k) 放大到 O(N)，也丢掉有序链 frontier 的结构优势。
//
// 7. 举一反三
//   这是 merge two sorted lists 的 k 路版，也是外部归并排序、多个有序日志流、多个有序迭代器合并的通用模型：每个来源只暴露当前最小 frontier，再用堆仲裁全局下一个。
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
