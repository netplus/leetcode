"""Chinese learner-facing metadata backed by an offline statement cache."""

from pathlib import Path
import json
import re
import textwrap

from chinese_titles import get_chinese_title


HERE = Path(__file__).resolve().parent
PROBLEM_INFO = {
    int(num): value
    for num, value in json.loads(
        (HERE / "chinese_problem_info.json").read_text(encoding="utf-8")
    ).items()
}

DIFFICULTY = {"Easy": "简单", "Medium": "中等", "Hard": "困难"}
PRIORITY = {"P0": "P0（必做）", "P1": "P1（进阶）"}


GOAL_REPLACEMENTS = {
    "total characters": "字符总数",
    "two binary searches": "两次二分查找",
    "complete knapsack": "完全背包",
    "topological sort": "拓扑排序",
    "multi-source BFS": "多源 BFS",
    "monotonic stack": "单调栈",
    "monotonic deque": "单调队列",
    "difference array": "差分数组",
    "union-find": "并查集",
    "binary search": "二分查找",
    "interval DP": "区间 DP",
    "backtracking": "回溯",
    "quickselect": "快速选择",
    "min-heap": "小根堆",
    "two heaps": "双堆",
    "two pointers": "双指针",
    "greedy by count": "按频次贪心",
    "output excluded": "不计输出结果",
    "0/1 knapsack": "0/1 背包",
    "in-place": "原地",
    "iterative": "迭代",
    "follow-up": "进阶要求",
    "greedy": "贪心",
    "extra": "额外空间",
    "expand": "中心扩展",
    "naive": "朴素匹配",
    "with hash map": "使用哈希表",
    "where": "其中",
    "build": "预处理",
    "query": "查询",
    "per operation": "每次操作",
    "per addNum": "每次 addNum",
    "time": "时间",
    "extra space": "额外空间",
    "space": "空间",
    "avg": "平均",
    "or": "或",
    "with": "使用",
}


IO_EXACT = {
    "Print each triplet \"a b c\" on its own line; triplets sorted, no duplicates.":
        "每行输出一个三元组 \"a b c\"；三元组按字典序排列且不重复。",
    "Print merged intervals one per line (start end), sorted by start.":
        "每行输出一个合并后的区间 start end，并按起点升序排列。",
    "Print merged intervals one per line (start end).":
        "每行输出一个合并后的区间 start end。",
    "Line 1: na (len listA) nb (len listB) (skipA skipB ignored here)":
        "第 1 行：na（listA 长度）、nb（listB 长度）；本地适配器忽略 skipA 和 skipB。",
    "Line 2: na values of listA (with shared suffix appended)":
        "第 2 行：listA 的 na 个节点值，末尾包含共享后缀。",
    "Line 3: nb values of listB (same shared suffix)":
        "第 3 行：listB 的 nb 个节点值，末尾使用相同的共享后缀。",
    "Line 4: interLen (number of shared nodes)":
        "第 4 行：interLen（共享节点数）。",
    "Build: last interLen nodes are shared. Print the intersected node value, or -1.":
        "构造时令最后 interLen 个节点共享；输出相交节点的值，不相交则输出 -1。",
    "pos = index where tail connects (-1 = no cycle)":
        "pos 表示链表尾连回的下标，-1 表示无环。",
    "Print the clone's adjacency list: for each node i, its neighbors space-separated,":
        "输出克隆图的邻接表：对每个节点 i，以空格分隔其所有邻居，",
    "one node per line.": "每行对应一个节点。",
    "Then k blocks: each block is a line with li (length) then a line with li values":
        "随后读取 k 个数据块：每块先用一行给出 li（链表长度），再用一行给出 li 个节点值。",
    "op = 1 x  -> push(x)": "op = 1 x：执行 push(x)。",
    "op = 2    -> pop()": "op = 2：执行 pop()。",
    "op = 3    -> top()  -> print x": "op = 3：执行 top() 并输出 x。",
    "op = 4    -> getMin() -> print min": "op = 4：执行 getMin() 并输出最小值。",
    "op = 1 x  -> addNum(x)": "op = 1 x：执行 addNum(x)。",
    "op = 2    -> findMedian() -> print median (as float)":
        "op = 2：执行 findMedian() 并以浮点数输出中位数。",
}


IO_REPLACEMENTS = {
    "the whole string s (may have leading/trailing/multiple spaces)":
        "完整字符串 s（可含前导、尾随或连续空格）",
    "each row as a string of n chars ('0'/'1')":
        "每行是由 n 个字符（'0'/'1'）组成的字符串",
    "each row as a string of n chars": "每行是由 n 个字符组成的字符串",
    "number of operations": "操作数量",
    "number of intervals": "区间数量",
    "number of queries": "查询数量",
    "number of strings": "字符串数量",
    "number of equations": "方程数量",
    "number of tasks": "任务数量",
    "number of words in wordList": "wordList 中的单词数量",
    "number of words": "单词数量",
    "number of nodes, labeled 1..n": "节点数，编号为 1..n",
    "number of edges == n": "边数，等于 n",
    "number of edges": "边数",
    "number of nodes": "节点数",
    "number of shared nodes": "共享节点数",
    "length of list1": "list1 的长度",
    "length of list2": "list2 的长度",
    "nth from end to remove": "要删除的倒数第 k 个节点",
    "space-separated integers per row": "个以空格分隔的整数",
    "space-separated integers": "个以空格分隔的整数",
    "space-separated node values": "个以空格分隔的节点值",
    "space-separated coin denominations": "个以空格分隔的硬币面额",
    "space-separated words": "个以空格分隔的单词",
    "space-separated candidates": "个以空格分隔的候选数",
    "space-separated": "以空格分隔",
    "level-order values": "个层序节点值",
    "n space-separated integers (level order, -1 = null)":
        "n 个以空格分隔的层序节点值（-1 表示空节点）",
    "n level-order values (-1 = null)":
        "n 个层序节点值（-1 表示空节点）",
    "preorder values": "个前序遍历值",
    "inorder values": "个中序遍历值",
    "values of list1": "个 list1 节点值",
    "values of list2": "个 list2 节点值",
    "n values": "n 个节点值",
    "gas values": "个 gas 值",
    "cost values": "个 cost 值",
    "each block is": "每个数据块是",
    "each row": "每行",
    "one string each": "每行一个字符串",
    "one equation string each": "每行一个方程字符串",
    "a string of n characters (the tasks)": "由 n 个任务字符组成的字符串",
    "edge between u and v": "u 与 v 之间的边",
    "u is parent of v": "u 是 v 的父节点",
    "must take b before a": "选修 a 之前必须先选修 b",
    "b before a": "a 之前必须先处理 b",
    "the two nodes": "两个节点",
    "p q (values of the two nodes)": "p、q（两个节点的值）",
    "the new interval": "新区间",
    "the string s": "字符串 s",
    "the word": "待查找单词",
    "the tasks": "任务序列",
    "queries": "查询",
    "bookings": "预订记录",
    "flights": "航班数",
    "length": "长度",
    "cooldown": "冷却时间",
}


OUTPUT_REPLACEMENTS = {
    "the count of subarrays summing to k": "和等于 k 的连续子数组数量",
    "the number of nice subarrays": "优美子数组的数量",
    "the maximum subarray sum": "最大子数组和",
    "the maximum area": "最大面积",
    "the minimal length": "最短长度",
    "the length": "长度",
    "the index (0-based) or -1": "从 0 开始计数的下标；未找到时输出 -1",
    "1 if can attend all (no overlap), else 0": "能参加全部会议（无重叠）时输出 1，否则输出 0",
    "the longest consecutive sequence length": "最长连续序列的长度",
    "1 if cycle, else 0": "链表有环时输出 1，否则输出 0",
    "the value of the cycle-entry node, or -1": "入环节点值；无环时输出 -1",
    "1 if palindrome, else 0": "是回文链表时输出 1，否则输出 0",
    "the maximum depth": "二叉树的最大深度",
    "the LCA node value": "最近公共祖先节点的值",
    "the diameter (number of edges)": "二叉树直径（边数）",
    "1 if valid BST, else 0": "是有效二叉搜索树时输出 1，否则输出 0",
    "1 if valid, else 0": "有效时输出 1，否则输出 0",
    "the largest rectangle area": "柱状图中的最大矩形面积",
    "the maximum path sum": "最大路径和",
    "the number of islands": "岛屿数量",
    "the maximum island area": "岛屿的最大面积",
    "1 if all courses can be finished, else 0": "能完成所有课程时输出 1，否则输出 0",
    "the minimum minutes, or -1": "所需的最少分钟数；无法完成时输出 -1",
    "the number of provinces": "省份数量",
    "the redundant edge \"u v\"": "冗余边 \"u v\"",
    "1 if satisfiable, else 0": "方程组可满足时输出 1，否则输出 0",
    "the number of ways": "方案数",
    "the maximum amount": "可获得的最大金额",
    "the minimum cost": "最小花费",
    "the number of unique paths": "不同路径数量",
    "the minimum path sum": "最小路径和",
    "the LIS length": "最长递增子序列长度",
    "the LCS length": "最长公共子序列长度",
    "1 if partitionable into equal sums, else 0": "能分割为两个等和子集时输出 1，否则输出 0",
    "the number of expressions evaluating to target": "计算结果等于 target 的表达式数量",
    "the fewest number of coins, or -1": "最少硬币数；无法凑出时输出 -1",
    "the maximum coins": "可获得的最大硬币数",
    "1 if the word exists, else 0": "单词存在时输出 1，否则输出 0",
    "1 if s can be segmented, else 0": "s 可被拆分时输出 1，否则输出 0",
    "the maximum product": "最大乘积",
    "the index, or -1": "下标；未找到时输出 -1",
    "\"left right\" (the bounds), or \"-1 -1\"": "边界 \"left right\"；未找到时输出 \"-1 -1\"",
    "the minimum": "最小值",
    "the minimized largest sum": "最小化后的最大子数组和",
    "the minimum eating speed": "最小吃香蕉速度",
    "the least ship capacity": "运载能力的最小值",
    "the number of content children": "可以满足的孩子数量",
    "1 if reachable, else 0": "能到达最后一个下标时输出 1，否则输出 0",
    "the minimum number of jumps": "最少跳跃次数",
    "the starting index, or -1": "可行的起点下标；不存在时输出 -1",
    "the kth largest element": "第 k 大元素",
    "the minimum number of rooms": "最少会议室数量",
    "the total units of trapped water": "可接雨水的总量",
    "the minimum units of time": "完成所有任务的最少时间单位",
    "the length of the shortest transformation sequence (number of words)":
        "最短转换序列的长度（单词数）",
    "the median (as a float, e.g. 2.0 or 2.5)":
        "中位数（以浮点数表示，例如 2.0 或 2.5）",
    "the reconstructed queue, one \"h k\" per line":
        "重建后的队列，每行输出一组 \"h k\"",
    "each subset on its own line, space-separated (empty subset = blank line)":
        "所有子集，每行一个且元素以空格分隔（空集输出空行）",
    "each permutation on its own line, space-separated":
        "所有排列，每行一个且元素以空格分隔",
    "each combination on its own line, space-separated":
        "所有组合，每行一个且元素以空格分隔",
    "each combination on its own line": "所有组合，每行一个",
    "the constructed tree's level order (-1 = null)":
        "构造后二叉树的层序序列（-1 表示空节点）",
    "the inverted tree's level order (-1 = null)":
        "翻转后二叉树的层序序列（-1 表示空节点）",
    "the distance matrix, one row per line space-separated":
        "距离矩阵，每行输出矩阵的一行，元素以空格分隔",
    "each coordinate \"r c\" on its own line, sorted":
        "所有坐标，排序后每行输出一组 \"r c\"",
    "the ordering space-separated, or -1 if impossible":
        "一种以空格分隔的课程顺序；无法完成时输出 -1",
    "the k most frequent elements space-separated (any order)":
        "频率最高的 k 个元素，以空格分隔，顺序任意",
    "the array after moving zeroes (space-separated)":
        "移动零之后的数组，元素以空格分隔",
    "the start indices (space-separated, ascending)":
        "所有起始下标，以空格分隔并升序排列",
    "the two indices (space-separated, ascending)":
        "两个下标，以空格分隔并升序排列",
    "two 1-indexed indices (space-separated, ascending)":
        "两个从 1 开始计数的下标，以空格分隔并升序排列",
    "one level per line, space-separated": "层序遍历结果，每层一行且以空格分隔",
    "each sum on its own line": "每次查询的区间和，每个结果单独一行",
    "each region sum on its own line": "每次查询的二维区域和，每个结果单独一行",
    "n space-separated integers (seats per flight, 1-indexed)":
        "n 个以空格分隔的整数，表示从 1 开始编号的每个航班的座位数",
    "n space-separated integers (the products)": "n 个以空格分隔的乘积结果",
    "(n-k+1) space-separated maxes": "n-k+1 个以空格分隔的窗口最大值",
    "n space-separated integers (days to wait)": "n 个以空格分隔的等待天数",
    "the bounds": "左右边界",
    "the minimum window substring": "最小覆盖子串",
    "the longest palindromic substring": "最长回文子串",
    "the longest common prefix": "最长公共前缀",
    "the reversed, single-space-joined words": "反转单词顺序后、以单个空格连接的字符串",
    "the largest number string": "可拼接出的最大数字符串",
    "the smallest missing positive integer": "缺失的最小正整数",
    "the next permutation (space-separated)": "下一个排列，元素以空格分隔",
    "the reversed list values space-separated": "反转后的链表节点值，以空格分隔",
    "the resulting list values space-separated": "操作后的链表节点值，以空格分隔",
    "the merged list values space-separated": "合并后的链表节点值，以空格分隔",
    "the sorted list values space-separated": "排序后的链表节点值，以空格分隔",
    "the reordered list values space-separated": "重排后的链表节点值，以空格分隔",
    "the preorder traversal space-separated": "前序遍历结果，以空格分隔",
    "the inorder traversal space-separated": "中序遍历结果，以空格分隔",
    "the postorder traversal space-separated": "后序遍历结果，以空格分隔",
    "the right-side-view values space-separated": "右视图节点值，以空格分隔",
}


def localize_goal(goal: str) -> str:
    result = goal
    for source, target in sorted(GOAL_REPLACEMENTS.items(), key=lambda item: -len(item[0])):
        result = result.replace(source, target)
    result = result.replace(", ", "，").rstrip(".")
    if " (" in result and result.endswith(")"):
        prefix, note = result.rsplit(" (", 1)
        result = prefix + "（" + note[:-1] + "）"
    return result + "。"


def _replace_all(text: str, replacements: dict[str, str]) -> str:
    for source, target in sorted(replacements.items(), key=lambda item: -len(item[0])):
        text = text.replace(source, target)
    return text


def localize_io_line(line: str) -> str:
    line = line.removeprefix("//").strip()
    if line in IO_EXACT:
        return IO_EXACT[line]

    match = re.fullmatch(r"Line (\d+): (.+)", line)
    if match:
        body = _replace_all(match.group(2), IO_REPLACEMENTS)
        return f"第 {match.group(1)} 行：{body}。"
    match = re.fullmatch(r"Line: (.+)", line)
    if match:
        return f"一行：{_replace_all(match.group(1), IO_REPLACEMENTS)}。"
    match = re.fullmatch(r"Next (\w+) lines: (.+)", line)
    if match:
        body = _replace_all(match.group(2), IO_REPLACEMENTS)
        return f"接下来 {match.group(1)} 行：{body}。"
    match = re.fullmatch(r"Last line: (.+)", line)
    if match:
        return f"最后一行：{_replace_all(match.group(1), IO_REPLACEMENTS)}。"
    match = re.fullmatch(r"Print (.+)\.", line)
    if match:
        body = _replace_all(match.group(1), OUTPUT_REPLACEMENTS)
        body = _replace_all(body, IO_REPLACEMENTS)
        return f"输出：{body}。"
    return _replace_all(line, IO_REPLACEMENTS)


def localize_io(iofmt: str) -> list[str]:
    return [localize_io_line(line) for line in iofmt.splitlines() if line.strip()]


def comment_lines(lines: list[str], prefix: str = "// ", width: int = 106) -> str:
    rendered = []
    for line in lines:
        wrapped = textwrap.wrap(
            line,
            width=width - len(prefix),
            break_long_words=False,
            break_on_hyphens=False,
        ) or [""]
        rendered.extend(prefix + part for part in wrapped)
    return "\n".join(rendered)


def get_chinese_metadata(problem: dict) -> dict:
    info = PROBLEM_INFO[problem["num"]]
    return {
        "title": get_chinese_title(problem["num"]),
        "difficulty": DIFFICULTY[problem["diff"]],
        "priority": PRIORITY[problem["prio"]],
        "description": comment_lines(info["description"]),
        "constraints": comment_lines(info["constraints"], prefix="//   - "),
        "goal": localize_goal(problem["goal"]),
        "iofmt": comment_lines(localize_io(problem["iofmt"]), prefix="//   "),
    }


def validate_chinese_metadata(problems: list[dict]) -> None:
    expected = {problem["num"] for problem in problems}
    actual = set(PROBLEM_INFO)
    if expected != actual:
        raise RuntimeError(
            f"Chinese metadata coverage mismatch: "
            f"missing={sorted(expected - actual)}, extra={sorted(actual - expected)}"
        )
    for problem in problems:
        data = get_chinese_metadata(problem)
        if any(not value for value in data.values()):
            raise RuntimeError(f"LC-{problem['num']}: empty Chinese metadata field")
