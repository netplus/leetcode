// ============================================================================
// LC-406：根据身高重建队列
// 难度：中等
// 优先级：P1（进阶）
// 学习进度：第 4 周 / 第 26 天
// ----------------------------------------------------------------------------
// 题目描述：
// 假设有打乱顺序的一群人站成一个队列，数组 people 表示队列中一些人的属性（不一定按顺序）。
// 每个 people[i] = [h_i, k_i] 表示第 i 个人的身高为 h_i，前面正好有 k_i 个身高大于或等于 h_i 的人。
// 请你重新构造并返回输入数组 people 所表示的队列。
// 返回的队列应该格式化为数组 queue，其中 queue[j] = [h_j, k_j] 是队列中第 j 个人的属性（queue[0] 是排在队列前面的人）。
//
// 约束与要求：
//   - 1 <= people.length <= 2000
//   - 0 <= h_i <= 10^6
//   - 0 <= k_i < people.length
//   - 题目数据确保队列可以被重建
//
// 复杂度目标：O(n^2) 或 O(n log n)。
//
// ----------------------------------------------------------------------------
// 解法精讲｜高个优先排序 + 按 k 插入
// - 核心要点：
//   1. 思路起点：先处理更高的人时，当前队列中的人都会计入新人的“前方不矮人数”；把新人插到下标 k 就能立即满足其约束。
//   2. 执行逻辑：1. 按身高降序、同高按 k 升序排序；2. 依次把 person 插入 answer.begin()+k；3. 返回构造队列。
//   3. 为什么这样做：插入时前面恰有 k 个已处理且身高不低于当前者；以后插入的更矮者不影响该计数。同高按 k 升序确保插入位置有效且计数正确。
// - 边界与易错点：排序规则两关键字都不可反；vector 中间插入使总复杂度 O(n^2)；题目保证存在合法重建。
// - 举一反三：先安排对后续更“有约束力”的对象，再让弱对象填空，是重建队列和嵌套调度中的常用贪心。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n。
//   接下来 n 行：h k。
//   输出：重建后的队列，每行输出一组 "h k"。
// test.in 的预期输出：5 0 | 7 0 | 5 2 | 6 1 | 4 4 | 7 1
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    vector<vector<int>> reconstructQueue(vector<vector<int>>& people) {
        sort(people.begin(), people.end(), [](const vector<int>& a, const vector<int>& b) {
            if (a[0] != b[0]) return a[0] > b[0];
            return a[1] < b[1];
        });
        vector<vector<int>> answer;
        for (const auto& person : people) {
            answer.insert(answer.begin() + person[1], person);
        }
        return answer;
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<vector<int>> p(n, vector<int>(2));
    for (int i = 0; i < n; ++i) cin >> p[i][0] >> p[i][1];
    auto res = sol.reconstructQueue(p);
    for (auto& x : res) cout << x[0] << " " << x[1] << "\n";
    return 0;
}
