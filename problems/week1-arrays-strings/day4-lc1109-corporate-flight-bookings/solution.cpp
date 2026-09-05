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
// 题目示例：
// 示例 1：
//   Input: bookings = [[1,2,10],[2,3,20],[2,5,25]], n = 5
//   Output: [10,55,45,25,25]
//   Explanation:
//   Flight labels: 1 2 3 4 5
//   Booking 1 reserved: 10 10
//   Booking 2 reserved: 20 20
//   Booking 3 reserved: 25 25 25 25
//   Total seats: 10 55 45 25 25
//   Hence, answer = [10,55,45,25,25]
//
// 示例 2：
//   Input: bookings = [[1,2,10],[2,2,15]], n = 2
//   Output: [10,25]
//   Explanation:
//   Flight labels: 1 2
//   Booking 1 reserved: 10 10
//   Booking 2 reserved: 15
//   Total seats: 10 25
//   Hence, answer = [10,25]
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
// 解法精讲｜从逐航班模拟到差分：事件 + 累积状态
//
// 1. 图像直觉
//   第一步永远先写最直观的算法。
//
//   假设只有一条：
//   booking = [2,5,25]
//
//   朴素做法就是：
//   航班：      1    2    3    4    5    6
//   增加量：    0   25   25   25   25    0
//
//   也就是依次执行：
//   flight[2] += 25
//   flight[3] += 25
//   flight[4] += 25
//   flight[5] += 25
//
//   这个算法完全正确。差分只是在问：
//   既然 2..5 每个位置做的事情都一模一样，能不能不要立刻写 4 次？
//
//   可以。我们只记录“状态变化”：
//
//   位置：      1    2    3    4    5    6
//   事件：      0   +25   0    0    0   -25
//
//   这里真正的实现技巧，不只是“记两个端点”，而是利用 running 的累积性让状态自动延续。
//   扫描规则只有一句：
//
//   running += diff[i]
//
//   逐位置看：
//   位置 1：diff=0    -> running = 0
//   位置 2：diff=+25  -> running = 25    <- 开启一份 +25 状态
//   位置 3：diff=0    -> running = 25    <- 没有新事件，状态自动继承
//   位置 4：diff=0    -> running = 25    <- 继续继承
//   位置 5：diff=0    -> running = 25    <- 继续继承
//   位置 6：diff=-25  -> running = 0     <- 撤销之前那份 +25 状态
//
//   注意位置 3、4、5 并没有再次执行 +25。
//   它们之所以仍得到 25，是因为 running 是累积状态：
//
//   running[i] = running[i-1] + diff[i]
//
//   当 diff[i] == 0 时：
//   running[i] = running[i-1]
//
//   也就是说，“没有新变化”本身就意味着“沿用之前的状态”。
//   这正是差分能把区间内成千上万次重复 +=seats 压缩掉的原因。
//
//   所以可以把三者分工记成：
//
//   diff[i]   ：当前位置发生的状态变化量
//   running   ：扫描到当前位置时，当前仍然生效的总状态
//   answer[i] ：把当前状态落到这个具体航班上的结果
//
//   这样再看右侧的 -25 就非常自然：
//   它不是“6 号航班减 25”，而是一个状态变化事件。
//   之前 running 里一直带着 +25；从位置 6 开始不该再带了，于是执行：
//
//   running = running + (-25)
//           = 25 - 25
//           = 0
//
//   也就是把之前加入 running 的那份状态移除。
//
//   如果没有这个 -25：
//   事件：      0   +25   0    0    0    0    0 ...
//   running：   0    25  25   25   25   25   25 ...
//
//   因为累积值会继续继承，+25 就会错误地永久延续下去。
//   因此 +25 和 -25 必须成对理解：
//
//   +25：把一份贡献加入 running，使它从这里开始持续生效
//   -25：把同一份贡献从 running 中移除，使它从这里开始停止生效
//
//   再看大区间 booking=[2,10000,25]。
//   朴素算法要对 2..10000 每个航班执行一次 +=25。
//   差分却只做：
//
//   2 号对应位置：              +25   <- 将 +25 加入 running
//   10000 号之后的边界：         -25   <- 将这份 +25 从 running 中移除
//
//   中间 3..10000 根本不需要继续记录事件，因为每走一步都会自然继承 running=25。
//   这就是“用累积值延续可加状态”的核心技巧。
//
//   如果题目恰好 n=10000，那么根本不存在 10001 号航班。
//   代码里的额外位置只是 diff 的哨兵槽位，用来承接“10000 号航班之后结束”这个事件；最终 answer 只读取 1..10000，不会输出这个哨兵位置。
//
//   第二步，再从数学上看为什么只需两个事件。
//   若最终每点增加量为 A，则差分记录相邻位置的变化：
//
//   diff[0] = A[0]
//   diff[i] = A[i] - A[i-1]
//
//   对闭区间 [L,R] 整体 +x 后，区间内部任意相邻两点都会同时 +x：
//
//   (A[i] + x) - (A[i-1] + x)
//   = A[i] - A[i-1]
//
//   因此内部差值完全不变，只有：
//   进入区间：diff[L]     += x
//   离开区间：diff[R + 1] -= x
//
//   最后做一次前缀和，就是把这些“状态变化事件”重新展开成最初那个逐航班累加的结果。
//
// 2. 一句话核心
//   差分真正的实现技巧是“事件 + 累积状态”：diff 只记录状态什么时候改变，running 负责把当前状态自动延续到后续位置。左端 +seats 是把一份贡献加入 running；中间 diff=0 时 running 原样继承，所以无需重复 +=seats；右端后一位 -seats 是把同一份贡献从 running 中移除。
//
// 3. 公式 / 不变量
//   最关键的不变量不是先背 diff[L]+=x、diff[R+1]-=x，而是：
//
//   diff[i] = 位置 i 发生的“状态变化量”
//   running[i] = running[i-1] + diff[i]
//   answer[i] = running[i]
//
//   因此：
//
//   若 diff[i] == 0：
//   running[i] = running[i-1]
//
//   这就是状态能够跨位置持续的原因。
//
//   对一条 0-based 区间 [L,R] 增加 seats：
//
//   diff[L]     += seats      // 把 seats 加入持续状态
//   diff[R + 1] -= seats      // 把同一份 seats 从持续状态移除
//
//   于是扫描时：
//
//   位置 < L：running 不包含这份 seats
//   L <= 位置 <= R：running 一直包含这份 seats
//   位置 >= R+1：running 已经减掉这份 seats
//
//   本题输入是 1-based 的 [first,last]：
//   L = first - 1
//   R = last - 1
//   R + 1 = last
//
//   代码因此是：
//   first = booking[0] - 1
//   afterLast = booking[1]
//   diff[first]     += seats
//   diff[afterLast] -= seats
//
//   特别注意：当 last == n 时，afterLast == n 是 vector 中额外的哨兵下标，不对应任何真实航班。它只表示“最后一个航班之后，这份持续状态结束”。
//
// 4. 执行步骤
//   1. 先写出朴素模型：answer[flight-1]+=seats，flight 从 first 走到 last；确认题目本质就是连续区间逐点加
//   2. 观察重复工作：同一条 booking 在整个区间内不断执行完全相同的 +=seats
//   3. 把 diff 看成事件表，而不是答案表：它只记录当前位置要让当前状态改变多少
//   4. 在 first 对应位置放 +seats，把这份贡献加入 running；从这里开始状态生效
//   5. 中间位置如果 diff=0，就执行 running+=0；由于 running 保留前值，这份贡献自然持续，不需要再次 +=seats
//   6. 在 last 后面的边界放 -seats，把之前加入 running 的同一份贡献移除；从这里开始状态失效
//   7. 多条 booking 的状态可以同时存在，running 始终等于当前位置所有仍处于 active 状态的 seats 之和
//   8. 最后把每个真实航班位置的 running 写进 answer；额外 diff 槽位只用于承接结束事件
//   记忆：diff 记变化，running 记状态。+x 把状态加进来；中间靠 running 自动延续；-x 把同一状态拿出去。真正省掉区间循环的是 running 的继承性。
//
// 5. 为什么不会漏 / 不会重
//   单看一条 booking=[first,last,seats]：在 first 前，running 中没有这份 seats；first 位置的 +seats 把它加入 running。之后只要没有对应的 -seats，哪怕每个中间位置的 diff 都是 0，running[i]=running[i-1]+0 仍会把这份 seats 原样带到下一个位置，因此 first..last 每个航班都会得到它。走过 last 后，-seats 把完全相同的贡献从 running 中移除，后续位置便不再包含它。多条 booking 只是在 running 中同时加入多份独立状态，结束时各自减掉自己的贡献，所以可以线性叠加。
//
// 6. 边界与易错点
//   不要把 diff 当成每个航班最终的座位数，也不要把 -seats 理解成业务上的负座位。diff 是状态变化量，running 才是当前状态。这个技巧成立的关键正是 running 会继承前值：diff=0 不是“当前位置没有座位”，而是“当前位置没有新的状态变化”。另外要区分题面 1-based 航班号和 vector 0-based 下标：真实区间 [first,last] 对应 [first-1,last-1]，关闭事件下标恰好是 last。
//
// 7. 举一反三
//   这其实是一类通用的“事件驱动 + 累积状态”技巧：公交上车时人数加入当前人数、下车后移除；会议开始时 active meeting +1、结束时 -1；扫描线进入区间时加入贡献、离开时删除贡献。共同模式都是：只记录状态变化点，让一个累积变量在没有事件的区间里自动保持状态。若还需要在线查询或更复杂的状态删除，才进一步升级到 Fenwick Tree、Segment Tree 或更一般的扫描线数据结构。
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
        // diff[i] 不是第 i 个航班的答案，而是“走到边界 i 时，当前有效座位数要改变多少”。
        // 多出的 diff[n] 不对应真实航班，只承接“最后一个航班之后停止生效”的结束事件。
        vector<int> diff(n + 1, 0);

        for (const auto& booking : bookings) {
            // 题面航班号是 1-based 闭区间 [first,last]。
            // 转成 0-based 后真实范围是 [first-1,last-1]，因此：
            // 开始事件在 first-1；结束事件在 R+1 = last。
            const int first = booking[0] - 1;
            const int afterLast = booking[1];
            const int seats = booking[2];

            // +seats：从 first 开始把这份贡献加入持续状态；
            // -seats：从 last 后一个边界开始把同一份贡献移除。
            // 当 last==n 时 afterLast==n，正好安全写入额外哨兵槽位 diff[n]。
            diff[first] += seats;
            diff[afterLast] -= seats;
        }

        vector<int> answer(n);

        // running = 扫描到当前位置时，所有“已经开始但尚未结束”的 booking 贡献之和。
        // 它才是当前真实航班的座位总数；diff 只负责告诉 running 在哪里发生变化。
        int running = 0;
        for (int i = 0; i < n; ++i) {
            // diff[i]==0 时 running 原样继承前值，这正是区间内部不必重复 +=seats 的原因。
            running += diff[i];
            answer[i] = running;
        }

        // 只读取 0..n-1 个真实航班；diff[n] 仅用于关闭末尾状态，不属于答案。
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
