"""Reviewed C++ key-code comments for Week 3 / Day 18.

Entries are added only after an independent per-problem review.
"""


CODE_COMMENTS_WEEK3_DAY18 = {
    70: r'''// ---------- Solution ----------
class Solution {
public:
    int climbStairs(int n) {
        // ways[1]=1、ways[2]=2 是滚动递推的两个基线；n<=2 时可直接返回，避免访问不存在的第二状态。
        if (n <= 2) return n;

        // 进入 step=3 前保持不变量：previous2=ways[1]，previous1=ways[2]。
        // 后续进入任意 step=i 时，它们分别代表 ways[i-2] 与 ways[i-1]。
        int previous2 = 1, previous1 = 2;

        for (int step = 3; step <= n; ++step) {
            // 到 step 的最后一步只能来自 step-1 或 step-2，两类方案互斥，因此直接相加。
            // 必须先用两个“旧状态”算出 current，之后才能覆盖 previous2/previous1。
            int current = previous1 + previous2;

            // 整体向前滚动一格：旧 ways[i-1] 成为下一轮的 ways[(i+1)-2]，
            // current 则成为下一轮的 ways[(i+1)-1]。
            previous2 = previous1;
            previous1 = current;
        }

        // 循环结束后 previous1 恰好对应 ways[n]。
        return previous1;
    }
};''',

    746: r'''// ---------- Solution ----------
class Solution {
public:
    int minCostClimbingStairs(vector<int>& cost) {
        // 状态定义：dp[i] = 到达位置 i 的最低累计花费；位置 n 是楼顶这个虚拟位置。
        // 题目允许免费从 0 或 1 开始，所以 dp[0]=dp[1]=0。
        int previous2 = 0, previous1 = 0;

        for (int i = 2; i <= static_cast<int>(cost.size()); ++i) {
            // 进入 i 时：previous2=dp[i-2]，previous1=dp[i-1]。
            // cost[j] 是“从台阶 j 离开”时支付，因此最后一步从 i-1 来要加 cost[i-1]，
            // 从 i-2 来则加 cost[i-2]；两类合法最后来源取较小值。
            int current = min(previous1 + cost[i - 1], previous2 + cost[i - 2]);

            // 与 LC-70 一样，必须先算 current 再覆盖旧状态；否则会丢掉当前递推仍需要的 dp[i-2]/dp[i-1]。
            previous2 = previous1;
            previous1 = current;
        }

        // 循环允许 i==n，所以最终 previous1 对应 dp[n]，也就是已经到达楼顶而非最后一级台阶。
        return previous1;
    }
};''',

    198: r'''// ---------- Solution ----------
class Solution {
public:
    int rob(vector<int>& nums) {
        // previous1 不是“上一间房的钱”，而是前一整个前缀的最优总收益；
        // previous2 则是再少一间房的前缀最优。空前缀的最优收益为 0。
        int previous2 = 0, previous1 = 0;

        for (int money : nums) {
            // 当前房只有两类互斥决策：
            // skip：不偷当前房，直接继承前一前缀最优 previous1；
            // take：偷当前房，则相邻前一房不能偷，只能接 previous2 + money。
            const int skip = previous1;
            const int take = previous2 + money;
            const int current = max(skip, take);

            // 必须先用旧 previous2/previous1 算完 current，再整体向前滚动；
            // 若提前覆盖 previous1，会破坏下一轮对“前两前缀最优”的状态含义。
            previous2 = previous1;
            previous1 = current;
        }

        // 所有房处理完后，previous1 就是整条线性街道的最大合法收益。
        return previous1;
    }
};''',
}
