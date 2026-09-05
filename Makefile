# Makefile - LeetCode exam practice workspace (C/C++)
# Usage:
#   make lc1            build & run LC-1 (feeds its test.in, prints stdin + stdout)
#   make w1d1           build & run the single problem for week1/day1
#   make w2d10-3        build & run the 3rd problem on week2/day10
#   make all            compile every solution.{cpp,c}
#   make list           list all compilable targets
#   make clean          remove build artifacts
#
# All convenience targets just dispatch to tools/run.sh, which does the
# directory lookup and execution. No GNU make foreach/eval (kept simple + robust).

CXX      := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra
CC       := gcc
CFLAGS   := -std=c11 -O2 -Wall -Wextra

# Discover all solutions under problems/
SRCS := $(shell find problems -name 'solution.cpp' -o -name 'solution.c' 2>/dev/null)
BINS := $(patsubst %.cpp,%,$(patsubst %.c,%,$(SRCS)))

.PHONY: all clean list help pch verify-meta verify
all: $(BINS)

verify-meta:
	@python3 tools/gen_review_report.py --check
	@python3 tools/sync_plan_titles.py --check
	@python3 tools/check_plan_counts.py
	@python3 tools/check_learning_metadata.py
	@python3 tools/check_statement_fidelity.py
	@python3 tools/check_generated_fidelity.py

verify: verify-meta all
	@$(MAKE) --no-print-directory judge-all

pch:
	@mkdir -p .build
	@echo '#include <bits/stdc++.h>' > .build/all.h
	@g++ -std=c++17 -O2 -x c++-header .build/all.h -o .build/all.h.gch
	@echo "Built precompiled header (.build/all.h.gch) — builds/judges now faster."

# Generic compile rules.
%: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

%: %.c
	$(CC) $(CFLAGS) $< -o $@

list:
	@echo "Compilable solution targets:"
	@for b in $(BINS); do echo "  make $$b"; done
	@echo ""
	@echo "Convenience targets:"
	@echo "  make lc<NUM>      e.g. make lc1"
	@echo "  make w<WK>d<DY>   e.g. make w1d1, or w2d10-3 (i-th problem on a day)"

clean:
	rm -f $(BINS) /tmp/run_w*d* /tmp/run_lc*

help:
	@echo "Usage:"
	@echo "  make lc1            build & run LC-1 (feeds test.in)"
	@echo "  make w1d1           build & run week1/day1's problem"
	@echo "  make w2d10-3        build & run the 3rd problem on week2/day10"
	@echo "  make all            compile every solution"
	@echo "  make list           list all targets"
	@echo ""
	@echo "  Batch (judge all cases per problem):"
	@echo "  make judge-all       judge every problem, roll-up summary"
	@echo "  make judge-w1        judge all of Week 1 (judge-w2..judge-w4 analogous)"
	@echo "  make judge-d1        judge Day 1 (judge-d2..judge-d28 analogous)"
	@echo "  make status          which problems are implemented vs still stub"
	@echo "  make verify          canonical audit + metadata + compile + all judges"
	@echo "  make clean           clean"

# ---------------------------------------------------------------------------
# Convenience targets (lc<NUM> and w<N>d<M>[-<i>]) generated into this file by
# tools/gen_targets.py so they are plain static rules — no foreach/eval magic.
# Regenerate after adding problems:  python3 tools/gen_targets.py >> /dev/null
# (the rules below are written by that script into tools/_targets.mk and included)
-include tools/_targets.mk
