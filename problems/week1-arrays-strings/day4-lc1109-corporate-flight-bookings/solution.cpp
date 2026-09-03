// ============================================================================
// LC-1109：航班预订统计
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 1 周 / 第 4 天
// ----------------------------------------------------------------------------
// 题目描述：
// 这里有 n 个航班，它们分别从 1 到 n 进行编号。
// 有一份航班预订表 bookings，表中第 i 条预订记录 bookings[i] = [first_i, last_i, seats_i] 意味着在从 first_i 到 last_i （包含
// first_i 和 last_i）的每个航班上预订了 seats_i 个座位。
// 请你返回一个长度为 n 的数组 answer，里面的元素是每个航班预定的座位总数。
//
// 约束与要求：
//   - 1 <= n <= 2 * 10^4
//   - 1 <= bookings.length <= 2 * 10^4
//   - bookings[i].length == 3
//   - 1 <= first_i <= last_i <= n
//   - 1 <= seats_i <= 10^4
//
// 复杂度目标：O(n + m) 使用 差分数组。
//
// ----------------------------------------------------------------------------
// 解法精讲｜差分数组：区间更新转端点更新
//
// 1. 图像直觉
//   差分数组不直接记录“每个位置有多少”，而只记录“从哪里开始变化、从哪里恢复”。
//
//   例如给航班 2..4 每个都加 10：
//
//   最终想增加：  [0, 10, 10, 10, 0]
//   位置：          1   2   3   4   5
//
//   真正需要记录的只有两个事件：
//                  +10         -10
//                   ↓            ↓
//   diff:          [0, 10, 0, 0, -10]
//
//   对 diff 做一次从左到右前缀累加：
//   0 -> 10 -> 10 -> 10 -> 0
//   正好恢复出区间 2..4 的持续影响。
//
//   所以差分可以看成“只记录台阶在哪里升高、在哪里降回去”。
//
// 2. 一句话核心
//   区间整体加值时，不逐点修改；只在区间开始处打开影响，在区间结束后的第一个位置关闭影响，最后前缀累加恢复真实值。
//
// 3. 公式 / 不变量
//   原题航班编号是 1-based。
//   转成 0-based：
//   first = booking[0] - 1
//   afterLast = booking[1]
//
//   对区间 [first, afterLast-1] 加 seats：
//   diff[first] += seats
//   diff[afterLast] -= seats
//
//   最后：
//   running += diff[i]
//   answer[i] = running
//
// 4. 执行步骤
//   1. 建立长度 n+1 的 diff，额外位置用于安全记录最后一个区间之后的关闭事件
//   2. 每条 booking 只更新两个边界：开始位置 +seats，结束后一位 -seats
//   3. 所有 booking 的边界事件可以直接叠加
//   4. 从左到右对 diff 求前缀和，得到每个航班最终座位数
//   记忆：区间开始 +x，区间结束后一位 -x，最后做前缀和。
//
// 5. 为什么不会漏 / 不会重
//   从 first 开始，running 多出 seats；在到达 afterLast 之前都没有相反事件，因此影响持续存在；
//   到 afterLast 时减去 seats，影响恰好从原闭区间 last 的下一位结束。多个区间满足线性叠加。
//
// 6. 边界与易错点
//   题面航班是 1-based，而 vector 是 0-based；结束标记不是 last-1，而是原题编号 last 映射后的“下一位置” afterLast；
//   不要真的对区间内每个元素逐项加。
//
// 7. 举一反三
//   LC-303 是“已知每个点，预处理累计值以快速查区间”；本题反过来是“已知很多区间更新，只记录边界变化，再累计恢复每个点”。
//   差分与前缀和互为一对操作。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n (航班数) m (预订记录)。
//   接下来 m 行：first last seats。
//   输出：n 个以空格分隔的整数，表示从 1 开始编号的每个航班的座位数。
// test.in 的预期输出：10 55 45 25 25 0 0 0 0 0
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    vector<int> corpFlightBookings(vector<vector<int>>& bookings, int n) {
        vector<int> diff(n + 1, 0);
        for (const auto& booking : bookings) {
            const int first = booking[0] - 1;
            const int afterLast = booking[1];
            diff[first] += booking[2];
            diff[afterLast] -= booking[2];
        }
        vector<int> answer(n);
        int running = 0;
        for (int i = 0; i < n; ++i) {
            running += diff[i];
            answer[i] = running;
        }
        return answer;
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n, m; if (!(cin >> n >> m)) return 0;
    vector<vector<int>> bk(m, vector<int>(3));
    for (int i = 0; i < m; ++i) cin >> bk[i][0] >> bk[i][1] >> bk[i][2];
    auto ans = sol.corpFlightBookings(bk, n);
    for (int i = 0; i < n; ++i) cout << ans[i] << " \n"[i + 1 == n];
    return 0;
}
