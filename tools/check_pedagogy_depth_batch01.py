#!/usr/bin/env python3
"""Focused executable evidence for LC39/LC300; not a full-repository gate.

Default: explanation examples + canonical patch/generated section agreement.
--cpp: also compile the actual two solutions and exercise their adapters and
       batched public methods against independent enumeration oracles.
--examples-only: run only mathematical examples, with no repository/C++ checks.
"""
from __future__ import annotations

import argparse
from bisect import bisect_left, bisect_right
from itertools import combinations, product
from pathlib import Path
import re
import runpy
import shutil
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]
PATHS = {
    39: "problems/week2-list-tree-bsp/day13-lc39-combination-sum/solution.cpp",
    300: "problems/week3-graph-dp/day19-lc300-longest-increasing-subsequence/solution.cpp",
}
TITLES = {
    "visual": "题目直觉 / 图形模型",
    "general_solution": "最自然的一般性解法",
    "limitations": "一般解法的问题与限制",
    "generalization": "从具体问题抽象规律",
    "core": "核心算法",
    "transfer": "举一反三",
}


def require(condition: bool, message: str) -> None:
    if not condition:
        raise AssertionError(message)


def lis_brute(values: tuple[int, ...]) -> int:
    best = 0
    for mask in range(1 << len(values)):
        seq = [x for i, x in enumerate(values) if mask & (1 << i)]
        if all(a < b for a, b in zip(seq, seq[1:])):
            best = max(best, len(seq))
    return best


def lis_dp(values: tuple[int, ...]) -> int:
    dp = [1] * len(values)
    for i, x in enumerate(values):
        for j in range(i):
            if values[j] < x:
                dp[i] = max(dp[i], dp[j] + 1)
    return max(dp, default=0)


def tail_summary(values: tuple[int, ...], nondecreasing: bool = False) -> list[int]:
    tails: list[int] = []
    for x in values:
        p = (bisect_right if nondecreasing else bisect_left)(tails, x)
        if p == len(tails):
            tails.append(x)
        else:
            tails[p] = x
    return tails


def count_vector_oracle(values: tuple[int, ...], target: int) -> set[tuple[int, ...]]:
    # Enumerate usage counts, independently of the solution's start-index DFS.
    result: set[tuple[int, ...]] = set()
    for counts in product(*(range(target // x + 1) for x in values)):
        if sum(x * count for x, count in zip(values, counts)) == target:
            result.add(tuple(sorted(x for x, count in zip(values, counts) for _ in range(count))))
    return result


def sequence_baseline(values: tuple[int, ...], target: int) -> list[tuple[int, ...]]:
    result: list[tuple[int, ...]] = []

    def visit(path: tuple[int, ...], remaining: int) -> None:
        if remaining == 0:
            result.append(path)
            return
        for x in values:
            if x <= remaining:
                visit(path + (x,), remaining - x)

    visit((), target)
    return result


def start_search(values: tuple[int, ...], target: int, *, reuse: bool = True,
                 sort_values: bool = True, break_large: bool = True) -> list[tuple[int, ...]]:
    data = tuple(sorted(values)) if sort_values else values
    result: list[tuple[int, ...]] = []

    def visit(start: int, path: tuple[int, ...], remaining: int) -> None:
        if remaining == 0:
            result.append(path)
            return
        for i in range(start, len(data)):
            if data[i] > remaining:
                if break_large:
                    break
                continue
            visit(i if reuse else i + 1, path + (data[i],), remaining - data[i])

    visit(0, (), target)
    return result


def example_evidence() -> tuple[list[tuple[int, ...]], list[tuple[tuple[int, ...], int]]]:
    lis_inputs = [values for n in range(1, 8) for values in product((-1, 0, 1), repeat=n)]
    lis_inputs += [(10, 9, 2, 5, 3, 7, 101, 18), (0, 1, 0, 3, 2, 3), (7,) * 7,
                   (3, 5, 6, 2, 4), (1, 100, 3), (1, 2, 3)]
    for values in lis_inputs:
        expected = lis_brute(values)
        require(lis_dp(values) == expected, f"DP mismatch: {values}")
        require(len(tail_summary(values)) == expected, f"tails mismatch: {values}")
    require(tail_summary((3, 5, 6, 2, 4)) == [2, 4, 6], "tails example drift")
    require(not any(tuple((3, 5, 6, 2, 4)[i] for i in indexes) == (2, 4, 6)
                    for indexes in combinations(range(5), 3)), "example must not be a subsequence")
    require(lis_brute((1, 100, 3)) == 2 and lis_brute((1, 2, 3)) == 3,
            "length-only information-loss example drift")

    combo_inputs: list[tuple[tuple[int, ...], int]] = []
    for n in range(1, 6):
        for values in combinations(range(2, 7), n):
            for order in (values, values[::-1]):
                combo_inputs.extend((order, target) for target in range(1, 13))
    combo_inputs += [((2, 3, 6, 7), 7), ((2, 3, 5), 8), ((2,), 1), ((7, 2, 3), 4)]
    for values, target in combo_inputs:
        expected = count_vector_oracle(values, target)
        normalized = {tuple(sorted(seq)) for seq in sequence_baseline(values, target)}
        actual = start_search(values, target)
        require(normalized == expected, f"baseline mismatch: {values}, {target}")
        require(set(actual) == expected and len(actual) == len(set(actual)),
                f"start search missing/duplicate output: {values}, {target}")
    sequences = sequence_baseline((2, 3), 7)
    require(set(sequences) == {(2, 2, 3), (2, 3, 2), (3, 2, 2)}, "sequence example drift")
    require(len({tuple(sorted(seq)) for seq in sequences}) == 1, "dedup example drift")
    require(start_search((2, 3, 5), 4) == [(2, 2)] and start_search((3, 5), 4) == [],
            "remaining alone is insufficient")
    require({tuple(sorted(seq)) for seq in start_search((7, 2, 3), 4,
             sort_values=False, break_large=False)} == {(2, 2)}, "fixed-index order should work")

    # Deliberately wrong variants must disagree with the original problem oracle.
    require(len(tail_summary((2, 2), nondecreasing=True)) != lis_brute((2, 2)),
            "upper_bound mutant was not killed")
    require(set(start_search((2, 3), 7, reuse=False)) != count_vector_oracle((2, 3), 7),
            "i+1 mutant was not killed")
    require(set(start_search((7, 2, 3), 4, sort_values=False)) != {(2, 2)},
            "unsorted-break mutant was not killed")
    print(f"PASS Python differential cases: LC300={len(lis_inputs)}, LC39={len(combo_inputs)}")
    print("PASS explanatory examples and 3 deliberately wrong variants rejected")
    return lis_inputs, combo_inputs


def rendered_sections() -> None:
    patches = runpy.run_path(str(ROOT / "tools/pedagogy_depth_batch01.py"))["DEPTH_BATCH01_OVERRIDES"]
    checked = 0
    for num, patch in patches.items():
        text = (ROOT / PATHS[num]).read_text(encoding="utf-8")
        for field, value in patch.items():
            title = TITLES[field]
            matches = list(re.finditer(r"^// (\d+)\. " + re.escape(title) +
                           r"\n(.*?)(?=^// \d+\. |^// -{20,}\n)", text, re.M | re.S))
            require(len(matches) == 1, f"LC{num}: missing/duplicate section {field}")
            m = matches[0]
            lines = [f"// {m.group(1)}. {title}"]
            lines += ["//   " + line if line else "//" for line in value.strip("\n").splitlines()]
            if field != "transfer":
                lines.append("//")
            require(m.group(0) == "\n".join(lines) + "\n", f"LC{num}: rendered field drift: {field}")
            checked += 1
    print(f"PASS {checked} patch/generated sections (focused check, not full canonical merge)")


def execute(command: list[str], stdin: str = "") -> str:
    result = subprocess.run(command, input=stdin, text=True, capture_output=True, timeout=60)
    if result.returncode:
        raise RuntimeError(f"Command failed: {' '.join(command)}\n{result.stderr}")
    return result.stdout


def cpp_evidence(lis_inputs: list[tuple[int, ...]], combo_inputs: list[tuple[tuple[int, ...], int]]) -> None:
    compiler = shutil.which("g++")
    if not compiler:
        raise RuntimeError("--cpp requires g++")
    with tempfile.TemporaryDirectory(prefix="pedagogy-batch01-") as work:
        directory = Path(work)
        for num in (300, 39):
            source = (ROOT / PATHS[num]).resolve()
            binary = directory / f"lc{num}"
            execute([compiler, "-std=c++14", "-O2", "-Wall", "-Wextra", str(source), "-o", str(binary)])
            samples = [("8\n10 9 2 5 3 7 101 18\n", "4\n"),
                       ("6\n0 1 0 3 2 3\n", "4\n"), ("7\n7 7 7 7 7 7 7\n", "1\n")] if num == 300 else [
                       ("4 7\n2 3 6 7\n", "2 2 3\n7\n"),
                       ("3 8\n2 3 5\n", "2 2 2 2\n2 3 3\n3 5\n"), ("1 1\n2\n", "")]
            for stdin, expected in samples:
                require(execute([str(binary)], stdin) == expected, f"LC{num}: adapter sample mismatch")

            # Include the actual Solution, rename only its local main, then reuse
            # one Solution instance across many inputs (also checks LC39 clearing).
            header = f'#define main original_adapter_main\n#include "{source.as_posix()}"\n#undef main\n'
            if num == 300:
                main = '''int main() { Solution sol; int n; while(cin>>n) {
vector<int> a(n); for(int &x:a) cin>>x; cout<<sol.lengthOfLIS(a)<<"\\n"; } }\n'''
                stdin = "".join(f"{len(a)}\n{' '.join(map(str, a))}\n" for a in lis_inputs)
            else:
                main = '''int main() { Solution sol; int n,t; while(cin>>n>>t) {
vector<int>a(n);for(int &x:a)cin>>x;auto r=sol.combinationSum(a,t);
cout<<r.size();for(auto &v:r){cout<<" |";for(int x:v)cout<<" "<<x;}cout<<"\\n";} }\n'''
                stdin = "".join(f"{len(a)} {t}\n{' '.join(map(str, a))}\n" for a, t in combo_inputs)
            harness = directory / f"batch{num}.cpp"
            harness.write_text(header + main, encoding="utf-8")
            batched = directory / f"batch{num}"
            execute([compiler, "-std=c++14", "-O2", "-Wall", "-Wextra", str(harness), "-o", str(batched)])
            lines = execute([str(batched)], stdin).splitlines()
            inputs = lis_inputs if num == 300 else combo_inputs
            require(len(lines) == len(inputs), f"LC{num}: wrong number of outputs")
            for entry, line in zip(inputs, lines):
                if num == 300:
                    require(int(line) == lis_brute(entry), f"LC300 C++ mismatch: {entry}")
                else:
                    values, target = entry
                    parts = line.split("|")
                    rows = [tuple(map(int, p.split())) for p in parts[1:]]
                    expected = count_vector_oracle(values, target)
                    require(int(parts[0]) == len(rows) == len(set(rows)), "LC39 duplicate/count mismatch")
                    require(set(rows) == expected, f"LC39 C++ mismatch: {entry}")
            print(f"PASS C++14 LC{num}: 3 adapter examples + {len(inputs)} differential cases")


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cpp", action="store_true")
    parser.add_argument("--examples-only", action="store_true")
    args = parser.parse_args()
    if args.cpp and args.examples_only:
        parser.error("--cpp and --examples-only cannot be combined")
    lis_inputs, combo_inputs = example_evidence()
    if not args.examples_only:
        rendered_sections()
    if args.cpp:
        cpp_evidence(lis_inputs, combo_inputs)
    print("NOT RUN by this checker: full canonical merge, verify-meta, all-repository judges")


if __name__ == "__main__":
    main()
