"""Reviewed C++ key-code comments for Week 3 / Day 19.

Entries are added only after an independent per-problem review.
"""


CODE_COMMENTS_WEEK3_DAY19 = {
    62: r'''// ---------- Solution ----------
class Solution {
public:
    int uniquePaths(int m, int n) {
        // 第一行每个格子都只能一路向右到达，因此初始化全 1；
        // 后续每一行的 dp[0] 也会一直保持 1，对应第一列只能一路向下。
        vector<int> dp(n, 1);

        for (int row = 1; row < m; ++row) {
            // 必须从左向右更新：处理 (row,col) 之前，
            // dp[col] 仍是上一行同列 ways[row-1][col]，
            // dp[col-1] 已更新成当前行左邻 ways[row][col-1]。
            for (int col = 1; col < n; ++col) {
                // 到当前格的路径按最后一步只分成“从上来”和“从左来”两类，所以两者相加。
                dp[col] += dp[col - 1];
            }
        }

        // 最后一行处理结束后，dp[n-1] 就是右下角格子的路径数。
        return dp[n - 1];
    }
};''',
}
