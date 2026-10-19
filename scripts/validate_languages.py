#!/usr/bin/env python3
"""Validate the files in languages/.

Every translation has to provide the same keys as languages/template.json and
must keep the printf placeholders of the English source intact, otherwise the
application either falls back to English for a single label or prints a
malformed status line.

Usage: python scripts/validate_languages.py
Exit code 0 = all good, 1 = at least one problem was found.
"""

import json
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
LANG_DIR = ROOT / "languages"
TEMPLATE = LANG_DIR / "template.json"

# %s, %llu, %.2f, %d ... - the literal placeholder tokens of a C format string.
PLACEHOLDER_RE = re.compile(r"%[-+ #0]*[0-9*]*(?:\.[0-9*]+)?(?:hh|h|ll|l|L|z|j|t)?[diouxXeEfFgGaAcspn%]")


def placeholders(text):
    return sorted(PLACEHOLDER_RE.findall(text))


def load(path):
    with path.open("r", encoding="utf-8") as handle:
        return json.load(handle)


def main():
    if not TEMPLATE.is_file():
        print(f"error: {TEMPLATE} not found")
        return 1

    template = load(TEMPLATE)
    reference = template["strings"]
    reference_keys = set(reference)

    problems = []
    files = sorted(p for p in LANG_DIR.glob("*.json") if p.name != "template.json")
    if not files:
        print("error: no translation files found")
        return 1

    for path in files:
        name = path.name
        try:
            data = load(path)
        except json.JSONDecodeError as exc:
            problems.append(f"{name}: invalid JSON ({exc})")
            continue

        for field in ("language_name", "language_code"):
            value = data.get(field)
            if not isinstance(value, str) or not value.strip():
                problems.append(f"{name}: missing or empty '{field}'")
            elif field == "language_code" and not re.fullmatch(r"[A-Za-z]{2,3}(-[A-Za-z0-9]{2,8})*", value):
                problems.append(f"{name}: '{field}' is not a BCP-47 style code: {value!r}")

        strings = data.get("strings")
        if not isinstance(strings, dict):
            problems.append(f"{name}: missing 'strings' object")
            continue

        missing = sorted(reference_keys - set(strings))
        if missing:
            problems.append(f"{name}: missing {len(missing)} key(s): {', '.join(missing)}")

        extra = sorted(set(strings) - reference_keys)
        if extra:
            problems.append(f"{name}: unknown key(s): {', '.join(extra)}")

        for key in sorted(reference_keys & set(strings)):
            value = strings[key]
            if not isinstance(value, str):
                problems.append(f"{name}: '{key}' is not a string")
                continue
            if not value.strip():
                problems.append(f"{name}: '{key}' is empty")
                continue
            expected = placeholders(reference[key])
            actual = placeholders(value)
            if expected != actual:
                problems.append(
                    f"{name}: '{key}' placeholder mismatch "
                    f"(expected {expected or ['none']}, got {actual or ['none']})"
                )

    for problem in problems:
        print(f"FAIL {problem}")

    if problems:
        print(f"\n{len(problems)} problem(s) found in {len(files)} translation file(s).")
        return 1

    print(f"OK: {len(files)} translation file(s) validated against template.json "
          f"({len(reference_keys)} keys each).")
    return 0


if __name__ == "__main__":
    sys.exit(main())
