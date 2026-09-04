// ============================================================================
// LC-143：重排链表
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 2 周 / 第 14 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给定一个单链表 L 的头节点 head，单链表 L 表示为：L_0 → L_1 → … → L_n - 1 → L_n 请将其重新排列后变为：L_0 → L_n → L_1 → L_n - 1 → L_2
// → L_n - 2 → … 不能只是单纯的改变节点内部的值，而是需要实际的进行节点交换。
//
// 题目示例：
// 示例 1：
//   Input: head = [1,2,3,4]
//   Output: [1,4,2,3]
//
// 示例 2：
//   Input: head = [1,2,3,4,5]
//   Output: [1,5,2,4,3]
//
// 约束与要求：
//   - 链表的长度范围为 [1, 5 * 10^4]
//   - 1 <= node.val <= 1000
//
// 复杂度目标：O(n) 时间，O(1) 空间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜链表模型组合：找中点 + 反转后半段 + 拉链式交替合并
//
// 0. 优化是怎么来的
//   最自然的正确做法，是先把所有 ListNode* 放进 vector。这样既能 O(1) 访问头部，也能 O(1) 访问尾部，再用左右下标按 L0,Ln,L1,Ln-1... 重新连 next。时间 O(n)，但需要 O(n) 额外空间。
//
//   若要求 O(1) 额外空间，真正要消掉的是“为了从尾往前读而保存整张节点索引表”。单链表本身只能向后走，所以把问题变形：不要让指针去尾部倒着读，而是把后半段整个反过来，让尾部顺序主动变成可向前读取。
//
//   于是已有工具可以直接拼起来：
//   1. 快慢指针找到中点并断链；
//   2. 复用 LC-206 反转第二段；
//   3. 两条链都能沿 next 正向读取，再做交替合并。
//
//   优化类型：完整节点索引 -> 结构变换让“倒序访问”变成“顺序访问”，空间从 O(n) 降为 O(1)。
//
// 1. 图像直觉
//   例：
//   1 -> 2 -> 3 -> 4 -> 5
//
//   目标：
//   1 -> 5 -> 2 -> 4 -> 3
//
//   第一步切开：
//   first : 1 -> 2 -> 3 -> null
//   second: 4 -> 5 -> null
//
//   第二步反转 second：
//   first : 1 -> 2 -> 3
//   second: 5 -> 4
//
//   现在目标顺序已经变成“两条正向链交替取节点”：
//
//   first :  1 ----> 2 ----> 3
//             \      \
//   second:    5 ----> 4
//
//   交织：
//   1 -> 5 -> 2 -> 4 -> 3
//
//   关键不是记三段代码，而是看清：
//   “尾部倒着取”被反转操作变成了“第二条链从头顺着取”。
//
// 2. 一句话核心
//   先把后半段反转，让原链尾部变成一条可顺向读取的链；然后两条链各取一个节点交替连接，就自然得到头、尾、次头、次尾的顺序。
//
// 3. 公式 / 不变量
//   切分后保持：
//   first  = L0 -> L1 -> ...
//   second = ... -> Ln-1 -> Ln
//
//   反转 second 后：
//   second = Ln -> Ln-1 -> ...
//
//   合并阶段每轮前的不变量：
//   - first 指向下一枚“前半段节点”；
//   - second 指向下一枚“原尾部节点”；
//   - 已连接前缀已经严格满足 L0,Ln,L1,Ln-1... 的目标顺序。
//
//   每轮执行：
//   nextFirst  = first->next
//   nextSecond = second->next
//   first->next = second
//   second->next = nextFirst
//   然后两指针推进到保存好的 next。
//
// 4. 执行步骤
//   1. 用 slow=head、fast=head->next 的快慢指针定位前半段尾节点 slow，使奇数长度时前半段自然多一个节点
//   2. 令 second=slow->next，并立即执行 slow->next=nullptr 把两段断开；这是后续安全交织的结构边界
//   3. 完全复用 LC-206：逐节点保存 next、反转 next，得到从原尾部开始的 reversed 第二段
//   4. first=head、second=reversed；每轮先保存两边 next，再连接 first->second->nextFirst，最后推进到下一对节点
//   5. 当 second 用完时停止；前半段长度不少于后半段，剩余的单个中间节点已经自然位于结果末尾
//   记忆：拿不到尾部前驱，就把后半段翻过来；两条正向链再拉链合并。
//
// 5. 为什么不会漏 / 不会重
//   切分后，第一段按 L0,L1,L2... 排列；第二段反转后按 Ln,Ln-1,Ln-2... 排列。交替取两段头节点，因此第 k 轮恰好追加一对 Lk 与 Ln-k，直接生成题目要求的顺序。两段先断开，且每个节点只从自己的链中被取一次，所以不会重复节点，也不会形成旧连接残留造成的环。
//
// 6. 边界与易错点
//   必须先断链 `slow->next=nullptr`，否则后半段反转和交织时旧连接仍存在，容易形成环。合并时必须先保存 nextFirst/nextSecond，再改 next，否则会丢失尚未处理的后缀。这里 fast 从 head->next 出发，是为了让偶数长度时两半等长、奇数长度时前半段多一个；不同初始化也能写对，但对应的切分边界必须一起调整。
//
// 7. 举一反三
//   这是 LC-234 的直接兄弟题：两题前两步都完全一样——找中点 + 反转后半段。LC-234 第三步是逐项比较，LC-143 第三步则是交替合并。它也复用了 LC-206 的反转和 LC-21 的“维护结果尾部/连接剩余链”思维。以后看到链表需要交替访问前后两端，优先想能否先把后一半反过来。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n。
//   第 2 行：n 个节点值。
//   输出：重排后的链表节点值，以空格分隔。
// test.in 的预期输出：1 4 2 3
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
    void reorderList(ListNode* head) {
        if (!head || !head->next) return;
        ListNode* slow = head;
        ListNode* fast = head->next;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }

        ListNode* second = slow->next;
        slow->next = nullptr;  // 先断链，避免交织时产生环
        ListNode* reversed = nullptr;
        while (second) {
            ListNode* next = second->next;
            second->next = reversed;
            reversed = second;
            second = next;
        }

        ListNode* first = head;
        second = reversed;
        while (second) {
            ListNode* nextFirst = first->next;
            ListNode* nextSecond = second->next;
            first->next = second;
            second->next = nextFirst;
            first = nextFirst;
            second = nextSecond;
        }
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
    ListNode* head = buildList(a);
    sol.reorderList(head);
    printList(head);
    return 0;
}
