#!/usr/bin/env python3
"""Compare gen_data.py desc/constraints against cached official statements.

For every problem, extract the local `desc` and `constraints` blocks from
gen_data.py and the official statement/constraints from tools/official/lc<N>.txt,
then emit a compact per-problem view for semantic review.

This does NOT auto-judge; it prepares side-by-side material. Judgement of
"meaning-changing" vs "cosmetic" is done by the reviewer.

Usage: python3 tools/compare_official.py [lcN ...]
"""
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
OFFICIAL = os.path.join(HERE, "official")


def parse_gen_data():
    """Yield (num, slug, desc, constraints) from gen_data.py."""
    src = open(os.path.join(HERE, "gen_data.py"), encoding="utf-8").read()
    # Each problem block starts with P(num=... and desc/constraints are triple-quoted.
    for m in re.finditer(
        r'P\(num=(\d+)[^)]*?name="([^"]+)"[^)]*?'
        r'desc="""(.*?)""",\s*'
        r'constraints="""(.*?)""",',
        src, re.S,
    ):
        num, slug, desc, cons = m.groups()
        yield int(num), slug, desc.strip(), cons.strip()


def strip_comment(block: str) -> str:
    """Remove leading '// ' from each line of a gen_data comment block."""
    lines = []
    for ln in block.splitlines():
        ln = re.sub(r"^\s*//\s?", "", ln)
        lines.append(ln.rstrip())
    return "\n".join(lines).strip()


def official_parts(path: str):
    """Split official text into (statement, constraints) by the Constraints: header."""
    txt = open(path, encoding="utf-8").read()
    # drop first two header lines (LC-id title / slug)
    body = txt.split("\n\n", 1)[1] if "\n\n" in txt else txt
    m = re.split(r"\n\s*Constraints?\s*:?\s*\n", body, flags=re.I)
    if len(m) == 2:
        stmt, cons = m
    else:
        stmt, cons = body, ""
    # statement: cut off examples section for the core-description comparison
    core = re.split(r"\n\s*Example\s*1\s*:", stmt, flags=re.I)[0].strip()
    cons = re.split(r"\n\s*Follow[- ]?up", cons, flags=re.I)[0].strip()
    return core, cons, stmt.strip()


def norm_nums(s: str):
    """Extract comparable numeric/exponent tokens from a constraints string."""
    s = s.replace("10^", "1e").replace("^", "")
    return re.findall(r"-?\d+(?:\.\d+)?(?:e\d+)?|2\^\d+|31|32", s)


def main():
    only = {int(x[2:]) for x in sys.argv[1:] if x.startswith("lc")}
    for num, slug, desc, cons in parse_gen_data():
        if only and num not in only:
            continue
        path = os.path.join(OFFICIAL, f"lc{num}.txt")
        if not os.path.exists(path):
            print(f"### lc{num} {slug}: NO OFFICIAL CACHE\n")
            continue
        core, ocons, full_stmt = official_parts(path)
        print(f"### lc{num} {slug}")
        print("-- LOCAL desc --")
        print(strip_comment(desc))
        print("-- OFFICIAL core statement --")
        print(core)
        print("-- LOCAL constraints --")
        print(strip_comment(cons))
        print("-- OFFICIAL constraints --")
        print(ocons)
        print("-- constraint-number sets local vs official --")
        print("L:", norm_nums(strip_comment(cons)))
        print("O:", norm_nums(ocons))
        print()


if __name__ == "__main__":
    main()
