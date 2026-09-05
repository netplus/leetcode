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
// 题目示例：
// 示例 1：
//   Input
//   ["MedianFinder", "addNum", "addNum", "findMedian", "addNum", "findMedian"]
//   [[], [1], [2], [], [3], []]
//   Output
//   [null, null, null, 1.5, null, 2.0]
//
//   Explanation
//   MedianFinder medianFinder = new MedianFinder();
//   medianFinder.addNum(1); // arr = [1]
//   medianFinder.addNum(2); // arr = [1, 2]
//   medianFinder.findMedian(); // return 1.5 (i.e., (1 + 2) / 2)
//   medianFinder.addNum(3); // arr[1, 2, 3]
//   medianFinder.findMedian(); // return 2.0
//
// 约束与要求：
//   - -10^5 <= num <= 10^5
//   - 在调用 findMedian 之前，数据结构中至少有一个元素
//   - 最多 5 * 10^4 次调用 addNum 和 findMedian
//
// 复杂度目标：O(log n) 每次 addNum（双堆）。
//
// ----------------------------------------------------------------------------
// 解法精讲｜双堆动态分割：持续维护有序序列中间边界
//
// 0. 优化是怎么来的
//   直接算法可以把新数加入数组，每次 findMedian 时排序后读取中间位置；若查询频繁，就会反复重排大量旧数据。即使始终维护一个有序 vector，插入仍可能搬动 O(n) 个元素。
//
//   真正需要长期维护的不是完整顺序，而是中间分割线：左边有多少元素、右边有多少元素，以及分割线两侧的最大/最小边界。最大堆能 O(1) 看到左半最大值，最小堆能 O(1) 看到右半最小值，插入与跨边界搬运都是 O(log n)。于是把完整动态有序表压成两个‘只暴露边界’的集合。优化类型：状态压缩 + 动态边界维护。
//
// 1. 图像直觉
//   当前数据排序后：
//   1 2 3 | 7 9
//   lower   upper
//
//   lower=max-heap：top=3
//   upper=min-heap：top=7
//
//   加入 4：
//   先进入 upper：1 2 3 | 4 7 9
//   此时 upper 比 lower 多，需要把 upper 最小值 4 搬到 lower：
//   1 2 3 4 | 7 9
//
//   现在 lower 多一个，奇数总数的中位数就是 lower.top()=4。
//
//   关键不是维护两边内部排序，
//   而是维护‘左边都 <= 右边’和大小差至多 1。
//
// 2. 一句话核心
//   把数据流持续切成大小平衡的左右两半，只维护分割线两侧的极值；中位数始终由堆顶直接给出。
//
// 3. 公式 / 不变量
//   顺序不变量：
//   forall x in lower, y in upper: x <= y
//
//   大小不变量：
//   lower.size() == upper.size()
//   或
//   lower.size() == upper.size() + 1
//
//   因此：
//   总数奇数：median = lower.top()
//   总数偶数：median = (lower.top() + upper.top()) / 2
//
//   插入后若大小失衡，只搬运较大一侧的边界元素即可恢复平衡。
//
// 4. 执行步骤
//   1. 新数 <= lower.top() 时进入 lower，否则进入 upper；lower 为空时直接进入 lower
//   2. 若 lower 比 upper 多超过 1，把 lower.top() 移到 upper；若 upper 更大，把 upper.top() 移到 lower
//   3. 查询时，lower 多一个就返回 lower.top()；大小相等就返回两个堆顶的平均值
//   记忆：左半看最大，右半看最小；顺序对、数量平，中位数就在堆顶。
//
// 5. 为什么不会漏 / 不会重
//   插入时按 lower.top() 分边，结合插入前的顺序不变量，新元素不会破坏‘左不大于右’。若某侧元素过多，只把该侧最靠近分割线的边界元素移到另一侧：lower.top() 是左侧最大值，upper.top() 是右侧最小值，因此搬运后顺序关系仍成立。大小恢复后，中间排名恰落在一个或两个堆顶上，所以查询不会漏掉真正中位数。
//
// 6. 边界与易错点
//   findMedian 按题意只在至少一个元素时调用。偶数时两个 int 相加可能溢出，现有实现先转 long long。必须固定并始终遵守‘lower 可以比 upper 多一个’的大小约定；若换约定，查询逻辑也必须一起换。
//
// 7. 举一反三
//   与 LC-23 一样，堆只暴露当前最关键边界；区别是 LC-23 维护多个来源的最小 frontier，LC-295 维护一个动态全体的中间分割线。滑动窗口中位数会在双堆上再加入延迟删除。
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
    // lower 保存较小一半，使用大根堆以 O(1) 暴露左半最大值；
    // upper 保存较大一半，使用小根堆以 O(1) 暴露右半最小值。
    priority_queue<int> lower;
    priority_queue<int, vector<int>, greater<int>> upper;

public:
    void addNum(int num) {
        // 先按当前分割线放到正确一侧，保持任意 lower 元素 <= 任意 upper 元素。
        if (lower.empty() || num <= lower.top()) lower.push(num);
        else upper.push(num);

        // 再恢复大小不变量：lower 与 upper 等大，或 lower 恰好多一个。
        // 搬运的必须是靠近中间分割线的边界元素，才能同时保持两侧的值域顺序。
        if (lower.size() > upper.size() + 1) {
            upper.push(lower.top());
            lower.pop();
        } else if (upper.size() > lower.size()) {
            lower.push(upper.top());
            upper.pop();
        }
    }

    double findMedian() {
        // 奇数总量时，多出的一个元素按约定在 lower，中位数就是左半最大值。
        if (lower.size() > upper.size()) return lower.top();

        // 偶数时中位数是两个分割边界的平均；先提升到 long long 再相加，避免 int 溢出。
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
