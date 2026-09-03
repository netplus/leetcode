#!/usr/bin/env python3
"""Triage all 106 problems: flag likely meaning-changing gaps vs official.

Two automated signals per problem:
  A. constraint-number mismatch (local set vs official set) -> hard error candidate
  B. key official sentences/clauses absent from local desc   -> review candidate

Output is a ranked list; the reviewer reads flagged problems to decide real fixes.
"""
import os
import re

HERE = os.path.dirname(os.path.abspath(__file__))
OFFICIAL = os.path.join(HERE, "official")


def parse_gen_data():
    src = open(os.path.join(HERE, "gen_data.py"), encoding="utf-8").read()
    for m in re.finditer(
        r'P\(num=(\d+)[^)]*?name="([^"]+)"[^)]*?'
        r'desc="""(.*?)""",\s*constraints="""(.*?)""",'
        r'[^)]*?goal="([^"]*)"',
        src, re.S,
    ):
        num, slug, desc, cons, goal = m.groups()
        yield int(num), slug, desc.strip(), cons.strip(), goal.strip()


def strip_comment(block):
    return "\n".join(re.sub(r"^\s*//\s?", "", ln).rstrip() for ln in block.splitlines()).strip()


def official_parts(path):
    txt = open(path, encoding="utf-8").read()
    body = txt.split("\n\n", 1)[1] if "\n\n" in txt else txt
    m = re.split(r"\n\s*Constraints?\s*:?\s*\n", body, flags=re.I)
    stmt, cons = (m + [""])[:2] if len(m) >= 2 else (body, "")
    core = re.split(r"\n\s*Example\s*1\s*:", stmt, flags=re.I)[0].strip()
    cons = re.split(r"\n\s*Follow[- ]?up", cons, flags=re.I)[0].strip()
    return core, cons


def nums(s):
    s = s.replace("10^", "1e").replace("2^31", "2147483648").replace("2^32", "4294967296")
    s = s.replace("^", "")
    return sorted(set(re.findall(r"-?\d+(?:\.\d+)?(?:e\d+)?", s)))


def words(s):
    return set(re.findall(r"[a-z]{4,}", s.lower()))


# Official clauses that change how you solve it (negations / prohibitions / guarantees).
KEY_PAT = re.compile(
    r"(you may not|may not|not allowed|cannot|can't|guaranteed|exactly one|"
    r"in-place|without using|must|only one|it is guaranteed|do not|don't|"
    r"notice that|note that|assume that|no two|at most|at least one)",
    re.I,
)


def key_clauses(stmt):
    parts = re.split(r"(?<=[.!?])\s+|\n", stmt)
    return [p.strip() for p in parts if KEY_PAT.search(p) and len(p.strip()) > 12]


def clause_covered(clause, local_desc):
    """Rough coverage: most content words of the clause appear in local desc."""
    cw = {w for w in re.findall(r"[a-z]{4,}", clause.lower())}
    if not cw:
        return True
    lw = words(local_desc)
    hit = sum(1 for w in cw if w in lw)
    return hit / len(cw) >= 0.6


def main():
    rows = []
    for num, slug, desc, cons, goal in parse_gen_data():
        path = os.path.join(OFFICIAL, f"lc{num}.txt")
        if not os.path.exists(path):
            continue
        core, ocons = official_parts(path)
        ldesc, lcons = strip_comment(desc), strip_comment(cons)
        # A complexity/space requirement counts as covered if it appears in the goal field.
        local_all = ldesc + "\n" + goal

        ln, on = nums(lcons), nums(ocons)
        num_miss = [x for x in on if x not in ln]
        num_extra = [x for x in ln if x not in on]

        missing = [c for c in key_clauses(core) if not clause_covered(c, local_all)]

        score = len(num_miss) * 3 + len(missing) * 2 + (1 if num_extra else 0)
        if score:
            rows.append((score, num, slug, num_miss, num_extra, missing))

    rows.sort(reverse=True)
    for score, num, slug, num_miss, num_extra, missing in rows:
        print(f"[{score:2d}] lc{num} {slug}")
        if num_miss:
            print(f"      constraint nums missing locally: {num_miss}")
        if num_extra:
            print(f"      constraint nums extra locally:   {num_extra}")
        for c in missing:
            print(f"      missing-clause: {c[:110]}")
    print(f"\n{len(rows)} flagged / 106")


if __name__ == "__main__":
    main()
