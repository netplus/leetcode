#!/usr/bin/env python3
"""Refresh the offline Chinese statement/constraint cache from LeetCode China.

The normal generator never accesses the network.  This maintenance command
stores only the statement paragraphs and constraints; examples and editorial
material are intentionally excluded.
"""

from concurrent.futures import ThreadPoolExecutor, as_completed
from pathlib import Path
import json
import re
import subprocess
import sys
import time

from bs4 import BeautifulSoup

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))

import gen_data  # noqa: E402


ENDPOINT = "https://leetcode.cn/graphql/"
QUERY = """
query questionData($titleSlug: String!) {
  question(titleSlug: $titleSlug) {
    questionFrontendId
    translatedTitle
    translatedContent
  }
}
"""

PREMIUM = {
    252: {
        "title": "会议室",
        "description": [
            "给定一组会议时间区间 intervals，判断一个人能否参加全部会议。",
            "本地适配器把会议表示为半开区间 [start, end)；两个区间有时间重叠时，无法同时参加。",
        ],
        "constraints": [
            "0 <= intervals.length <= 10^4",
            "intervals[i].length == 2",
            "0 <= start < end <= 10^6",
        ],
    },
    253: {
        "title": "会议室 II",
        "description": [
            "给定一组会议时间区间 intervals，返回为了安排所有会议至少需要的会议室数量。",
            "本地适配器使用半开区间 [start, end)：一场会议在另一场开始时结束，可以复用同一间会议室。",
        ],
        "constraints": [
            "0 <= intervals.length <= 10^4",
            "intervals[i].length == 2",
            "0 <= start < end <= 10^6",
        ],
    },
}


def normalize(text: str) -> str:
    text = text.replace("\xa0", " ")
    text = re.sub(r"\s+", " ", text).strip()
    text = re.sub(r"\s+([，。；：！？、])", r"\1", text)
    text = re.sub(r"([，。；：！？、])\s+", r"\1", text)
    text = re.sub(r"([（【])\s+", r"\1", text)
    text = re.sub(r"\s+([）】])", r"\1", text)
    text = re.sub(r"(?<=[\u3400-\u9fff])\s+(?=[\u3400-\u9fff])", "", text)
    text = re.sub(r"\s*,\s*and\s+", "、", text, flags=re.I)
    text = re.sub(r"\band\b", "和", text, flags=re.I)
    text = re.sub(r"\bis either\b", "为", text, flags=re.I)
    text = re.sub(r"\bor\b", "或", text, flags=re.I)
    text = re.sub(r"\.$", "。", text)
    return text


def parse_content(content: str) -> tuple[list[str], list[str]]:
    content = re.sub(r"<sup>(.*?)</sup>", r"^\1", content, flags=re.S | re.I)
    content = re.sub(r"<sub>(.*?)</sub>", r"_\1", content, flags=re.S | re.I)
    soup = BeautifulSoup(content, "html.parser")

    # A few older statements begin with a bare text node rather than <p>.
    # Slice the raw HTML before the first example, then extract all visible text.
    prefix = re.split(
        r"<(?:p[^>]*>\s*)?(?:strong|b)[^>]*>(?:\s*<[^>]+>)*\s*示例",
        content,
        maxsplit=1,
        flags=re.S | re.I,
    )[0]
    description_text = normalize(BeautifulSoup(prefix, "html.parser").get_text(" ", strip=True))
    description = [
        normalize(sentence)
        for sentence in re.findall(r".+?(?:。|$)", description_text)
        if normalize(sentence)
    ]

    constraints = []
    label = soup.find(string=lambda value: value and normalize(value).startswith("提示"))
    if label:
        listing = label.parent.find_next(["ul", "ol"])
        if listing:
            constraints = [
                normalize(item.get_text(" ", strip=True))
                for item in listing.find_all("li")
            ]
    return description, constraints


def fetch_one(problem: dict) -> tuple[int, dict]:
    payload = json.dumps(
        {"query": QUERY, "variables": {"titleSlug": problem["name"]}},
        ensure_ascii=False,
    )
    last_error = "unknown error"
    for attempt in range(3):
        command = [
            "curl", "-sS", "--max-time", "30", ENDPOINT,
            "-H", "Content-Type: application/json", "--data-binary", payload,
        ]
        result = subprocess.run(command, capture_output=True, text=True)
        try:
            question = json.loads(result.stdout)["data"]["question"]
            if not question or not question["translatedContent"]:
                raise ValueError("translated content unavailable")
            description, constraints = parse_content(question["translatedContent"])
            if not description or not constraints:
                raise ValueError(
                    f"incomplete parse: description={len(description)}, constraints={len(constraints)}"
                )
            return problem["num"], {
                "title": question["translatedTitle"],
                "description": description,
                "constraints": constraints,
            }
        except Exception as exc:  # noqa: BLE001
            last_error = f"{type(exc).__name__}: {exc}; curl={result.stderr[:120]}"
            time.sleep(attempt + 1)
    raise RuntimeError(f"LC-{problem['num']} {problem['name']}: {last_error}")


def main() -> None:
    problems = []
    gen_data.build(lambda **problem: problems.append(problem))
    output = {str(num): value for num, value in PREMIUM.items()}
    failures = []

    with ThreadPoolExecutor(max_workers=6) as executor:
        futures = {
            executor.submit(fetch_one, problem): problem
            for problem in problems if problem["num"] not in PREMIUM
        }
        for future in as_completed(futures):
            problem = futures[future]
            try:
                num, value = future.result()
                output[str(num)] = value
            except Exception as exc:  # noqa: BLE001
                failures.append(str(exc))

    if failures:
        raise SystemExit("\n".join(sorted(failures)))
    if len(output) != len(problems):
        raise SystemExit(f"expected {len(problems)} entries, got {len(output)}")

    path = HERE / "chinese_problem_info.json"
    path.write_text(
        json.dumps(output, ensure_ascii=False, indent=2, sort_keys=True) + "\n",
        encoding="utf-8",
    )
    print(f"wrote {path.relative_to(HERE.parent)} ({len(output)}/106)")


if __name__ == "__main__":
    main()
