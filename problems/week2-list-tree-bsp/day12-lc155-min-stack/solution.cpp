// ============================================================================
// LC-155：最小栈
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 2 周 / 第 12 天
// ----------------------------------------------------------------------------
// 题目描述：
// 设计一个支持 push，pop，top 操作，并能在常数时间内检索到最小元素的栈。
// 实现 MinStack 类: MinStack() 初始化堆栈对象。
// void push(int value) 将元素 value 推入堆栈。
// void pop() 删除堆栈顶部的元素。
// int top() 获取堆栈顶部的元素。
// int getMin() 获取堆栈中的最小元素。
//
// 约束与要求：
//   - -2^31 <= val <= 2^31 - 1
//   - pop、top 和 getMin 操作总是在非空栈上调用
//   - push , pop , top、getMin 最多被调用 3 * 10^4 次
//
// 复杂度目标：O(1) 每次操作。
//
// ----------------------------------------------------------------------------
// 解法精讲｜栈顶携带前缀摘要：把历史最小值一起压栈
//
// 0. 优化是怎么来的
//   如果只使用普通栈，push/pop/top 都是 O(1)，但 getMin 只能临时扫描整个栈找最小值，单次 O(n)。若 getMin 被频繁调用，就会反复扫描同一批历史元素。
//
//   最小值其实可以在 push 时增量维护：新状态的最小值只取决于“上一层最小值”和“新压入的 val”。所以每层除了保存 value，再保存进入这一层后的 minimum 摘要。
//
//   pop 时也无需重新计算——弹掉当前层后，下面一层保存的摘要天然就是恢复后的正确最小值。
//
//   优化类型：查询时重复扫描 -> 更新时提前维护可回滚摘要，使四种操作全部 O(1)。
//
// 1. 图像直觉
//   普通栈只记“这一层放了什么”；Min Stack 让每一层再带一张摘要卡：
//
//   push -2 : (-2, min=-2)
//   push  0 : ( 0, min=-2)
//   push -3 : (-3, min=-3)  <- top
//
//   pop 后：
//             ( 0, min=-2)  <- top
//
//   无需重新扫描下面所有元素，上一层早已保存了当时的最小值。
//
// 2. 一句话核心
//   每次 push 都把“到这一层为止的最小值”一起保存，因此 pop 天然回滚，getMin 只读栈顶。
//
// 3. 公式 / 不变量
//   设第 i 层实际值为 value[i]，摘要为 min[i]：
//
//   min[0] = value[0]
//   min[i] = min(value[i], min[i-1])
//
//   栈顶 pair = {当前值, 当前整个栈的最小值}
//
//   因此：
//   top()    = stack.top().value
//   getMin() = stack.top().minimum
//
// 4. 执行步骤
//   1. push(val) 时读取旧栈顶最小值，并计算 minimum = min(val, oldMinimum)；空栈时 minimum=val
//   2. 把 {val, minimum} 作为一个整体压栈；pop 时也整体弹出，不做任何重新计算
//   3. top 读取 pair 的实际值字段，getMin 读取同一 pair 的最小值字段，二者都是 O(1)
//   记忆：不要在查询时回头找答案；在状态进入栈时，就把未来查询需要的摘要一起存好。
//
// 5. 为什么不会漏 / 不会重
//   归纳看每一层：底层摘要显然就是底层值；若上一层摘要已经是此前所有元素的最小值，新层取 min(newValue, oldMin)，便得到包含新元素后的全局最小值。
//   pop 不需要“恢复”任何变量，因为弹掉当前层后，上一层保存的摘要本身就是删除该元素之前的正确状态。
//   重复最小值也不会丢：每一层都独立记录自己的前缀最小值。
//
// 6. 边界与易错点
//   若只额外保存一个全局 minimum，弹出当前最小值后就不知道第二小是谁；辅助最小栈若只在严格变小时压入，则重复最小值需要额外计数，而 pair-per-layer 方案天然避开这个坑。
//
// 7. 举一反三
//   这和持久化/回滚思想很接近：每一层保存进入该状态后的摘要。把 minimum 换成 maximum、gcd、按位与等，就得到一系列“栈 + 可回滚聚合状态”结构。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：q (操作数量)。
//   接下来 q 行：op [arg]。
//   op = 1 x：执行 push(x)。
//   op = 2：执行 pop()。
//   op = 3：执行 top() 并输出 x。
//   op = 4：执行 getMin() 并输出最小值。
// test.in 的预期输出：-3 | 0 | -2
// ============================================================================
#include <bits/stdc++.h>
using namespace std;

#include <climits>

// ---------- 题解实现 ----------
class Solution {
    stack<pair<int, int>> values;  // {实际值, 压入该值后的最小值}

public:
    void push(int val) {
        const int minimum = values.empty() ? val : min(val, values.top().second);
        values.push({val, minimum});
    }

    void pop() { values.pop(); }
    int top() { return values.top().first; }
    int getMin() { return values.top().second; }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int q; if (!(cin >> q)) return 0;
    while (q--) {
        int op; cin >> op;
        if (op == 1) { int x; cin >> x; sol.push(x); }
        else if (op == 2) { sol.pop(); }
        else if (op == 3) { cout << sol.top() << "\n"; }
        else { cout << sol.getMin() << "\n"; }
    }
    return 0;
}
