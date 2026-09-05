"""Offline learner-facing examples aligned to the LeetCode Wiki statement.

Examples are statement facts, not pedagogy.  The Chinese doocs page at
``https://leetcode.doocs.org/lc/<num>/`` is the learner-facing baseline.  Use
cached examples when they agree with that page; otherwise keep a reviewed
override.  Premium problems whose cached snapshot has no body use the examples
published by doocs rather than invented explanatory text.
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


# Premium snapshots currently contain only title/slug metadata.  These bodies
# mirror the Input/Output examples shown on their doocs Chinese pages; do not
# add an Explanation when the source page does not contain one.
EXAMPLE_OVERRIDES = {
    252: [
        """Input: intervals = [[0,30],[5,10],[15,20]]
Output: false""",
        """Input: intervals = [[7,10],[2,4]]
Output: true""",
    ],
    253: [
        """Input: intervals = [[0,30],[5,10],[15,20]]
Output: 2""",
        """Input: intervals = [[7,10],[2,4]]
Output: 1""",
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
    """Return doocs-aligned cached/reviewed example bodies for one LC number."""
    if num in CHINESE_EXAMPLES:
        return list(CHINESE_EXAMPLES[num])
    if num in EXAMPLE_OVERRIDES:
        return list(EXAMPLE_OVERRIDES[num])

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
        raise RuntimeError(f"official examples missing for problems: {missing}")
