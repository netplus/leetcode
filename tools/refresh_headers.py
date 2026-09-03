#!/usr/bin/env python3
"""Surgically refresh ONLY the header comment of specific solution.cpp files.

Why not `gen_all.py`? That regenerates every solution.cpp with an EMPTY method
body, wiping the user's implementations. This script re-renders the header via
gen_all.render() but splices it onto the EXISTING code body (everything from the
first `struct`/`#include`/`class Solution` marker onward), leaving the user's
Solution implementation and main() untouched.

Usage: python3 tools/refresh_headers.py lc3 lc11 ...   (dry-run by default)
       python3 tools/refresh_headers.py lc3 lc11 ... --write
"""
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
sys.path.insert(0, HERE)
import gen_all  # noqa: E402
import gen_data  # noqa: E402

HEADER_END = "// ============================================================================\n"


def problems_by_num():
    probs = []
    gen_data.build(lambda **kw: probs.append(kw))
    return {p["num"]: p for p in probs}


def find_solution(num):
    matches = []
    for weekdir in gen_all.WEEKS.values():
        base = os.path.join(ROOT, "problems", weekdir)
        if not os.path.isdir(base):
            continue
        for d in os.listdir(base):
            if re.match(rf"day\d+-lc{num}-", d):
                matches.append(os.path.join(base, d, "solution.cpp"))
    return matches[0] if matches else None


def split_header(text):
    """Return (header_comment, body). Header = up to & including the 2nd ==== line
    (the one closing the banner); body = the rest starting at #include."""
    lines = text.splitlines(keepends=True)
    banner = 0
    for i, ln in enumerate(lines):
        if ln.startswith(HEADER_END.rstrip("\n")):
            banner += 1
            if banner == 2:  # closing banner line
                return "".join(lines[: i + 1]), "".join(lines[i + 1:])
    raise ValueError("no closing banner found")


def new_header(p):
    """Render full stub, take its banner (through closing ==== line)."""
    full = gen_all.render(p)
    header, _ = split_header(full)
    return header


def main():
    write = "--write" in sys.argv
    nums = [int(a[2:]) for a in sys.argv[1:] if a.startswith("lc")]
    probs = problems_by_num()
    for num in nums:
        path = find_solution(num)
        if not path:
            print(f"lc{num}: NO solution.cpp found")
            continue
        old = open(path, encoding="utf-8").read()
        _, body = split_header(old)
        header = new_header(probs[num])
        new = header + body
        if new == old:
            print(f"lc{num}: unchanged ({os.path.relpath(path, ROOT)})")
            continue
        if write:
            open(path, "w", encoding="utf-8").write(new)
            print(f"lc{num}: WROTE header ({os.path.relpath(path, ROOT)})")
        else:
            # show a compact diff of header lines only
            import difflib
            old_h, _ = split_header(old)
            diff = difflib.unified_diff(
                old_h.splitlines(), header.splitlines(), lineterm="", n=1)
            print(f"lc{num}: would update ({os.path.relpath(path, ROOT)})")
            for d in diff:
                print("   ", d)


if __name__ == "__main__":
    main()
