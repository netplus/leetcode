#!/usr/bin/env python3
"""Validate PLAN/PROGRESS counts against canonical problem metadata."""

from collections import Counter, defaultdict
from pathlib import Path
import re
import sys

HERE = Path(__file__).resolve().parent
ROOT = HERE.parent
sys.path.insert(0, str(HERE))

import gen_data  # noqa: E402


MOCK_UNITS = {1: 0, 2: 0, 3: 0, 4: 2}


def main() -> None:
    problems = []
    gen_data.build(lambda **problem: problems.append(problem))

    if len(problems) != 106:
        raise SystemExit(f"expected 106 formal problems, found {len(problems)}")

    week_priority = defaultdict(Counter)
    day_p0 = Counter()
    week_total = Counter()

    for problem in problems:
        week = problem["week"]
        day = problem["day"]
        priority = problem["prio"]
        if priority not in {"P0", "P1"}:
            raise SystemExit(f"LC-{problem['num']}: unexpected priority {priority!r}")
        week_priority[week][priority] += 1
        week_total[week] += 1
        if priority == "P0":
            day_p0[day] += 1

    plan = (ROOT / "PLAN.md").read_text(encoding="utf-8")
    progress = (ROOT / "PROGRESS.md").read_text(encoding="utf-8")
    errors: list[str] = []

    total_p0 = 0
    total_p1 = 0
    total_lc = 0
    total_mock = 0

    for week in range(1, 5):
        p0 = week_priority[week]["P0"]
        p1 = week_priority[week]["P1"]
        lc = week_total[week]
        mocks = MOCK_UNITS[week]
        units = lc + mocks

        total_p0 += p0
        total_p1 += p1
        total_lc += lc
        total_mock += mocks

        plan_row = f"| W{week} | {p0} | {p1} | {lc} | {mocks} | {units} |"
        if plan_row not in plan:
            errors.append(f"PLAN.md: stale W{week} count row; expected {plan_row}")

        dashboard_pattern = re.compile(
            rf"^\| W{week} \|[^\n]*\| {p0} \|[^\n]*\| {units} \|",
            re.MULTILINE,
        )
        if not dashboard_pattern.search(progress):
            errors.append(
                f"PROGRESS.md: W{week} dashboard must use P0={p0}, total={units}"
            )

    expected_total_row = (
        f"| **Total** | **{total_p0}** | **{total_p1}** | **{total_lc}** | "
        f"**{total_mock}** | **{total_lc + total_mock}** |"
    )
    if expected_total_row not in plan:
        errors.append(f"PLAN.md: stale total row; expected {expected_total_row}")

    if f"| **Total** | | **{total_p0}** |" not in progress:
        errors.append(f"PROGRESS.md: dashboard total P0 must be {total_p0}")

    for day, p0 in sorted(day_p0.items()):
        daily_pattern = re.compile(
            rf"^\|\s*D{day}\s*\|[^|]*\|[^|]*\|[^|]*\|\s*/{p0}\s*\|",
            re.MULTILINE,
        )
        if not daily_pattern.search(progress):
            errors.append(f"PROGRESS.md: D{day} P0 denominator must be /{p0}")

    if errors:
        raise SystemExit("plan/progress count check failed:\n  - " + "\n  - ".join(errors))

    print(
        f"Plan counts OK: {total_p0} P0 + {total_p1} P1 = {total_lc} LC problems; "
        f"+ {total_mock} mock units = {total_lc + total_mock} plan units."
    )


if __name__ == "__main__":
    main()
