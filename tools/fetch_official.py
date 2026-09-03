#!/usr/bin/env python3
"""Fetch official LeetCode problem statements for all problems in gen_data.py.

Caches each problem's cleaned statement + constraints to tools/official/lc<N>.txt
so the semantic review does not depend on repeated network access.

Usage:
    python3 tools/fetch_official.py [--proxy socks5://10.8.1.104:20020] [--force]
"""
import argparse
import html
import json
import os
import re
import subprocess
import sys
import time

HERE = os.path.dirname(os.path.abspath(__file__))
OUTDIR = os.path.join(HERE, "official")


def extract_problems():
    """Return list of (num, slug) from gen_data.py P(...) calls."""
    src = open(os.path.join(HERE, "gen_data.py"), encoding="utf-8").read()
    pairs = re.findall(r'P\(num=(\d+)[^)]*?name="([^"]+)"', src, re.S)
    seen, out = set(), []
    for num, slug in pairs:
        num = int(num)
        if num not in seen:
            seen.add(num)
            out.append((num, slug))
    return sorted(out)


GRAPHQL = "https://leetcode.com/graphql"
QUERY = (
    "query{question(titleSlug:\"%s\"){questionFrontendId title content}}"
)


def clean(content: str) -> str:
    """Strip HTML, keep readable plain text of statement + constraints."""
    c = content or ""
    # keep superscript exponent readable
    c = re.sub(r"<sup>", "^", c)
    c = re.sub(r"</sup>", "", c)
    # drop example images / figures entirely
    c = re.sub(r"<img[^>]*>", " ", c)
    # line breaks for block tags
    c = re.sub(r"</(p|li|ul|ol|pre|div)>", "\n", c)
    c = re.sub(r"<li[^>]*>", "- ", c)
    c = re.sub(r"<br\s*/?>", "\n", c)
    c = re.sub(r"<pre[^>]*>", "\n```\n", c)
    c = re.sub(r"</pre>", "\n```\n", c)
    # strip all remaining tags
    c = re.sub(r"<[^>]+>", "", c)
    c = html.unescape(c)
    # normalize whitespace
    c = re.sub(r"[ \t\xa0]+", " ", c)
    c = re.sub(r" *\n *", "\n", c)
    c = re.sub(r"\n{3,}", "\n\n", c)
    return c.strip()


def fetch(slug: str, proxy: str | None) -> str:
    q = QUERY % slug
    cmd = [
        "curl", "-s", "--max-time", "30",
    ]
    if proxy:
        cmd += ["--proxy", proxy]
    cmd += [GRAPHQL, "-H", "Content-Type: application/json", "-d", json.dumps({"query": q})]
    out = subprocess.run(cmd, capture_output=True, text=True).stdout
    data = json.loads(out)
    qobj = data.get("data", {}).get("question")
    if not qobj:
        raise RuntimeError(f"no question for slug={slug}: {out[:200]}")
    return qobj["questionFrontendId"], qobj["title"], clean(qobj["content"])


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--proxy", default="socks5://10.8.1.104:20020")
    ap.add_argument("--force", action="store_true")
    ap.add_argument("--sleep", type=float, default=0.6)
    args = ap.parse_args()

    os.makedirs(OUTDIR, exist_ok=True)
    problems = extract_problems()
    print(f"{len(problems)} problems in gen_data.py", flush=True)
    ok, fail, skip = 0, [], 0
    for num, slug in problems:
        path = os.path.join(OUTDIR, f"lc{num}.txt")
        if os.path.exists(path) and not args.force:
            skip += 1
            continue
        try:
            fid, title, text = fetch(slug, args.proxy)
            with open(path, "w", encoding="utf-8") as f:
                f.write(f"LC-{fid} {title}\n")
                f.write(f"slug: {slug}\n\n")
                f.write(text + "\n")
            ok += 1
            print(f"  ok lc{num} {slug}", flush=True)
        except Exception as e:  # noqa: BLE001
            fail.append((num, slug, str(e)))
            print(f"  FAIL lc{num} {slug}: {e}", flush=True)
        time.sleep(args.sleep)
    print(f"done: ok={ok} skip={skip} fail={len(fail)}", flush=True)
    for num, slug, e in fail:
        print(f"  RETRY-NEEDED lc{num} {slug}: {e}", flush=True)


if __name__ == "__main__":
    main()
