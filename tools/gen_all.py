#!/usr/bin/env python3
"""
Deterministic generator for all 4 weeks of LeetCode practice stubs.

Each problem is a dict in gen_data.build(P). The generator writes:
  problems/<weekdir>/dayN-lcNUM-short-name/solution.cpp   (method body EMPTY)
  problems/<weekdir>/dayN-lcNUM-short-name/test.in         (Example 1 input)

solution.cpp header: LC#, title, difficulty, priority, week/day, full English
description, real constraints, local I/O format, expected output.
main() is COMPLETE: once the user implements the Solution method, `make lc<N>`
reproduces the expected output.

Run:  python3 tools/gen_all.py
"""
from __future__ import annotations
import os

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
// LC-__NUM__: __TITLE__
// Difficulty: __DIFF__
// Priority: __PRIO__
// Week __WEEK__ / Day __DAY__
// ----------------------------------------------------------------------------
__DESC__
//
// Constraints:
__CONSTRAINTS__
//
// Goal: __GOAL__
//
// Local I/O format (for test.in):
__IOFMT__
// Expected output for test.in: __EXPECTED__
// ============================================================================
#include <bits/stdc++.h>
using namespace std;

__PRESTRUCT__
// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    __SIG__ {
        // Your implementation here.
        return __RETDEFAULT__;
    }
__EXTRA__
};

// ---------- Local test harness ----------
__MAIN__
"""


def render(p: dict) -> str:
    s = TEMPLATE
    replacements = {
        "__NUM__": str(p["num"]),
        "__TITLE__": p["title"],
        "__DIFF__": p["diff"],
        "__PRIO__": p["prio"],
        "__WEEK__": str(p["week"]),
        "__DAY__": str(p["day"]),
        "__DESC__": p["desc"].strip(),
        "__CONSTRAINTS__": p["constraints"].strip(),
        "__GOAL__": p.get("goal", "see patterns.md"),
        "__IOFMT__": p["iofmt"].strip(),
        "__EXPECTED__": p["expected"],
        "__PRESTRUCT__": p.get("prestruct", ""),
        "__SIG__": p["sig"],
        "__RETDEFAULT__": p["retdefault"],
        "__EXTRA__": p.get("extra", ""),
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
    PROBLEMS = []

    def P(**kw):
        PROBLEMS.append(kw)

    gen_data.build(P)
    for p in PROBLEMS:
        write_problem(p)
    print(f"Generated {len(PROBLEMS)} problems.")

    # Mock exam directories (Day 27 & 28 of Week 4) — MOCK.md only, no solution.cpp.
    from gen_mocks import write_mocks
    write_mocks(ROOT)
    print("Wrote mock exam packages (Day 27, Day 28).")
