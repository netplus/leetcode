"""Reviewed C++ key-code comments for Week 1 / Day 5.

Entries are added only after an independent per-problem review.
"""


CODE_COMMENTS_WEEK1_DAY5 = {
    14: r'''// ---------- Solution ----------
class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        // 公共前缀不可能比任意一个字符串更长，因此先用 strs[0]
        // 作为“最长可能前缀”的上界，随后逐列让其他字符串来验收。
        const string& first = strs[0];

        for (int i = 0; i < static_cast<int>(first.size()); ++i) {
            // 进入第 i 列时，不变量是：所有字符串的 [0,i) 已经逐列验证完全相同。
            // 因此前面 i 个字符已经确定属于公共前缀；现在只需判断第 i 列能否继续延伸。
            for (int j = 1; j < static_cast<int>(strs.size()); ++j) {
                // 必须先判断 i 是否已经到达当前字符串末尾，再访问 strs[j][i]。
                // “有人没有第 i 个字符”与“第 i 个字符不同”都会立即终止前缀；
                // 一旦这里失败，任何更长前缀都不可能成立，所以可以直接返回 [0,i)。
                if (i == static_cast<int>(strs[j].size()) || strs[j][i] != first[i]) {
                    return first.substr(0, i);
                }
            }
        }

        // 若 first 的每一列都通过所有字符串验收，first 本身就是最长公共前缀。
        return first;
    }
};''',

    151: r'''// ---------- Solution ----------
class Solution {
public:
    string reverseWords(string s) {
        // 整体反转一次性把“单词块”的顺序倒过来：
        // W1 W2 ... Wk -> reverse(Wk) ... reverse(W2) reverse(W1)。
        // 副作用是每个单词内部也反了，后面会在写回每个单词后局部 reverse 修复。
        reverse(s.begin(), s.end());

        // read 负责读取整体反转后的原内容；write 指向整理后结果的下一写入位置。
        // 由于我们只删除多余空格、不会插入比已消费内容更多的字符，write 始终不会跑到 read 前面，
        // 因此原地写 s[write] 不会覆盖尚未读取的有效字符。
        int read = 0, write = 0;
        const int n = static_cast<int>(s.size());

        while (read < n) {
            // 所有连续空格都只是输入分隔冗余；先跳到下一个真实单词起点。
            while (read < n && s[read] == ' ') ++read;
            if (read == n) break;  // 只剩尾部空格时结束，避免输出尾随空格

            // 只有前面已经输出过单词时才补一个分隔空格。
            // 这样同时消除了前导空格、连续空格，并保证最终单词之间恰好一个空格。
            if (write > 0) s[write++] = ' ';

            // wordStart 记录当前单词在“压缩后结果”中的起点。
            // 下面复制完成时，[wordStart, write) 恰好就是这个仍处于反转状态的完整单词。
            const int wordStart = write;
            while (read < n && s[read] != ' ') {
                s[write++] = s[read++];
            }

            // 整体 reverse 已经解决了词序；这里仅修复当前单词内部字符顺序。
            // 使用刚才记录的半开区间 [wordStart,write)，不会碰到相邻单词或分隔空格。
            reverse(s.begin() + wordStart, s.begin() + write);
        }

        // write 之后仍可能残留原字符串的旧字符/旧空格；它们已不属于结果，必须截掉。
        s.resize(write);
        return s;
    }
};''',

    28: r'''// ---------- Solution ----------
class Solution {
public:
    int strStr(string haystack, string needle) {
        if (needle.empty()) return 0;

        // lps[i] = needle[0..i] 的最长“真前缀 = 后缀”长度。
        // 它预先记录：若已经匹配到 i 后下一字符失配，最多还能保留多少个已确认字符。
        vector<int> lps(needle.size(), 0);

        // 构造阶段：i 是当前要计算 lps 的位置；len 是当前正在尝试延伸的边界长度。
        for (int i = 1, len = 0; i < static_cast<int>(needle.size());) {
            if (needle[i] == needle[len]) {
                // 当前字符能接到边界后面，最长边界长度增加 1；
                // 只有确定 lps[i] 后 i 才前进。
                lps[i++] = ++len;
            } else if (len > 0) {
                // 当前长度 len 的边界延伸失败，但较短边界仍可能成功。
                // needle[0..len-1] 已知存在长度 lps[len-1] 的次长边界，
                // 所以直接跳到它继续尝试；i 不动，因为当前 needle[i] 还没得到答案。
                len = lps[len - 1];
            } else {
                // 连长度 1 的边界都不存在，当前位置的最长真前后缀只能是 0。
                lps[i++] = 0;
            }
        }

        // 匹配阶段：i 指向尚未处理的文本字符；j 表示 needle[0..j-1]
        // 已经与文本末尾连续匹配，亦即“当前已匹配长度”。
        for (int i = 0, j = 0; i < static_cast<int>(haystack.size());) {
            if (haystack[i] == needle[j]) {
                // 当前字符匹配后，同时消费文本字符并把模式已匹配长度加 1。
                ++i;
                if (++j == static_cast<int>(needle.size())) {
                    // i 此时已经指向完整匹配片段的后一位，长度为 j，
                    // 因而起点 = 右边界后一位 - 匹配长度 = i-j。
                    return i - j;
                }
            } else if (j > 0) {
                // 文本当前字符 haystack[i] 还没有被成功消费，i 不能前进。
                // 已匹配的 j 个字符中，后缀已有一部分等于模式前缀；
                // 回退到 lps[j-1] 就能保留这段已验证信息，直接拿同一个文本字符继续比较。
                j = lps[j - 1];
            } else {
                // j==0 时没有任何已匹配边界可复用，当前文本字符不可能作为本次匹配开头，
                // 这时才安全推进 i。
                ++i;
            }
        }
        return -1;
    }
};''',

    5: r'''// ---------- Solution ----------
class Solution {
public:
    string longestPalindrome(string s) {
        int bestStart = 0, bestLength = 0;

        // expand(left,right) 接收一个几何中心：
        // left==right 表示字符中心（奇数长度），right==left+1 表示字符间隙中心（偶数长度）。
        auto expand = [&](int left, int right) {
            // 只要左右仍在边界内且字符相同，就把当前回文同时向两侧扩一格。
            while (left >= 0 && right < static_cast<int>(s.size()) && s[left] == s[right]) {
                --left;
                ++right;
            }

            // 循环退出时，left/right 已经各自越过真实回文一格：
            // 真正的最大回文是闭区间 [left+1, right-1]，长度因此为 right-left-1。
            const int length = right - left - 1;
            if (length > bestLength) {
                bestLength = length;
                bestStart = left + 1;
            }
        };

        for (int center = 0; center < static_cast<int>(s.size()); ++center) {
            // 每个非空回文都有唯一的几何中心；枚举这两种中心即可覆盖全部奇偶回文。
            expand(center, center);       // 奇数长度：中心落在 s[center]
            expand(center, center + 1);   // 偶数长度：中心落在 center 与 center+1 的缝隙
        }

        // bestStart/bestLength 始终描述当前发现的最长真实回文区间。
        return s.substr(bestStart, bestLength);
    }
};''',
}
