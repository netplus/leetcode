// ============================================================================
// LC-206：反转链表
// 难度：简单
// 优先级：P0（必做）
// 学习进度：第 2 周 / 第 8 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给你单链表的头节点 head，请你反转链表，并返回反转后的链表。
//
// 题目示例：
// 示例 1：
//   Input: head = [1,2,3,4,5]
//   Output: [5,4,3,2,1]
//
// 示例 2：
//   Input: head = [1,2]
//   Output: [2,1]
//
// 示例 3：
//   Input: head = []
//   Output: []
//
// 约束与要求：
//   - 链表中节点的数目范围是 [0, 5000]
//   - -5000 <= Node.val <= 5000
//
// 复杂度目标：O(n) 时间，O(1) 空间（迭代）。
//
// ----------------------------------------------------------------------------
// 解法精讲｜从题目直觉到可复用算法
//
// 1. 题目直觉 / 图形模型
//   反转过程中把链表分成两段：
//
//   null <- 1 <- 2    3 -> 4 -> null
//               ^     ^
//              prev  current
//
//   左边已经反转完成；右边仍保持原方向。处理 current 时，必须先记住它原来的 next，再把箭头反过来。
//
// 2. 最自然的一般性解法
//   从头到尾逐个处理节点。对 current：先保存 next=current->next；再令 current->next 指向已经反转好的前缀头 prev；最后让 prev=current、current=next 继续。循环结束时 prev 就是新头。
//
// 3. 一般解法的问题与限制
//   这已经是 O(n) 时间、O(1) 额外空间的直接解法，难点不在进一步优化，而在破坏性更新的顺序：若先改 current->next 却没保存原 next，未处理后缀会立刻丢失。
//
// 4. 从具体问题抽象规律
//   原地指针变换应先画清‘已处理区 / 当前对象 / 未处理区’，并在断开旧连接之前保存通往未处理部分的唯一入口。这个不变量可直接迁移到局部反转和链表重排。
//
// 5. 专项算法｜链表原地反转：三指针
//   前面的推理已经得到可复用机制；现在才给它一个检索名称：链表原地反转的三指针不变量。通用机制见 docs/algorithms/linked-list-pointer.md。回到本题时，只需要把其中的状态、边界和更新事件映射到当前题意，不要反过来用模板猜题。
//
// 6. 核心算法
//   每轮把未处理后缀的第一个节点搬到已反转前缀的最前面，同时始终保住剩余链表的入口。
//
// 7. 公式 / 不变量
//   循环开始时保持两个不变量：
//   1. prev 是已经反转好的前缀头；
//   2. current 是仍保持原方向的未处理后缀头。
//
//   一轮更新顺序必须是：
//   next = current->next
//   current->next = prev
//   prev = current
//   current = next
//
//   更新后，这两个不变量再次成立。
//
// 8. 执行步骤
//   1. previous=nullptr、current=head，先把“已处理”和“未处理”两段分开。
//   2. 保存 current->next 到 next，确保改链后仍能进入原后缀。
//   3. 令 current->next=previous，把当前节点接到已反转前缀前面。
//   4. previous 移到 current，current 再移到 next，继续处理下一个节点。
//   记忆：先保后路，再掉头，最后两根指针一起前进。
//
// 9. 为什么不会漏 / 不会重
//   每个节点恰好在成为 current 时处理一次：先保存它原来的后继，再只改一次 next 指针，把它接入反转前缀。
//   因此不会漏节点；current 每轮都沿保存的原链后缀前进，也不会重复处理。循环结束 current=null，说明未处理后缀为空，此时 previous 覆盖原链全部节点且方向完全反转。
//
// 10. 边界与易错点
//   最危险的是先写 current->next=previous 再取原 next，这会直接丢失剩余链表；返回值应是 previous，而不是已经变成尾节点的原 head。空链表和单节点都自然满足循环逻辑。
//
// 11. 举一反三
//   这是本周最基础的链表指针模型。LC-92 的局部反转本质仍是“摘节点 + 改 next + 保住剩余入口”；后面的 LC-234 回文链表和 LC-143 重排链表也会直接复用整段反转。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n。
//   第 2 行：n 个以空格分隔的节点值。
//   输出：反转后的链表节点值，以空格分隔。
// test.in 的预期输出：5 4 3 2 1
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
    ListNode* reverseList(ListNode* head) {
        // 循环开始时把链表划成两段：
        // previous = 已经完成反转的前缀头；current = 仍保持原方向的未处理后缀头。
        // 初始没有已处理节点，所以 previous=nullptr；整条原链都从 current=head 开始。
        ListNode* previous = nullptr;
        ListNode* current = head;

        while (current) {
            // 必须先保存原来的后继。下一行会改写 current->next；
            // 若不先记住它，未处理后缀的入口会从当前指针图中永久丢失。
            ListNode* next = current->next;

            // 把 current 从未处理后缀摘下，并让它指回已反转前缀的头部。
            // 这一步之后，current 已正式成为反转前缀的新头。
            current->next = previous;

            // 两个边界指针依次前进：previous 接管刚处理完的 current，
            // current 再沿刚保存的 next 进入原链剩余部分。
            previous = current;
            current = next;
        }

        // current==nullptr 说明未处理后缀已经为空；
        // previous 此时覆盖原链全部节点，并指向新的链表头。
        return previous;
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
    printList(sol.reverseList(head));
    return 0;
}
