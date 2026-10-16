"""Assemble reviewed manual reasoning corrections without changing callers.

The previous module is preserved byte-for-byte as pedagogy_reasoning_manual_base.
New batches are isolated so their explanation changes can be reviewed separately.
"""

from pedagogy_reasoning_manual_base import (
    MANUAL_REASONING_OVERRIDES as BASE_OVERRIDES,
    guide,
)
from pedagogy_depth_batch01 import DEPTH_BATCH01_OVERRIDES

_overlap = set(BASE_OVERRIDES) & set(DEPTH_BATCH01_OVERRIDES)
if _overlap:
    raise RuntimeError(f"duplicate manual reasoning overrides: {sorted(_overlap)}")

MANUAL_REASONING_OVERRIDES = {**BASE_OVERRIDES, **DEPTH_BATCH01_OVERRIDES}
