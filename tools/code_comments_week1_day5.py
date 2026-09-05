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
}
