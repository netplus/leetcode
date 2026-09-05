"""Reviewed C++ key-code comments for the remaining Week 1 problems.

Entries are added strictly one problem at a time after reading and reviewing the
actual implementation. Presence in the same module does not imply batch review.
"""


CODE_COMMENTS_WEEK1_REST = {
    209: r'''// ---------- Solution ----------
class Solution {
public:
    int minSubArrayLen(int target, vector<int>& nums) {
        // 当前窗口始终是闭区间 [left, right]；sum 保存这个窗口内的元素和。
        int left = 0;

        // 合法答案最长只有 n，所以 n+1 是“不可能成为真实答案”的哨兵。
        // 扫描结束仍为该值，就说明从未出现过 sum >= target 的窗口。
        int best = static_cast<int>(nums.size()) + 1;
        long long sum = 0;

        for (int right = 0; right < static_cast<int>(nums.size()); ++right) {
            // 所有 nums[i] > 0，因此右端加入新元素只会让 sum 增大。
            // 这使 right 可以单向前进，不需要为了旧窗口重新回退。
            sum += nums[right];

            // 一旦窗口合法，就持续右移 left 寻找“同一个 right 下更短的合法窗口”。
            // 正数保证每删一个左端元素，sum 只会减小，所以收缩方向也是单调的。
            while (sum >= target) {
                // 当前 [left,right] 此刻仍然合法，必须先记录长度。
                // 若先减 nums[left]，窗口可能立刻变成非法，从而漏掉当前候选。
                best = min(best, right - left + 1);

                // 先从 sum 中移除旧 left，再让 left 指向新的窗口起点。
                // 当 while 停止时，当前 right 下已经缩到“再也不合法”；
                // 旧的 left 永远无需回退，因为未来扩张 right 只会提供新的正贡献。
                sum -= nums[left++];
            }
        }
        return best == static_cast<int>(nums.size()) + 1 ? 0 : best;
    }
};''',

    3: r'''// ---------- Solution ----------
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
};''',
}
