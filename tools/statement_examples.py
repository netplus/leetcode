"""Offline learner-facing examples aligned to the LeetCode Wiki statement.

Examples are statement facts, not pedagogy.  The Chinese doocs page at
``https://leetcode.doocs.org/lc/<num>/`` is the learner-facing baseline.
Reviewed doocs overrides take precedence over the refreshable cache; otherwise
use cached examples, then the offline snapshot fallback.  Premium problems
whose cached snapshot has no body use reviewed examples from doocs.
"""

from pathlib import Path
import json
import re


HERE = Path(__file__).resolve().parent


_CHINESE_INFO = json.loads(
    (HERE / "chinese_problem_info.json").read_text(encoding="utf-8")
)
CHINESE_EXAMPLES = {
    int(num): value.get("examples", [])
    for num, value in _CHINESE_INFO.items()
    if value.get("examples")
}


# Reviewed doocs examples.  Keep Input/Output values faithful to the page; brief
# Chinese explanations may be normalized/paraphrased for learner readability.
EXAMPLE_OVERRIDES = {
    438: [
        """输入：s = "cbaebabacd", p = "abc"
输出：[0,6]
解释：从下标 0 开始的 "cba" 和从下标 6 开始的 "bac" 都与 "abc" 具有相同字符频次，因此都是异位词。""",
        """输入：s = "abab", p = "ab"
输出：[0,1,2]
解释：从下标 0、1、2 开始的长度 2 子串分别是 "ab"、"ba"、"ab"，都与 p 互为异位词。""",
    ],
    704: [
        """输入：nums = [-1,0,3,5,9,12], target = 9
输出：4
解释：9 位于 nums 的下标 4。""",
        """输入：nums = [-1,0,3,5,9,12], target = 2
输出：-1
解释：nums 中不存在 2，因此返回 -1。""",
    ],
    252: [
        """输入：intervals = [[0,30],[5,10],[15,20]]
输出：false""",
        """输入：intervals = [[7,10],[2,4]]
输出：true""",
    ],
    253: [
        """输入：intervals = [[0,30],[5,10],[15,20]]
输出：2""",
        """输入：intervals = [[7,10],[2,4]]
输出：1""",
    ],
}


EXAMPLE_RE = re.compile(r"(?m)^Example(?:\s+(\d+))?:\s*$")
CONSTRAINTS_RE = re.compile(r"(?m)^Constraints:\s*$")


def _clean_example_block(text: str) -> str:
    """Remove Markdown fences/outer blank lines while preserving multiline I/O."""
    lines = []
    for line in text.splitlines():
        stripped = line.strip()
        if stripped.startswith("```"):
            continue
        lines.append(line.rstrip())

    while lines and not lines[0].strip():
        lines.pop(0)
    while lines and not lines[-1].strip():
        lines.pop()
    return "\n".join(lines)


def get_examples(num: int) -> list[str]:
    """Return doocs-aligned reviewed/cached example bodies for one LC number."""
    if num in EXAMPLE_OVERRIDES:
        return list(EXAMPLE_OVERRIDES[num])
    if num in CHINESE_EXAMPLES:
        return list(CHINESE_EXAMPLES[num])

    path = HERE / "official" / f"lc{num}.txt"
    if not path.exists():
        return []

    text = path.read_text(encoding="utf-8")
    matches = list(EXAMPLE_RE.finditer(text))
    if not matches:
        return []

    constraints = CONSTRAINTS_RE.search(text)
    examples = []
    for index, match in enumerate(matches):
        start = match.end()
        if index + 1 < len(matches):
            end = matches[index + 1].start()
        elif constraints:
            end = constraints.start()
        else:
            end = len(text)
        block = _clean_example_block(text[start:end])
        if block:
            examples.append(block)
    return examples


def render_examples(num: int) -> str:
    """Render examples as learner-facing C++ header comments."""
    examples = get_examples(num)
    if not examples:
        return ""

    lines = ["// 题目示例："]
    for index, example in enumerate(examples, 1):
        if index > 1:
            lines.append("//")
        lines.append(f"// 示例 {index}：")
        for line in example.splitlines():
            lines.append(f"//   {line}" if line else "//")
    return "\n".join(lines)


def validate_example_coverage(problems: list[dict]) -> None:
    """Require at least one statement-level example for every generated problem."""
    missing = [problem["num"] for problem in problems if not get_examples(problem["num"])]
    if missing:
        raise RuntimeError(f"statement examples missing for problems: {missing}")
