#!/usr/bin/env python3
"""Check Chinese titles and core-logic sections in generated learning files."""

from pathlib import Path
import sys

HERE = Path(__file__).resolve().parent
ROOT = HERE.parent
sys.path.insert(0, str(HERE))

import gen_data  # noqa: E402
from chinese_metadata import get_chinese_metadata  # noqa: E402
from chinese_titles import get_chinese_title  # noqa: E402
from gen_all import WEEKS  # noqa: E402
from refined_data import validate_coverage  # noqa: E402


CORE_MARKERS = (
    "// - 核心要点：",
    "//   1. 思路起点：",
    "//   2. 执行逻辑：",
    "//   3. 为什么这样做：",
)


def main() -> None:
    problems = []
    gen_data.build(lambda **item: problems.append(item))
    validate_coverage(problems)
    errors = []

    for problem in problems:
        num = problem["num"]
        directory = (
            ROOT / "problems" / WEEKS[problem["week"]]
            / f"day{problem['day']}-lc{num}-{problem['name']}"
        )
        solution = directory / "solution.cpp"
        makefile = directory / "Makefile"
        if not solution.exists():
            errors.append(f"LC-{num}: missing solution.cpp")
            continue

        source = solution.read_text(encoding="utf-8")
        metadata = get_chinese_metadata(problem)
        expected_title = f"// LC-{num}：{get_chinese_title(num)}"
        if expected_title not in source:
            errors.append(f"LC-{num}: Chinese solution title is missing or stale")
        expected_fields = (
            f"// 难度：{metadata['difficulty']}",
            f"// 优先级：{metadata['priority']}",
            "// 题目描述：",
            "// 约束与要求：",
            f"// 复杂度目标：{metadata['goal']}",
            "// 本地输入输出格式（用于 test.in）：",
            f"// test.in 的预期输出：{problem['expected']}",
        )
        for field in expected_fields:
            if field not in source:
                errors.append(f"LC-{num}: missing or stale Chinese field {field}")
        for field_name in ("description", "constraints", "iofmt"):
            if metadata[field_name] not in source:
                errors.append(f"LC-{num}: stale Chinese {field_name}")
        forbidden = (
            "// English Title:", "// Difficulty:", "// Priority:",
            "// Week ", "// Constraints:", "// Goal:",
            "// Local I/O format", "// Expected output",
        )
        if any(marker in source for marker in forbidden):
            errors.append(f"LC-{num}: obsolete English metadata remains")
        for marker in CORE_MARKERS:
            if marker not in source:
                errors.append(f"LC-{num}: missing core-logic marker {marker}")

        expected_make_title = f"# Problem: LC-{num} {get_chinese_title(num)}"
        if not makefile.exists() or expected_make_title not in makefile.read_text(
            encoding="utf-8"
        ):
            errors.append(f"LC-{num}: Chinese Makefile title is missing or stale")

    if errors:
        raise SystemExit("\n".join(errors))
    print("learning metadata is current (106 fully Chinese headers and core-logic sections)")


if __name__ == "__main__":
    main()
