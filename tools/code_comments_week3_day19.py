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

    64: r'''// ---------- Solution ----------
class Solution {
public:
    int minPathSum(vector<vector<int>>& grid) {
        const int cols = static_cast<int>(grid[0].size());

        // dp[col] 保存“到当前扫描位置对应列的最小累计路径和”。
        // 初始把所有列设为 INT_MAX 表示尚不可达，只把 dp[0]=0 作为左上角之前的虚拟起点。
        vector<int> dp(cols, INT_MAX);
        dp[0] = 0;

        for (const auto& row : grid) {
            // 和 LC-62 一样必须从左向右：更新 col 前，旧 dp[col] 仍代表上方，
            // 而 dp[col-1] 已更新成当前行左方。
            for (int col = 0; col < cols; ++col) {
                if (col == 0) {
                    // 第一列没有左邻，只能从上方到达；第一行第一次执行时，dp[0]=0 正好让
                    // 左上角统一得到 0 + grid[0][0]，之后 dp[0] 就持续累加第一列路径成本。
                    dp[col] = dp[col] + row[col];
                } else {
                    // 内部格只可能从上或左进入：旧 dp[col] 是上方最优，
                    // 新 dp[col-1] 是左方最优，取较小者后再支付当前格固定成本。
                    dp[col] = min(dp[col], dp[col - 1]) + row[col];
                }
            }
        }

        // 所有行完成后，最后一列保存的就是右下角最小路径和。
        return dp[cols - 1];
    }
};''',
}
