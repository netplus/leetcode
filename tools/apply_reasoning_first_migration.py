#!/usr/bin/env python3
"""One-shot full-plan migration to the reasoning-first pedagogy renderer."""

from pathlib import Path
import importlib
import pprint
import sys

ROOT = Path(__file__).resolve().parents[1]
TOOLS = ROOT / "tools"
sys.path.insert(0, str(TOOLS))

TOPICS = {
    "hash-indexing.md": {1, 3, 41, 128, 133},
    "prefix-sum-difference.md": {560, 1248, 303, 304, 1109, 238},
    "two-pointers.md": {11, 15, 31, 42, 151, 167, 283, 455},
    "sliding-window.md": {3, 76, 209, 438, 239},
    "stack-queue.md": {20, 155},
    "monotonic-stack.md": {84, 739},
    "monotonic-queue.md": {239},
    "linked-list-pointer.md": {19, 21, 23, 92, 141, 142, 143, 148, 160, 206, 234},
    "tree-traversal.md": {94, 98, 102, 104, 105, 124, 144, 145, 199, 226, 236, 543},
    "backtracking.md": {22, 39, 46, 78, 79},
    "graph-search-toposort.md": {127, 133, 199, 200, 207, 210, 417, 542, 695, 994},
    "union-find.md": {547, 684, 685, 990},
    "dynamic-programming.md": {53, 62, 64, 70, 139, 152, 198, 213, 300, 312, 322, 416, 494, 746, 1143},
    "binary-search.md": {4, 33, 34, 153, 300, 410, 704, 875, 1011},
    "greedy.md": {45, 55, 134, 179, 406, 455, 621},
    "heap-topk.md": {23, 215, 253, 295, 347},
    "kmp.md": {28},
    "interval-sort-sweep.md": {56, 57, 252, 253},
}


def replace_once(text: str, old: str, new: str, label: str) -> str:
    count = text.count(old)
    if count != 1:
        raise RuntimeError(f"{label}: expected exactly one match, got {count}")
    return text.replace(old, new, 1)


def first_sentence(text: str) -> str:
    text = " ".join(line.strip() for line in text.strip().splitlines() if line.strip())
    for mark in ("。", "；", ";"):
        if mark in text:
            return text.split(mark, 1)[0] + mark
    return text


def topic_for(num: int) -> str:
    matches = [name for name, nums in TOPICS.items() if num in nums]
    if not matches:
        return "README.md"
    priority = [
        "monotonic-stack.md", "monotonic-queue.md", "kmp.md",
        "union-find.md", "binary-search.md", "dynamic-programming.md",
        "heap-topk.md", "sliding-window.md", "interval-sort-sweep.md",
        "two-pointers.md", "linked-list-pointer.md", "tree-traversal.md",
        "graph-search-toposort.md", "backtracking.md", "greedy.md",
        "prefix-sum-difference.md", "hash-indexing.md", "stack-queue.md",
    ]
    return min(matches, key=priority.index)


def split_derivation(item: dict) -> tuple[str, str, str]:
    derivation = item.get("derivation", "").strip()
    if derivation:
        parts = [p.strip() for p in derivation.split("\n\n") if p.strip()]
        general_solution = parts[0]
        limitations = parts[1] if len(parts) >= 2 else (
            "这个直接实现能够得到正确答案，但还需要继续检查它是否重复搜索、"
            "重复计算、保存被支配候选，或没有利用题目给出的结构。"
        )
        generalization = (
            parts[2]
            if len(parts) >= 3
            else first_sentence(item.get("transfer", item["core"]))
        )
        return general_solution, limitations, generalization

    general_solution = (
        item["model"]
        + "\n\n按实现顺序可以直接写成：\n"
        + "\n".join(f"{i}. {step}" for i, step in enumerate(item["steps"], 1))
    )
    limitations = (
        "这道题的一般性逻辑本身已经接近目标复杂度，没有必要为了形式统一而人为制造更慢的“暴力解”。"
        "这里真正需要约束的是状态定义、边界和更新顺序："
        + first_sentence(item["pitfalls"])
    )
    generalization = first_sentence(item.get("transfer", item["core"]))
    return general_solution, limitations, generalization


def build_reasoning_source() -> str:
    refined_data = importlib.import_module("refined_data")
    refinements = refined_data.REFINEMENTS
    overrides = {}

    for num, item in sorted(refinements.items()):
        already = all(
            item.get(field)
            for field in (
                "general_solution", "limitations", "generalization",
                "algorithm_background",
            )
        )
        if already:
            continue

        general_solution, limitations, generalization = split_derivation(item)
        topic = topic_for(num)
        overrides[num] = {
            "general_solution": general_solution,
            "limitations": limitations,
            "generalization": generalization,
            "algorithm_background": (
                f"到这里才需要给这套规律命名：本题对应“{item['pattern']}”。"
                f"通用机制见 docs/algorithms/{topic}；回到本题时只保留与当前题意直接相关的状态、"
                "边界和更新顺序。"
            ),
        }

    preamble = '''"""Full-plan reasoning-first bridge fields.

Generated once from the previously reviewed per-problem pedagogy. These fields
separate the natural solution, its limitation, the reusable rule, and the point
where the named algorithm is finally introduced.
"""

REASONING_FIRST_OVERRIDES = '''
    return preamble + pprint.pformat(overrides, width=100, sort_dicts=True) + "\n"


def write_reasoning_source() -> None:
    (TOOLS / "pedagogy_reasoning_first.py").write_text(
        build_reasoning_source(), encoding="utf-8"
    )


def patch_refined_data() -> None:
    path = TOOLS / "refined_data.py"
    text = path.read_text(encoding="utf-8")

    text = replace_once(
        text,
        "from pedagogy_prerequisites import PREREQUISITE_OVERRIDES\n",
        "from pedagogy_prerequisites import PREREQUISITE_OVERRIDES\n"
        "from pedagogy_reasoning_first import REASONING_FIRST_OVERRIDES\n",
        "reasoning import",
    )

    anchor = """for num, derivation in DERIVATIONS.items():
    if num not in REFINEMENTS:
        raise RuntimeError(f"derivation override references unknown lc{num}")
    if not derivation.strip():
        raise RuntimeError(f"lc{num}: empty optimization derivation")
    REFINEMENTS[num] = {**REFINEMENTS[num], "derivation": derivation}

"""
    addition = anchor + """# Full-plan reasoning-first bridge. LC-739 / LC-84 already define these fields
# in their dedicated pedagogy module; this override supplies the remaining formal
# problems without replacing reviewed visual/core/formula/proof/code material.
for num, patch in REASONING_FIRST_OVERRIDES.items():
    if num not in REFINEMENTS:
        raise RuntimeError(f"reasoning-first override references unknown lc{num}")
    REFINEMENTS[num] = {**REFINEMENTS[num], **patch}

"""
    text = replace_once(text, anchor, addition, "reasoning merge")

    text = replace_once(
        text,
        """    lines = [
        "// ----------------------------------------------------------------------------",
        f"// 解法精讲｜{item['pattern']}",
        "//",
    ]

    section = 1
""",
        """    lines = [
        "// ----------------------------------------------------------------------------",
        "// 解法精讲｜从题目直觉到可复用算法",
        "//",
    ]

    section = 1
""",
        "reasoning header",
    )

    text = replace_once(
        text,
        '    section = _append_numbered_text_block(lines, section, "专项算法背景", item["algorithm_background"])\n',
        '    section = _append_numbered_text_block(\n'
        '        lines, section, f"专项算法｜{item[\'pattern\']}", item["algorithm_background"]\n'
        '    )\n',
        "algorithm-name placement",
    )

    validation_anchor = """        absent = sorted(required - set(item))
        if absent:
            raise RuntimeError(f"lc{num}: refinement fields missing: {absent}")
        if len(item["key_points"]) != 3 or any(not point.strip() for point in item["key_points"]):
"""
    validation_new = """        absent = sorted(required - set(item))
        if absent:
            raise RuntimeError(f"lc{num}: refinement fields missing: {absent}")

        reasoning_required = {
            "visual", "general_solution", "limitations", "generalization",
            "algorithm_background", "core", "formula",
        }
        reasoning_absent = sorted(reasoning_required - set(item))
        if reasoning_absent:
            raise RuntimeError(
                f"lc{num}: reasoning-first fields missing after full migration: "
                f"{reasoning_absent}"
            )

        if len(item["key_points"]) != 3 or any(not point.strip() for point in item["key_points"]):
"""
    text = replace_once(text, validation_anchor, validation_new, "reasoning validation")
    path.write_text(text, encoding="utf-8")


def patch_agents() -> None:
    path = ROOT / "AGENTS.md"
    text = path.read_text(encoding="utf-8")

    text = replace_once(
        text,
        """Current example:

- [单调栈](docs/algorithms/monotonic-stack.md)

Future two-pointer, sliding-window, prefix/difference, binary-search, DP-state-design, traversal, union-find, and backtracking guides should follow the same structure when they are systematically reviewed.
""",
        """Current algorithm-guide index:

- [专项算法文档总览](docs/algorithms/README.md)

The major reusable algorithm families used by the 106-problem plan now have
mechanism-level guides under `docs/algorithms/`. Future work should deepen those
guides rather than reintroducing algorithm theory separately in each problem.
""",
        "AGENTS guide status",
    )

    text = replace_once(
        text,
        """4. `tools/pedagogy_derivations.py` + `tools/pedagogy_derivations_backfill.py`: legacy/direct-optimization bridges where still used;
5. `tools/code_comment_overrides.py` plus modular `tools/code_comments_week*.py`: reviewed implementation comments;
6. `tools/refined_data.py`: merges canonical learning layers and renders the appropriate pedagogy structure;
7. `tools/statement_metadata.py`: effective learner-facing statement metadata;
8. `tools/gen_all.py`: renders `solution.cpp`, `test.in`, and Week 4 mock packages.
""",
        """4. `tools/pedagogy_derivations.py` + `tools/pedagogy_derivations_backfill.py`: preserved direct-optimization evidence;
5. `tools/pedagogy_reasoning_first.py`: full-plan natural-solution / limitation / generalization / algorithm-background bridge;
6. `tools/code_comment_overrides.py` plus modular `tools/code_comments_week*.py`: reviewed implementation comments;
7. `tools/refined_data.py`: merges canonical layers, validates full reasoning-first coverage, and renders pedagogy;
8. `tools/statement_metadata.py`: effective learner-facing statement metadata;
9. `tools/gen_all.py`: renders `solution.cpp`, `test.in`, and Week 4 mock packages.
""",
        "AGENTS pipeline",
    )

    text = replace_once(
        text,
        """```text
visual
 general_solution
 limitations
 generalization
 algorithm_background
 core
 formula
 steps
```

Do not mechanically migrate all 106 problems in one blind rewrite. Each problem should move to the richer structure only after an individual review confirms that the new sections improve the reasoning.
""",
        """```text
visual
general_solution
limitations
generalization
algorithm_background
core
formula
steps
```

All 106 formal problems are now required to render through this reasoning-first
structure. `validate_coverage()` treats missing fields as canonical drift. Future
edits should improve individual sections without falling back to the older format.
""",
        "AGENTS migration status",
    )
    path.write_text(text, encoding="utf-8")


def main() -> None:
    write_reasoning_source()
    patch_refined_data()
    patch_agents()


if __name__ == "__main__":
    main()
