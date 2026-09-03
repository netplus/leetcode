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
// - 核心要点：
//   1. 思路起点：prefix[r][c] 表示原矩阵左上角 [0,r)×[0,c) 的和；目标矩形用“大矩形减上减左加回重叠”得到。
//   2. 执行逻辑：1. 构造带额外首行首列的 prefix；2. 逐格使用上+左-左上+当前值；3. 查询按四个角做容斥。
//   3. 为什么这样做：上方与左方区域各减一次，而它们的交集被减了两次，必须加回；半开坐标让边界无需分支。
// - 边界与易错点：row2/col2 是闭区间坐标，所以查询中要加 1；漏掉加回左上角会得到系统性偏小结果。
// - 举一反三：二维区域计数、图像积分图和固定矩形卷积都可用二维前缀；更高维仍是同样的容斥思想。
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
    vector<vector<long long>> prefix;

public:
    void build(vector<vector<int>>& matrix) {
        const int rows = static_cast<int>(matrix.size());
        const int cols = static_cast<int>(matrix[0].size());
        prefix.assign(rows + 1, vector<long long>(cols + 1, 0));
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                prefix[r + 1][c + 1] = matrix[r][c] + prefix[r][c + 1]
                                     + prefix[r + 1][c] - prefix[r][c];
            }
        }
    }

    int sumRegion(int row1, int col1, int row2, int col2) {
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

