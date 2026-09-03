"""Learner-facing statement metadata with persistent official-text corrections.

The raw Chinese cache is intentionally refreshable.  Curated corrections live in
statement_overrides.py and are applied after the cache is loaded so a refresh
cannot silently undo a reviewed fix.
"""

from chinese_metadata import (
    comment_lines,
    get_chinese_metadata as get_cached_chinese_metadata,
    validate_chinese_metadata,
)
from statement_overrides import STATEMENT_OVERRIDES


def get_statement_metadata(problem: dict) -> dict:
    data = dict(get_cached_chinese_metadata(problem))
    patch = STATEMENT_OVERRIDES.get(problem["num"], {})

    if "title" in patch:
        data["title"] = patch["title"]
    if "description" in patch:
        data["description"] = comment_lines(patch["description"])
    if "constraints" in patch:
        data["constraints"] = comment_lines(
            patch["constraints"], prefix="//   - "
        )
    return data


def validate_statement_metadata(problems: list[dict]) -> None:
    validate_chinese_metadata(problems)
    expected = {problem["num"] for problem in problems}
    unknown = sorted(set(STATEMENT_OVERRIDES) - expected)
    if unknown:
        raise RuntimeError(f"statement overrides reference unknown problems: {unknown}")

    for problem in problems:
        data = get_statement_metadata(problem)
        required = {"title", "difficulty", "priority", "description", "constraints", "goal", "iofmt"}
        missing = sorted(required - set(data))
        if missing:
            raise RuntimeError(f"LC-{problem['num']}: statement metadata missing {missing}")
        if any(not data[field] for field in required):
            raise RuntimeError(f"LC-{problem['num']}: empty statement metadata field")
