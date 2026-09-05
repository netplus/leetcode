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

    1109: r'''// ---------- Solution ----------
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
};''',

    238: r'''// ---------- Solution ----------
class Solution {
public:
    vector<int> productExceptSelf(vector<int>& nums) {
        // answer 最终承载输出，因此先借它暂存每个位置的“左侧乘积”，
        // 不再额外申请一张 prefix 数组。
        vector<int> answer(nums.size(), 1);

        // 进入下标 i 时，leftProduct 恰好等于 nums[0..i-1] 的乘积，
        // 明确不包含 nums[i]；初始 1 是空乘积的单位元。
        int leftProduct = 1;
        for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
            // 必须先把“不含自己”的左侧乘积写给 answer[i]，
            // 再把 nums[i] 纳入状态，供下一位置 i+1 使用；顺序反了就会把自己乘进去。
            answer[i] = leftProduct;
            leftProduct *= nums[i];
        }

        // 从右往左完全对称：进入 i 时，rightProduct 只包含 nums[i+1..n-1]。
        int rightProduct = 1;
        for (int i = static_cast<int>(nums.size()) - 1; i >= 0; --i) {
            // answer[i] 此时已经是左侧乘积，先乘当前“不含自己”的右侧乘积，
            // 才得到最终的 except-self 结果；随后再把 nums[i] 纳入右侧滚动状态。
            answer[i] *= rightProduct;
            rightProduct *= nums[i];
        }

        // 整个过程从未做除法，所以 nums 中含 0 也无需特殊分支。
        return answer;
    }
};''',
}
