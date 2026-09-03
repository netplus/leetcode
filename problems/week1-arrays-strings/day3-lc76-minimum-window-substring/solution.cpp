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
// - 核心要点：
//   1. 思路起点：need[c] 保存目标频次，missing 保存窗口尚缺的字符总数（包含重复）；missing==0 时窗口覆盖 t，可以尝试收缩。
//   2. 执行逻辑：1. 统计 t 并令 missing=|t|；2. 扩张右端：若该字符仍有需求则 missing--，随后 need[c]--；3. 覆盖后反复收缩并更新最短答案。
//   3. 为什么这样做：need 允许为负，表示窗口内冗余；missing 为 0 当且仅当所有需求均满足。每次收缩到再次缺字符前，会考察该右端对应的最短合法窗口。
// - 边界与易错点：t 中重复字符必须按次数满足；无解返回空串；更新答案应发生在移除左字符之前；字符数组索引用 unsigned char。
// - 举一反三：“满足配额后求最短”统一使用扩右至可行、缩左至刚好失效；反向可得到至多/至少型窗口计数。
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
            if (need[rc] > 0) --missing;
            --need[rc];

            while (missing == 0) {
                if (right - left + 1 < bestLength) {
                    bestStart = left;
                    bestLength = right - left + 1;
                }
                unsigned char lc = static_cast<unsigned char>(s[left++]);
                ++need[lc];
                if (need[lc] > 0) ++missing;  // 刚移除了一个必需字符
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

