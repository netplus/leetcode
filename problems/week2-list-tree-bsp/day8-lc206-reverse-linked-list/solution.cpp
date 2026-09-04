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
// 解法精讲｜链表原地反转：三指针
//
// 1. 图像直觉
//   把链表想成被 current 切成两段：
//
//       已反转前缀          未处理后缀
//   nullptr <- 1 <- 2      3 -> 4 -> 5 -> nullptr
//                 prev     current
//
//   这一轮只做一件事：把 current=3 从右半段摘出来，让 3->next 指回 prev。
//   但一旦改写 3->next，原来的 4 就再也找不到，所以必须先用 next 保存后缀入口。
//
// 2. 一句话核心
//   每轮把未处理后缀的第一个节点搬到已反转前缀的最前面，同时始终保住剩余链表的入口。
//
// 3. 公式 / 不变量
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
// 4. 执行步骤
//   1. previous=nullptr、current=head，先把“已处理”和“未处理”两段分开。
//   2. 保存 current->next 到 next，确保改链后仍能进入原后缀。
//   3. 令 current->next=previous，把当前节点接到已反转前缀前面。
//   4. previous 移到 current，current 再移到 next，继续处理下一个节点。
//   记忆：先保后路，再掉头，最后两根指针一起前进。
//
// 5. 为什么不会漏 / 不会重
//   每个节点恰好在成为 current 时处理一次：先保存它原来的后继，再只改一次 next 指针，把它接入反转前缀。
//   因此不会漏节点；current 每轮都沿保存的原链后缀前进，也不会重复处理。循环结束 current=null，说明未处理后缀为空，此时 previous
//   覆盖原链全部节点且方向完全反转。
//
// 6. 边界与易错点
//   最危险的是先写 current->next=previous 再取原 next，这会直接丢失剩余链表；返回值应是 previous，而不是已经变成尾节点的原 head。空链表和单节点都自然满足循环逻辑。
//
// 7. 举一反三
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
        ListNode* previous = nullptr;
        ListNode* current = head;
        while (current) {
            ListNode* next = current->next;  // 改链前保存尚未处理的后缀
            current->next = previous;
            previous = current;
            current = next;
        }
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

