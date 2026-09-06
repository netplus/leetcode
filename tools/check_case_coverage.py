#!/usr/bin/env python3
"""Audit per-problem validation cases.

The repository is migrating from "a handful of sample inputs" to deliberate
validation suites.  During migration the default mode keeps the historical
4-case floor as a hard requirement, while reporting two debts:

- problem suites with fewer than the recommended 6 cases;
- suites that do not yet have ``cases/meta.tsv`` intent metadata.

Use ``--strict`` once the repository-wide migration is complete.  Strict mode
requires at least 6 cases and complete metadata for every problem.
"""

from __future__ import annotations

import argparse
import collections
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
PROBLEMS = ROOT / "problems"
HARD_MIN_CASES = 4
RECOMMENDED_MIN_CASES = 6
MAX_CASES = 8
ALLOWED_KINDS = {
    "official",
    "minimum",
    "boundary",
    "trap",
    "no-solution",
    "duplicates",
    "ordering",
    "overflow",
    "stress",
    "variant",
}


def problem_dirs() -> list[Path]:
    dirs = {path.parent for path in PROBLEMS.rglob("solution.cpp")}
    dirs.update(path.parent for path in PROBLEMS.rglob("solution.c"))
    return sorted(dirs)


def numeric_case_files(cases_dir: Path, suffix: str) -> dict[int, Path]:
    result: dict[int, Path] = {}
    for path in cases_dir.glob(f"*.{suffix}"):
        if not path.stem.isdigit():
            continue
        number = int(path.stem)
        result[number] = path
    return result


def read_intent_metadata(meta_path: Path) -> tuple[dict[int, tuple[set[str], str]], list[str]]:
    metadata: dict[int, tuple[set[str], str]] = {}
    errors: list[str] = []

    for lineno, raw in enumerate(meta_path.read_text(encoding="utf-8").splitlines(), 1):
        line = raw.strip()
        if not line or line.startswith("#"):
            continue
        parts = raw.split("\t", 2)
        if len(parts) != 3:
            errors.append(f"{meta_path}:{lineno}: expected CASE<TAB>KINDS<TAB>PURPOSE")
            continue

        case_text, kinds_text, purpose = (part.strip() for part in parts)
        if not case_text.isdigit():
            errors.append(f"{meta_path}:{lineno}: case id must be a positive integer")
            continue
        case_id = int(case_text)
        if case_id <= 0:
            errors.append(f"{meta_path}:{lineno}: case id must be >= 1")
            continue
        if case_id in metadata:
            errors.append(f"{meta_path}:{lineno}: duplicate metadata for case {case_id}")
            continue

        kinds = {kind.strip() for kind in kinds_text.split(",") if kind.strip()}
        if not kinds:
            errors.append(f"{meta_path}:{lineno}: at least one validation kind is required")
            continue
        unknown = sorted(kinds - ALLOWED_KINDS)
        if unknown:
            errors.append(
                f"{meta_path}:{lineno}: unknown kind(s): {', '.join(unknown)}; "
                f"allowed: {', '.join(sorted(ALLOWED_KINDS))}"
            )
            continue
        if not purpose:
            errors.append(f"{meta_path}:{lineno}: purpose must explain what bug the case can catch")
            continue

        metadata[case_id] = (kinds, purpose)

    return metadata, errors


def rel(path: Path) -> str:
    return str(path.relative_to(ROOT))


def audit(strict: bool) -> int:
    errors: list[str] = []
    short_suites: list[str] = []
    missing_metadata: list[str] = []
    metadata_suites = 0
    total_cases = 0
    dirs = problem_dirs()

    if not dirs:
        print("ERROR: no solution.cpp/solution.c files found under problems/", file=sys.stderr)
        return 1

    for problem_dir in dirs:
        cases_dir = problem_dir / "cases"
        if not cases_dir.is_dir():
            errors.append(f"{rel(problem_dir)}: missing cases/ directory")
            continue

        ins = numeric_case_files(cases_dir, "in")
        outs = numeric_case_files(cases_dir, "out")
        in_ids = set(ins)
        out_ids = set(outs)

        missing_out = sorted(in_ids - out_ids)
        missing_in = sorted(out_ids - in_ids)
        if missing_out:
            errors.append(f"{rel(problem_dir)}: missing .out for case(s) {missing_out}")
        if missing_in:
            errors.append(f"{rel(problem_dir)}: missing .in for case(s) {missing_in}")

        paired_ids = sorted(in_ids & out_ids)
        count = len(paired_ids)
        total_cases += count

        hard_min = RECOMMENDED_MIN_CASES if strict else HARD_MIN_CASES
        if count < hard_min:
            errors.append(f"{rel(problem_dir)}: only {count} paired case(s), need at least {hard_min}")
        elif count < RECOMMENDED_MIN_CASES:
            short_suites.append(rel(problem_dir))

        if count > MAX_CASES:
            errors.append(f"{rel(problem_dir)}: {count} cases exceeds repository maximum {MAX_CASES}")

        if paired_ids:
            expected_ids = list(range(1, max(paired_ids) + 1))
            if paired_ids != expected_ids:
                errors.append(
                    f"{rel(problem_dir)}: case numbering must be contiguous from 1; "
                    f"found {paired_ids}"
                )

        # Exact duplicate inputs add no validation power and usually indicate a
        # copy/paste mistake.  Compare normalized line endings but preserve all
        # other bytes so intentionally different whitespace-oriented string
        # cases remain distinguishable.
        seen_inputs: dict[str, int] = {}
        for case_id in paired_ids:
            text = ins[case_id].read_text(encoding="utf-8").replace("\r\n", "\n")
            if text in seen_inputs:
                errors.append(
                    f"{rel(problem_dir)}: case {case_id}.in duplicates case "
                    f"{seen_inputs[text]}.in"
                )
            else:
                seen_inputs[text] = case_id

        test_in = problem_dir / "test.in"
        if paired_ids and 1 in ins:
            if not test_in.is_file():
                errors.append(f"{rel(problem_dir)}: missing test.in")
            elif test_in.read_bytes() != ins[1].read_bytes():
                errors.append(f"{rel(problem_dir)}: test.in must exactly match cases/1.in")

        meta_path = cases_dir / "meta.tsv"
        if not meta_path.is_file():
            missing_metadata.append(rel(problem_dir))
            if strict:
                errors.append(f"{rel(problem_dir)}: strict mode requires cases/meta.tsv")
            continue

        metadata_suites += 1
        metadata, metadata_errors = read_intent_metadata(meta_path)
        errors.extend(metadata_errors)
        meta_ids = set(metadata)
        paired_set = set(paired_ids)
        missing_meta = sorted(paired_set - meta_ids)
        stale_meta = sorted(meta_ids - paired_set)
        if missing_meta:
            errors.append(f"{rel(problem_dir)}: meta.tsv missing case(s) {missing_meta}")
        if stale_meta:
            errors.append(f"{rel(problem_dir)}: meta.tsv references absent case(s) {stale_meta}")

    print(
        f"case-audit: {len(dirs)} problems, {total_cases} paired cases, "
        f"{metadata_suites} suites with intent metadata"
    )
    if short_suites:
        print(
            f"migration debt: {len(short_suites)} suite(s) have fewer than "
            f"{RECOMMENDED_MIN_CASES} cases"
        )
        for item in short_suites:
            print(f"  - {item}")
    if missing_metadata:
        print(
            f"migration debt: {len(missing_metadata)} suite(s) still lack cases/meta.tsv"
        )
        if len(missing_metadata) <= 20:
            for item in missing_metadata:
                print(f"  - {item}")
        else:
            print("  (use --strict after metadata migration is complete)")

    if errors:
        print(f"case-audit: {len(errors)} error(s)", file=sys.stderr)
        for error in errors:
            print(f"ERROR: {error}", file=sys.stderr)
        return 1

    print("case-audit: structural checks passed")
    return 0


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--strict",
        action="store_true",
        help="require >=6 cases and complete cases/meta.tsv for every problem",
    )
    args = parser.parse_args()
    return audit(strict=args.strict)


if __name__ == "__main__":
    raise SystemExit(main())
