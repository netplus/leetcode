"""Curated learner-facing statement fixes derived from official LeetCode text.

`chinese_problem_info.json` is a refreshable cache of LeetCode China content.  A
few official translations are older/less explicit than the English statement,
and HTML-to-text extraction can occasionally leave malformed prose.  Keep such
reviewed fixes here so refreshing the cache cannot silently re-introduce them.

Only statement/title/constraint content belongs here.  Algorithm explanations
remain in the pedagogy modules.
"""

STATEMENT_OVERRIDES = {
    # LeetCode CN currently says only "小写字母"; the English official statement
    # explicitly says "lowercase English letters".  Preserve the stricter input
    # domain wording for learner-facing material.
    438: {
        "constraints": [
            "1 <= s.length, p.length <= 3 * 10^4",
            "s 和 p 仅包含小写英文字母",
        ],
    },

    # The current CN statement uses an older compact constraint block that does
    # not mention target.  The English official statement has the complete four
    # constraints below.
    704: {
        "constraints": [
            "1 <= nums.length <= 10^4",
            "-10^4 < nums[i], target < 10^4",
            "nums 中的所有整数互不相同",
            "nums 按严格升序排列",
        ],
    },

    # The remaining entries repair HTML/subscript extraction artifacts while
    # preserving the official semantics.
    127: {
        "description": [
            "字典 wordList 中从单词 beginWord 到 endWord 的转换序列是一个按下述规格形成的序列 beginWord -> s_1 -> s_2 -> ... -> s_k。",
            "每一对相邻的单词只差一个字母。",
            "对于 1 <= i <= k，每个 s_i 都在 wordList 中。",
            "注意，beginWord 不需要在 wordList 中。",
            "s_k == endWord。",
            "给你两个单词 beginWord 和 endWord 以及一个字典 wordList，返回从 beginWord 到 endWord 的最短转换序列中的单词数目；如果不存在这样的转换序列，返回 0。",
        ],
    },
    207: {
        "description": [
            "你这个学期必须选修 numCourses 门课程，记为 0 到 numCourses - 1。",
            "在选修某些课程之前需要一些先修课程。",
            "先修课程按数组 prerequisites 给出，其中 prerequisites[i] = [a_i, b_i]，表示如果要学习课程 a_i，则必须先学习课程 b_i。",
            "例如，先修课程对 [0, 1] 表示：想要学习课程 0，你需要先完成课程 1。",
            "请你判断是否可能完成所有课程的学习？如果可以，返回 true；否则，返回 false。",
        ],
    },
    236: {
        "description": [
            "给定一个二叉树，找到该树中两个指定节点的最近公共祖先。",
            "最近公共祖先是满足以下条件的节点 x：x 是 p、q 的祖先，并且 x 的深度尽可能大；一个节点也可以是它自己的祖先。",
        ],
    },
    406: {
        "description": [
            "假设有打乱顺序的一群人站成一个队列，数组 people 表示队列中一些人的属性（不一定按顺序）。",
            "每个 people[i] = [h_i, k_i] 表示第 i 个人的身高为 h_i，前面正好有 k_i 个身高大于或等于 h_i 的人。",
            "请你重新构造并返回输入数组 people 所表示的队列。",
            "返回的队列应该格式化为数组 queue，其中 queue[j] = [h_j, k_j] 是队列中第 j 个人的属性（queue[0] 是排在队列前面的人）。",
        ],
    },
    455: {
        "description": [
            "假设你是一位很棒的家长，想要给你的孩子们一些小饼干。",
            "但是，每个孩子最多只能给一块饼干。",
            "对每个孩子 i，都有一个胃口值 g[i]，这是能让孩子满足胃口的饼干的最小尺寸；并且每块饼干 j 都有一个尺寸 s[j]。",
            "如果 s[j] >= g[i]，我们可以将饼干 j 分配给孩子 i，这个孩子会得到满足。",
            "你的目标是满足尽可能多的孩子，并输出这个最大数值。",
        ],
    },
    45: {
        "description": [
            "给定一个长度为 n 的 0 索引整数数组 nums。",
            "初始位置在下标 0。",
            "每个元素 nums[i] 表示从索引 i 向后跳转的最大长度。",
            "如果你在索引 i 处，可以跳转到任意 i + j，其中 0 <= j <= nums[i] 且 i + j < n。",
            "返回到达 n - 1 的最小跳跃次数。",
            "测试用例保证可以到达 n - 1。",
        ],
    },
}
