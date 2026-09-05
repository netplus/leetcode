"""Reviewed C++ key-code comments for Week 4 / Day 23.

Entries are added only after an independent per-problem review.
"""

CODE_COMMENTS_WEEK4_DAY23 = {
    875: r'''// ---------- Solution ----------
class Solution {
public:
    int minEatingSpeed(vector<int>& piles, int h) {
        // 速度至少为 1；取最大堆大小时，每堆最多一小时，因此一定可行。
        // 所以最小可行速度必位于闭区间 [1,maxPile]。
        int left = 1, right = *max_element(piles.begin(), piles.end());

        // canFinish(speed) 是答案轴上的单调谓词：速度越大，总耗时只会不增。
        auto canFinish = [&](int speed) {
            long long hours = 0;
            for (int pile : piles) {
                // 每一堆必须单独向上取整：一个小时只能处理当前这一堆，
                // 吃完后剩余的“小时容量”不能转移到下一堆。
                // 先提升 pile 到 long long，避免 pile+speed-1 的 int 加法溢出。
                hours += (static_cast<long long>(pile) + speed - 1) / speed;

                // 一旦已经超过 h，后面的堆只会继续增加耗时，可以提前判定不可行。
                if (hours > h) return false;
            }
            return true;
        };

        // 可行速度构成连续后缀 F...F | T...T，寻找第一个 true。
        while (left < right) {
            int middle = left + (right - left) / 2;
            // middle 可行时它仍可能就是最小答案，所以保留 middle；
            // 不可行时所有更小速度同样不可行，可一次全部丢弃。
            if (canFinish(middle)) right = middle;
            else left = middle + 1;
        }
        return left;
    }
};''',

    1011: r'''// ---------- Solution ----------
class Solution {
public:
    int shipWithinDays(vector<int>& weights, int days) {
        // 容量不能小于最重单件；容量等于总重量时一天即可运完，一定可行。
        long long left = *max_element(weights.begin(), weights.end());
        long long right = accumulate(weights.begin(), weights.end(), 0LL);

        auto daysNeeded = [&](long long capacity) {
            int used = 1;
            long long load = 0;
            for (int weight : weights) {
                // 包裹顺序不能改变。固定 capacity 后，当前天能继续装就继续装；
                // 若提前换天，只会浪费当前天剩余容量，不可能减少总天数。
                if (load + weight > capacity) {
                    ++used;
                    load = 0;
                }
                // 若刚开启新的一天，触发换天的这个包裹就是新一天的第一件，不能漏掉。
                load += weight;
            }
            // 这是固定 capacity 下按原顺序运输所需的最少天数。
            return used;
        };

        // capacity 越大，daysNeeded 只会不增，所以 feasible(capacity)=[daysNeeded<=days]
        // 呈 F...F | T...T；二分寻找第一个可行容量。
        while (left < right) {
            long long middle = left + (right - left) / 2;
            if (daysNeeded(middle) <= days) right = middle;
            else left = middle + 1;
        }
        return static_cast<int>(left);
    }
};''',

    410: r'''// ---------- Solution ----------
class Solution {
public:
    int splitArray(vector<int>& nums, int k) {
        // 最大段和至少要容纳最大的单个元素；把整个数组作为一段时，总和一定可行。
        long long left = *max_element(nums.begin(), nums.end());
        long long right = accumulate(nums.begin(), nums.end(), 0LL);

        auto groupsNeeded = [&](long long limit) {
            int groups = 1;
            long long current = 0;
            for (int value : nums) {
                // nums 非负。固定 limit 后，当前段能继续放就不提前切；
                // 只有加入 value 会超限时才切，得到该 limit 下所需的最少连续段数。
                if (current + value > limit) {
                    ++groups;
                    current = 0;
                }
                // 触发切分的 value 属于新段，必须在清零后继续计入。
                current += value;
            }
            return groups;
        };

        // limit 越大，最少段数只会不增，因此 groupsNeeded(limit)<=k 构成可行后缀。
        // 题目虽要求“恰好 k 段”，这里检查 <=k 仍正确：若只需少于 k 段，
        // 因为 k<=n 且 nums 非负，可以继续把非空段拆开，最大段和不会增加。
        while (left < right) {
            long long middle = left + (right - left) / 2;
            if (groupsNeeded(middle) <= k) right = middle;
            else left = middle + 1;
        }
        return static_cast<int>(left);
    }
};''',

    4: r'''// ---------- Solution ----------
class Solution {
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        // 始终在较短数组上二分 cut1。这样 cut1∈[0,m] 时，
        // 由固定左侧总元素数推导出的 cut2 也会落在 [0,n]，避免额外越界分支。
        if (nums1.size() > nums2.size()) return findMedianSortedArrays(nums2, nums1);
        const int m = static_cast<int>(nums1.size());
        const int n = static_cast<int>(nums2.size());

        // cut1/cut2 表示“左半部分各取多少个元素”，不是数组下标。
        // +1 让总长度为奇数时，多出来的那个中位元素统一放在左半部分。
        const int leftSize = (m + n + 1) / 2;
        int left = 0, right = m;

        while (left <= right) {
            const int cut1 = left + (right - left) / 2;
            const int cut2 = leftSize - cut1;

            // 切口落在数组端点时，用 ±∞ 作为“空侧”的虚拟边界，
            // 这样所有切法都能使用同一组交叉比较，不必为首尾单独分支。
            const int left1 = cut1 == 0 ? INT_MIN : nums1[cut1 - 1];
            const int right1 = cut1 == m ? INT_MAX : nums1[cut1];
            const int left2 = cut2 == 0 ? INT_MIN : nums2[cut2 - 1];
            const int right2 = cut2 == n ? INT_MAX : nums2[cut2];

            // 两个交叉不等式同时成立，才说明“合并后的所有左侧元素 <= 所有右侧元素”。
            if (left1 <= right2 && left2 <= right1) {
                const int leftMax = max(left1, left2);
                // 奇数时左半多一个元素，中位数就是左侧最大值。
                if ((m + n) % 2 == 1) return leftMax;
                const int rightMin = min(right1, right2);
                // 偶数时取两个中间边界的平均；先提升为 long long，避免两个 int 相加溢出。
                return (static_cast<long long>(leftMax) + rightMin) / 2.0;
            }

            // left1>right2 表示 nums1 左侧拿得太多，cut1 必须左移；
            // 否则必是 left2>right1，说明 nums1 左侧拿得太少，cut1 必须右移。
            if (left1 > right2) right = cut1 - 1;
            else left = cut1 + 1;
        }
        return 0.0;  // 两个输入有序且总长度非零时不会到达
    }
};''',
}
