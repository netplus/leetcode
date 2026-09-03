// ============================================================================
// LC-5：最长回文子串
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 1 周 / 第 5 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给你一个字符串 s，找到 s 中最长的回文子串。
//
// 约束与要求：
//   - 1 <= s.length <= 1000
//   - s 仅由数字和英文字母组成
//
// 复杂度目标：O(n^2) (中心扩展) 或 O(n)（Manacher）。
//
// ----------------------------------------------------------------------------
// 解法精讲｜中心扩展：统一奇偶回文
// - 核心要点：
//   1. 思路起点：每个回文都有唯一中心：奇数长度中心是字符，偶数长度中心是字符间隙；枚举中心并向两侧扩展即可覆盖全部候选。
//   2. 执行逻辑：1. 对每个 i 分别扩展 (i,i) 和 (i,i+1)；2. 字符相等时持续扩大；3. 仅在更长时更新起点和长度。
//   3. 为什么这样做：任意最长回文必在其中心对应的那次扩展中被完整发现；扩展停止恰是再扩大已不可能保持回文。
// - 边界与易错点：偶数中心不能遗漏；同长度不更新可稳定保留最先发现者；空串虽不在官方约束内仍自然返回空。
// - 举一反三：中心扩展还可统计所有回文子串；若规模更大，可进一步学习 Manacher 的对称半径复用。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：字符串 s。
//   输出：最长回文子串。
// test.in 的预期输出：bab
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    string longestPalindrome(string s) {
        int bestStart = 0, bestLength = 0;
        auto expand = [&](int left, int right) {
            while (left >= 0 && right < static_cast<int>(s.size()) && s[left] == s[right]) {
                --left;
                ++right;
            }
            const int length = right - left - 1;
            if (length > bestLength) {
                bestLength = length;
                bestStart = left + 1;
            }
        };

        for (int center = 0; center < static_cast<int>(s.size()); ++center) {
            expand(center, center);       // 奇数长度
            expand(center, center + 1);   // 偶数长度
        }
        return s.substr(bestStart, bestLength);
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    string s;
    if (!(cin >> s)) return 0;
    cout << sol.longestPalindrome(s) << "\n";
    return 0;
}

