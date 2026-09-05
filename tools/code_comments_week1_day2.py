"""Reviewed C++ key-code comments for Week 1 / Day 2.

Entries are added only after an independent per-problem review. Do not fill the
whole learning day mechanically.
"""


CODE_COMMENTS_WEEK1_DAY2 = {
    167: r'''// ---------- Solution ----------
class Solution {
public:
    vector<int> twoSum(vector<int>& numbers, int target) {
        // [left, right] 始终表示“尚未被证明不可能”的候选区间。
        // left < right 同时保证两数来自不同下标，不会复用同一个元素。
        int left = 0, right = static_cast<int>(numbers.size()) - 1;

        while (left < right) {
            const long long sum = static_cast<long long>(numbers[left]) + numbers[right];
            if (sum == target) {
                // 内部指针使用 0-based；题目要求返回 1-based 下标。
                return {left + 1, right + 1};
            }

            if (sum < target) {
                // numbers 已非递减排序，right 已是当前可选的最大值。
                // 连最大值都只能得到 sum < target，则对任何 j <= right：
                // numbers[left] + numbers[j] <= sum < target。
                // 因而 left 不可能参与任何剩余答案，可以把这一整批配对一次排除。
                ++left;
            } else {
                // 对称地，left 已是当前可选的最小值。
                // 连最小值都得到 sum > target，则对任何 i >= left：
                // numbers[i] + numbers[right] >= sum > target。
                // 因而 right 不可能参与任何剩余答案，可以安全排除。
                --right;
            }
        }
        return {};
    }
};''',
}
