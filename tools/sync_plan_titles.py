#!/usr/bin/env python3
"""Synchronize learner-facing problem titles in PLAN.md to Chinese."""

from pathlib import Path
import re
import sys

HERE = Path(__file__).resolve().parent
ROOT = HERE.parent
sys.path.insert(0, str(HERE))

from chinese_titles import get_chinese_title  # noqa: E402


PROBLEM_ROW = re.compile(
    r"^(\|\s*)(\d+(?:/\d+)*)(\s*\|\s*)([^|]+?)(\s*\|.*)$"
)


def render_plan(source: str) -> str:
    """Replace only the title column, preserving schedule and hint text."""
    rendered = []
    for line in source.splitlines():
        if line == "| # | Title | Priority | Hint / Pattern |":
            rendered.append("| # | 中文标题 | Priority | Hint / Pattern |")
            continue
        match = PROBLEM_ROW.match(line)
        if not match:
            rendered.append(line)
            continue
        numbers = [int(value) for value in match.group(2).split("/")]
        title = " / ".join(get_chinese_title(num) for num in numbers)
        rendered.append(
            f"{match.group(1)}{match.group(2)}{match.group(3)}"
            f"{title}{match.group(5)}"
        )
    return "\n".join(rendered) + "\n"


def main() -> None:
    path = ROOT / "PLAN.md"
    source = path.read_text(encoding="utf-8")
    rendered = render_plan(source)
    if "--check" in sys.argv:
        if source != rendered:
            raise SystemExit("PLAN.md Chinese titles are stale; synchronize them")
        print("PLAN.md Chinese titles are current")
        return
    path.write_text(rendered, encoding="utf-8")
    print("synchronized Chinese problem titles in PLAN.md")


if __name__ == "__main__":
    main()
