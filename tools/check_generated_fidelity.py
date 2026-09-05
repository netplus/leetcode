#!/usr/bin/env python3
"""Verify canonical learning data exactly matches generated workspace files.

Unlike gen_all.py, this check is non-mutating: it renders every problem in memory
and compares that result with the checked-in solution.cpp / test.in files.  It
also verifies the two Week 4 mock packages and enforces the repository's current
high-touch pedagogy/code-comment baseline.
"""

from pathlib import Path
import sys

HERE = Path(__file__).resolve().parent
ROOT = HERE.parent
sys.path.insert(0, str(HERE))

import gen_data  # noqa: E402
from gen_all import WEEKS, render  # noqa: E402
from gen_mocks import MOCK1, MOCK2  # noqa: E402
from refined_data import get_refinement, validate_coverage  # noqa: E402
from statement_metadata import validate_statement_metadata  # noqa: E402


def main() -> None:
    problems = []
    gen_data.build(lambda **problem: problems.append(problem))
    validate_coverage(problems)
    validate_statement_metadata(problems)

    errors: list[str] = []

    for problem in problems:
        num = problem["num"]
        refinement = get_refinement(num)

        # The historical migration is complete: every reviewed problem should
        # now use the high-touch visual/core/invariant teaching structure.
        for field in ("visual", "core", "formula"):
            value = refinement.get(field)
            if not isinstance(value, str) or not value.strip():
                errors.append(f"LC-{num}: missing high-touch pedagogy field {field}")

        # This is deliberately a minimal mechanical guard.  Semantic comment
        # quality still requires per-problem review, but a completely bare
        # Solution block should never re-enter the reviewed corpus.
        code = refinement.get("code", "")
        solution_body = code.split("class Solution", 1)[-1]
        if "//" not in solution_body and "/*" not in solution_body:
            errors.append(f"LC-{num}: reviewed Solution has no implementation comments")

        directory = (
            ROOT
            / "problems"
            / WEEKS[problem["week"]]
            / f"day{problem['day']}-lc{num}-{problem['name']}"
        )
        solution_path = directory / "solution.cpp"
        test_path = directory / "test.in"

        if not solution_path.exists():
            errors.append(f"LC-{num}: missing {solution_path.relative_to(ROOT)}")
        else:
            actual = solution_path.read_text(encoding="utf-8")
            expected = render(problem)
            if actual != expected:
                errors.append(
                    f"LC-{num}: solution.cpp differs from canonical gen_all.render() output"
                )

        if not test_path.exists():
            errors.append(f"LC-{num}: missing {test_path.relative_to(ROOT)}")
        elif test_path.read_text(encoding="utf-8") != problem["testin"]:
            errors.append(f"LC-{num}: test.in differs from canonical gen_data fixture")

    mocks = {
        ROOT / "problems" / WEEKS[4] / "day27-mock-exam-1" / "MOCK.md": MOCK1,
        ROOT / "problems" / WEEKS[4] / "day28-mock-exam-2" / "MOCK.md": MOCK2,
    }
    for path, expected in mocks.items():
        if not path.exists():
            errors.append(f"missing {path.relative_to(ROOT)}")
        elif path.read_text(encoding="utf-8") != expected:
            errors.append(f"{path.relative_to(ROOT)} differs from tools/gen_mocks.py")

    if errors:
        raise SystemExit(
            "generated fidelity check failed:\n  - " + "\n  - ".join(errors)
        )

    print(
        f"Generated fidelity OK: {len(problems)}/106 solution.cpp + test.in files "
        "and 2 mock packages exactly match canonical render data."
    )


if __name__ == "__main__":
    main()
