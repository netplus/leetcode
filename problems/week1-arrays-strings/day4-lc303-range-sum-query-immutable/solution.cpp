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
// - 核心要点：
//   1. 思路起点：prefix[i] 定义为前 i 个元素之和，并在开头放一个 0；任意闭区间 [left,right] 可由两个前缀相减得到。
//   2. 执行逻辑：1. build 时构造长度 n+1 的 prefix；2. 查询返回 prefix[right+1]-prefix[left]；3. 不修改原数组。
//   3. 为什么这样做：prefix[right+1] 包含 0..right，prefix[left] 包含 0..left-1，相减后恰好只剩目标闭区间。
// - 边界与易错点：最常见错误是 right 少加 1；对象必须先 build；若约束扩大可将接口也改为 long long。
// - 举一反三：静态区间和、频次累计、差分还原以及二维前缀和，本质都是用可逆的累计状态消去重复计算。
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
    vector<long long> prefix;

public:
    void build(vector<int>& nums) {
        prefix.assign(nums.size() + 1, 0);
        for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
            prefix[i + 1] = prefix[i] + nums[i];
        }
    }

    int sumRange(int left, int right) {
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

