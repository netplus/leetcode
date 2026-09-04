#!/usr/bin/env python3
"""
Deterministic generator for all 4 weeks of reviewed LeetCode solutions.

Each problem is a dict in gen_data.build(P), while the reviewed algorithm notes
and implementations live in refined_data.py / refined_week*.py. The generator
writes:
  problems/<weekdir>/dayN-lcNUM-short-name/solution.cpp
  problems/<weekdir>/dayN-lcNUM-short-name/test.in         (Example 1 input)
  problems/<weekdir>/dayN-lcNUM-short-name/Makefile        (local commands)

solution.cpp header: LC#, Chinese title, difficulty, priority, week/day,
Chinese description, official examples, constraints, complexity goal, local I/O
format, and expected output.
main() is a local adapter rather than part of the LeetCode submission. The
generated Solution class is complete and can be judged immediately.

Run:  python3 tools/gen_all.py
"""
from __future__ import annotations
import os

from statement_metadata import get_statement_metadata

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

WEEKS = {
    1: "week1-arrays-strings",
    2: "week2-list-tree-bsp",
    3: "week3-graph-dp",
    4: "week4-binary-search-greedy-mock",
}

LISTNODE = """struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

"""

TREENODE = """struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

"""

TEMPLATE = """// ============================================================================
// LC-__NUM__：__TITLE__
// 难度：__DIFF__
// 优先级：__PRIO__
// 学习进度：第 __WEEK__ 周 / 第 __DAY__ 天
// ----------------------------------------------------------------------------
// 题目描述：
__DESC__
//
__EXAMPLES__
//
// 约束与要求：
__CONSTRAINTS__
//
// 复杂度目标：__GOAL__
//
__ANALYSIS__
//
// 本地输入输出格式（用于 test.in）：
__IOFMT__
// test.in 的预期输出：__EXPECTED__
// ============================================================================
#include <bits/stdc++.h>
using namespace std;

__PRESTRUCT__
__SOLUTION__

// ---------- 本地测试适配器 ----------
__MAIN__
"""


def render(p: dict) -> str:
    # Keep problem metadata and solution pedagogy separate: gen_data.py remains
    # a compact statement/harness catalogue, while refined_data.py owns the
    # reviewed explanation and complete implementation.
    try:
        from refined_data import get_refinement, render_analysis
        refinement = get_refinement(p["num"])
    except (ImportError, KeyError):
        refinement = None

    if refinement:
        analysis = render_analysis(refinement)
        solution = refinement["code"].strip().replace(
            "// ---------- Solution ----------", "// ---------- 题解实现 ----------"
        )
    else:
        # Development fallback for a newly added problem that has not yet been
        # reviewed. The coverage assertion in __main__ prevents release builds
        # from silently shipping such a stub.
        analysis = "// 解法精讲：尚待补充。"
        solution = f"""// ---------- Solution (implement this) ----------
class Solution {{
public:
    // TODO: implement
    {p['sig']} {{
        return {p['retdefault']};
    }}
{p.get('extra', '')}
}};"""

    metadata = get_statement_metadata(p)
    s = TEMPLATE
    replacements = {
        "__NUM__": str(p["num"]),
        "__TITLE__": metadata["title"],
        "__DIFF__": metadata["difficulty"],
        "__PRIO__": metadata["priority"],
        "__WEEK__": str(p["week"]),
        "__DAY__": str(p["day"]),
        "__DESC__": metadata["description"],
        "__EXAMPLES__": metadata["examples"],
        "__CONSTRAINTS__": metadata["constraints"],
        "__GOAL__": metadata["goal"],
        "__ANALYSIS__": analysis,
        "__IOFMT__": metadata["iofmt"],
        "__EXPECTED__": p["expected"],
        "__PRESTRUCT__": p.get("prestruct", ""),
        "__SOLUTION__": solution,
        "__MAIN__": p["main"],
    }
    for k, v in replacements.items():
        s = s.replace(k, v)
    return s


def write_problem(p: dict) -> None:
    weekdir = WEEKS[p["week"]]
    name = p["name"]
    d = os.path.join(ROOT, "problems", weekdir, f"day{p['day']}-lc{p['num']}-{name}")
    os.makedirs(d, exist_ok=True)
    with open(os.path.join(d, "solution.cpp"), "w") as f:
        f.write(render(p))
    with open(os.path.join(d, "test.in"), "w") as f:
        f.write(p["testin"])


if __name__ == "__main__":
    import gen_data
    from statement_metadata import validate_statement_metadata
    from refined_data import validate_coverage
    PROBLEMS = []

    def P(**kw):
        PROBLEMS.append(kw)

    gen_data.build(P)
    validate_coverage(PROBLEMS)
    validate_statement_metadata(PROBLEMS)
    for p in PROBLEMS:
        write_problem(p)
    print(f"Generated {len(PROBLEMS)} problems.")

    from sync_plan_titles import main as sync_plan_titles
    sync_plan_titles()

    from gen_makefiles import main as write_makefiles
    write_makefiles()

    # Mock exam directories (Day 27 & 28 of Week 4) — MOCK.md only, no solution.cpp.
    from gen_mocks import write_mocks
    write_mocks(ROOT)
    print("Wrote mock exam packages (Day 27, Day 28).")
