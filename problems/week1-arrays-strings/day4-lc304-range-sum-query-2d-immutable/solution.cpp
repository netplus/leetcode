// ============================================================================
// LC-304：二维区域和检索 - 矩阵不可变
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 1 周 / 第 4 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给定一个二维矩阵 matrix，以下类型的多个请求：计算其子矩形范围内元素的总和，该子矩阵的左上角为 (row1, col1)，右下角为 (row2, col2)。
// 实现 NumMatrix 类：NumMatrix(int[][] matrix) 给定整数矩阵 matrix 进行初始化 int sumRegion(int row1, int col1, int
// row2, int col2) 返回左上角 (row1, col1)、右下角 (row2, col2) 所描述的子矩阵的元素总和。
//
// 题目示例：
// 示例 1：
//   Input
//   ["NumMatrix", "sumRegion", "sumRegion", "sumRegion"]
//   [[[[3, 0, 1, 4, 2], [5, 6, 3, 2, 1], [1, 2, 0, 1, 5], [4, 1, 0, 1, 7], [1, 0, 3, 0, 5]]], [2, 1, 4, 3], [1, 1, 2, 2], [1, 2, 2, 4]]
//   Output
//   [null, 8, 11, 12]
//
//   Explanation
//   NumMatrix numMatrix = new NumMatrix([[3, 0, 1, 4, 2], [5, 6, 3, 2, 1], [1, 2, 0, 1, 5], [4, 1, 0, 1, 7], [1, 0, 3, 0, 5]]);
//   numMatrix.sumRegion(2, 1, 4, 3); // return 8 (i.e sum of the red rectangle)
//   numMatrix.sumRegion(1, 1, 2, 2); // return 11 (i.e sum of the green rectangle)
//   numMatrix.sumRegion(1, 2, 2, 4); // return 12 (i.e sum of the blue rectangle)
//
// 约束与要求：
//   - m == matrix.length
//   - n == matrix[i].length
//   - 1 <= m, n <= 200
//   - -10^5 <= matrix[i][j] <= 10^5
//   - 0 <= row1 <= row2 < m
//   - 0 <= col1 <= col2 < n
//   - 最多调用 10^4 次 sumRegion 方法
//
// 复杂度目标：O(mn) 预处理，O(1) 查询。
//
// ----------------------------------------------------------------------------
// 解法精讲｜二维前缀和 + 容斥
//
// 0. 优化是怎么来的
//   二维情况下，最直接的 sumRegion 会遍历查询矩形里的每个单元格；矩形面积越大，单次查询越慢，而且多次查询会反复累加大量重叠区域。
//
//   矩阵本身在构造后不会修改，因此可以把“从左上原点到每个二维边界的矩形和”预处理一次。以后任何目标矩形都不再逐格扫描，而是用四个已经保存的左上矩形做容斥。
//
//   这和 LC-303 完全同源：一维用两个边界累计值消掉公共前缀，二维用四个累计矩形消掉公共区域。
//
//   优化类型：重复计算 -> 二维预处理 + 容斥查询。预处理 O(mn)，单次查询 O(1)。
//
// 1. 图像直觉
//   一维前缀是“从起点到某个边界”的累计；二维前缀只是把线段换成左上角矩形。
//
//   prefix[r][c] 表示原矩阵中：
//   行 [0,r)，列 [0,c) 这个左上矩形的总和。
//
//   查询目标矩形时，可以先拿一个覆盖它的“大左上矩形”：
//
//   +-----------------------+
//   |        TOP            |
//   |   +-------------------|
//   | L |      TARGET       |
//   | E |                   |
//   | F |                   |
//   | T |                   |
//   +---+-------------------+
//
//   大矩形 - TOP - LEFT 会把左上角交叠区减两次，所以必须再加回来一次。
//   这就是二维容斥，不需要死记四项公式。
//
// 2. 一句话核心
//   目标子矩形 = 大左上矩形 - 上方多余部分 - 左侧多余部分 + 被重复减掉的左上交集。
//
// 3. 公式 / 不变量
//   定义：prefix[r][c] = matrix[0..r-1][0..c-1] 的和
//
//   构造：
//   prefix[r+1][c+1]
//   = matrix[r][c]
//   + prefix[r][c+1]
//   + prefix[r+1][c]
//   - prefix[r][c]
//
//   查询闭矩形 (row1,col1) .. (row2,col2)：
//   ans = prefix[row2+1][col2+1]
//       - prefix[row1][col2+1]
//       - prefix[row2+1][col1]
//       + prefix[row1][col1]
//
// 4. 执行步骤
//   1. 建立 (rows+1)×(cols+1) 的 prefix，额外首行首列全部为 0
//   2. 逐格用“上 + 左 - 左上 + 当前值”构造二维累计矩形
//   3. 查询时先取右下边界对应的大矩形
//   4. 减去目标上方和左侧，再加回它们重复减掉的左上交集
//   记忆：大矩形 - 上 - 左 + 左上重叠。
//
// 5. 为什么不会漏 / 不会重
//   大左上矩形覆盖了目标区以及上方、左侧多余区域；减去上方和左侧后，二者交集被减了两次，所以加回一次。
//   每个目标单元最终保留一次，其他单元最终系数为 0。
//
// 6. 边界与易错点
//   row2/col2 是原矩阵闭区间下标，因此映射到 prefix 边界时要 +1；构造式和查询式都存在“减两次、加回一次”的左上交集，漏掉它是最典型错误。
//
// 7. 举一反三
//   这是 LC-303 的二维版本：一维是两个边界相减，二维是四个矩形做容斥。图像积分图、二维频次查询都使用相同思想。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：m n。
//   接下来 m 行：n 个以空格分隔的整数。
//   一行：q (查询数量)。
//   接下来 q 行：row1 col1 row2 col2。
//   输出：每次查询的二维区域和，每个结果单独一行。
// test.in 的预期输出：8
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
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
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int m, n; if (!(cin >> m >> n)) return 0;
    vector<vector<int>> mat(m, vector<int>(n));
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j) cin >> mat[i][j];
    sol.build(mat);
    int q; cin >> q;
    while (q--) {
        int r1, c1, r2, c2; cin >> r1 >> c1 >> r2 >> c2;
        cout << sol.sumRegion(r1, c1, r2, c2) << "\n";
    }
    return 0;
}
