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
// 题目示例：
// 示例 1：
//   Input: haystack = "sadbutsad", needle = "sad"
//   Output: 0
//   Explanation: "sad" occurs at index 0 and 6.
//   The first occurrence is at index 0, so we return 0.
//
// 示例 2：
//   Input: haystack = "leetcode", needle = "leeto"
//   Output: -1
//   Explanation: "leeto" did not occur in "leetcode", so we return -1.
//
// 约束与要求：
//   - 1 <= haystack.length, needle.length <= 10^4
//   - haystack 和 needle 仅由小写英文字符组成
//
// 复杂度目标：O(n+m) (KMP) 或 O(n*m) 朴素匹配。
//
// ----------------------------------------------------------------------------
// 解法精讲｜KMP：失配时复用已匹配前缀
//
// 0. 优化是怎么来的
//   最自然的字符串匹配，是让 needle 从 haystack 的每个可能起点重新比较；一旦后面失配，就把文本起点右移一格并从 needle[0] 重来。最坏情况下，大段已经比较相等的字符会被反复比较，达到 O(nm)。
//
//   真正可以复用的是“已经匹配成功的那一段”本身。若它的某个后缀同时也是 needle 的前缀，那么失配后无需让文本指针回退；这段后缀已经被证明和模式前缀相同，只要让模式指针跳到对应长度继续比较即可。
//
//   lps 就是在预处理 needle 时，把每个状态失配后还能保留的最长边界提前算好。
//
//   优化类型：重复比较 -> 复用已确认的前后缀状态。最终文本指针不回退，复杂度 O(n+m)。
//
// 1. 图像直觉
//   KMP 最难的地方不是“怎么比较字符”，而是：已经匹配了一大段后失配，哪些工作可以不重做？
//
//   模式串举例：needle = "ababaca"
//
//   假设文本刚刚成功匹配了 needle 的前 5 个字符：
//
//   已匹配：a b a b a
//   模式串：a b a b a c a
//                      ^
//                下一位期待 c
//
//   此时如果文本下一字符不是 c，就发生失配。
//   但已匹配部分 "ababa" 的最长真前后缀是 "aba"：
//
//   前缀：a b a
//   后缀：    a b a
//
//   所以文本指针不用回退；文本末尾的 "aba" 已经确定等于 needle[0..2]，
//   只需让模式指针从 j=5 回到 j=lps[4]=3，再继续比较当前文本字符。
//
//   lps 保存的就是这种“失配后还能保留下来的最长已匹配边界”。
//
// 2. 一句话核心
//   失配时不把已匹配内容全部丢掉，而是把模式指针退到最长的“前缀 = 当前已匹配后缀”的位置，复用已经确认过的字符。
//
// 3. 公式 / 不变量
//   lps[i] = needle[0..i] 的最长真前缀长度，
//            且这个前缀同时也是 needle[0..i] 的后缀。
//
//   文本匹配时若 needle[j] 失配：
//     j > 0  -> j = lps[j-1]
//     j == 0 -> 文本 i++
//
//   完整匹配时：
//     j == needle.size()
//     起点 = i - j
//
// 4. 执行步骤
//   1. 先为 needle 构造 lps：记录每个前缀在失配时可退回的最长边界
//   2. 扫描 haystack，i 指向文本位置，j 表示 needle 已匹配长度
//   3. 字符相等时同时推进；若 j 到达 needle.size()，返回 i-j
//   4. 字符失配且 j>0 时，只回退 j=lps[j-1]，i 不动
//   5. 字符失配且 j==0 时，说明没有可复用边界，才推进文本 i
//   记忆：文本不回头；模式失配时退到还能复用的最长边界。
//
// 5. 为什么不会漏 / 不会重
//   当 j 个字符已经匹配后发生失配，lps[j-1] 对应的前缀与这段已匹配文本的后缀完全相同，
//   因此这些字符无需再次与文本比较；所有可能更长的边界都已经被排除。
//   文本指针从不回退，每次模式回退又严格缩短 j，所以总复杂度是 O(n+m)。
//
// 6. 边界与易错点
//   lps 表示最长真前后缀，不包含整个字符串自身；构造 lps 时失配也可能连续回退多次，不能直接清零；
//   文本失配且 j>0 时不能推进 i，否则会跳过潜在匹配。接口约束 needle 非空，但实现兼容空串返回 0。
//
// 7. 举一反三
//   KMP 的核心不是背代码，而是“状态失败后退到仍然成立的最长边界”。
//   字符串周期、重复子串、前后缀边界问题都在复用同一个 lps 状态。
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
