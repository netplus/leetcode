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
// - 核心要点：
//   1. 思路起点：对闭区间 [first,last] 增加 seats，只需在差分数组 first-1 处加、last 处减，最后一次前缀和恢复每个航班的总座位。
//   2. 执行逻辑：1. 建立长度 n+1 的 diff；2. 每条预订更新两个边界；3. 从左到右累加 diff 生成答案。
//   3. 为什么这样做：端点标记使 seats 对 first..last 的累计贡献保持有效，并从 last+1 起被抵消；所有预订线性叠加后仍成立。
// - 边界与易错点：题面航班是 1-based，数组是 0-based；使用 n+1 哨兵可无分支写结束标记；不是对每个区间逐项更新。
// - 举一反三：批量区间加法、公交载客、覆盖次数和扫描线事件都可先记录变化点，再通过累计恢复状态。
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

