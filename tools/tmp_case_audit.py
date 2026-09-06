#!/usr/bin/env python3
"""Temporary repo-wide case audit for semantic coverage expansion."""

from __future__ import annotations

import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
PROBLEM_RE = re.compile(r"day\d+-lc(\d+)-")


def compact_main(text: str) -> str:
    pos = text.rfind("int main()")
    if pos < 0:
        return "<no-main>"
    body = text[pos:]
    body = re.sub(r"//.*", "", body)
    body = re.sub(r"/\*.*?\*/", "", body, flags=re.S)
    body = re.sub(r"\s+", " ", body).strip()
    return body[:1200]


def main() -> None:
    rows = []
    for meta in sorted(ROOT.glob("problems/**/cases/meta.tsv")):
        problem_dir = meta.parent.parent
        match = PROBLEM_RE.search(problem_dir.name)
        if not match:
            continue
        lc = int(match.group(1))
        ins = sorted(meta.parent.glob("*.in"), key=lambda p: int(p.stem))
        lines = [line.rstrip("\n") for line in meta.read_text(encoding="utf-8").splitlines() if line.strip()]
        entries = []
        kinds = set()
        for line in lines:
            parts = line.split("\t", 2)
            if len(parts) != 3:
                entries.append(f"BAD:{line}")
                continue
            case, labels, purpose = parts
            kinds.update(x for x in labels.split(",") if x)
            entries.append(f"{case}:{labels}:{purpose}")
        solution = problem_dir / "solution.cpp"
        adapter = compact_main(solution.read_text(encoding="utf-8")) if solution.exists() else "<missing-solution>"
        rows.append((lc, problem_dir, len(ins), sorted(kinds), entries, adapter))

    print(f"FORMAL={len(rows)} TOTAL_CASES={sum(r[2] for r in rows)}")
    hist = {}
    for _, _, count, *_ in rows:
        hist[count] = hist.get(count, 0) + 1
    print("COUNT_HIST=" + ",".join(f"{k}:{v}" for k, v in sorted(hist.items())))
    print()
    for lc, problem_dir, count, kinds, entries, adapter in rows:
        print(f"=== LC{lc} | {problem_dir.relative_to(ROOT)} | cases={count} ===")
        print("KINDS=" + ",".join(kinds))
        for entry in entries:
            print("META=" + entry)
        print("ADAPTER=" + adapter)
        print()


if __name__ == "__main__":
    main()
