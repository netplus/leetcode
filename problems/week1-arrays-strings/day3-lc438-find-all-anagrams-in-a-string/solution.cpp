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
//   - s 和 p 仅包含小写字母
//
// 复杂度目标：O(|s|) 时间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜定长滑动窗口 + 频次差
// - 核心要点：
//   1. 思路起点：异位词长度必等于 p；用 need 表示窗口相对 p 仍缺的字符数，用 missing 表示总缺口。
//   2. 执行逻辑：1. 统计 p 的频次；2. 加入右字符并更新 missing；3. 窗口超过 |p| 时移除左字符；长度相等且 missing=0 就记录起点。
//   3. 为什么这样做：窗口始终保持长度不超过 |p|；当长度恰等且总缺口为 0，等长性保证既无缺少也无多余，故频次向量完全相同。
// - 边界与易错点：p 比 s 长时自然无答案；先加入再裁剪；输出起点按扫描顺序天然递增。
// - 举一反三：固定长度的排列匹配、DNA 模式计数和滚动校验通常都采用“进一个、出一个”的窗口更新。
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

