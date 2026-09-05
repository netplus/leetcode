// ============================================================================
// LC-76：最小覆盖子串
// 难度：困难
// 优先级：P0（必做）
// 学习进度：第 1 周 / 第 3 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给定两个字符串 s 和 t，长度分别是 m 和 n，返回 s 中的最短窗口子串，使得该子串包含 t 中的每一个字符（包括重复字符）。
// 如果没有这样的子串，返回空字符串 ""。
// 测试用例保证答案唯一。
//
// 题目示例：
// 示例 1：
//   Input: s = "ADOBECODEBANC", t = "ABC"
//   Output: "BANC"
//   Explanation: The minimum window substring "BANC" includes 'A', 'B', and 'C' from string t.
//
// 示例 2：
//   Input: s = "a", t = "a"
//   Output: "a"
//   Explanation: The entire string s is the minimum window.
//
// 示例 3：
//   Input: s = "a", t = "aa"
//   Output: ""
//   Explanation: Both 'a's from t must be included in the window.
//   Since the largest window of s only has one 'a', return empty string.
//
// 约束与要求：
//   - m == s.length
//   - n == t.length
//   - 1 <= m, n <= 10^5
//   - s 和 t 由英文字母组成
//
// 复杂度目标：O(|s| + |t|) 时间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜最小覆盖滑动窗口
//
// 0. 优化是怎么来的
//   最自然的做法，是枚举每个 left，再向右寻找第一个覆盖 t 的窗口；换一个 left 后又从头统计字符需求，很多相邻候选会反复扫描和计数同一段字符。
//
//   真正可复用的是当前窗口的字符账本：right 加入一个字符只修改一项 need，left 移出一个字符也只恢复一项 need。更关键的是，left 没必要回退——当窗口已经覆盖 t 时，只需向右收缩 left 寻找当前 right 下的最短合法窗口；一旦刚刚失去覆盖，再继续推进 right。
//
//   因此算法并不是“套一个滑动窗口模板”，而是把大量重叠窗口的重复统计压缩为两个单调边界上的增量事件。
//
//   优化类型：状态可延续 + 边界单调移动 -> 可变长度滑动窗口。
//
// 1. 图像直觉
//   仍然把 t 看成“欠账表”。例：t = "ABC"，初始欠 A/B/C 各 1 份，missing=3。
//
//   s = A D O B E C O D E B A N C
//       L         R
//
//   right 扩到第一个 C 时，A/B/C 都已经收齐，missing=0：
//   窗口 "ADOBEC" 合法。
//
//   这时不再继续盲目扩右，而是开始移动 left，尝试把无关或富余字符挤掉。
//   一旦移走 A：need[A] 从 0 变回 1，missing 从 0 变 1，说明窗口刚刚失去覆盖能力。
//   于是停止收缩，继续向右寻找下一份 A。
//
//   整个过程就是：欠账未清 -> 扩右；欠账清零 -> 缩左；刚重新欠账 -> 再扩右。
//
// 2. 一句话核心
//   right 负责把缺的字符收齐，missing==0 后 left 负责尽量丢掉多余字符；直到刚丢掉一个必需字符，再重新扩张。
//
// 3. 公式 / 不变量
//   need[c] > 0：当前窗口还欠 c
//   need[c] = 0：c 的需求刚好满足
//   need[c] < 0：c 有富余
//   missing：所有尚未满足的字符槽位总数
//
//   收入 rc：
//   if (need[rc] > 0) missing--;
//   need[rc]--;
//
//   移出 lc：
//   need[lc]++;
//   if (need[lc] > 0) missing++;  // 刚失去一份必需字符
//
//   missing == 0 <=> 当前窗口覆盖 t。
//
// 4. 执行步骤
//   1. 统计 t 的频次到 need，missing 初始化为 |t|
//   2. right 向右收入字符；若它能偿还欠账则 missing--，随后 need--
//   3. 当 missing==0 时，当前窗口已覆盖 t：先更新最短答案
//   4. 然后不断移出 left 字符；若某次移出让 need[lc]>0，则 missing++，说明窗口刚变非法，停止收缩
//   5. 继续扩张 right，直到再次把欠账补齐
//   记忆：欠账没清就扩右；清零后缩左；刚重新欠一份就停。
//
// 5. 为什么不会漏 / 不会重
//   每次 right 固定后，只要窗口仍合法就持续收缩，因此会检查这个 right 对应的所有合法左边界，并在刚变非法前得到该 right 的最短合法窗口。need 的正/零/负精确区分欠缺、刚好和富余，missing=0 因而等价于全部配额满足。
//
// 6. 边界与易错点
//   t 中重复字符是按次数欠账，不是按字符种类；更新最优答案必须发生在移除 left 之前；need 为负表示富余是正常状态；无解时返回空串。
//
// 7. 举一反三
//   LC-438 与本题共用同一套字符配额账本：438 固定窗口长度，只判断是否完全匹配；76 窗口长度可变，在满足配额后继续缩左求最短。以后看到“包含某些字符及其次数”的窗口题，优先想到 need/missing。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：s。
//   第 2 行：t。
//   输出：最小覆盖子串。
// test.in 的预期输出：BANC
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    string minWindow(string s, string t) {
        if (t.size() > s.size()) return "";

        // need[c] 仍是一张“欠账表”：正数=还欠，0=刚好，负数=窗口中有富余。
        // 这里使用 256 个字节槽位，因此非 t 字符初始 need=0，进入窗口后自然变成负数；
        // 不需要再维护一张“字符是否属于 t”的额外表。
        array<int, 256> need{};
        for (unsigned char c : t) ++need[c];

        // missing 表示尚未满足的字符槽位总数；missing==0 <=> 当前窗口覆盖了 t。
        int missing = static_cast<int>(t.size());
        int left = 0;

        // INT_MAX 是“尚未找到任何合法窗口”的哨兵；
        // bestStart/bestLength 只在窗口合法时更新。
        int bestStart = 0, bestLength = INT_MAX;

        for (int right = 0; right < static_cast<int>(s.size()); ++right) {
            unsigned char rc = static_cast<unsigned char>(s[right]);

            // 与 LC-438 相同：进入前如果 need[rc] > 0，当前字符真正偿还了一份欠账。
            // 随后无条件 --need[rc]；非 t 字符只是从 0 变 -1，missing 不受影响。
            if (need[rc] > 0) --missing;
            --need[rc];

            // 一旦 missing==0，当前 [left,right] 已合法。
            // 对固定 right，继续右移 left 才有机会得到更短窗口；
            // 所以这里必须用 while，把这个 right 下所有仍合法的左边界都检查到刚失效为止。
            while (missing == 0) {
                // 当前窗口此刻仍覆盖 t，必须在删除左端之前先记录候选。
                // 若先移除 left，可能恰好删掉一份必需字符，从而漏掉当前合法窗口。
                if (right - left + 1 < bestLength) {
                    bestStart = left;
                    bestLength = right - left + 1;
                }

                unsigned char lc = static_cast<unsigned char>(s[left++]);

                // 移出字符与收入字符严格对称：先把 need[lc] 恢复 1。
                // 若恢复后 need[lc] > 0，说明刚刚移走的是一份不可缺少的字符，
                // 窗口从这一刻重新变非法，因此 missing++，while 会停止收缩。
                // 非 t 字符或富余字符最多把 need 恢复到 <=0，不会影响 missing。
                ++need[lc];
                if (need[lc] > 0) ++missing;
            }
        }

        // 哨兵未被改写说明整个 s 中从未出现覆盖 t 的窗口。
        return bestLength == INT_MAX ? "" : s.substr(bestStart, bestLength);
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    string s, t; if (!(cin >> s >> t)) return 0;
    cout << sol.minWindow(s, t) << "\n";
    return 0;
}
