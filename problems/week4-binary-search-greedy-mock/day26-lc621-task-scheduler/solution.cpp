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
// 约束与要求：
//   - 1 <= tasks.length <= 10^4
//   - tasks[i] 是大写英文字母
//   - 0 <= n <= 100
//
// 复杂度目标：O(n) 时间（按频次贪心）。
//
// ----------------------------------------------------------------------------
// 解法精讲｜最高频任务构造骨架
// - 核心要点：
//   1. 思路起点：频次最高任务之间需要 maxFrequency-1 个完整间隔，每段长度至少 n+1；并列最高频任务占据骨架各列，最后一组无需冷却尾巴。
//   2. 执行逻辑：1. 统计 26 类任务频次；2. 求最高频 maxFrequency 及达到它的类别数 maxCount；3. 答案取任务总数与 (maxFrequency-1)*(n+1)+maxCount 的较大值。
//   3. 为什么这样做：当空闲槽足够时骨架下界可构造达到；当其他任务填满所有槽时不存在空闲，实际长度就是任务数。两种下界取大覆盖全部情况。
// - 边界与易错点：冷却是相同任务之间至少 n 个区间；多个最高频任务要加 maxCount 而不是 1；n=0 时公式自然退化。
// - 举一反三：频次骨架适合单位时长且任务可任意重排；若任务时长/冷却不同，应改用堆加时间模拟。
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
        array<int, 26> frequency{};
        for (char task : tasks) ++frequency[task - 'A'];
        const int maxFrequency = *max_element(frequency.begin(), frequency.end());
        const int maxCount = static_cast<int>(count(frequency.begin(), frequency.end(), maxFrequency));
        const int skeleton = (maxFrequency - 1) * (n + 1) + maxCount;
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

