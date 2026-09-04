#!/usr/bin/env python3
"""Check generated learner-facing statements and canonical pedagogy blocks."""

from pathlib import Path
import sys

HERE = Path(__file__).resolve().parent
ROOT = HERE.parent
sys.path.insert(0, str(HERE))

import gen_data  # noqa: E402
from chinese_titles import get_chinese_title  # noqa: E402
from gen_all import WEEKS  # noqa: E402
from refined_data import get_refinement, render_analysis, validate_coverage  # noqa: E402
from statement_metadata import get_statement_metadata, validate_statement_metadata  # noqa: E402


def main() -> None:
    problems = []
    gen_data.build(lambda **item: problems.append(item))
    validate_coverage(problems)
    validate_statement_metadata(problems)
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
        metadata = get_statement_metadata(problem)
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
                errors.append(f"LC-{num}: stale learner-facing {field_name}")

        forbidden = (
            "// English Title:", "// Difficulty:", "// Priority:",
            "// Week ", "// Constraints:", "// Goal:",
            "// Local I/O format", "// Expected output",
        )
        if any(marker in source for marker in forbidden):
            errors.append(f"LC-{num}: obsolete English metadata remains")

        expected_analysis = render_analysis(get_refinement(num))
        if expected_analysis not in source:
            errors.append(
                f"LC-{num}: generated pedagogy differs from canonical refinement"
            )

        expected_make_title = f"# Problem: LC-{num} {get_chinese_title(num)}"
        if not makefile.exists() or expected_make_title not in makefile.read_text(
            encoding="utf-8"
        ):
            errors.append(f"LC-{num}: Chinese Makefile title is missing or stale")

    if errors:
        raise SystemExit("\n".join(errors))
    print(
        "learning metadata is current "
        "(106 effective statements + canonical pedagogy blocks)"
    )


if __name__ == "__main__":
    main()
