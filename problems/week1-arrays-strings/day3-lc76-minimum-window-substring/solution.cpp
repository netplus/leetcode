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
//   为什么收入/移出都“无条件” ++/-- need，而不先判定字符是否属于 t？
//   - need 的三态已经把“归属”信息编码进数值：t 中字符 need 初始 ≥1，非 t 字符初始 =0。
//     非 t 字符进来 -> need 从 0 变 -1（富余），出去 -> 从 -1 变 0，全程 need>0 为假，missing 不动。
//     于是“是否在 t 中”这个外部判定被 need>0 单一判据完整替代，无需额外维护 inT[] 表。
//   - 收入端 --need 与移出端 ++need 严格对称，need 总和守恒，进出多少加减多少，一眼可验正确性。
//     若改成“非 t 字符跳过”，则两端都要套 if 且条件必须一致，多一张表、多一层分支、多一个出错面，
//     且 inT 判据相对 need>0 是冗余的（区分能力更弱：它分不清“t 中但已满足”与“t 中仍欠缺”）。
//   记忆：别问“字符在不在 t”，只问“need>0 吗”——后者才是 missing 动不动的充要条件。
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
//   每次 right 固定后，只要窗口仍合法就持续收缩，因此会检查这个 right 对应的所有合法左边界，并在刚变非法前得到该 right 的最短合法窗口。
//   need 的正/零/负精确区分欠缺、刚好和富余，missing=0 因而等价于全部配额满足。
//
// 6. 边界与易错点
//   t 中重复字符是按次数欠账，不是按字符种类；更新最优答案必须发生在移除 left 之前；need 为负表示富余是正常状态；无解时返回空串。
//
// 7. 举一反三
//   LC-438 与本题共用同一套字符配额账本：438 固定窗口长度，只判断是否完全匹配；76 窗口长度可变，在满足配额后继续缩左求最短。
//   以后看到“包含某些字符及其次数”的窗口题，优先想到 need/missing。
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
        array<int, 256> need{};
        for (unsigned char c : t) ++need[c];

        int missing = static_cast<int>(t.size());
        int left = 0, bestStart = 0, bestLength = INT_MAX;
        for (int right = 0; right < static_cast<int>(s.size()); ++right) {
            unsigned char rc = static_cast<unsigned char>(s[right]);
            if (need[rc] > 0) --missing;  // 只有“还欠的”算偿还；非 t 字符 need=0 不触发
            --need[rc];                   // 无条件 --：非 t 字符 0->-1 富余，不影响 missing

            while (missing == 0) {
                if (right - left + 1 < bestLength) {
                    bestStart = left;
                    bestLength = right - left + 1;
                }
                unsigned char lc = static_cast<unsigned char>(s[left++]);
                ++need[lc];                   // 无条件 ++：与收入端 -- 对称，need 总和守恒
                if (need[lc] > 0) ++missing;  // 刚移除了一个必需字符（非 t 字符最多到 0，不会触发）
            }
        }
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
