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
// 解法精讲｜双堆动态分割：持续维护有序序列中间边界
//
// 0. 优化是怎么来的
//   直接算法可以把所有数放进数组，每次查询时排序再取中间；若查询频繁，会反复重排旧数据。
//   即使始终维护有序 vector，插入也可能搬动 O(n) 个元素。真正要长期维护的不是完整顺序，而是
//   中间分割线：左边元素数量、右边元素数量，以及分割线两侧的最大/最小边界。
//   最大堆暴露左半最大值，最小堆暴露右半最小值，插入与搬运都只需 O(log n)。
//
// 1. 图像直觉
//   有序视角：1 2 3 | 7 9
//             lower upper
//   lower 是最大堆，top=3；upper 是最小堆，top=7。
//   加入 4 后先进入 upper：1 2 3 | 4 7 9；upper 多一个，于是把最小边界 4 搬到 lower：
//   1 2 3 4 | 7 9。现在总数为奇数，中位数就是 lower.top()=4。
//
// 2. 一句话核心
//   把数据流持续切成大小平衡的左右两半，只维护分割线两侧极值；中位数始终由堆顶给出。
//
// 3. 公式 / 不变量
//   顺序：对任意 x in lower, y in upper，都有 x<=y。
//   大小：lower.size()==upper.size()，或 lower.size()==upper.size()+1。
//   奇数总数：median=lower.top()；偶数总数：median=(lower.top()+upper.top())/2。
//   插入失衡后只搬运较大一侧最靠近分割线的边界元素即可恢复大小。
//
// 4. 执行步骤
//   1. lower 为空或 num<=lower.top() 时进 lower，否则进 upper。
//   2. lower 比 upper 多超过 1 时，把 lower.top() 搬到 upper。
//   3. upper 比 lower 多时，把 upper.top() 搬到 lower。
//   4. 查询时按两堆大小返回一个堆顶或两个堆顶平均值。
//   记忆：左半看最大，右半看最小；顺序对、数量平，中位数就在堆顶。
//
// 5. 为什么不会漏 / 不会重
//   新数按 lower.top() 分边不会破坏“左<=右”。若大小失衡，lower.top() 是左侧最大边界，
//   upper.top() 是右侧最小边界，只搬这个边界元素仍保持两侧顺序。大小恢复后，中间排名恰落在
//   一个或两个堆顶上，因此 findMedian 读取的就是当前真实中位数。
//
// 6. 边界与易错点
//   findMedian 按题意只会在非空时调用。偶数情况下两个 int 相加可能溢出，现有实现先转 long long。
//   必须固定“lower 可以多一个”的约定；若改变平衡约定，查询公式也必须同步改变。
//
// 7. 举一反三
//   与 LC-23 一样，堆只暴露当前最关键边界；LC-23 维护多个来源的最小 frontier，LC-295 维护
//   动态全集的中间分割线。滑动窗口中位数可在双堆之上增加延迟删除。
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
