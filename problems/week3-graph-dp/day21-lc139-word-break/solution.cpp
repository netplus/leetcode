// ============================================================================
// LC-139：单词拆分
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 3 周 / 第 21 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给你一个字符串 s 和一个字符串列表 wordDict 作为字典。
// 如果可以利用字典中出现的一个或多个单词拼接出 s 则返回 true。
// 注意：不要求字典中出现的单词全部都使用，并且字典中的单词可以重复使用。
//
// 题目示例：
// 示例 1：
//   Input: s = "leetcode", wordDict = ["leet","code"]
//   Output: true
//   Explanation: Return true because "leetcode" can be segmented as "leet code".
//
// 示例 2：
//   Input: s = "applepenapple", wordDict = ["apple","pen"]
//   Output: true
//   Explanation: Return true because "applepenapple" can be segmented as "apple pen apple".
//   Note that you are allowed to reuse a dictionary word.
//
// 示例 3：
//   Input: s = "catsandog", wordDict = ["cats","dog","sand","and","cat"]
//   Output: false
//
// 约束与要求：
//   - 1 <= s.length <= 300
//   - 1 <= wordDict.length <= 1000
//   - 1 <= wordDict[i].length <= 20
//   - s 和 wordDict[i] 仅由小写英文字母组成
//   - wordDict 中的所有字符串互不相同
//
// 复杂度目标：O(n^2) 或 O(n*L) 时间（DP）。
//
// ----------------------------------------------------------------------------
// 解法精讲｜前缀可达 DP：把指数分割搜索压缩成字符串位置上的 DAG 可达性
//
// 前置概念
//   字符串分词是一个“整条路径是否能走到结尾”的全局可达性问题，不能仅凭当前最漂亮的局部匹配做贪心决定。
//
//   反例：s="cars"，wordDict={"car","ca","rs"}。
//   如果每一步都贪心选择当前最长前缀，首先会拿 "car"，剩下 "s"，于是失败；但真正存在合法切分："ca" + "rs"。
//
//   因此“当前最长词”“当前最短词”都没有天然的安全贪心性质。DP / memo 要保留的是切分位置的可达性，让不同前缀选择可以在同一个位置重新汇合。
//
// 0. 优化是怎么来的
//   最自然且正确的直接算法是 DFS：从 start 开始枚举下一段结束位置 end，只要 s[start:end] 在字典中，就递归处理 end；到达 n 时成功。它枚举所有合法切分前缀，因此不会漏解。
//
//   真正的浪费在于不同切分路径会反复到达同一个 start。例如 s 的前缀既可以按若干短词到达位置 i，也可能按一个长词到达同一 i；一旦 start 相同，后缀 s[start:] 能否拆分与之前怎样到达这里完全无关。直接 DFS 会重复计算同一“从位置 start 出发能否成功”的子问题。
//
//   把这个重复状态记下来就得到 memo；再反过来自底向前传播，就是前缀可达 DP。我们甚至可以把它看成一个隐式 DAG：位置 i 到 j 有边，当且仅当 s[i:j] 是字典词。reachable[i]=true 就是节点 i 已经从 0 可达。
//
//   此外，直接枚举每个 start 的所有 end 仍会检查大量长度明显超过所有字典词的子串；预先保存 maxLength 后，长度只需尝试 1..maxLength。字典本身放入 unordered_set，把“这个候选词是否存在”的重复线性扫描改成哈希查询。
//
//   优化链：枚举所有切分路径 -> 相同 start 的后缀被重复求解 -> 用位置状态复用结果 -> 只从可达前缀扩展 -> 哈希字典 + 最大词长限制候选，这就是前缀可达 DP。
//
// 1. 图像直觉
//   s = "leetcode"，wordDict={"leet","code"}
//
//   切分位置：
//   0 1 2 3 4 5 6 7 8
//   |---leet---|---code---|
//   0 --------> 4 -------> 8
//
//   reachable[0]=true 表示空前缀已经完成。
//   从 0 发现 "leet"，于是 4 可达；
//   从 4 发现 "code"，于是 8 可达。
//
//   如果某个位置 i 不可达，即使 s[i:j] 恰好是字典词，也不能用它：因为前面的 s[0:i] 尚未形成合法切分。
//   这和图搜索中的 frontier 完全一样，只扩展已经真正到达的状态。
//
// 2. 一句话核心
//   把每个切分位置当成状态；只有当前前缀已经可达时，才尝试接一个字典词并把新的结束位置标成可达。
//
// 3. 公式 / 不变量
//   reachable[i] = s[0:i) 是否能被字典词完整切分。
//
//   初始化：
//   reachable[0] = true
//
//   转移：
//   若 reachable[start] 为 true，且
//   1 <= length <= maxLength，
//   start+length <= n，
//   s[start:start+length] 属于字典，
//   则：
//   reachable[start+length] = true。
//
//   等价的经典写法是：
//   reachable[end] = OR over start<end (
//       reachable[start] AND s[start:end] in dict
//   )
//
//   不变量：扫描到 start 时，reachable[0..start] 已准确描述这些前缀的可切分性；任何新置 true 的终点都由一个合法前缀再追加一个完整字典词得到。
//
// 4. 执行步骤
//   1. 把 wordDict 建成 unordered_set，同时计算 maxLength，避免每次候选都线性扫描整个字典并限制无意义长度
//   2. 建立 n+1 个前缀状态并令 reachable[0]=true；位置 0 代表空前缀这个合法起点
//   3. 按 start 从左到右扫描；若 reachable[start] 为 false，说明根本没有合法切分能到达这里，直接跳过
//   4. 从该可达位置枚举 1..maxLength 的候选长度，检查对应 substring 是否在哈希字典中
//   5. 命中时把 reachable[start+length] 置 true；最终返回 reachable[n]
//   记忆：位置是节点，字典词是边；只从已经可达的切分点继续走。
//
// 5. 为什么不会漏 / 不会重
//   每次转移都从一个已经能完整切分的前缀开始，并只追加一个完整字典词，因此所有被标记为 true 的状态都真实可达。对任意合法的完整切分 w1,w2,...,wk，其切分边界依次为 p0=0,p1,...,pk=n；reachable[0] 初始为真，若 pi 已被标记，算法枚举长度 |w(i+1)| 时必发现该字典词并标记 p(i+1)，归纳可得 n 最终必被标记，因此不会漏解。
//
// 6. 边界与易错点
//   reachable[0] 不能漏掉，否则所有切分都没有起点。字典词允许重复使用，所以这里不是 LC-416 那种‘每个物品一次’的倒序背包；同一个 word 可以从不同 start 多次形成边。substr 会产生临时字符串，约束下足够；若追求更强性能可改 trie 或 string_view 风格索引，但不能无意使用 C++20 API。
//
// 7. 举一反三
//   这是 Day 19/20 DP 的另一种视角：状态仍是‘前缀是否可达’，只是边由字典词决定。它也可以直接看成隐式 DAG 的 reachability；解码方法、字符串分段、最少分词数都能在同一位置图上把布尔值换成计数或最小代价。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：s。
//   第 2 行：L (单词数量)。
//   第 3 行：L 个以空格分隔的单词。
//   输出：s 可被拆分时输出 1，否则输出 0。
// test.in 的预期输出：1
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        // 字典只承担“候选子串是否是一条合法边”的查询，因此用哈希集合避免每次线性扫描 wordDict。
        unordered_set<string> words(wordDict.begin(), wordDict.end());

        // 从某个切分点出发，任何长度超过最长字典词的子串都不可能命中；
        // maxLength 直接裁掉这些确定无用的候选结束位置。
        int maxLength = 0;
        for (const string& word : wordDict) maxLength = max(maxLength, static_cast<int>(word.size()));

        // reachable[i] 表示前缀 s[0:i) 能否被字典词完整切分。
        // 空前缀天然是一个合法起点，因此 reachable[0]=true；没有它，任何后续位置都无法被首次到达。
        vector<char> reachable(s.size() + 1, false);
        reachable[0] = true;

        for (int start = 0; start < static_cast<int>(s.size()); ++start) {
            // 不可达位置没有任何合法前缀能走到这里，即使后面恰好存在字典词也不能作为完整切分使用。
            if (!reachable[start]) continue;

            // 只从真正可达的切分点向前连边；同一个字典词可以在不同 start 被重复使用，
            // 因为题目限制的是字符串拼接关系，而不是“每个字典词只能消费一次”。
            for (int length = 1; length <= maxLength && start + length <= static_cast<int>(s.size()); ++length) {
                if (words.count(s.substr(start, length))) reachable[start + length] = true;
            }
        }

        // 位置 n 可达，等价于完整前缀 s[0:n) 可以被若干字典词首尾相接地覆盖。
        return reachable[s.size()];
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    string s; if (!(cin >> s)) return 0;
    int L; cin >> L;
    vector<string> w(L);
    for (int i = 0; i < L; ++i) cin >> w[i];
    cout << (sol.wordBreak(s, w) ? 1 : 0) << "\n";
    return 0;
}
