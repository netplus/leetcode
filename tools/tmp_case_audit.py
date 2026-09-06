#!/usr/bin/env python3
"""Temporary compact repo-wide case audit for semantic coverage expansion."""

from __future__ import annotations

import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
PROBLEM_RE = re.compile(r"day\d+-lc(\d+)-")


def main() -> None:
    rows = []
    for meta in sorted(ROOT.glob("problems/**/cases/meta.tsv")):
        problem_dir = meta.parent.parent
        match = PROBLEM_RE.search(problem_dir.name)
        if not match:
            continue
        lc = int(match.group(1))
        count = len(list(meta.parent.glob("*.in")))
        labels = set()
        for line in meta.read_text(encoding="utf-8").splitlines():
            if not line.strip() or line.startswith("#"):
                continue
            parts = line.split("\t", 2)
            if len(parts) == 3:
                labels.update(x for x in parts[1].split(",") if x)
        rows.append((lc, count, sorted(labels), problem_dir.relative_to(ROOT)))

    print(f"FORMAL={len(rows)} TOTAL_CASES={sum(r[1] for r in rows)}")
    counts = sorted(set(r[1] for r in rows))
    for count in counts:
        ids = sorted(lc for lc, c, _, _ in rows if c == count)
        print(f"COUNT_{count}={','.join(map(str, ids))}")
    print("LOW_COVERAGE_DETAIL")
    for lc, count, labels, path in sorted(rows):
        if count <= 6:
            print(f"LC{lc}\t{path}\t{count}\t{','.join(labels)}")


if __name__ == "__main__":
    main()
