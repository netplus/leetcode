"""Reviewed C++ key-code comments for Week 4 / Day 22.

Entries are added only after an independent per-problem review.
"""

CODE_COMMENTS_WEEK4_DAY22 = {
    704: r'''// ---------- Solution ----------
class Solution {
public:
    int search(vector<int>& nums, int target) {
        // 搜索区间采用闭区间 [left,right]；只要 left<=right，就仍至少有一个候选位置。
        int left = 0, right = static_cast<int>(nums.size()) - 1;
        while (left <= right) {
            // 这样写与 (left+right)/2 等价，但避免两个大下标直接相加可能溢出。
            const int middle = left + (right - left) / 2;
            if (nums[middle] == target) return middle;

            // 数组严格递增：middle 值偏小时，middle 及其左侧都不可能等于 target；
            // 反之 middle 及其右侧都可整体排除。更新后仍保持“若答案存在，它一定在 [left,right]”。
            if (nums[middle] < target) left = middle + 1;
            else right = middle - 1;
        }
        // left>right 表示候选闭区间已经为空。
        return -1;
    }
};''',

    34: r'''// ---------- Solution ----------
class Solution {
    static int firstAtLeast(const vector<int>& nums, long long value) {
        // 搜索半开区间 [left,right)，函数契约是返回第一个 nums[i]>=value 的位置；
        // 若所有元素都更小，则返回 nums.size() 这个合法的“尾后位置”。
        int left = 0, right = static_cast<int>(nums.size());
        while (left < right) {
            int middle = left + (right - left) / 2;
            // nums[middle] < value 时，middle 不可能是答案，连同左侧一起丢弃；
            // 否则 middle 仍可能就是第一个满足位置，所以 right=middle 而不是 middle-1。
            if (nums[middle] < value) left = middle + 1;
            else right = middle;
        }
        return left;
    }

public:
    vector<int> searchRange(vector<int>& nums, int target) {
        // first 是第一个 >=target 的位置；只有它真实等于 target 时，目标才存在。
        int first = firstAtLeast(nums, target);
        if (first == static_cast<int>(nums.size()) || nums[first] != target) return {-1, -1};

        // 第一个 >=target+1 的位置，就是所有 target 的尾后边界；减一得到最后一个 target。
        // 用 long long 做 target+1，避免 target==INT_MAX 时整数溢出。
        int afterLast = firstAtLeast(nums, static_cast<long long>(target) + 1);
        return {first, afterLast - 1};
    }
};''',

    33: r'''// ---------- Solution ----------
class Solution {
public:
    int search(vector<int>& nums, int target) {
        // 仍维护闭区间 [left,right]：若 target 存在，它始终在这个候选区间内。
        int left = 0, right = static_cast<int>(nums.size()) - 1;
        while (left <= right) {
            int middle = left + (right - left) / 2;
            if (nums[middle] == target) return middle;

            // 旋转数组没有重复值，因此 [left,middle] 与 [middle,right] 至少有一半保持严格有序。
            if (nums[left] <= nums[middle]) {
                // 左半段有序，只有 target 落在 [nums[left],nums[middle]) 时才保留左半段；
                // 否则整个左半段都可排除，去右侧继续找。
                if (nums[left] <= target && target < nums[middle]) right = middle - 1;
                else left = middle + 1;
            } else {
                // 否则右半段必有序；同样用它的值域判断 target 是否可能在其中。
                if (nums[middle] < target && target <= nums[right]) left = middle + 1;
                else right = middle - 1;
            }
        }
        return -1;
    }
};''',

    153: r'''// ---------- Solution ----------
class Solution {
public:
    int findMin(vector<int>& nums) {
        // [left,right] 始终包含全局最小值；left<right 时至少还有两个候选位置。
        int left = 0, right = static_cast<int>(nums.size()) - 1;
        while (left < right) {
            int middle = left + (right - left) / 2;

            // 与右端比较能判断 middle 是否落在旋转前的“大值段”。
            // nums[middle] > nums[right]：断点/最小值一定严格在 middle 右侧，所以丢掉 middle。
            if (nums[middle] > nums[right]) left = middle + 1;
            // 否则 middle 位于包含最小值的“小值段”，最小值可能就是 middle，因此保留 middle。
            else right = middle;
        }
        // 区间收缩到一个位置时，该位置就是旋转断点，也就是全局最小值。
        return nums[left];
    }
};''',
}
