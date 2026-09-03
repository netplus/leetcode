"""Reviewed analysis and implementation registry for all practice problems.

The week modules keep each problem's baseline reviewed explanation next to its
code.  High-touch pedagogy modules add visual learning rewrites one problem at a
time.  `gen_all.py` consumes the merged registry, so regenerating the workspace
preserves both reviewed implementations and the improved pedagogy.
"""

from refined_week1 import REFINEMENTS as WEEK1
from refined_week2 import REFINEMENTS as WEEK2
from refined_week3 import REFINEMENTS as WEEK3
from refined_week4 import REFINEMENTS as WEEK4
from pedagogy_overrides import PEDAGOGY_OVERRIDES as INITIAL_PEDAGOGY_OVERRIDES
from pedagogy_week1 import PEDAGOGY_WEEK1
from pedagogy_week1_day4 import PEDAGOGY_WEEK1_DAY4
from pedagogy_week1_day5 import PEDAGOGY_WEEK1_DAY5
from chinese_titles import validate_title_coverage


REFINEMENTS = {**WEEK1, **WEEK2, **WEEK3, **WEEK4}

_PEDAGOGY_MODULES = (
    INITIAL_PEDAGOGY_OVERRIDES,
    PEDAGOGY_WEEK1,
    PEDAGOGY_WEEK1_DAY4,
    PEDAGOGY_WEEK1_DAY5,
)

PEDAGOGY_OVERRIDES = {}
for module in _PEDAGOGY_MODULES:
    overlap = set(PEDAGOGY_OVERRIDES) & set(module)
    if overlap:
        raise RuntimeError(f"duplicate pedagogy overrides: {sorted(overlap)}")
    PEDAGOGY_OVERRIDES.update(module)

# Apply only explicitly reviewed learning rewrites. Recompute key_points so
# legacy consumers remain consistent when an override changes model/steps/proof.
for num, patch in PEDAGOGY_OVERRIDES.items():
    if num not in REFINEMENTS:
        raise RuntimeError(f"pedagogy override references unknown lc{num}")
    merged = {**REFINEMENTS[num], **patch}
    merged["key_points"] = [
        merged["model"],
        "；".join(f"{i}. {step}" for i, step in enumerate(merged["steps"], 1)) + "。",
        merged["proof"],
    ]
    REFINEMENTS[num] = merged


def get_refinement(num: int) -> dict:
    """Return the reviewed material for one LeetCode number."""
    return REFINEMENTS[num]


def _append_text_block(lines: list[str], title: str, text: str) -> None:
    """Append a learner-facing multiline block as C++ comments."""
    lines.append(f"// {title}")
    for line in text.strip("\n").splitlines():
        lines.append(f"//   {line}" if line else "//")


def _render_visual_analysis(item: dict) -> str:
    """Render the AGENTS.md visual -> formula -> steps learning structure."""
    lines = [
        "// ----------------------------------------------------------------------------",
        f"// 解法精讲｜{item['pattern']}",
        "//",
    ]

    _append_text_block(lines, "1. 图像直觉", item["visual"])
    lines.append("//")
    _append_text_block(lines, "2. 一句话核心", item["core"])
    lines.append("//")
    _append_text_block(lines, "3. 公式 / 不变量", item["formula"])

    lines.extend(["//", "// 4. 执行步骤"])
    for i, step in enumerate(item["steps"], 1):
        lines.append(f"//   {i}. {step}")
    if item.get("memory"):
        lines.append(f"//   记忆：{item['memory']}")

    lines.append("//")
    _append_text_block(lines, "5. 为什么不会漏 / 不会重", item["proof"])
    lines.append("//")
    _append_text_block(lines, "6. 边界与易错点", item["pitfalls"])
    lines.append("//")
    _append_text_block(lines, "7. 举一反三", item["transfer"])

    if item.get("advanced"):
        lines.append("//")
        _append_text_block(lines, "进阶优化", item["advanced"])

    lines.append("// ----------------------------------------------------------------------------")
    return "\n".join(lines)


def render_analysis(item: dict) -> str:
    """Render one explanation, preserving legacy format until individually reviewed."""
    if all(item.get(field) for field in ("visual", "core", "formula")):
        return _render_visual_analysis(item)

    # Untouched problems keep their existing reviewed material. They migrate
    # to the richer structure only after an individual pedagogy pass.
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
