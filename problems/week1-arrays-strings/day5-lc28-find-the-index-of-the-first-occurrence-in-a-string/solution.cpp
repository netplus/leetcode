// ============================================================================
// LC-28：找出字符串中第一个匹配项的下标
// 难度：简单
// 优先级：P1（进阶）
// 学习进度：第 1 周 / 第 5 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给你两个字符串 haystack 和 needle，请你在 haystack 字符串中找出 needle 字符串的第一个匹配项的下标（下标从 0 开始）。
// 如果 needle 不是 haystack 的一部分，则返回 -1。
//
// 约束与要求：
//   - 1 <= haystack.length, needle.length <= 10^4
//   - haystack 和 needle 仅由小写英文字符组成
//
// 复杂度目标：O(n+m) (KMP) 或 O(n*m) 朴素匹配。
//
// ----------------------------------------------------------------------------
// 解法精讲｜KMP：失配时复用已匹配前缀
// - 核心要点：
//   1. 思路起点：lps[i] 表示模式串前缀 p[0..i] 的最长真前后缀长度；文本失配时跳到这一边界继续，不回退文本指针。
//   2. 执行逻辑：1. 预处理 needle 的 lps 数组；2. 双指针扫描 haystack 与 needle；3. 失配按 lps 回退模式指针，完整匹配时返回起点。
//   3. 为什么这样做：回退后的前缀等于刚匹配后缀，之前字符无需重验；每次比较要么推进文本，要么缩短模式边界，总复杂度线性。
// - 边界与易错点：构造 lps 时失配也要链式回退；真前缀不能等于整个串；接口约束 needle 非空，但代码兼容空串返回 0。
// - 举一反三：KMP 的失配函数也用于字符串周期、重复子串、流式匹配和多个模式拼接后的边界计算。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：haystack。
//   第 2 行：needle。
//   输出：从 0 开始计数的下标；未找到时输出 -1。
// test.in 的预期输出：0
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    int strStr(string haystack, string needle) {
        if (needle.empty()) return 0;
        vector<int> lps(needle.size(), 0);
        for (int i = 1, len = 0; i < static_cast<int>(needle.size());) {
            if (needle[i] == needle[len]) {
                lps[i++] = ++len;
            } else if (len > 0) {
                len = lps[len - 1];  // 尝试更短的、仍可能成立的边界
            } else {
                lps[i++] = 0;
            }
        }

        for (int i = 0, j = 0; i < static_cast<int>(haystack.size());) {
            if (haystack[i] == needle[j]) {
                ++i;
                if (++j == static_cast<int>(needle.size())) return i - j;
            } else if (j > 0) {
                j = lps[j - 1];
            } else {
                ++i;
            }
        }
        return -1;
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    string h, n; if (!(cin >> h >> n)) return 0;
    cout << sol.strStr(h, n) << "\n";
    return 0;
}

