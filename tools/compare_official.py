#!/usr/bin/env python3
"""Compare effective learner-facing statements against cached official text.

For every problem, render the actual metadata used by gen_all.py (official
Chinese cache + persistent reviewed overrides) and compare it side-by-side with
tools/official/lc<N>.txt.

This does NOT auto-judge semantic equivalence; it prepares auditable material for
review.  Unlike the older version, it no longer compares the legacy English
summary fields in gen_data.py, so the reviewed source is the same source that
produces solution.cpp.

Usage: python3 tools/compare_official.py [lcN ...]
"""
from pathlib import Path
import re
import sys

HERE = Path(__file__).resolve().parent
OFFICIAL = HERE / "official"
sys.path.insert(0, str(HERE))

import gen_data  # noqa: E402
from statement_metadata import get_statement_metadata  # noqa: E402


def problems():
    items = []
    gen_data.build(lambda **item: items.append(item))
    return items


def strip_comment(block: str) -> str:
    """Remove generated C++ comment prefixes from rendered metadata."""
    lines = []
    for line in block.splitlines():
        line = re.sub(r"^\s*//\s*(?:-\s*)?", "", line)
        lines.append(line.rstrip())
    return "\n".join(lines).strip()


def official_parts(path: Path):
    """Split official text into (statement, constraints) by Constraints:."""
    text = path.read_text(encoding="utf-8")
    body = text.split("\n\n", 1)[1] if "\n\n" in text else text
    parts = re.split(r"\n\s*Constraints?\s*:?\s*\n", body, flags=re.I)
    if len(parts) == 2:
        statement, constraints = parts
    else:
        statement, constraints = body, ""
    core = re.split(r"\n\s*Example\s*1\s*:", statement, flags=re.I)[0].strip()
    constraints = re.split(
        r"\n\s*(?:Follow[- ]?up|Note:)\b", constraints, flags=re.I
    )[0].strip()
    return core, constraints


def norm_nums(text: str):
    """Extract comparable numeric/exponent tokens for quick discrepancy scans."""
    text = text.replace("10^", "1e").replace("^", "")
    return re.findall(r"-?\d+(?:\.\d+)?(?:e\d+)?|2\^\d+|31|32", text)


def main() -> None:
    only = {int(arg[2:]) for arg in sys.argv[1:] if arg.startswith("lc")}
    for problem in problems():
        num = problem["num"]
        if only and num not in only:
            continue

        path = OFFICIAL / f"lc{num}.txt"
        if not path.exists():
            print(f"### lc{num} {problem['name']}: NO OFFICIAL CACHE\n")
            continue

        metadata = get_statement_metadata(problem)
        local_desc = strip_comment(metadata["description"])
        local_constraints = strip_comment(metadata["constraints"])
        official_desc, official_constraints = official_parts(path)

        print(f"### lc{num} {problem['name']}")
        print("-- EFFECTIVE learner description --")
        print(local_desc)
        print("-- OFFICIAL core statement --")
        print(official_desc)
        print("-- EFFECTIVE learner constraints --")
        print(local_constraints)
        print("-- OFFICIAL constraints --")
        print(official_constraints)
        print("-- constraint-number tokens learner vs official --")
        print("L:", norm_nums(local_constraints))
        print("O:", norm_nums(official_constraints))
        print()


if __name__ == "__main__":
    main()
