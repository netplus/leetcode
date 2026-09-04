// ============================================================================
// LC-234：回文链表
// 难度：简单
// 优先级：P1（进阶）
// 学习进度：第 2 周 / 第 9 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给你一个单链表的头节点 head，请你判断该链表是否为回文链表。
// 如果是，返回 true；否则，返回 false。
//
// 约束与要求：
//   - 链表中节点数目在范围 [1, 10^5] 内
//   - 0 <= Node.val <= 9
//
// 复杂度目标：O(n) 时间，O(1) 空间（进阶要求）。
//
// ----------------------------------------------------------------------------
// 解法精讲｜快慢指针找中点 + 复用 LC-206 反转 + 同向比较
//
// 1. 图像直觉
//   数组判断回文可以左右夹逼，但单链表不能从尾部向前走：
//
//   1 -> 2 -> 3 -> 2 -> 1
//   ^                   ^
//   left             想要 right，但拿不到前驱
//
//   所以把“从尾往前”改造成“把后半段翻过来后从前往后”：
//
//   原链：   1 -> 2 -> 3 -> 2 -> 1
//                       slow
//
//   反转 slow 开始的后半段后：
//   left:    1 -> 2 -> 3 ...
//   right:   1 -> 2 -> 3
//
//   现在两个指针都只需沿 next 向右，就在比较原链的镜像位置。
//   这里其实没有新指针技巧：找中点来自 fast/slow，反转完全复用 Day 8 的 LC-206。
//
// 2. 一句话核心
//   单链表不能从右往左比较，就把后半段原地反转，把“左右夹逼”转换成两条都向 next 前进的链表比较。
//
// 3. 公式 / 不变量
//   第一阶段：fast 每次 2 步、slow 每次 1 步。
//   fast 到尾时，slow 位于后半段起点（奇数长度时包含中间节点）。
//
//   第二阶段调用 LC-206：
//   reversed = reverse(slow)。
//
//   比较不变量：
//   left 指向前半段当前镜像位置；
//   right 指向反转后半段当前镜像位置；
//   right 未结束前必须满足 left->val == right->val。
//
//   只需比较 reversed 的长度，因为后半段长度 <= 前半段长度。
//
// 4. 执行步骤
//   1. 长度 0/1 直接返回 true；否则 fast/slow 从 head 出发，以 2:1 速度找到后半段起点 slow。
//   2. 完全复用 LC-206 的 reverse(slow)，得到从原链尾部向中心排列的 reversed。
//   3. left=head、right=reversed，同时向后比较；任一对应值不同就把 equal 记为 false。
//   4. 比较结束后再次 reverse(reversed)，把后半段恢复为原方向，再返回 equal。
//   记忆：拿不到从尾往前的指针，就把后半段翻过来；比较完再翻回去。
//
// 5. 为什么不会漏 / 不会重
//   链表回文等价于第 1 个值=倒数第 1 个值、第 2 个值=倒数第 2 个值……。反转后半段恰好把这些“从尾往前”的节点改成从 reversed 头开始的 next 顺序，因此 left/right 每一轮比较的是一对镜像位置。
//   若所有 right 节点都匹配，则所有需要检查的镜像对都相等；奇数长度多出来的中间节点只会和自己对应，不影响结论。最后再次反转恢复原链，使算法不留下隐藏结构副作用。
//
// 6. 边界与易错点
//   不要为了方便把值拷进数组，否则会失去 O(1) 额外空间目标。奇数长度时 slow 包含中间节点是合法的；真正容易遗漏的是比较后恢复链表。reverse 的连接顺序继续遵守 LC-206 的“先保后路，再掉头”。
//
// 7. 举一反三
//   这是一次很重要的模型组合：LC-141/19 提供快慢指针定位能力，LC-206 提供原地反转能力，把两个已学模块拼起来就得到 O(1) 空间回文检查。后面的 LC-143 重排链表还会再次使用“找中点 + 反转后半段”，只是最后一步从比较改成穿插合并。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n。
//   第 2 行：n 个节点值。
//   输出：是回文链表时输出 1，否则输出 0。
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
    static ListNode* reverse(ListNode* head) {
        ListNode* previous = nullptr;
        while (head) {
            ListNode* next = head->next;
            head->next = previous;
            previous = head;
            head = next;
        }
        return previous;
    }

public:
    bool isPalindrome(ListNode* head) {
        if (!head || !head->next) return true;
        ListNode* slow = head;
        ListNode* fast = head;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }

        ListNode* reversed = reverse(slow);
        ListNode* right = reversed;
        ListNode* left = head;
        bool equal = true;
        while (right) {
            if (left->val != right->val) equal = false;
            left = left->next;
            right = right->next;
        }
        reverse(reversed);  // 恢复原链表，避免隐藏副作用
        return equal;
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
    cout << (sol.isPalindrome(buildList(a)) ? 1 : 0) << "\n";
    return 0;
}
