// ============================================================================
// LC-303：区域和检索 - 数组不可变
// 难度：简单
// 优先级：P0（必做）
// 学习进度：第 1 周 / 第 4 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给定一个整数数组 nums，处理以下类型的多个查询: 计算索引 left 和 right （包含 left 和 right）之间的 nums 元素的和，其中 left <= right 实现
// NumArray 类：NumArray(int[] nums) 使用数组 nums 初始化对象 int sumRange(int left, int right) 返回数组 nums 中索引 left 和
// right 之间的元素的总和，包含 left 和 right 两点（也就是 nums[left] + nums[left + 1] + ... + nums[right] )
//
// 题目示例：
// 示例 1：
//   Input
//   ["NumArray", "sumRange", "sumRange", "sumRange"]
//   [[[-2, 0, 3, -5, 2, -1]], [0, 2], [2, 5], [0, 5]]
//   Output
//   [null, 1, -1, -3]
//
//   Explanation
//   NumArray numArray = new NumArray([-2, 0, 3, -5, 2, -1]);
//   numArray.sumRange(0, 2); // return (-2) + 0 + 3 = 1
//   numArray.sumRange(2, 5); // return 3 + (-5) + 2 + (-1) = -1
//   numArray.sumRange(0, 5); // return (-2) + 0 + 3 + (-5) + 2 + (-1) = -3
//
// 约束与要求：
//   - 1 <= nums.length <= 10^4
//   - -10^5 <= nums[i] <= 10^5
//   - 0 <= left <= right < nums.length
//   - 最多调用 10^4 次 sumRange 方法
//
// 复杂度目标：O(n) 预处理，O(1) 查询。
//
// ----------------------------------------------------------------------------
// 解法精讲｜一维前缀和
//
// 0. 优化是怎么来的
//   如果 sumRange 只调用一次，最自然也最合适的做法就是直接遍历 [left,right] 求和，时间 O(n)，完全没有必要使用前缀和。
//
//   题目的关键是同一个不可变数组会被查询很多次。不同查询会一次又一次把相同的前缀元素重新相加；这些累计结果与具体查询无关，可以只预处理一次。
//
//   于是把“每次查询现场重复求和”改成“构造时保存每个边界的累计值”。查询 [left,right] 时，两份已保存累计值相减即可消掉共同前缀。
//
//   优化类型：重复计算 -> 预处理换查询时间。构造 O(n)，单次查询从 O(n) 降为 O(1)。
//
// 1. 图像直觉
//   不要先把 prefix[i] 理解成“第 i 个元素的前缀和”，而把它看成数组边界上的累计值：
//
//   nums:       [ a0 ][ a1 ][ a2 ][ a3 ][ a4 ]
//   boundary:  0     1     2     3     4     5
//   prefix:    ^           ^                 ^
//              0        prefix[2]         prefix[5]
//
//   prefix[i] 表示走到边界 i 时，已经跨过了前 i 个元素。
//
//   如果要问闭区间 [2,4]：
//
//   |------ prefix[5]：包含 a0..a4 ------|
//   |-- prefix[2]：包含 a0..a1 --|
//                                     ^
//                            相减后只剩 a2..a4
//
//   这样 right 为什么要写成 right+1 就很自然：right 是元素下标，而 prefix 存在于元素右侧的边界。
//
// 2. 一句话核心
//   先把每个数组边界的累计和存下来；任意区间就是“右边界累计 - 左边界累计”。
//
// 3. 公式 / 不变量
//   prefix[0] = 0
//   prefix[i+1] = prefix[i] + nums[i]
//
//   因此 prefix[i] = nums[0] + ... + nums[i-1]
//
//   闭区间 [left,right]：
//   sum(left,right) = prefix[right+1] - prefix[left]
//
// 4. 执行步骤
//   1. 建立长度 n+1 的 prefix，并令 prefix[0]=0
//   2. 从左到右执行 prefix[i+1]=prefix[i]+nums[i]
//   3. 每次查询 [left,right] 直接返回 prefix[right+1]-prefix[left]
//   记忆：prefix 存边界；区间和 = 右边界累计 - 左边界累计。
//
// 5. 为什么不会漏 / 不会重
//   prefix[right+1] 包含 nums[0..right]，prefix[left] 包含 nums[0..left-1]；
//   两段共同的左侧部分被相减抵消，恰好只留下 nums[left..right]。
//
// 6. 边界与易错点
//   最常见错误是把 right 写成 prefix[right]，本质上是混淆了“元素下标”和“边界下标”；额外的 prefix[0]=0 让 left=0 时无需特殊分支。
//
// 7. 举一反三
//   LC-560 其实就是在扫描过程中动态使用这些边界累计值；LC-304 把一维边界扩成二维矩形边界；LC-1109 的差分数组则可以看成前缀和的逆过程。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n。
//   第 2 行：n 个以空格分隔的整数。
//   第 3 行：q (查询数量)。
//   接下来 q 行：left right。
//   输出：每次查询的区间和，每个结果单独一行。
// test.in 的预期输出：1 | -1 | -3
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
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
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    sol.build(a);
    int q; cin >> q;
    while (q--) {
        int l, r; cin >> l >> r;
        cout << sol.sumRange(l, r) << "\n";
    }
    return 0;
}
