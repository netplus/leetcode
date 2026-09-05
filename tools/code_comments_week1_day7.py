"""Reviewed C++ key-code comments for Week 1 / Day 7.

Entries are added only after an independent per-problem review.
"""


CODE_COMMENTS_WEEK1_DAY7 = {
    239: r'''// ---------- Solution ----------
class Solution {
public:
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        // candidates 存下标，而不是值：既要比较候选大小，也要知道它何时滑出窗口。
        // 队列中下标递增、对应 nums 值严格递减；因此队首始终是“仍未过期的最强候选”。
        deque<int> candidates;
        vector<int> answer;

        for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
            // 当前窗口的左边界是 i-k+1；下标 <= i-k 已经不属于窗口。
            // 过期只可能从队首发生，因为队列下标始终递增。
            if (!candidates.empty() && candidates.front() <= i - k) {
                candidates.pop_front();
            }

            // 当前 nums[i] 比队尾旧候选更晚过期。
            // 若它还 >= 旧候选值，那么从现在直到旧候选过期前，当前元素都与它同窗且不更小；
            // 等当前元素离开时，旧候选反而早已先过期。因此这些队尾候选已被永久支配，可以删除。
            while (!candidates.empty() && nums[candidates.back()] <= nums[i]) {
                candidates.pop_back();
            }

            // 当前下标在删除所有被支配候选后加入队尾，重新恢复“下标递增、值严格递减”。
            candidates.push_back(i);

            // i==k-1 时第一个完整窗口才形成。
            // 此时队首既未过期，又不小于队内其它候选，所以它就是当前窗口最大值。
            // 这里只读取、不弹出队首，因为同一个最大值还可能继续服务后续窗口。
            if (i >= k - 1) answer.push_back(nums[candidates.front()]);
        }

        return answer;
    }
};''',
}
