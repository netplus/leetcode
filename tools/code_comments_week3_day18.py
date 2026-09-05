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
}
