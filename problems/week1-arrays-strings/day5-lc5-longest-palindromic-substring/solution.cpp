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
//
// 1. 图像直觉
//   任何回文都可以从“中心”向两边展开。
//
//   奇数长度："racecar"
//                 e
//              <- | ->
//   中心落在一个字符上。
//
//   偶数长度："abba"
//                 ||
//              <-    ->
//   中心落在两个字符之间的缝隙上。
//
//   所以对每个位置 i，只需要枚举两种中心：
//   (i,i)     -> 奇数回文
//   (i,i+1)   -> 偶数回文
//
//   然后只要左右字符相等，就继续同时向外走。
//
// 2. 一句话核心
//   枚举所有可能的回文中心，并从中心向两侧扩展；字符一旦不同，该中心能得到的最长回文就已经确定。
//
// 3. 公式 / 不变量
//   扩展条件：
//   left >= 0
//   right < n
//   s[left] == s[right]
//
//   循环结束时已经多走了一步，因此真实回文区间是：
//   [left+1, right-1]
//   长度：
//   right - left - 1
//
// 4. 执行步骤
//   1. 遍历每个 center
//   2. 调用 expand(center,center)，覆盖所有奇数长度回文
//   3. 调用 expand(center,center+1)，覆盖所有偶数长度回文
//   4. expand 中左右字符相等就同时向外扩张
//   5. 停止后计算 length=right-left-1；若更长就更新 bestStart/bestLength
//   记忆：每个位置看两种中心；左右相等就扩，第一次不等就停。
//
// 5. 为什么不会漏 / 不会重
//   每个非空回文都有且只有一个几何中心：奇数回文中心是某个字符，偶数回文中心是相邻字符之间的间隙；
//   我们枚举了全部这两类中心。对固定中心，向外扩展直到首次失败，得到的就是该中心下最大的回文，
//   因此全局最长回文必会被某次扩展完整发现。
//
// 6. 边界与易错点
//   最容易漏掉偶数中心 (i,i+1)；循环退出时 left/right 已经越过真实回文一格，所以长度是 right-left-1，起点是 left+1；
//   若只在 length>bestLength 时更新，可稳定保留最早发现的同长度答案。
//
// 7. 举一反三
//   中心扩展不仅能求最长回文，也能把每次成功扩展计数后得到“回文子串总数”。
//   如果规模更大，再学习 Manacher，本质仍是复用已经得到的回文半径。
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
