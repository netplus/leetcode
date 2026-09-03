"""Reviewed analysis and implementation registry for all practice problems.

The week modules deliberately keep each problem's explanation next to its code.
`gen_all.py` consumes this registry, so regenerating the workspace cannot turn
completed solutions back into empty stubs.
"""

from refined_week1 import REFINEMENTS as WEEK1
from refined_week2 import REFINEMENTS as WEEK2
from refined_week3 import REFINEMENTS as WEEK3
from refined_week4 import REFINEMENTS as WEEK4
from chinese_titles import validate_title_coverage


REFINEMENTS = {**WEEK1, **WEEK2, **WEEK3, **WEEK4}


def get_refinement(num: int) -> dict:
    """Return the reviewed material for one LeetCode number."""
    return REFINEMENTS[num]


def render_analysis(item: dict) -> str:
    """Render a uniform Chinese explanation block into a C++ header comment."""
    labels = ("思路起点", "执行逻辑", "为什么这样做")
    key_points = [
        f"//   {i}. {label}：{point}"
        for i, (label, point) in enumerate(zip(labels, item["key_points"]), 1)
    ]
    lines = [
        "// ----------------------------------------------------------------------------",
        f"// 解法精讲｜{item['pattern']}",
        "// - 核心要点：",
        *key_points,
        f"// - 边界与易错点：{item['pitfalls']}",
        f"// - 举一反三：{item['transfer']}",
        "// ----------------------------------------------------------------------------",
    ]
    return "\n".join(lines)


def validate_coverage(problems: list[dict]) -> None:
    """Fail fast if metadata and reviewed implementations drift apart."""
    validate_title_coverage(problems)
    expected = {p["num"] for p in problems}
    actual = set(REFINEMENTS)
    missing = sorted(expected - actual)
    extra = sorted(actual - expected)
    if missing or extra:
        raise RuntimeError(
            f"refinement coverage mismatch: missing={missing}, extra={extra}"
        )
    for num, item in REFINEMENTS.items():
        required = {
            "pattern", "key_points", "model", "steps", "proof", "pitfalls",
            "transfer", "code",
        }
        absent = sorted(required - set(item))
        if absent:
            raise RuntimeError(f"lc{num}: refinement fields missing: {absent}")
        if len(item["key_points"]) != 3 or any(not point.strip() for point in item["key_points"]):
            raise RuntimeError(f"lc{num}: expected exactly three non-empty key points")
        if "class Solution" not in item["code"]:
            raise RuntimeError(f"lc{num}: code does not define class Solution")
