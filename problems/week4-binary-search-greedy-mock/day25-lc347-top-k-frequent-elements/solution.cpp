// ============================================================================
// LC-347：前 K 个高频元素
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 4 周 / 第 25 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给你一个整数数组 nums 和一个整数 k，请你返回其中出现频率前 k 高的元素。
// 你可以按任意顺序返回答案。
//
// 约束与要求：
//   - 1 <= nums.length <= 10^5
//   - -10^4 <= nums[i] <= 10^4
//   - k 的取值范围是 [1, 数组中不相同的元素的个数]
//   - 题目数据保证答案唯一，换句话说，数组中前 k 个高频元素的集合是唯一的
//
// 复杂度目标：O(n log k) 时间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜频次哈希 + 大小为 k 的最小堆
// - 核心要点：
//   1. 思路起点：先统计每个值出现次数；最小堆只保留目前频次最高的 k 个元素，堆顶是候选中最弱者，便于被更强者替换。
//   2. 执行逻辑：1. 哈希统计 value->frequency；2. 每项入堆，若大小超过 k 就弹出最小频次；3. 弹出剩余 k 项作为答案。
//   3. 为什么这样做：处理任意前缀后，堆中是该前缀频次最大的至多 k 项；新项加入后删除 k+1 项中的最小者维持不变量。
// - 边界与易错点：答案顺序任意；最小堆比较 pair 时会用值打破频次平局，不影响合法性；题目保证第 k 高频边界唯一。
// - 举一反三：Top-K 的通用选择是：全排序、大小 k 堆、桶排序或 Quickselect，取决于数据规模与是否流式。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n k。
//   第 2 行：n 个以空格分隔的整数。
//   输出：频率最高的 k 个元素，以空格分隔，顺序任意。
// test.in 的预期输出：1 2
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        unordered_map<int, int> frequency;
        for (int value : nums) ++frequency[value];

        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> top;
        for (auto [value, count] : frequency) {
            top.push({count, value});
            if (static_cast<int>(top.size()) > k) top.pop();
        }
        vector<int> answer;
        while (!top.empty()) {
            answer.push_back(top.top().second);
            top.pop();
        }
        return answer;
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n, k; if (!(cin >> n >> k)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    auto ans = sol.topKFrequent(a, k);
    for (size_t i = 0; i < ans.size(); ++i) cout << ans[i] << " \n"[i + 1 == ans.size()];
    return 0;
}

