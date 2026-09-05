// ============================================================================
// LC-127：单词接龙
// 难度：困难
// 优先级：P0（必做）
// 学习进度：第 3 周 / 第 16 天
// ----------------------------------------------------------------------------
// 题目描述：
// 字典 wordList 中从单词 beginWord 到 endWord 的转换序列是一个按下述规格形成的序列 beginWord -> s_1 -> s_2 -> ... -> s_k。
// 每一对相邻的单词只差一个字母。
// 对于 1 <= i <= k，每个 s_i 都在 wordList 中。
// 注意，beginWord 不需要在 wordList 中。
// s_k == endWord。
// 给你两个单词 beginWord 和 endWord 以及一个字典 wordList，返回从 beginWord 到 endWord 的最短转换序列中的单词数目；如果不存在这样的转换序列，返回 0。
//
// 题目示例：
// 示例 1：
//   Input: beginWord = "hit", endWord = "cog", wordList = ["hot","dot","dog","lot","log","cog"]
//   Output: 5
//   Explanation: One shortest transformation sequence is "hit" -> "hot" -> "dot" -> "dog" -> cog", which is 5 words long.
//
// 示例 2：
//   Input: beginWord = "hit", endWord = "cog", wordList = ["hot","dot","dog","lot","log"]
//   Output: 0
//   Explanation: The endWord "cog" is not in wordList, therefore there is no valid transformation sequence.
//
// 约束与要求：
//   - 1 <= beginWord.length <= 10
//   - endWord.length == beginWord.length
//   - 1 <= wordList.length <= 5000
//   - wordList[i].length == beginWord.length
//   - beginWord、endWord 和 wordList[i] 由小写英文字母组成
//   - beginWord != endWord
//   - wordList 中的所有字符串互不相同
//
// 复杂度目标：O(L^2 * N) 使用 BFS。
//
// ----------------------------------------------------------------------------
// 解法精讲｜隐式无权图最短路：按规则现场生成邻居，而不是先显式两两建图
//
// 0. 优化是怎么来的
//   一个完全正确的直接方案，是先显式建图：对 wordList 中每一对单词逐字符比较，若恰好一位不同就连边，再从 beginWord 做 BFS。它容易证明正确，但 N 个单词会产生 O(N^2) 对比较，每次比较还要看 O(L) 个字符，建图本身就可能达到 O(N^2·L)。
//
//   真正重复的工作是：为了知道当前 word 的邻居，提前把它与所有其他单词都比较了一遍。其实邻居必须只改变一个位置，因此候选空间可以由规则直接生成：固定位置 i，把该字符改成 a..z，最多只产生 25L 个真正可能相邻的字符串，再用哈希集合 O(1) 平均查询它是否在字典中。
//
//   于是“先枚举所有节点对找边”变成“BFS 走到一个节点时现场生成它可能存在的边”。图仍然完全相同，只是不再物化无用的 O(N^2) 候选边。
//
//   优化类型：重复两两比较 -> 利用局部变换规则生成隐式邻居；随后复用无权图 BFS 求最少边数。
//
// 1. 图像直觉
//   例：begin=hit，end=cog。
//
//   不要先画完整图，只看 BFS 当前节点如何生成邻居：
//
//   hit
//    |- 改第0位：ait, bit, ...
//    |- 改第1位：hat, hbt, ... hot ...
//    `- 改第2位：hia, hib, ...
//
//   只有出现在 unused 字典中的候选才是真实边，例如：
//
//   hit -> hot -> dot -> dog -> cog
//           \-> lot -> log --/
//
//   队列按层展开：
//   距离1: hit
//   距离2: hot
//   距离3: dot, lot
//   距离4: dog, log
//   距离5: cog
//
//   这里 distance 表示“序列中包含多少个单词”，所以起点不是 0，而是 1。
//
// 2. 一句话核心
//   把字符串变换规则当成隐式边生成器；BFS 每弹出一个单词，只枚举一位替换得到的真实候选，首次到达 endWord 时就是最短转换序列。
//
// 3. 公式 / 不变量
//   节点：合法单词。
//   边：两个单词长度相同且恰好一个字符不同。
//   每条边代价 = 1。
//
//   BFS 状态：
//   (word, distance)
//   distance = 从 beginWord 到 word 的序列单词数。
//
//   邻居生成：
//   对每个位置 i：
//     对 c in ['a','z'] 且 c != original：
//       candidate = word，仅第 i 位改成 c
//       candidate ∈ unused => 存在一条尚未访问的真实边
//
//   入队：
//   (candidate, distance + 1)
//   并立即从 unused 删除。
//
//   “入队即删除”是不变量的一部分：一个单词只允许第一次、也就是最短距离时进入队列。
//
// 4. 执行步骤
//   1. 把 wordList 放入 unordered_set unused；若 endWord 根本不在字典中，按题意直接返回 0
//   2. 从 unused 删除 beginWord，并以 (beginWord,1) 入队；1 表示序列已经包含起点这个单词
//   3. 弹出当前 word 后，对每个字符位置保存 original，再依次尝试 26 个小写英文字母
//   4. 候选若能在 unused 中 find 到，就以 distance+1 入队，并立刻 erase 该集合元素作为 visited 标记
//   5. 一个位置的所有替换完成后恢复 original，继续下一个位置；首次弹出 endWord 时返回 distance
//   6. 队列耗尽仍未到终点则返回 0
//   记忆：图不用先建出来：当前状态现场生成一跳候选；无权最短路仍然交给 BFS。
//
// 5. 为什么不会漏 / 不会重
//   现场生成邻居不会漏边：任何与 word 恰差一个字符的合法单词，都对应某个唯一位置 i 和替换字符 c，因此一定会在 25L 个候选中出现。BFS 在无权图中按边数非降序发现节点；一个单词第一次入队时已经获得最短距离，所以立即从 unused 删除不会丢失更短路径，只会阻止等长或更长路径重复搜索它。endWord 第一次被处理时的 distance 因而是最短序列长度。
//
// 6. 边界与易错点
//   返回的是序列中的单词数，不是变换次数，所以 beginWord 初始 distance=1。beginWord 不要求存在于 wordList，但 endWord 必须存在。修改 word[i] 后必须在完成该位置枚举时恢复 original，否则下一位置会在已经被修改的字符串上继续生成错误候选。纯查询使用 find，删除使用 erase(it)，均兼容 C++17。
//
// 7. 举一反三
//   直接复用 Day 15 的 BFS/visited，只把“邻接表”换成“邻居生成函数”。以后遇到锁组合、基因变换、拼图、最少操作数等问题，若每一步只做局部合法修改且代价相同，应优先问：状态是不是节点？操作是不是隐式边？是否根本不需要把整张图显式建出来？
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：beginWord endWord。
//   第 2 行：L (wordList 中的单词数量)。
//   第 3 行：L 个以空格分隔的单词。
//   输出：最短转换序列的长度（单词数）。
// test.in 的预期输出：5
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        // unused 同时承担两项职责：
        // 1) 字典：候选字符串只有仍存在于 unused 中，才是图里的合法单词节点；
        // 2) visited：一个单词第一次入队后立刻从 unused 删除，之后不会被等长/更长路径重复搜索。
        unordered_set<string> unused(wordList.begin(), wordList.end());

        // 题目要求转换序列最后必须是 wordList 中的 endWord；若它根本不在字典中，不存在合法答案。
        if (!unused.count(endWord)) return 0;

        // beginWord 本身不要求出现在 wordList；若恰好存在，先删除它，避免后续某次字符替换又绕回起点。
        unused.erase(beginWord);

        // BFS 状态的 distance 统计“序列中包含多少个单词”，不是已经做了多少次变换；
        // 因此起点 beginWord 本身已经占 1 个单词，初始距离必须是 1。
        queue<pair<string, int>> pending;
        pending.push({beginWord, 1});

        while (!pending.empty()) {
            auto [word, distance] = pending.front();
            pending.pop();

            // 无权图 BFS 按距离非降序出队；endWord 第一次出队时，这个 distance 就是最短序列长度。
            if (word == endWord) return distance;

            // 图没有预先显式建边：当前 word 的所有真实邻居，都由“只改一个字符”这个规则现场生成。
            for (int i = 0; i < static_cast<int>(word.size()); ++i) {
                const char original = word[i];

                for (char c = 'a'; c <= 'z'; ++c) {
                    if (c == original) continue;
                    word[i] = c;

                    // find 是纯查询：只判断这个一位替换后的字符串是否仍是未访问的合法字典节点。
                    // 若找到，第一次发现已经是最短距离，所以先入队，再 erase(it) 立即完成 visited 标记。
                    auto it = unused.find(word);
                    if (it != unused.end()) {
                        pending.push({word, distance + 1});
                        unused.erase(it);
                    }
                }

                // 内层枚举会原地改写 word[i]；处理完这一位后必须恢复原字符，
                // 否则下一位置会在“已经改过一位”的错误字符串上继续生成候选，等价于一次改多个字符。
                word[i] = original;
            }
        }

        // 所有从 beginWord 可达的未访问单词都已处理仍未到终点，说明不存在转换序列。
        return 0;
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    string b, e; if (!(cin >> b >> e)) return 0;
    int L; cin >> L;
    vector<string> wl(L);
    for (int i = 0; i < L; ++i) cin >> wl[i];
    cout << sol.ladderLength(b, e, wl) << "\n";
    return 0;
}
