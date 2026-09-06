#!/usr/bin/env python3
"""Temporary generator for the repo-wide semantic case expansion pass.

Each entry below adds exactly one new, independently reasoned scenario to a
formal suite that currently has six cases.  Complex enumeration outputs are
produced by small Python reference enumerators rather than the production C++
solution.
"""

from __future__ import annotations

import itertools
import re
from dataclasses import dataclass
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
PROBLEM_RE = re.compile(r"day\d+-lc(\d+)-")


@dataclass(frozen=True)
class Case:
    inp: str
    out: str | None
    labels: str
    purpose: str


def gen_parentheses(n: int) -> str:
    result: list[str] = []

    def dfs(s: str, opened: int, closed: int) -> None:
        if len(s) == 2 * n:
            result.append(s)
            return
        if opened < n:
            dfs(s + "(", opened + 1, closed)
        if closed < opened:
            dfs(s + ")", opened, closed + 1)

    dfs("", 0, 0)
    return "\n".join(result) + "\n"


def gen_permutations(values: list[int]) -> str:
    return "\n".join(" ".join(map(str, p)) for p in itertools.permutations(values)) + "\n"


def gen_subsets(values: list[int]) -> str:
    rows: list[str] = []

    def dfs(start: int, path: list[int]) -> None:
        rows.append(" ".join(map(str, path)))
        for i in range(start, len(values)):
            path.append(values[i])
            dfs(i + 1, path)
            path.pop()

    dfs(0, [])
    return "\n".join(rows) + "\n"


CASES: dict[int, Case] = {
    1: Case("5 1000000000\n1000000000 -1000000000 0 7 8\n", "0 2\n", "extreme,trap", "接近约束上界且补数跨正负：验证 complement 计算、历史哈希命中和大整数边界不依赖小数值样例"),
    3: Case("\n", "0\n", "empty-input,boundary", "空字符串：验证滑窗在没有任何字符时直接得到长度 0，而不是默认返回 1 或访问 s[0]"),
    4: Case("3 4\n1 2 2\n2 2 3 4\n", "2\n", "duplicates,trap", "两个有序数组在分割线附近存在大量重复 2：验证分割条件允许等号，偶/奇边界不会因重复值走错方向"),
    5: Case("forgeeksskeegfor\n", "geeksskeeg\n", "even-length,stress", "最长回文位于字符串内部且为较长偶数回文 geeksskeeg：验证中心在字符间并能连续向两侧扩展多层"),
    11: Case("6\n1 8 6 2 5 4\n", "16\n", "trap,two-pointers", "最优容器不是最外侧也不是简单选择数值最大的两根柱：验证双指针移动依据较矮边且同时权衡高度与宽度"),
    14: Case("3\ninterstellar\ninternet\ninternal\n", "inter\n", "late-mismatch,trap", "三个字符串共享较长前缀后才分叉：验证逐列扫描能在较晚位置准确停止，而不是只比较首字符或整串相等"),
    15: Case("7\n-2 0 1 1 2 -1 -4\n", "-2 0 2\n-2 1 1\n-1 0 1\n", "stress,duplicates", "同一输入同时产生三个唯一三元组，其中一个需要重复值 1：验证去重不能误删合法的不同下标组合"),
    19: Case("7 6\n1 2 3 4 5 6 7\n", "1 3 4 5 6 7\n", "boundary,stress", "较长链表删除正数第 2 个节点（倒数第 6 个）：验证快慢指针固定间距在靠近头部但非 head 的位置仍正确"),
    21: Case("3\n1 2 3\n3\n10 11 12\n", "1 2 3 10 11 12\n", "ordering,tail-append", "第一条链表所有节点都小于第二条：验证主循环耗尽一侧后可一次接上另一侧完整有序后缀"),
    22: Case("7\n", None, "stress,depth", "n=7 产生 429 个合法结果：以更深搜索树验证剪枝、递归终止与回溯恢复在大规模枚举下仍无遗漏和重复"),
    23: Case("3\n0\n0\n0\n", "\n", "empty-list,boundary", "k>0 但所有链表都为空：验证初始化堆时跳过所有 null，最终返回空链而不是访问空表头"),
    28: Case("aaaaab aab\n", "3\n", "partial-match,trap", "前面连续出现多个可作为部分前缀的 a，真正匹配只在尾部开始：验证部分匹配失败后仍能找到最左完整匹配"),
    31: Case("3\n1 3 2\n", "2 1 3\n", "suffix,trap", "pivot 位于最左侧且右侧后缀已降序：验证交换后必须把整个后缀恢复为最小升序，不能只交换两个元素"),
    33: Case("5 1\n5 1 2 3 4\n", "1\n", "rotation,boundary", "旋转点紧邻数组开头且 target 正好是旋转后的最小值：验证左右有序段判定在 pivot 边界不丢目标"),
    34: Case("4 0\n1 2 3 4\n", "-1 -1\n", "no-solution,boundary", "target 小于数组最小值：与已有 target>max 对称，验证左边界二分收缩到 0 后仍能正确判定未命中"),
    39: Case("4 7\n7 2 3 6\n", "2 2 3\n7\n", "ordering,trap", "candidates 故意无序且同时存在重复使用组合与单元素组合：验证先排序后剪枝，并保留两类答案"),
    41: Case("4\n2 2 1 1\n", "3\n", "duplicates,trap", "有效范围内的 1/2 都大量重复：验证索引放置循环必须识别目标槽已有同值，避免无限交换并正确得到缺失 3"),
    45: Case("4\n10 0 0 0\n", "1\n", "direct,boundary", "起点一步即可越过终点：验证第一次层边界结算后立即得到 1，不会继续扫描尾部 0 并多计步数"),
    46: Case("5\n1 2 3 4 5\n", None, "stress,backtracking", "五个不同元素产生 120 个排列：用更深选择树验证 used/path 的设置与撤销在大量兄弟分支间无状态污染"),
    53: Case("6\n5 -100 1 2 3 4\n", "10\n", "restart,trap", "早期有正收益但被巨大负数彻底切断，真正最优段位于末尾：验证 current 可以重启且 best 不会锁死在早期候选"),
    55: Case("5\n4 0 0 0 0\n", "1\n", "direct,boundary", "起点直接覆盖所有后续 0：验证只要当前位置可达就能用 i+nums[i] 扩大 farthest，不会把中间 0 误判成阻断"),
    56: Case("3\n1 4\n1 5\n1 3\n", "1 5\n", "duplicates,containment", "三个区间开始点完全相同但结束点不同：验证排序后合并右边界必须持续取 max，而不是被较短区间覆盖"),
    57: Case("1\n1 10\n3 5\n", "1 10\n", "containment,trap", "新区间完全被已有区间包含：验证合并后边界保持 [1,10]，不能错误缩窄成新区间或额外输出两段"),
    70: Case("45\n", "1836311903\n", "stress,upper-bound", "接近题目上界 n=45：验证滚动递推经过大量轮次仍保持正确更新顺序和 32 位结果边界"),
    78: Case("5\n1 2 3 4 5\n", None, "stress,backtracking", "五元素产生 32 个子集：验证每个中间 path 都被收集且 start 递增能在更深树中继续保证唯一构造路径"),
    92: Case("6 2 5\n1 2 2 3 3 4\n", "1 3 3 2 2 4\n", "duplicates,trap", "反转段内部有重复值：验证算法按节点链接而非数值身份操作，并正确重接区间两端"),
    94: Case("7\n1 2 -1 3 -1 4 -1\n", "4 3 2 1\n", "left-chain,ordering", "连续左链：与已有右链对称，验证迭代中序必须一路压栈到最左叶子后再逐层回退输出"),
    102: Case("7\n1 2 -1 3 -1 4 -1\n", "1\n2\n3\n4\n", "left-chain,ordering", "连续左链形成四个单节点层：验证层序逻辑不依赖 right child，且每一层仍单独结算"),
    104: Case("7\n1 -1 2 -1 3 -1 4\n", "4\n", "right-chain,ordering", "连续右链：与已有左链对称，验证深度计算不偏向某一子树并正确累计到 4"),
    105: Case("7\n4 2 1 3 6 5 7\n1 2 3 4 5 6 7\n", "4 2 6 1 3 5 7\n", "balanced,stress", "完整三层平衡树：验证每一层都同时构造左右子树，preorder 游标和 inorder 区间在多分支递归中保持同步"),
    124: Case("3\n0 2 3\n", "5\n", "zero,tree-dp", "根值为 0 且左右均为正：验证 0 可以合法连接两侧贡献，最大路径为 2-0-3 而不是只取单侧 3"),
    127: Case("aaa bbb\n5\naab\nabb\nbbb\naba\nbaa\n", "4\n", "branching,trap", "存在多个第一步候选但最短链为 aaa→aab→abb→bbb：验证 BFS 按层首次到达目标，而不是沿某条分支深搜到底"),
    128: Case("5\n1 2 2 3 4\n", "4\n", "duplicates,trap", "重复值嵌在连续序列内部：验证哈希集合去重后仍把 1..4 视为长度 4 的一条序列，重复 2 不增加也不截断长度"),
    133: Case("4 5\n1 2\n2 3\n3 4\n4 1\n1 3\n", "2 3 4\n1 3\n1 2 4\n1 3\n", "dense,cycle", "四节点图同时含多个环和一条弦：验证克隆映射在高复用邻居关系下仍保持唯一节点身份并复制全部边"),
    134: Case("3\n2 0 2\n1 2 1\n", "2\n", "exact-balance,boundary", "全局净油量恰好为 0 且只有末尾起点可完成一圈：验证 total==0 仍可行，并能在多次局部亏空后重置到正确起点"),
    141: Case("2 0\n1 2\n", "1\n", "minimum-cycle,cycle", "两个节点首尾相连形成最小非自环：验证快慢指针不仅能识别 self-cycle，也能识别长度 2 的环"),
    142: Case("2 0\n1 2\n", "1\n", "minimum-cycle,cycle", "两个节点构成长度 2 的整链环且入口为 head：验证相遇后第二阶段能精确返回入口节点 1"),
    143: Case("5\n1 1 2 2 3\n", "1 3 1 2 2\n", "duplicates,trap", "奇数链表含重复值：验证重排只改变 next 链接并保留每个节点一次，不能按值去重或误判中点"),
    144: Case("7\n1 -1 2 -1 3 -1 4\n", "1 2 3 4\n", "right-chain,ordering", "连续右链：验证显式栈在每层没有左孩子时仍按 root-right 的自然顺序逐级访问"),
    145: Case("7\n1 -1 2 -1 3 -1 4\n", "4 3 2 1\n", "right-chain,ordering", "连续右链：验证后序必须从最深叶子开始逐级回退，根节点最后输出"),
    148: Case("2\n2 1\n", "1 2\n", "minimum,ordering", "两个逆序节点：验证最小非平凡分治能够正确切成两个单节点并重新归并，且不会丢失尾指针"),
    151: Case("1  23   456\n", "456 23 1\n", "digits,whitespace", "单词由不同长度数字串组成且间隔多空格：验证解析只按空格切词、保留词内字符并逆转词序"),
    153: Case("5\n2 3 4 5 1\n", "1\n", "pivot-at-end,boundary", "最小值位于最后一个下标：验证二分在旋转断点紧贴右边界时不会错误保留左侧有序段"),
    155: Case("8\n1 2\n1 1\n2\n1 0\n4\n2\n4\n3\n", "0\n2\n2\n", "state-transition,trap", "最小值先下降、弹出、再降到新值并再次弹出：验证辅助最小状态能多次恢复而不是只维护全局历史最小"),
    160: Case("4 3\n1 2 3 9\n7 8 9\n1\n", "9\n", "minimum-intersection,boundary", "两条不同长度前缀只共享最后一个尾节点：验证交点可以是共享区域长度 1 的最小尾交汇"),
    167: Case("6 9\n1 2 4 5 7 11\n", "2 5\n", "interior,trap", "正确配对位于数组内部而非两个端点：验证二指针能连续收缩两侧并返回题目要求的 1-based 下标"),
    179: Case("2\n121 12\n", "12121\n", "common-prefix,trap", "121 与 12 有长共同前缀：验证应比较 12112 与 12121，因此 12 必须排在 121 前，不能用普通字典序"),
    199: Case("5\n1 2 3 -1 5\n", "1 3 5\n", "fallback-left,trap", "右子树在较浅层结束后，更深层可见节点来自左子树：验证右视图不是沿 right 指针走到底，而是每层取最右实际节点"),
    200: Case("1 6\n101101\n", "3\n", "single-row,component", "单行网格中同时存在单点岛和双格岛：验证二维搜索退化到一维时仍按 0 分隔连通分量并得到 3 座岛"),
    206: Case("7\n1 2 3 4 5 6 7\n", "7 6 5 4 3 2 1\n", "stress,odd-length", "较长奇数链表：验证多轮原地 rewiring 后所有节点次序完整反转且原 head 最终正确指向 null"),
    207: Case("4 4\n1 0\n2 0\n3 1\n3 2\n", "1\n", "branching,dag", "菱形 DAG 中课程 3 同时依赖 1/2：验证入度必须等两条前置边都删除后才降到 0，不能第一次访问就提前入队"),
    209: Case("4 7\n1 1 1 7\n", "1\n", "late-answer,trap", "最短合法窗口长度 1 只在数组末尾出现：验证窗口在已有较长候选后仍继续收缩并更新到更优答案"),
    210: Case("4 4\n1 0\n2 0\n2 1\n3 2\n", "0 1 2 3\n", "multi-prerequisite,ordering", "课程 2 同时依赖 0/1，且 3 再依赖 2：约束形成唯一拓扑序 0,1,2,3，验证多前置计数和输出顺序"),
    215: Case("6 3\n-1 -5 -3 -2 -4 -6\n", "-3\n", "negative,boundary", "全负数组求第 3 大：验证堆/quickselect 使用真实数值顺序，不会把绝对值或默认 0 当作比较基准"),
    226: Case("7\n1 2 3 4 -1 -1 5\n", "1 3 2 5 -1 -1 4\n", "asymmetric,stress", "左右子树内部形状不同：验证不仅交换根的两个孩子，还必须递归交换深层单侧孩子并保持完整结构"),
    236: Case("7\n1 2 3 4 5 6 7\n4 7\n", "1\n", "cross-subtree,stress", "两个目标都是深层叶子且分属根的左右子树：验证左右递归在根处各返回一个目标后根才成为 LCA"),
    238: Case("4\n0 1 0 3\n", "0 0 0 0\n", "multiple-zero,trap", "数组含两个 0：任意位置的除自身乘积都至少包含另一个 0，因此答案全 0，专门区分单零场景"),
    239: Case("5 1\n4 -1 7 7 2\n", "4 -1 7 7 2\n", "window-one,boundary", "k=1 且数组含负数/重复值：每个窗口最大值就是自身，验证过期逻辑不会多保留前一位置"),
    252: Case("3\n0 10\n2 3\n10 12\n", "0\n", "containment,trap", "一个会议完整包含另一个会议，后面还有首尾相接区间：验证任何包含重叠都应立即判冲突"),
    253: Case("3\n0 10\n1 9\n2 8\n", "3\n", "peak-overlap,stress", "三个会议在时间 2..8 同时进行：验证所需房间数取并发峰值 3，而不是只判断是否有重叠"),
    283: Case("1\n0\n", "0\n", "minimum,zero", "单元素就是 0：验证最小输入下写指针和补零阶段不会越界，也不会丢失唯一元素"),
    295: Case("8\n1 10\n2\n1 0\n2\n1 -10\n2\n1 20\n2\n", "10\n5\n0\n5\n", "state-transition,ordering", "插入值在大小两侧交替跳动并每次查询：验证两堆能反复双向重平衡，奇偶中位数依次为 10/5/0/5"),
    303: Case("4\n5 -2 7 1\n2\n0 0\n3 3\n", "5\n1\n", "endpoint-query,boundary", "同一数组分别查询最左和最右单点：验证 inclusive 端点转换 prefix[r+1]-prefix[l] 在两端都无 off-by-one"),
    304: Case("3 1\n1\n-2\n3\n2\n0 0 2 0\n1 0 2 0\n", "2\n1\n", "single-column,boundary", "3x1 单列矩阵含负数并查询整列/后缀：验证二维容斥退化到一维竖直前缀时仍正确"),
    312: Case("3\n1 5 1\n", "15\n", "interval-dp,stress", "三个气球首次出现真正的区间选择：最优收益 15 依赖正确选择最后戳破位置，验证不能用局部最大乘积贪心"),
    347: Case("7 2\n-1 -1 -1 -2 -2 3 4\n", "-2 -1\n", "negative,frequency", "最高频两个元素均为负数且频次分别 3/2：验证排名只由频次决定，负值不会被数值大小错误淘汰"),
    406: Case("4\n5 1\n4 2\n6 0\n5 0\n", "5 0\n5 1\n4 2\n6 0\n", "duplicates,ordering", "混合重复身高与不同 k：验证高者先处理、同身高按 k 升序，并按 k 位置插入得到完整有效队列"),
    417: Case("3 3\n3 3 3\n3 1 3\n3 3 3\n", "0 0\n0 1\n0 2\n1 0\n1 2\n2 0\n2 1\n2 2\n", "basin,trap", "边界等高形成可双向到海的环，但中心低洼 1 无法爬上高度 3：验证反向搜索不会错误把低盆地纳入两海交集"),
    438: Case("ab abc\n", "\n", "pattern-longer,no-solution", "pattern 比源串更长：不存在任何等长窗口，验证固定窗口初始化/收缩不会越界且直接返回空结果"),
    455: Case("4 3\n10 1 5 2\n2 10 5\n", "3\n", "ordering,greedy", "孩子需求和饼干尺寸都故意无序：排序后可满足三个孩子，验证贪心不能依赖输入原有顺序"),
    542: Case("3 3\n0 1 1\n1 1 1\n1 1 0\n", "0 1 2\n1 2 1\n2 1 0\n", "multi-source,stress", "两个 0 位于对角远端：中间区域需比较来自两个 BFS 源的首次到达层数，验证真正的最近源距离"),
    543: Case("9\n1 -1 2 3 4 5 -1 -1 6\n", "4\n", "right-subtree,trap", "全局最长路径 5-3-2-4-6 完全位于根的右子树内部：与已有左子树反例对称，验证直径不要求经过整棵树根"),
    547: Case("4\n1 1 0 0\n1 1 1 0\n0 1 1 1\n0 0 1 1\n", "1\n", "transitive,component", "四城市只通过链式间接关系连成一个省份：验证连通性具有传递性，不能只统计直接相邻或矩阵中的 1"),
    621: Case("9 2\nAAABBCDEF\n", "9\n", "total-dominates,trap", "最高频 A 构造出的冷却框架小于任务总数，其他任务足以填满所有槽位：验证答案必须取 max(total, frame)"),
    684: Case("4\n1 2\n3 4\n2 3\n1 4\n", "1 4\n", "deep-root,cycle", "两个独立集合先建立、再通过 2-3 合并，最后 1-4 才闭环：验证 union 后必须用最终根判断远距离连通"),
    695: Case("3 3\n1 0 0\n0 1 0\n0 0 1\n", "1\n", "diagonal,trap", "三个陆地仅沿对角线接触：验证岛屿面积只按四方向连通，每个分量面积都应为 1"),
    704: Case("1 2\n1\n", "-1\n", "minimum,no-solution", "单元素数组但 target 不存在：验证最小搜索区间在一次比较后正确收空并返回 -1"),
    746: Case("6\n10 1 1 10 1 1\n", "3\n", "trap,dp", "低成本台阶成簇分布在高成本之间：最优总代价 3 需要多次跨过昂贵台阶，验证不能逐级选择局部较小下一步"),
    875: Case("2 2\n1000000000 1000000000\n", "1000000000\n", "extreme,upper-bound", "两堆都取最大允许规模且 H 等于堆数：每堆必须一小时吃完，验证二分上界和大整数小时计算"),
    1011: Case("5 3\n5 1 5 1 5\n", "6\n", "grouping,trap", "交替重/轻包裹且必须保持顺序：容量 6 恰能分成 [5,1]/[5,1]/[5] 三天，容量 5 则需要五天"),
}


def expected_output(lc: int, case: Case) -> str:
    if case.out is not None:
        return case.out
    if lc == 22:
        return gen_parentheses(7)
    if lc == 46:
        return gen_permutations([1, 2, 3, 4, 5])
    if lc == 78:
        return gen_subsets([1, 2, 3, 4, 5])
    raise RuntimeError(f"lc{lc}: missing output oracle")


def discover() -> dict[int, Path]:
    result: dict[int, Path] = {}
    for meta in ROOT.glob("problems/**/cases/meta.tsv"):
        problem_dir = meta.parent.parent
        match = PROBLEM_RE.search(problem_dir.name)
        if match:
            result[int(match.group(1))] = problem_dir
    return result


def main() -> None:
    paths = discover()
    missing = sorted(set(CASES) - set(paths))
    if missing:
        raise RuntimeError(f"manifest references unknown problems: {missing}")

    added = 0
    skipped = 0
    for lc, case in sorted(CASES.items()):
        problem_dir = paths[lc]
        cases_dir = problem_dir / "cases"
        current_inputs = sorted(cases_dir.glob("*.in"), key=lambda p: int(p.stem))
        if len(current_inputs) != 6:
            skipped += 1
            continue
        new_id = 7
        if (cases_dir / f"{new_id}.in").exists() or (cases_dir / f"{new_id}.out").exists():
            raise RuntimeError(f"lc{lc}: case 7 already exists despite six-case count")
        if any(p.read_text(encoding="utf-8") == case.inp for p in current_inputs):
            raise RuntimeError(f"lc{lc}: proposed case input duplicates an existing case")

        (cases_dir / "7.in").write_text(case.inp, encoding="utf-8")
        (cases_dir / "7.out").write_text(expected_output(lc, case), encoding="utf-8")

        meta = cases_dir / "meta.tsv"
        text = meta.read_text(encoding="utf-8")
        if text and not text.endswith("\n"):
            text += "\n"
        text += f"7\t{case.labels}\t{case.purpose}\n"
        meta.write_text(text, encoding="utf-8")
        added += 1
        print(f"expanded lc{lc}: {case.labels}")

    print(f"ADDED={added} SKIPPED={skipped} MANIFEST={len(CASES)}")
    if added != 80:
        raise RuntimeError(f"expected to expand 80 six-case suites, actually expanded {added}")


if __name__ == "__main__":
    main()
