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
}
