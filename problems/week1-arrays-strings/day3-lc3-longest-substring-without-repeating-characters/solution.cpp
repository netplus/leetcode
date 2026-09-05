// ============================================================================
// LC-3：无重复字符的最长子串
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 1 周 / 第 3 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给定一个字符串 s，请你找出其中不含有重复字符的最长子串的长度。
//
// 题目示例：
// 示例 1：
//   Input: s = "abcabcbb"
//   Output: 3
//   Explanation: The answer is "abc", with the length of 3. Note that "bca" and "cab" are also correct answers.
//
// 示例 2：
//   Input: s = "bbbbb"
//   Output: 1
//   Explanation: The answer is "b", with the length of 1.
//
// 示例 3：
//   Input: s = "pwwkew"
//   Output: 3
//   Explanation: The answer is "wke", with the length of 3.
//   Notice that the answer must be a substring, "pwke" is a subsequence and not a substring.
//
// 约束与要求：
//   - 0 <= s.length <= 10^5
//   - s 由英文字母、数字、符号和空格组成
//
// 复杂度目标：O(n) 时间，O(min(alphabet,n)) 空间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜滑动窗口 + 字符最后位置
//
// 前置概念
//   子串（substring）必须由原字符串中的一段连续字符组成；不能跳过中间字符。
//
//   例如 s="abcde"：
//   - "bcd" 是子串；
//   - "ace" 不是子串，它只能算子序列（subsequence），因为中间跳过了字符。
//
//   因此本题维护的窗口 s[left..right] 必须始终对应原字符串中的一段连续区间。
//
// 0. 优化是怎么来的
//   最直接的做法可以枚举每个起点，并向右扩展直到遇到重复字符；不同起点之间会反复检查大量相同的字符串片段，最坏 O(n^2)。
//
//   扫描到新字符 c 时，旧窗口本来已经无重复，因此唯一新增的冲突只可能来自 c 自己。若记住 c 最近一次出现的位置，就无需从 left 开始一个字符一个字符重新检查；可以直接把 left 跳到那个冲突位置之后。
//
//   这里复用的是“最近冲突位置”这一小段历史状态。由于 left 只向右移动，被排除的前缀永远不会重新成为合法窗口的一部分。
//
//   优化类型：重复检查 -> 记录最近冲突位置 + 单调左边界。
//
// 1. 图像直觉
//   例：s = "abba"。
//
//   扫描到第二个 b（right=2）时：
//    a  b  b  a
//    0  1  2  3
//       L  R
//
//   b 上次在 1，所以为了不重复，left 必须跳到 2。
//
//   随后扫描到最后一个 a（right=3）：
//   a 上次在 0，last[a]+1 = 1；但当前 left 已经是 2：
//    a  b  b  a
//    0  1  2  3
//          L  R
//
//   如果把 left 退回 1，窗口会重新包含两个 b。
//   所以真正的更新不是 left=last[c]+1，而是：
//   left = max(left, last[c]+1)。
//
// 2. 一句话核心
//   新字符只会和它最近一次出现的位置发生冲突；若那个旧位置仍在当前窗口里，就把 left 直接跳过它，但 left 永远不能回退。
//
// 3. 公式 / 不变量
//   窗口不变量：s[left..right] 内没有重复字符。
//
//   当前字符 c = s[right]：
//   left = max(left, last[c] + 1)
//
//   更新后：
//   当前窗口中旧的 c 已被排除；
//   left 单调不减；
//   因此 [left,right] 仍无重复。
//
// 4. 执行步骤
//   1. last 全部初始化为 -1，left=0，best=0
//   2. right 从左到右扫描当前字符 c
//   3. 执行 left=max(left,last[c]+1)，必要时直接跨过最近一次冲突
//   4. 此时 [left,right] 是以 right 结尾的最长无重复后缀，用它更新 best
//   5. 最后记录 last[c]=right，供未来字符查询
//   记忆：看到 c -> 找 c 上次在哪 -> left 只向右跳过它，绝不回头。
//
// 5. 为什么不会漏 / 不会重
//   对固定 right，唯一新引入的重复风险来自当前字符 c，因为旧窗口在处理 right 前已经无重复。跳过 c 的最近旧位置后，新窗口重新合法；又因为 left 不回退，所以不会把过去已经排除的其他冲突重新放回来。
//
// 6. 边界与易错点
//   最容易错的是忘记 max(left,...) 导致 left 回退；字符串可为空；当前实现用 256 大小数组按字节记录位置，索引时转 unsigned char，避免有符号 char 造成负下标。
//
// 7. 举一反三
//   这是一类“遇到冲突时直接跳到冲突位置之后”的窗口，而不是一步一步 while 收缩。凡是能记录最近冲突位置的问题，都可以考虑这种跳跃式 left 更新。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：字符串 s。
//   输出：长度。
// test.in 的预期输出：3
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        // last[c] = 字节值 c 最近一次出现的下标；-1 表示此前从未出现。
        // 当前实现按 0..255 的字节值索引，所以读取 char 时会转成 unsigned char，
        // 避免某些平台上 signed char 为负而产生负数组下标。
        array<int, 256> last;
        last.fill(-1);

        // 在每轮处理 right 后，窗口 [left,right] 保持“无重复字符”；
        // left 只允许向右移动，best 保存历史最长合法窗口长度。
        int left = 0, best = 0;
        for (int right = 0; right < static_cast<int>(s.size()); ++right) {
            const unsigned char c = static_cast<unsigned char>(s[right]);

            // 处理 right 之前，旧窗口 [left,right-1] 已经无重复，
            // 因而加入 c 后唯一可能新增的冲突就是“窗口里已经有一个 c”。
            // 若 last[c] 仍位于当前窗口内，就必须把 left 跳到它后面；
            // 若 last[c] 已在 left 左侧，则那个旧冲突早已被排除，left 绝不能倒退。
            left = max(left, last[c] + 1);

            // 更新 left 后，[left,right] 已重新合法，而且它是“以 right 结尾”的
            // 最长无重复后缀，因此可以直接用这个长度更新全局 best。
            best = max(best, right - left + 1);

            // 最后再把 c 的最近位置更新为 right，供未来字符查询。
            // 必须让本轮窗口处理使用的是“旧的最近位置”；若先写 right，
            // last[c]+1 会变成 right+1，错误地把当前字符自己当成冲突对象。
            last[c] = right;
        }
        return best;
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    string s;
    getline(cin, s);
    cout << sol.lengthOfLongestSubstring(s) << "\n";
    return 0;
}
