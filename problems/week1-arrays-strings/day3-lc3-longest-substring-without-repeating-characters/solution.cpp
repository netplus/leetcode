// ============================================================================
// LC-3：无重复字符的最长子串
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 1 周 / 第 3 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给定一个字符串 s，请你找出其中不含有重复字符的最长子串的长度。
//
// 约束与要求：
//   - 0 <= s.length <= 10^5
//   - s 由英文字母、数字、符号和空格组成
//
// 复杂度目标：O(n) 时间，O(min(alphabet,n)) 空间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜滑动窗口 + 字符最后位置
// - 核心要点：
//   1. 思路起点：窗口 [left,right] 始终无重复；看到字符 c 时，left 直接跳到 c 上次位置后一格，但不能向左回退。
//   2. 执行逻辑：1. 把 256 个字节字符的最后位置初始化为 -1；2. 更新 left=max(left,last[c]+1)；3. 记录窗口长度并更新 last[c]。
//   3. 为什么这样做：更新后 c 在窗口内至多出现一次，其他字符未改变；left 单调递增，因此窗口始终合法，并保留每个 right 下最长的合法后缀。
// - 边界与易错点：字符串可能为空或含空格；char 可能有符号，索引数组时转 unsigned char；left 必须取 max 防止回退。
// - 举一反三：需要维护“最近一次冲突位置”的问题，如至多一次重复、替换后最长字符段，都可由该窗口状态扩展。
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
        array<int, 256> last;
        last.fill(-1);
        int left = 0, best = 0;
        for (int right = 0; right < static_cast<int>(s.size()); ++right) {
            const unsigned char c = static_cast<unsigned char>(s[right]);
            left = max(left, last[c] + 1);  // 冲突字符不在当前窗口时不能回退
            best = max(best, right - left + 1);
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

