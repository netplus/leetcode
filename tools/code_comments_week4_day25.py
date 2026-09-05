"""Reviewed C++ key-code comments for Week 4 / Day 25.

Entries are added only after an independent per-problem review.
"""

CODE_COMMENTS_WEEK4_DAY25 = {
    215: r'''// ---------- Solution ----------
class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {
        // “第 k 大”在完整升序序列中的下标是 n-k；重复值仍按出现次数参与排名。
        const int targetIndex = static_cast<int>(nums.size()) - k;

        // nth_element 只保证 targetIndex 这个排名位置放上完整排序后应在这里的元素：
        // 左侧元素不大于它、右侧元素不小于它，但两侧内部都不保证有序。
        // 因此它避免了 sort 为其余 n-1 个位置建立无用的完整全序，并会原地修改 nums。
        nth_element(nums.begin(), nums.begin() + targetIndex, nums.end());
        return nums[targetIndex];
    }
};''',

    347: r'''// ---------- Solution ----------
class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        // 必须先完成最终频率统计；若边扫描 nums 边做 Top-K，某个 value 后续还会继续增频，
        // 当前“弱者”并不一定真的弱，不能安全淘汰。
        unordered_map<int, int> frequency;
        for (int value : nums) ++frequency[value];

        // top 是大小最多为 k 的最小堆，pair.first=count；
        // 堆顶始终是“当前 Top-K 候选里最弱的一个”，方便候选过多时 O(log k) 淘汰门槛元素。
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> top;
        for (auto [value, count] : frequency) {
            top.push({count, value});
            // 新候选加入后若有 k+1 个，只需删除其中频率最低者；
            // 被删元素不可能属于已处理元素的 Top-K，且最终频率已固定，以后也不会重新变强。
            if (static_cast<int>(top.size()) > k) top.pop();
        }

        vector<int> answer;
        // 题目允许任意顺序，所以直接逐个弹出剩余 k 个候选即可；无需再排序。
        while (!top.empty()) {
            answer.push_back(top.top().second);
            top.pop();
        }
        return answer;
    }
};''',

    23: r'''// ---------- Solution ----------
class Solution {
public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        // priority_queue 默认是大根堆；比较器返回 a->val>b->val 后，值更小的节点优先出队。
        auto greaterNode = [](ListNode* a, ListNode* b) { return a->val > b->val; };
        priority_queue<ListNode*, vector<ListNode*>, decltype(greaterNode)> smallest(greaterNode);

        // 每条有序链只暴露一个 frontier：当前最小的未输出节点。
        // 同一链更后的节点被这个头节点挡住，没必要提前全部入堆。
        for (ListNode* head : lists) if (head) smallest.push(head);

        ListNode dummy;
        ListNode* tail = &dummy;
        while (!smallest.empty()) {
            // 所有链各自的 frontier 中，堆顶就是全局最小的剩余节点。
            ListNode* node = smallest.top();
            smallest.pop();

            // node 被输出后，只有它所属链的 frontier 发生变化；
            // 将 node->next 补入堆即可恢复“每条未耗尽链恰暴露一个头”的不变量。
            if (node->next) smallest.push(node->next);
            tail->next = node;
            tail = node;
        }
        return dummy.next;
    }
};''',

    295: r'''// ---------- Solution ----------
class Solution {
    // lower 保存较小一半，使用大根堆以 O(1) 暴露左半最大值；
    // upper 保存较大一半，使用小根堆以 O(1) 暴露右半最小值。
    priority_queue<int> lower;
    priority_queue<int, vector<int>, greater<int>> upper;

public:
    void addNum(int num) {
        // 先按当前分割线放到正确一侧，保持任意 lower 元素 <= 任意 upper 元素。
        if (lower.empty() || num <= lower.top()) lower.push(num);
        else upper.push(num);

        // 再恢复大小不变量：lower 与 upper 等大，或 lower 恰好多一个。
        // 搬运的必须是靠近中间分割线的边界元素，才能同时保持两侧的值域顺序。
        if (lower.size() > upper.size() + 1) {
            upper.push(lower.top());
            lower.pop();
        } else if (upper.size() > lower.size()) {
            lower.push(upper.top());
            upper.pop();
        }
    }

    double findMedian() {
        // 奇数总量时，多出的一个元素按约定在 lower，中位数就是左半最大值。
        if (lower.size() > upper.size()) return lower.top();

        // 偶数时中位数是两个分割边界的平均；先提升到 long long 再相加，避免 int 溢出。
        return (static_cast<long long>(lower.top()) + upper.top()) / 2.0;
    }
};''',
}
