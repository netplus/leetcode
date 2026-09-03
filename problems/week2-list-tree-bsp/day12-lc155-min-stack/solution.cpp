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
// 解法精讲｜同步辅助栈：每层携带当前最小值
// - 核心要点：
//   1. 思路起点：每次 push 同时保存该位置为止的最小值，使 top 与 getMin 都只读取栈顶，不需要扫描。
//   2. 执行逻辑：1. push(value,min(value,旧最小值))；2. pop 同时移除值与对应最小状态；3. top/getMin 分别读取 pair 两个字段。
//   3. 为什么这样做：栈中每个 pair 的 second 等于从底到该层所有值的最小值；入栈递推保持不变量，出栈自动恢复上一层状态。
// - 边界与易错点：重复最小值必须逐层保存，否则弹出一个后会丢失最小信息；题目保证空栈上不会调用查询/弹出。
// - 举一反三：用栈顶携带聚合状态可实现 MaxStack、括号深度、区间 gcd 栈和支持回滚的数据结构。
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

