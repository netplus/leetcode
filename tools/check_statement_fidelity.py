#!/usr/bin/env python3
"""Verify learner-facing statements against the reviewed doocs baseline.

This is an offline whole-corpus guard.  The effective learner-facing metadata is
the cached Chinese statement data plus reviewed doocs-aligned overrides and
examples.  Every generated solution.cpp must contain the exact rendered title,
description, example, and constraint blocks.

The checker intentionally does not fetch the network.  The statement source of
truth is ``https://leetcode.doocs.org/lc/<num>/``; refreshing/reviewing raw
website data is a separate maintenance action.  Once reviewed, the canonical
metadata is the CI/local baseline, so generated-file edits cannot silently drift
away from it.
"""

from pathlib import Path
import sys

HERE = Path(__file__).resolve().parent
ROOT = HERE.parent
sys.path.insert(0, str(HERE))

import gen_data  # noqa: E402
from statement_metadata import get_statement_metadata, validate_statement_metadata  # noqa: E402


WEEKS = {
    1: "week1-arrays-strings",
    2: "week2-list-tree-bsp",
    3: "week3-graph-dp",
    4: "week4-binary-search-greedy-mock",
}


def problem_path(problem: dict) -> Path:
    return (
        ROOT
        / "problems"
        / WEEKS[problem["week"]]
        / f"day{problem['day']}-lc{problem['num']}-{problem['name']}"
        / "solution.cpp"
    )


def extract_between(text: str, start_marker: str, end_marker: str) -> str | None:
    """Return text strictly between two unique statement-section markers."""
    start = text.find(start_marker)
    if start < 0:
        return None
    start += len(start_marker)
    end = text.find(end_marker, start)
    if end < 0:
        return None
    return text[start:end]


def extract_including_start(text: str, start_marker: str, end_marker: str) -> str | None:
    """Return a section including its opening marker but excluding the next one."""
    start = text.find(start_marker)
    if start < 0:
        return None
    end = text.find(end_marker, start + len(start_marker))
    if end < 0:
        return None
    return text[start:end]


def main() -> None:
    problems = []
    gen_data.build(lambda **problem: problems.append(problem))
    validate_statement_metadata(problems)

    failures = []
    for problem in problems:
        num = problem["num"]
        path = problem_path(problem)
        if not path.exists():
            failures.append(f"LC-{num}: missing {path.relative_to(ROOT)}")
            continue

        text = path.read_text(encoding="utf-8")
        metadata = get_statement_metadata(problem)

        title_line = text.splitlines()[1] if len(text.splitlines()) > 1 else ""
        expected_title = f"// LC-{num}：{metadata['title']}"
        if title_line != expected_title:
            failures.append(f"LC-{num}: generated title differs from reviewed doocs baseline")

        description = extract_between(
            text,
            "// 题目描述：\n",
            "\n//\n// 题目示例：",
        )
        if description != metadata["description"]:
            failures.append(
                f"LC-{num}: generated description differs from reviewed doocs baseline"
            )

        examples = extract_including_start(
            text,
            "// 题目示例：",
            "\n//\n// 约束与要求：",
        )
        if examples != metadata["examples"]:
            failures.append(
                f"LC-{num}: generated examples differ from reviewed doocs baseline"
            )

        constraints = extract_between(
            text,
            "// 约束与要求：\n",
            "\n//\n// 复杂度目标：",
        )
        if constraints != metadata["constraints"]:
            failures.append(
                f"LC-{num}: generated constraints differ from reviewed doocs baseline"
            )

        # Keep generated files whitespace-stable: exactly one final newline and
        # no extra blank line at EOF.  This mirrors render() in gen_all.py and
        # catches the drift that `git diff --check` reports as a new blank line.
        if text != text.rstrip("\n") + "\n":
            failures.append(
                f"LC-{num}: generated solution.cpp must end with exactly one newline"
            )

    if failures:
        raise SystemExit(
            "doocs statement fidelity check failed:\n  - " + "\n  - ".join(failures)
        )

    print(
        f"Doocs statement fidelity OK: {len(problems)} generated problems exactly match "
        "the reviewed title/description/examples/constraints baseline."
    )


if __name__ == "__main__":
    main()
