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
                // 第一列没有左邻，只能从上方累计；col>0 时才有“上/左两来源取 min”。
                // 第一行第一次访问 col=0 时，dp[0]=0 正好充当左上角之前的虚拟起点。
                if (col == 0) dp[col] = dp[col] + row[col];
                else dp[col] = min(dp[col], dp[col - 1]) + row[col];
            }
        }

        // 所有行完成后，最后一列保存的就是右下角最小路径和。
        return dp[cols - 1];
    }
};''',

    300: r'''// ---------- Solution ----------
class Solution {
public:
    int lengthOfLIS(vector<int>& nums) {
        // tails[len-1] 表示：当前已扫描前缀中，长度恰为 len 的严格递增子序列
        // 所能取得的最小结尾值。它是“各长度的最优摘要”，不保证整个 tails 本身对应同一条真实 LIS。
        vector<int> tails;

        for (int value : nums) {
            // tails 严格递增，因此可以二分定位第一个 >= value 的位置。
            // 严格递增必须用 lower_bound：相等值只能改善同一长度的结尾，不能把长度延长。
            auto position = lower_bound(tails.begin(), tails.end(), value);

            // 若 value 大于所有已有最小结尾，它可以接在当前最长序列之后，LIS 长度真正 +1；
            // 否则只把同一长度的结尾改善成更小/相同的 value，为未来留下更多可接续空间，长度不变。
            if (position == tails.end()) tails.push_back(value);
            else *position = value;
        }

        // tails 的槽位数与“当前存在多少种长度 1..L”一一对应，因此 size 就是 LIS 长度。
        return static_cast<int>(tails.size());
    }
};''',

    1143: r'''// ---------- Solution ----------
class Solution {
public:
    int longestCommonSubsequence(string text1, string text2) {
        // dp[j] 表示当前扫描进度下，text1 前缀与 text2 前 j 个字符的 LCS 长度。
        // 额外第 0 列恒为 0，对应任意字符串与空前缀的 LCS 长度为 0。
        vector<int> dp(text2.size() + 1, 0);

        for (char a : text1) {
            // 每进入 text1 的新一行，diagonal 从 0 开始，表示二维表这一行最左侧的旧左上角 LCS[i-1][0]。
            int diagonal = 0;

            for (int j = 1; j <= static_cast<int>(text2.size()); ++j) {
                // 覆盖 dp[j] 之前必须先保存它：此刻它仍是上一行同列“上方”状态，
                // 而下一列会把这个旧值当作自己的“左上角”。
                int oldAbove = dp[j];

                // 更新当前格之前三个状态的时间版本分别是：
                // diagonal = 旧左上；dp[j] = 旧上方；dp[j-1] = 已更新的当前行左方。
                if (a == text2[j - 1]) dp[j] = diagonal + 1;
                else dp[j] = max(dp[j], dp[j - 1]);

                // 当前格完成后，才把原来的上方 oldAbove 交给 diagonal，
                // 使它在下一列恰好代表新的“旧左上”。顺序提前会丢失二维依赖。
                diagonal = oldAbove;
            }
        }

        // 扫描完两个完整前缀后，最后一列就是两个完整字符串的 LCS 长度。
        return dp.back();
    }
};''',
}
