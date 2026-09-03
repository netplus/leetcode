// ============================================================================
// LC-295：数据流的中位数
// 难度：困难
// 优先级：P0（必做）
// 学习进度：第 4 周 / 第 25 天
// ----------------------------------------------------------------------------
// 题目描述：
// 中位数是有序整数列表中的中间值。
// 如果列表的大小是偶数，则没有中间值，中位数是两个中间值的平均值。
// 例如 arr = [2,3,4] 的中位数是 3。
// 例如 arr = [2,3] 的中位数是 (2 + 3) / 2 = 2.5。
// 实现 MedianFinder 类: MedianFinder() 初始化 MedianFinder 对象。
// void addNum(int num) 将数据流中的整数 num 添加到数据结构中。
// double findMedian() 返回到目前为止所有元素的中位数。
// 与实际答案相差 10^-5 以内的答案将被接受。
//
// 约束与要求：
//   - -10^5 <= num <= 10^5
//   - 在调用 findMedian 之前，数据结构中至少有一个元素
//   - 最多 5 * 10^4 次调用 addNum 和 findMedian
//
// 复杂度目标：O(log n) 每次 addNum（双堆）。
//
// ----------------------------------------------------------------------------
// 解法精讲｜双堆维护数据流中位数
// - 核心要点：
//   1. 思路起点：max-heap lower 保存较小一半，min-heap upper 保存较大一半；维持 lower.size() 等于 upper.size() 或多一。
//   2. 执行逻辑：1. 新数先按与 lower.top 比较放入一侧；2. 从较大堆顶向另一堆搬运以恢复大小；3. 奇数返回 lower.top，偶数返回两堆顶均值。
//   3. 为什么这样做：所有 lower 元素不大于 upper 元素，且大小平衡，所以两个堆顶正是有序序列中间的一个或两个元素。每次搬运边界元素保持顺序不变量。
// - 边界与易错点：偶数均值相加前转 long long；findMedian 只在非空时调用；平衡方向要与“lower 可多一个”约定一致。
// - 举一反三：双堆适合在线中位数；若还需删除滑窗旧值，可配合延迟删除哈希表。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：q (操作数量)。
//   接下来 q 行：op [arg]。
//   op = 1 x：执行 addNum(x)。
//   op = 2：执行 findMedian() 并以浮点数输出中位数。
// test.in 的预期输出：1.5 | 2 | 2.5
// ============================================================================
#include <bits/stdc++.h>
using namespace std;

#include <functional>

// ---------- 题解实现 ----------
class Solution {
    priority_queue<int> lower;  // 较小一半，堆顶是其中最大值
    priority_queue<int, vector<int>, greater<int>> upper;  // 较大一半

public:
    void addNum(int num) {
        if (lower.empty() || num <= lower.top()) lower.push(num);
        else upper.push(num);

        if (lower.size() > upper.size() + 1) {
            upper.push(lower.top());
            lower.pop();
        } else if (upper.size() > lower.size()) {
            lower.push(upper.top());
            upper.pop();
        }
    }

    double findMedian() {
        if (lower.size() > upper.size()) return lower.top();
        return (static_cast<long long>(lower.top()) + upper.top()) / 2.0;
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int q; if (!(cin >> q)) return 0;
    while (q--) {
        int op; cin >> op;
        if (op == 1) { int x; cin >> x; sol.addNum(x); }
        else cout << sol.findMedian() << "\n";
    }
    return 0;
}

