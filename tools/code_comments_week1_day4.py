"""Reviewed C++ key-code comments for Week 1 / Day 4.

Entries are added only after an independent per-problem review.
"""


CODE_COMMENTS_WEEK1_DAY4 = {
    303: r'''// ---------- Solution ----------
class Solution {
    // prefix[i] 不对应“下标 i 的元素”，而对应数组边界 i：
    // 它保存 nums[0..i-1] 的和。因此 prefix 比 nums 多一个起始边界 0。
    vector<long long> prefix;

public:
    void build(vector<int>& nums) {
        // n+1 个边界；prefix[0]=0 表示还没有跨过任何元素。
        // 这个空前缀让 left==0 的查询也能直接套统一公式，无需特殊分支。
        prefix.assign(nums.size() + 1, 0);
        for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
            // 从边界 i 跨过 nums[i] 到达边界 i+1，累计状态只需在上一前缀上加一个元素。
            prefix[i + 1] = prefix[i] + nums[i];
        }
    }

    int sumRange(int left, int right) {
        // 题目查询的是闭区间 [left,right]：
        // prefix[right+1] 包含 nums[0..right]，prefix[left] 包含 nums[0..left-1]；
        // 相减抵消共同前缀，恰好留下 nums[left..right]。
        // right+1 是“元素 right 右侧的边界”，不是额外包含一个数组元素。
        return static_cast<int>(prefix[right + 1] - prefix[left]);
    }
};''',

    304: r'''// ---------- Solution ----------
class Solution {
    // prefix[r][c] 表示原矩阵半开矩形 [0,r) × [0,c) 的元素和。
    // 因而 prefix 比原矩阵多一行一列“0 边界”，把 r==0 / c==0 的情况统一进同一公式。
    vector<vector<long long>> prefix;

public:
    void build(vector<vector<int>>& matrix) {
        const int rows = static_cast<int>(matrix.size());
        const int cols = static_cast<int>(matrix[0].size());
        prefix.assign(rows + 1, vector<long long>(cols + 1, 0));

        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                // 要得到以 (r,c) 为右下新格子的左上累计矩形：
                // “上方矩形 + 左侧矩形”会把共同的左上区域算两次，
                // 所以减掉 prefix[r][c] 一次，再加上当前 matrix[r][c]。
                // r+1/c+1 表示跨过当前元素后的二维边界。
                prefix[r + 1][c + 1] = matrix[r][c] + prefix[r][c + 1]
                                     + prefix[r + 1][c] - prefix[r][c];
            }
        }
    }

    int sumRegion(int row1, int col1, int row2, int col2) {
        // 题目给的是闭矩形 [row1..row2] × [col1..col2]，
        // 在 prefix 边界中右下角要映射成 (row2+1,col2+1)。
        // 先取覆盖目标的大左上矩形，再减去“目标上方”和“目标左侧”；
        // 两块被减区域的左上交集被减了两次，因此最后必须加回一次。
        long long sum = prefix[row2 + 1][col2 + 1] - prefix[row1][col2 + 1]
                      - prefix[row2 + 1][col1] + prefix[row1][col1];
        return static_cast<int>(sum);
    }
};''',
}
