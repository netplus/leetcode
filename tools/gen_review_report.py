#!/usr/bin/env python3
"""Generate the auditable 106-problem statement-review ledger."""

from pathlib import Path
import sys

HERE = Path(__file__).resolve().parent
ROOT = HERE.parent
sys.path.insert(0, str(HERE))

import gen_data  # noqa: E402
from refined_data import REFINEMENTS, validate_coverage  # noqa: E402
from chinese_titles import get_chinese_title  # noqa: E402
from chinese_metadata import validate_chinese_metadata  # noqa: E402


# Only meaning-bearing statement/constraint changes are listed here. Purely
# stylistic rewrites are intentionally not labelled as corrections.
CORRECTIONS = {
    1: "补充 target 取值范围",
    3: "将字符串长度上限校正为 1e5；本地输入改为整行读取",
    11: "补齐几何定义与容器不可倾斜约束",
    14: "本地输入改为逐行读取，从而覆盖空字符串约束",
    21: "明确复用并拼接原链表节点",
    23: "补充全部链表节点总数上限",
    57: "明确闭区间端点相接也算重叠",
    79: "修正字符集为大小写英文字母",
    98: "补齐 BST 严格左右子树定义",
    124: "明确路径非空、可从任意节点起止且不必经过根",
    127: "明确 endWord 必须在字典中、beginWord 不必在",
    139: "明确字典单词可以重复使用",
    142: "补充不得修改链表",
    151: "校正字符集并补充至少含一个单词",
    152: "明确任意子数组乘积均在 32 位范围内",
    160: "校正链表长度下界并补充无环/保持结构契约",
    167: "补齐数值范围、不可复用元素与常量空间要求",
    210: "改为自包含题意并补齐先修边约束",
    236: "明确节点可视为自身的后代",
    304: "校正矩阵值域并补充调用次数/O(1) 查询要求",
    347: "补充 Top-K 结果集合唯一保证",
    455: "明确孩子与饼干均只能匹配一次",
    542: "补充矩阵中至少存在一个 0",
    560: "明确子数组连续且非空",
    685: "修正为可能同时存在二父节点和有向环，并补齐边约束",
    704: "用官方严格不等式表达整数值域",
    1011: "明确每日容量限制、不可重排与 days 约束；修复默认样例",
}

PREMIUM_CANONICAL = {252, 253}

CASE_CORRECTIONS = [
    "LC-11：`[1,2,4,3,5]` 的最大面积由 9 修正为 8",
    "LC-15：补入合法且被漏写的三元组 `[-4,2,2]`",
    "LC-167：移除违反“唯一解”保证的双解输入",
    "LC-209：总和小于 target 的用例由长度 4 修正为无解 0",
    "LC-1109、304、57：修正手算错误的区间/矩形期望值",
    "LC-155、160、295、42：修正操作数、链表长度或数组长度与实际载荷不一致",
    "LC-124：避开本地 `-1=null` 序列化约定与合法节点值 -1 的歧义，保留全负树覆盖",
    "LC-127：单字符 `a -> c` 的最短序列长度由 3 修正为 2",
    "LC-417：将官方示例坐标集合修正为真实可达交集",
]


def build_report() -> str:
    problems = []
    gen_data.build(lambda **item: problems.append(item))
    validate_coverage(problems)
    validate_chinese_metadata(problems)
    if len(problems) != 106 or len(REFINEMENTS) != 106:
        raise RuntimeError("expected exactly 106 metadata and refinement entries")

    missing_cache = [
        p["num"] for p in problems
        if not (HERE / "official" / f"lc{p['num']}.txt").exists()
    ]
    if missing_cache:
        raise RuntimeError(f"official statement cache missing: {missing_cache}")

    lines = [
        "# 题面复核台账（106 题）",
        "",
        "本台账逐题核对 `tools/gen_data.py` 中的核心题意、关键保证、约束、复杂度目标与本地 I/O 说明。",
        "复核基线为仓库内 `tools/official/lc*.txt` 的 LeetCode 题面快照（文件时间 2026-08-05）；",
        "题解文件中的中文题面另离线保存于 `tools/chinese_problem_info.json`，日常生成不依赖网络。",
        "LC-252/253 的缓存不含正文（Premium），按经典题意人工核对，并在本地明确采用 `[start,end)` 会议区间语义。",
        "",
        "状态说明：`通过` 表示本地摘要在不复制整篇原题的前提下保留了所有影响解法的语义；",
        "`修订后通过` 表示本轮发现并修正了实质遗漏、错误约束或本地适配问题。每题的完整分析和实现位于对应 `solution.cpp`。",
        "",
        "## 复核结果",
        "",
        "| Week/Day | LC | 中文标题 | 结论 | 本轮关键修订 |",
        "|---|---:|---|---|---|",
    ]
    for p in problems:
        num = p["num"]
        if num in PREMIUM_CANONICAL:
            result = "通过（经典题意）"
            note = "Premium 正文缓存为空；人工核对半开会议区间语义"
        elif num in CORRECTIONS:
            result = "修订后通过"
            note = CORRECTIONS[num]
        else:
            result = "通过"
            note = "—"
        lines.append(
            f"| W{p['week']}/D{p['day']} | {num} | {get_chinese_title(num)} | {result} | {note} |"
        )

    lines += [
        "",
        "## 测试夹具复核",
        "",
    ]
    lines.extend(f"- {item}" for item in CASE_CORRECTIONS)
    lines += [
        "",
        "## 验收口径",
        "",
        "- 元数据与精修实现均必须恰好覆盖 106 个题号，生成器会在缺项或多项时失败。",
        "- 每道题的核心要点必须讲清思路起点、执行逻辑和为什么这样做，并补充边界/易错点、举一反三、复杂度目标及带注释实现。",
        "- `make all` 检查 106 个本地程序的 C++17 编译；`make judge-all` 运行每题 `cases/` 下的全部用例。",
        "- 非唯一答案使用 `.judge` 声明的集合归一化规则，避免把合法的不同输出顺序误判为失败。",
        "- 题面快照不是永久不变的在线真值；若后续刷新 `tools/official/`，应重新运行比较工具并更新本台账。",
        "",
    ]
    return "\n".join(lines)


def main() -> None:
    report = build_report()
    output = ROOT / "docs" / "problem-review.md"
    if "--check" in sys.argv:
        if not output.exists() or output.read_text(encoding="utf-8") != report:
            raise SystemExit("docs/problem-review.md is stale; regenerate it")
        print("problem review report is current (106/106)")
        return
    output.write_text(report, encoding="utf-8")
    print(f"wrote {output.relative_to(ROOT)} (106/106)")


if __name__ == "__main__":
    main()
