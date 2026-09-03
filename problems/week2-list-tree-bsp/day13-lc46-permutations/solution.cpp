// ============================================================================
// LC-46：全排列
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 2 周 / 第 13 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给定一个不含重复数字的数组 nums，返回其所有可能的全排列。
// 你可以按任意顺序返回答案。
//
// 约束与要求：
//   - 1 <= nums.length <= 6
//   - -10 <= nums[i] <= 10
//   - nums 中的所有整数互不相同
//
// 复杂度目标：O(n! * n) 时间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜回溯：路径 + 选择集合
// - 核心要点：
//   1. 思路起点：排列的每一层选择一个尚未使用的元素放到当前位置；used 标记选择状态，path 保存当前前缀。
//   2. 执行逻辑：1. 若 path 长度等于 n 就记录答案；2. 枚举所有未使用下标，做选择并递归；3. 递归返回后撤销 path 与 used。
//   3. 为什么这样做：搜索树第 d 层枚举所有未出现在前缀中的元素，因此每条根到叶路径对应一个唯一排列，反之每个排列都有唯一选择序列。
// - 边界与易错点：撤销顺序必须与选择对应；题目保证元素互异，若有重复值还需排序并做同层去重。
// - 举一反三：回溯统一写成选择、递归、撤销；组合、子集、棋盘搜索只是在选择空间和剪枝条件上不同。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n。
//   第 2 行：n 个以空格分隔的整数。
//   输出：所有排列，每行一个且元素以空格分隔。
// test.in 的预期输出：1 2 3 | 1 3 2 | 2 1 3 | 2 3 1 | 3 1 2 | 3 2 1
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
    vector<vector<int>> answer;
    vector<int> path;
    vector<char> used;

    void search(const vector<int>& nums) {
        if (path.size() == nums.size()) {
            answer.push_back(path);
            return;
        }
        for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
            if (used[i]) continue;
            used[i] = true;
            path.push_back(nums[i]);
            search(nums);
            path.pop_back();       // 撤销本层选择，恢复进入递归前的状态
            used[i] = false;
        }
    }

public:
    vector<vector<int>> permute(vector<int>& nums) {
        answer.clear();
        path.clear();
        used.assign(nums.size(), false);
        search(nums);
        return answer;
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
    auto res = sol.permute(a);
    for (auto& p : res) {
        for (size_t i = 0; i < p.size(); ++i) cout << p[i] << " \n"[i + 1 == p.size()];
    }
    return 0;
}

