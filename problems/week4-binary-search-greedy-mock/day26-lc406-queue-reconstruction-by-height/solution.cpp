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
// 题目示例：
// 示例 1：
//   Input: people = [[7,0],[4,4],[7,1],[5,0],[6,1],[5,2]]
//   Output: [[5,0],[7,0],[5,2],[6,1],[4,4],[7,1]]
//   Explanation:
//   Person 0 has height 5 with no other people taller or the same height in front.
//   Person 1 has height 7 with no other people taller or the same height in front.
//   Person 2 has height 5 with two persons taller or the same height in front, which is person 0 and 1.
//   Person 3 has height 6 with one person taller or the same height in front, which is person 1.
//   Person 4 has height 4 with four people taller or the same height in front, which are people 0, 1, 2, and 3.
//   Person 5 has height 7 with one person taller or the same height in front, which is person 1.
//   Hence [[5,0],[7,0],[5,2],[6,1],[4,4],[7,1]] is the reconstructed queue.
//
// 示例 2：
//   Input: people = [[6,0],[5,0],[4,0],[3,2],[2,2],[1,4]]
//   Output: [[4,0],[5,0],[2,2],[3,2],[1,4],[6,0]]
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
// 解法精讲｜约束支配顺序：高个先固定，再按 k 插入
//
// 0. 优化是怎么来的
//   一个自然且正确的构造思路是回溯：从尚未安排的人中选一个放到下一位置，维护前方身高计数；若某个部分队列已经违反某人的 k 就剪枝，最终枚举到合法排列。它能覆盖所有可能队列，但最坏仍接近排列枚举，因为大量顺序尝试其实不会影响某些人的约束。
//
//   关键观察是约束有单向依赖：身高 h 的人只被 >=h 的人影响，所有更矮者对他的 k 完全不可见。于是先处理“会影响更多人的”高个，把未来更矮的人从当前约束中排除。处理 [h,k] 时，队列里只包含身高 >=h 的人，所以把他插到位置 k，前面恰有 k 个会计数的人；以后插入更矮者不会破坏这个事实。这把全局排列搜索变成一个可逐步固定且无需回退的贪心构造。优化类型：利用约束支配关系消除无意义排列分支。
//
// 1. 图像直觉
//   people = [[7,0],[4,4],[7,1],[5,0],[6,1],[5,2]]
//
//   先按 h 降序、同 h 按 k 升序：
//   [7,0] [7,1] [6,1] [5,0] [5,2] [4,4]
//
//   逐个按 k 插入：
//   [7,0]
//   [7,0] [7,1]
//   [7,0] [6,1] [7,1]
//   [5,0] [7,0] [6,1] [7,1]
//   [5,0] [7,0] [5,2] [6,1] [7,1]
//   [5,0] [7,0] [5,2] [6,1] [4,4] [7,1]
//
//   插入 5 或 4 时，之后出现的更矮人不会改变已经固定人的 k。
//
// 2. 一句话核心
//   先安排会被当前人计数的高个；当轮到 [h,k] 时，已有队列全是 >=h 的人，因此插入下标 k 就一次性满足它的约束。
//
// 3. 公式 / 不变量
//   排序规则：
//   height 降序；height 相同则 k 升序。
//
//   处理 person=[h,k] 时的不变量：
//   answer 中所有已处理者 height >= h。
//
//   于是插入 answer.begin()+k 后：
//   当前 person 前面恰有 k 个 height >= h 的人。
//
//   以后插入 height < h 的人，不会进入 person 的计数集合。
//
// 4. 执行步骤
//   1. 按身高从高到低排序；同身高的人按 k 从小到大排序
//   2. 从排序后的第一人开始，依次取出 [h,k]
//   3. 把当前人插入 answer 的下标 k；此时已有元素都不矮于当前人
//   4. 处理完所有人后返回 answer；后来的更矮者不会破坏先前已经满足的约束
//   记忆：高个先站好；轮到你时，眼前的人全都算数，所以站到 k 就够了。
//
// 5. 为什么不会漏 / 不会重
//   对处理顺序做归纳。插入当前 [h,k] 前，answer 只含身高 >=h 的人，因此插到下标 k 后，前面恰有 k 个满足 >=h 的人，当前人的条件成立。以后只会插入更矮者，或在同高组中因 k 升序而按合法次序插入；更矮者不计入当前人的 k，而新插入动作只会把已有元素整体右移，不改变它们前面“不矮于自己”的人数。于是每一步固定的约束以后都不会失效。
//
// 6. 边界与易错点
//   两个排序关键字都不能反：高个必须先处理；同高时 k 小者必须先插，否则较大的 k 可能在当前 answer 中还没有合法下标，也会破坏同高计数。vector 中间插入是 O(n)，所以当前实现总时间 O(n^2)，这符合题目规模；不要误写成 O(n log n)。
//
// 7. 举一反三
//   这类贪心的核心是先找“谁的决定不会被后续弱对象影响”。与区间调度按结束时间排序类似，排序不是装饰，而是在制造一条决策一旦落下就不需要回退的支配顺序。若数据规模要求 O(n log n)，可把“找第 k 个空位”换成 Fenwick tree / segment tree。
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
        // 先处理高个：当轮到 [h,k] 时，answer 中所有人都满足 height>=h，
        // 因而“前面有多少个会被当前人计数的人”就等于插入位置本身。
        // 同身高必须按 k 升序，让 k 较小者先固定，后续同高者不会插到它前面破坏其计数。
        sort(people.begin(), people.end(), [](const vector<int>& a, const vector<int>& b) {
            if (a[0] != b[0]) return a[0] > b[0];
            return a[1] < b[1];
        });

        vector<vector<int>> answer;
        for (const auto& person : people) {
            // 此刻已有元素全都不矮于 person，所以插入下标 k 后，
            // person 前方恰有 k 个 height>=person[0] 的人；以后加入的更矮者不计入它的约束。
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
