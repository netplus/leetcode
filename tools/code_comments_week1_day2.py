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

    15: r'''// ---------- Solution ----------
class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        // 排序不是为了方便输出，而是为了两件事：
        // 1) 固定锚点后，剩余两数具有单调性，可以复用 LC-167 双指针；
        // 2) 相同值会相邻，便于按“值”去掉重复三元组。
        sort(nums.begin(), nums.end());
        vector<vector<int>> answer;
        const int n = static_cast<int>(nums.size());

        for (int i = 0; i < n - 2; ++i) {
            // 相同锚点值会产生完全相同的后续搜索空间。
            // 若不在开局前跳过，像 [-1,...] 的同一个值会重复生成同样的三元组。
            if (i > 0 && nums[i] == nums[i - 1]) continue;

            // 排序后 nums[i] 已是当前及后续三元组中的最小值。
            // 一旦 nums[i] > 0，后面的两个数只会 >= nums[i] > 0，
            // 三数之和不可能再回到 0，因此可以结束整个外层循环，而不只是 continue。
            if (nums[i] > 0) break;

            // 固定 nums[i] 后，问题降维成：
            // 在有序区间 [i+1, n-1] 中寻找两数和 == -nums[i]。
            int left = i + 1, right = n - 1;
            while (left < right) {
                const long long sum = static_cast<long long>(nums[i]) + nums[left] + nums[right];
                if (sum < 0) {
                    // 当前 right 已是最大候选；连它都使总和偏小，
                    // 当前 left 与任何更小/相等的右端配对都不可能成功，所以排除 left。
                    ++left;
                } else if (sum > 0) {
                    // 对称地，当前 left 已是最小候选；连它都使总和偏大，
                    // 当前 right 与任何更大/相等的左端配对都不可能成功，所以排除 right。
                    --right;
                } else {
                    // 此时三个“值”已经确定为一个合法答案。
                    answer.push_back({nums[i], nums[left], nums[right]});

                    // 题目要求的是不重复的值三元组，而不是所有不同下标组合。
                    // 记住本次命中的左右值，然后一次跨过所有相同值；
                    // 否则同一个锚点会反复输出完全相同的三元组。
                    const int lv = nums[left], rv = nums[right];
                    while (left < right && nums[left] == lv) ++left;
                    while (left < right && nums[right] == rv) --right;
                }
            }
        }
        return answer;
    }
};''',
}
