// ============================================================================
// LC-438：找到字符串中所有字母异位词
// 难度：中等
// 优先级：P1（进阶）
// 学习进度：第 1 周 / 第 3 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给定两个字符串 s 和 p，找到 s 中所有 p 的异位词的子串，返回这些子串的起始索引。
// 不考虑答案输出的顺序。
//
// 约束与要求：
//   - 1 <= s.length, p.length <= 3 * 10^4
//   - s 和 p 仅包含小写英文字母
//
// 复杂度目标：O(|s|) 时间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜定长滑动窗口 + 频次差
//
// 1. 图像直觉
//   把 p 看成一张“欠账表”。例：p = "abc"：
//
//   need[a]=1, need[b]=1, need[c]=1, missing=3
//
//   当窗口依次收入 c、b、a：
//   收入 c：欠 c 的 1 份被还掉，missing 3->2
//   收入 b：missing 2->1
//   收入 a：missing 1->0
//
//   窗口长度又恰好等于 |p|=3，于是这个窗口一定就是 p 的一个异位词。
//
//   need 可以变成负数：负数表示某字符“收多了”。窗口向右滑时始终做两件事：右边进一个，左边出一个。
//
// 2. 一句话核心
//   固定长度 |p| 的窗口维护一张字符欠账表；当总欠账 missing 为 0 时，等长窗口的字符频次就与 p 完全一致。
//
// 3. 公式 / 不变量
//   need[c] > 0：窗口还欠 c
//   need[c] = 0：c 的配额刚好
//   need[c] < 0：c 在窗口里有富余
//
//   字符进入窗口：
//   if (need[in] > 0) missing--;
//   need[in]--;
//
//   字符离开窗口：
//   need[out]++;
//   if (need[out] > 0) missing++;
//
//   窗口长度 == |p| 且 missing == 0 => 异位词。
//
// 4. 执行步骤
//   1. 统计 p 的字符频次到 need，并令 missing=|p|
//   2. right 每次收入 s[right]：若它正在欠账则 missing--，然后 need[in]--
//   3. 若窗口长度超过 |p|，从 left 移出一个字符并恢复它的 need
//   4. 窗口长度恰为 |p| 且 missing==0 时，记录 left
//   记忆：右边进一个、左边出一个；长度固定，missing==0 就命中。
//
// 5. 为什么不会漏 / 不会重
//   missing 统计的是尚未满足的字符槽位总数。固定窗口长度等于 |p| 时，如果 missing=0，说明 p 要求的每一份字符都已满足；
//   窗口总字符数又没有额外空间，因此不可能还有某字符多出来而另一个字符不够，频次向量必然完全相同。
//
// 6. 边界与易错点
//   need 允许为负，不要把负数误判为错误；字符离开时要先 ++need[out]，再判断是否重新产生欠账；
//   p 比 s 长时自然不会形成长度 |p| 的窗口。
//
// 7. 举一反三
//   LC-76 使用完全相同的 need/missing 欠账模型，只是 LC-438 的窗口长度固定，而 LC-76 在欠账清零后会主动收缩以求最短覆盖。
//   先理解 LC-438，再看 LC-76 会更顺。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：s。
//   第 2 行：p。
//   输出：所有起始下标，以空格分隔并升序排列。
// test.in 的预期输出：0 6
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    vector<int> findAnagrams(string s, string p) {
        array<int, 26> need{};
        for (char c : p) ++need[c - 'a'];

        vector<int> answer;
        int missing = static_cast<int>(p.size());
        int left = 0;
        for (int right = 0; right < static_cast<int>(s.size()); ++right) {
            int in = s[right] - 'a';
            if (need[in] > 0) --missing;
            --need[in];

            if (right - left + 1 > static_cast<int>(p.size())) {
                int out = s[left++] - 'a';
                ++need[out];
                if (need[out] > 0) ++missing;
            }
            if (right - left + 1 == static_cast<int>(p.size()) && missing == 0) {
                answer.push_back(left);
            }
        }
        return answer;
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    string s, p; if (!(cin >> s >> p)) return 0;
    auto ans = sol.findAnagrams(s, p);
    for (size_t i = 0; i < ans.size(); ++i) cout << ans[i] << " \n"[i + 1 == ans.size()];
    return 0;
}
