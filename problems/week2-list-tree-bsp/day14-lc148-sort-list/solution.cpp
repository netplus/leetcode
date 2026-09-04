// ============================================================================
// LC-148：排序链表
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 2 周 / 第 14 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给你链表的头结点 head，请将其按升序排列并返回排序后的链表。
//
// 约束与要求：
//   - 链表中节点的数目在范围 [0, 5 * 10^4] 内
//   - -10^5 <= Node.val <= 10^5
//
// 复杂度目标：O(n log n) 时间，O(1) 空间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜自底向上归并排序：把 LC-21 的有序链合并扩展成长度 1、2、4... 的有序段
//
// 0. 优化是怎么来的
//   最容易想到的办法，是把所有节点值拷进 vector，调用 sort，再写回链表；时间 O(n log n)，但需要 O(n) 额外空间，而且没有利用链表本身“改指针即可重排节点”的特点。
//
//   若直接在链表上做插入排序，可以 O(1) 空间，但最坏 O(n^2)，达不到题目的时间目标。
//
//   要同时满足 O(n log n) 时间和常数级额外指针空间，就需要一种只做顺序访问、又能稳定把问题规模翻倍解决的排序。LC-21 已经提供核心积木：两个有序链表可以 O(a+b) 合并。
//
//   于是把每个单节点当作已排序 run：
//   长度 1 的相邻 run 两两 merge -> 长度 2；
//   长度 2 再两两 merge -> 长度 4；
//   不断翻倍，经过 O(log n) 轮后整链有序。
//
//   选择自底向上而非递归归并，是为了不额外消耗 O(log n) 递归栈。
//
//   优化类型：O(n) 中间数组或 O(n^2) 原地插排 -> 顺序切段 + 复用有序归并；自底向上进一步去掉递归栈。
//
// 1. 图像直觉
//   例：4 -> 2 -> 1 -> 3
//
//   runSize = 1：每个节点天然有序
//   [4] [2] [1] [3]
//     \ /       \ /
//    [2,4]      [1,3]
//
//   runSize = 2：两个长度 2 的 run 已有序
//   [2,4] + [1,3]
//          \ /
//       [1,2,3,4]
//
//   每一轮只做两件事：
//   1. split：从当前链中切出 left / right 两个长度至多 runSize 的段；
//   2. merge：复用 LC-21，把两段按升序接成一个更长的有序段。
//
//   runSize：1 -> 2 -> 4 -> 8 ...
//   不是魔法数字，而是在表达“不变量中的有序段长度每轮翻倍”。
//
// 2. 一句话核心
//   把单节点视作有序段，反复把相邻两个长度 runSize 的有序链归并成更长有序链；runSize 每轮翻倍，直到覆盖整条链。
//
// 3. 公式 / 不变量
//   第 k 轮开始时：
//   runSize = 2^k
//   整条链可被看成若干个“长度至多 runSize、且内部已有序”的连续段。
//
//   每次：
//   left  = split(current, runSize)
//   right = split(left 后的剩余链, runSize)
//   rest  = 第二次 split 后的未处理部分
//
//   merge(left,right)
//   => 得到长度至多 2*runSize 的有序段。
//
//   一轮结束后：
//   所有 run 的有序长度从 runSize 扩大到 2*runSize。
//
//   轮数 O(log n)，每轮所有节点总共被扫描/归并 O(n)：
//   总时间 O(n log n)。
//
// 4. 执行步骤
//   1. 先线性统计链表长度 length；dummy 指向 head，统一处理每轮第一个合并段的接回操作
//   2. 从 runSize=1 开始，每轮令 current=dummy.next、sortedTail=&dummy；当前轮只合并固定长度的相邻 run
//   3. 对 current 调 split 得到 left/right/rest 三段：split 走到段尾后必须把 `head->next=nullptr`，真正断开边界
//   4. 调用 mergeRuns(left,right)；其核心就是 LC-21 的双表头比较，并同时返回 mergedHead 与 mergedTail
//   5. 用 sortedTail->next 接回 mergedHead，再令 sortedTail=mergedTail；current 指向 rest，继续处理本轮下一对 run
//   6. 一整轮结束后 runSize*=2；当 runSize>=length 时，整条链已经成为单个有序 run
//   记忆：LC-21 是积木；runSize=1,2,4... 每轮把两个已排序小段拼成一个更大的已排序段。
//
// 5. 为什么不会漏 / 不会重
//   归纳证明阶段不变量：runSize=1 时每个单节点显然有序。假设某轮开始时每个长度至多 runSize 的连续段内部有序，那么 mergeRuns 对相邻两段执行标准有序归并，得到的长度至多 2*runSize 且仍有序；整轮覆盖全部节点后，下轮不变量成立。runSize 最终覆盖 length 时，只剩一个有序段，即整条链有序。每轮节点只参与常数次顺序扫描，且轮数为 O(log n)。
//
// 6. 边界与易错点
//   `split` 的关键不是“找到第二段头”，而是必须同时把第一段尾的 next 置空；不真正断链，merge 会跨过 run 边界。`mergeRuns` 返回尾节点是为了 O(1) 接上下一段；若每次接回后重新从 mergedHead 扫到尾，会引入额外重复工作。题头写 O(1) 空间时，自顶向下递归归并严格说会占 O(log n) 调用栈，因此当前实现选择自底向上。
//
// 7. 举一反三
//   核心积木就是 Day 8 / LC-21：以前只合并两条完整有序链，现在把同一操作嵌进多轮 run 合并。这个“先把小块视为已解，再两两归并成更大块”的结构也直接对应数组归并排序、外部排序和大文件多路归并。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n。
//   第 2 行：n 个节点值。
//   输出：排序后的链表节点值，以空格分隔。
// test.in 的预期输出：1 2 3 4
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
    static ListNode* split(ListNode* head, int size) {
        while (--size > 0 && head) head = head->next;
        if (!head) return nullptr;
        ListNode* second = head->next;
        head->next = nullptr;
        return second;
    }

    static pair<ListNode*, ListNode*> mergeRuns(ListNode* a, ListNode* b) {
        ListNode dummy;
        ListNode* tail = &dummy;
        while (a && b) {
            ListNode*& chosen = (a->val <= b->val) ? a : b;
            tail->next = chosen;
            chosen = chosen->next;
            tail = tail->next;
        }
        tail->next = a ? a : b;
        while (tail->next) tail = tail->next;
        return {dummy.next, tail};
    }

public:
    ListNode* sortList(ListNode* head) {
        int length = 0;
        for (ListNode* node = head; node; node = node->next) ++length;
        ListNode dummy(0, head);

        for (int runSize = 1; runSize < length; runSize *= 2) {
            ListNode* current = dummy.next;
            ListNode* sortedTail = &dummy;
            while (current) {
                ListNode* left = current;
                ListNode* right = split(left, runSize);
                current = split(right, runSize);
                auto [mergedHead, mergedTail] = mergeRuns(left, right);
                sortedTail->next = mergedHead;
                sortedTail = mergedTail;
            }
        }
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
    printList(sol.sortList(buildList(a)));
    return 0;
}
