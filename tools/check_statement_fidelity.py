#!/usr/bin/env python3
"""Verify learner-facing statements for every generated problem.

This is an offline 106-problem guard.  The effective learner-facing metadata is
`official Chinese cache + reviewed statement overrides + official examples`.
Every generated solution.cpp must contain the exact rendered description,
example, and constraint blocks.

It intentionally does not fetch the network.  Refreshing the raw official cache
is a separate maintenance action (`fetch_chinese_problem_info.py` /
`fetch_official.py`); reviewed overrides survive that refresh.
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


def main() -> None:
    problems = []
    gen_data.build(lambda **problem: problems.append(problem))
    validate_statement_metadata(problems)

    failures = []
    for problem in problems:
        path = problem_path(problem)
        if not path.exists():
            failures.append(f"LC-{problem['num']}: missing {path.relative_to(ROOT)}")
            continue

        text = path.read_text(encoding="utf-8")
        metadata = get_statement_metadata(problem)
        checks = {
            "title": f"// LC-{problem['num']}：{metadata['title']}",
            "description": metadata["description"],
            "examples": metadata["examples"],
            "constraints": metadata["constraints"],
        }
        for field, expected in checks.items():
            if expected not in text:
                failures.append(
                    f"LC-{problem['num']}: generated {field} differs from effective official metadata"
                )

    if failures:
        raise SystemExit(
            "statement fidelity check failed:\n  - " + "\n  - ".join(failures)
        )

    print(
        f"Statement fidelity OK: {len(problems)}/106 generated problems "
        "match effective metadata, including examples."
    )


if __name__ == "__main__":
    main()
