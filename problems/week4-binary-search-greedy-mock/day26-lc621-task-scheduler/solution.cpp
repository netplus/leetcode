// ============================================================================
// LC-621：任务调度器
// 难度：中等
// 优先级：P1（进阶）
// 学习进度：第 4 周 / 第 26 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给你一个用字符数组 tasks 表示的 CPU 需要执行的任务列表，用字母 A 到 Z 表示，以及一个冷却时间 n。
// 每个周期或时间间隔允许完成一项任务。
// 任务可以按任何顺序完成，但有一个限制：两个相同种类的任务之间必须有长度为 n 的冷却时间。
// 返回完成所有任务所需要的最短时间间隔。
//
// 题目示例：
// 示例 1：
//   Input: tasks = ["A","A","A","B","B","B"], n = 2
//
//   Output: 8
//
//   Explanation: A possible sequence is: A -> B -> idle -> A -> B -> idle -> A -> B.
//
//   After completing task A, you must wait two intervals before doing A again. The same applies to task B. In the 3^rd interval, neither A nor B can be done, so you idle. By the 4^th interval, you can do A again as 2 intervals have passed.
//
// 示例 2：
//   Input: tasks = ["A","C","A","B","D","B"], n = 1
//
//   Output: 6
//
//   Explanation: A possible sequence is: A -> B -> C -> D -> A -> B.
//
//   With a cooling interval of 1, you can repeat a task after just one other task.
//
// 示例 3：
//   Input: tasks = ["A","A","A", "B","B","B"], n = 3
//
//   Output: 10
//
//   Explanation: A possible sequence is: A -> B -> idle -> idle -> A -> B -> idle -> idle -> A -> B.
//
//   There are only two types of tasks, A and B, which need to be separated by 3 intervals. This leads to idling twice between repetitions of these tasks.
//
// 约束与要求：
//   - 1 <= tasks.length <= 10^4
//   - tasks[i] 是大写英文字母
//   - 0 <= n <= 100
//
// 复杂度目标：O(n) 时间（按频次贪心）。
//
// ----------------------------------------------------------------------------
// 解法精讲｜频率骨架：由最拥挤任务给出调度长度下界
//
// 0. 优化是怎么来的
//   一个直接正确的算法是逐个时间单位模拟：维护每类任务剩余次数和下一次允许执行时间；每一时刻从当前可执行任务里挑剩余次数最多者，否则 idle。这个堆/冷却队列模拟能得到最短调度，但题目只有 26 种、每项耗时固定且冷却规则完全相同，实际保留了远多于答案需要的逐时刻状态。
//
//   要问最短总长度，最紧的约束来自最高频任务。设最高频为 f：它的前 f-1 次出现后面都必须留出 n 个位置，所以形成 f-1 个长度至少 n+1 的完整块；若有 c 种任务都达到频率 f，最后一列需要容纳这 c 个最高频任务，于是骨架下界为 (f-1)(n+1)+c。另一方面任何调度都至少要执行 tasks.size() 个真实任务。其他任务若能填进骨架空位，就不会增加长度；若多到填满所有空位，它们本身把长度推到 tasks.size()。因此答案正好是两者最大值。优化类型：从逐时刻状态模拟压缩为决定答案的频率下界。
//
// 1. 图像直觉
//   tasks = A A A B B B, n=2
//   f=3, c=2
//
//   先放最高频骨架：
//   A _ _ | A _ _ | A
//
//   B 也是最高频，把它放成另一列：
//   A B _ | A B _ | A B
//
//   长度 = (3-1)*(2+1)+2 = 8
//
//   如果还有足够多 C/D/E 去填两个 '_'，
//   就不会再有 idle；此时总长度最多由真实任务数主导。
//
// 2. 一句话核心
//   先用最高频任务算出任何合法调度都绕不过去的冷却骨架，再与真实任务总数取较大值。
//
// 3. 公式 / 不变量
//   f = maxFrequency
//   c = count(frequency == f)
//
//   skeleton = (f - 1) * (n + 1) + c
//   answer = max(tasks.size(), skeleton)
//
//   含义：
//   f-1 个完整间隔块，每块至少 n+1；
//   最后一组不需要再补冷却尾巴，只需放下 c 个并列最高频任务。
//
// 4. 执行步骤
//   1. 统计 26 类任务频次，求最高频 f
//   2. 统计有多少种任务频率同样等于 f，记为 c
//   3. 按 (f-1)*(n+1)+c 计算最高频任务强制撑开的骨架长度
//   4. 返回 max(tasks.size(), skeleton)：任务足够多时填满 idle，任务不足时骨架中的空闲不可避免
//   记忆：最高频任务先把时间轴撑开；空位能填就填，填满后答案就是任务总数。
//
// 5. 为什么不会漏 / 不会重
//   下界一：f 次同类任务之间有 f-1 个间隔，每个间隔至少需要 n 个其他时间单位，因此最高频任务必然撑开相应冷却距离；c 个并列最高频任务共同占据最后一组，得到 skeleton 下界。下界二：所有真实任务都必须执行一次，所以长度至少为 tasks.size()。可构造性方面，先按最高频任务形成这些块，再把其他任务分散填入块内空位；若任务数不超过骨架容量，可以不突破 skeleton，若任务更多，则空位全部被填满且剩余任务继续占据时间单位，不再需要额外 idle，于是达到 tasks.size()。所以两个下界的较大值可达。
//
// 6. 边界与易错点
//   冷却 n 表示两个相同任务之间至少隔 n 个时间单位，因此块长度是 n+1。并列最高频必须加 c，不能只加 1。n=0 时公式自然退化为任务数。这个 O(26+|tasks|) 公式依赖：任务单位时长、任意重排、所有类型共享同一个冷却 n；条件变化时应回到堆 + 时间模拟。
//
// 7. 举一反三
//   与 Day 25 的 Top-K/边界维护不同，这里进一步问‘能否不模拟每一步，只保留决定最优值的紧约束’。遇到调度题先区分：如果只求最短长度且约束高度对称，可尝试频率/容量下界；如果要输出具体顺序、任务时长不同或冷却不同，通常需要堆、队列或事件模拟。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n (任务数量) 冷却时间。
//   第 2 行：由 n 个任务字符组成的字符串。
//   输出：完成所有任务的最少时间单位。
// test.in 的预期输出：8
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    int leastInterval(vector<char>& tasks, int n) {
        // 任务只有 A..Z，先统计最终频次；真正撑开冷却时间轴的是最高频任务。
        array<int, 26> frequency{};
        for (char task : tasks) ++frequency[task - 'A'];

        const int maxFrequency = *max_element(frequency.begin(), frequency.end());
        // 可能有多种任务并列最高频，它们都会占据骨架最后一列，因此不能只按一种最高频任务计算。
        const int maxCount = static_cast<int>(count(frequency.begin(), frequency.end(), maxFrequency));

        // 最高频任务有 maxFrequency 次：前 maxFrequency-1 组之间都必须隔出 n 个冷却位置，
        // 所以形成 (maxFrequency-1) 个长度 n+1 的完整块；最后再放 maxCount 个并列最高频任务。
        const int skeleton = (maxFrequency - 1) * (n + 1) + maxCount;

        // skeleton 是冷却约束强制出的下界；tasks.size() 是“每个真实任务至少占一个时间单位”的下界。
        // 其他任务足够多时会把骨架 idle 全填满，答案由任务数主导；否则不可避免的 idle 让骨架主导。
        return max(static_cast<int>(tasks.size()), skeleton);
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int nt, cooldown; if (!(cin >> nt >> cooldown)) return 0;
    string s; cin >> s;
    vector<char> tasks(s.begin(), s.end());
    cout << sol.leastInterval(tasks, cooldown) << "\n";
    return 0;
}
