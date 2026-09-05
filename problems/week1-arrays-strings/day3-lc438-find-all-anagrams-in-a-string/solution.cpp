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
// 题目示例：
// 示例 1：
//   Input: s = "cbaebabacd", p = "abc"
//   Output: [0,6]
//   Explanation:
//   The substring with start index = 0 is "cba", which is an anagram of "abc".
//   The substring with start index = 6 is "bac", which is an anagram of "abc".
//
// 示例 2：
//   Input: s = "abab", p = "ab"
//   Output: [0,1,2]
//   Explanation:
//   The substring with start index = 0 is "ab", which is an anagram of "ab".
//   The substring with start index = 1 is "ba", which is an anagram of "ab".
//   The substring with start index = 2 is "ab", which is an anagram of "ab".
//
// 约束与要求：
//   - 1 <= s.length, p.length <= 3 * 10^4
//   - s 和 p 仅包含小写字母
//
// 复杂度目标：O(|s|) 时间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜定长滑动窗口 + 频次差
//
// 前置概念
//   异位词（anagram）要求两个字符串包含完全相同的字符及其出现次数，字符顺序可以不同。
//
//   例如 "abc" 与 "cba" 是异位词；"abb" 不是 "abc" 的异位词，因为字符频次不同。
//
//   因此一个窗口要成为 p 的异位词，必须同时满足：窗口长度等于 |p|，并且每个字符的频次与 p 完全一致。
//
// 0. 优化是怎么来的
//   最直接的做法，是对 s 中每个长度 |p| 的子串重新统计 26 个字符频次，再与 p 比较。相邻窗口只有两个字符不同，却把中间 |p|-1 个字符反复统计；按窗口长度重新计数会造成大量重复工作。
//
//   滑动一格时真正发生的变化只有两个事件：右边进入一个字符、左边离开一个字符。因此只要维护一张可增量更新的 need 账本，就可以把上一窗口的状态直接延续到下一窗口，而不是从零重建频次。
//
//   missing 再把“检查 26 个字符是否全部满足”压缩成一个总欠账数，使每次窗口移动只做 O(1) 状态更新。
//
//   优化类型：重复计算 -> 相邻窗口增量维护 + 摘要状态。
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
//   missing 统计的是尚未满足的字符槽位总数。固定窗口长度等于 |p| 时，如果 missing=0，说明 p 要求的每一份字符都已满足；窗口总字符数又没有额外空间，因此不可能还有某字符多出来而另一个字符不够，频次向量必然完全相同。
//
// 6. 边界与易错点
//   need 允许为负，不要把负数误判为错误；字符离开时要先 ++need[out]，再判断是否重新产生欠账；p 比 s 长时自然不会形成长度 |p| 的窗口。
//
// 7. 举一反三
//   LC-76 使用完全相同的 need/missing 欠账模型，只是 LC-438 的窗口长度固定，而 LC-76 在欠账清零后会主动收缩以求最短覆盖。先理解 LC-438，再看 LC-76 会更顺。
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
        // need[c] 是一张“欠账表”：初始表示 p 还需要多少个字符 c。
        // 窗口收入字符时做 --need；因此 need[c] < 0 并不是错误，
        // 而是表示当前窗口里的 c 超过了 p 所需的数量。
        array<int, 26> need{};
        for (char c : p) ++need[c - 'a'];

        vector<int> answer;

        // missing 不是“还缺多少种字符”，而是“还缺多少个字符槽位”。
        // 初始等于 |p|；每真正填上一份仍在欠的字符，才减少 1。
        int missing = static_cast<int>(p.size());
        int left = 0;

        for (int right = 0; right < static_cast<int>(s.size()); ++right) {
            int in = s[right] - 'a';

            // 必须先判断 need[in] > 0，再执行 --need[in]：
            // 只有“进入之前仍有欠账”的这一个字符，才真正填掉一个 missing 槽位；
            // 若 need[in] <= 0，说明这个字符只是额外富余，不能让 missing 继续下降。
            if (need[in] > 0) --missing;
            --need[in];

            // 上一轮窗口长度至多为 |p|，本轮只新加入 1 个字符，
            // 所以一旦超长只需移出最左边 1 个字符即可恢复定长窗口。
            if (right - left + 1 > static_cast<int>(p.size())) {
                int out = s[left++] - 'a';

                // 离开窗口相当于撤销此前的一次 --need[out]，所以先 ++need[out]。
                // 如果恢复后 need[out] > 0，说明窗口重新缺少一份 out，
                // 此时才需要把 missing 加回 1。顺序不能反，否则会误判富余字符。
                ++need[out];
                if (need[out] > 0) ++missing;
            }

            // 窗口长度恰好等于 |p| 时，missing==0 表示 p 要求的每一个字符槽位都已满足。
            // 由于总长度又没有额外空间，不可能同时存在“某字符多了、另一字符少了”，
            // 因而频次向量必然与 p 完全一致，当前 left 就是一个异位词起点。
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
