# Progress Tracker

> 当前训练入口：[题型掌握训练计划](TRAINING.md)。新版个人状态见文末[题型掌握度记录](#题型掌握度记录)。
> 下方原四周 Dashboard、Daily Log 与模考表保留为历史模板，其中空白和 0 不表示当前实际掌握率。新训练不补造历史日期或成绩。

> Fill in one row at the end of each day. Be honest about getting stuck and running over time — wrong problems are worth more than ACs.

## Legend
- ✅ AC within 50 min
- ⏱ AC but over time (idea right, implementation slow)
- ❌ Did not solve / read the answer
- 🔁 Second pass done
- 📝 Review note written (see `notes/`)

---

## Dashboard

| Week | Topic | Planned P0 | Done P0 | Planned total | Done | Rate | Status |
|------|-------|------------|---------|----------------|------|------|--------|
| W1 | Arrays / Strings / Hashing / 2-Ptr | 18 | 0 | 28 | 0 | 0% | In progress |
| W2 | Lists / Trees / BFS-DFS / Stack | 23 | 0 | 30 | 0 | 0% | Not started |
| W3 | Graphs / Union-Find / DP | 20 | 0 | 28 | 0 | 0% | Not started |
| W4 | Binary Search / Greedy / Heap / Mock | 14 | 0 | 22 | 0 | 0% | Not started |
| **Total** | | **75** | **0** | **108** | **0** | **0%** | |

> `Planned P0` counts only the 106 formal LeetCode problems. `Planned total` additionally includes the 2 Week 4 mock-exam units (Day 27/28), so the overall plan has 108 units.
>
> Plan start: 2026-07-07   Plan end: 2026-08-03 (4 weeks)
> Actual end: __________

---

## Wrong-Problem List (priority for second pass)

> Log only ❌ and ⏱ problems. Review this page before the exam.

| Date | W-Day | LC# | 中文标题 | Status | Stuck point | Pattern | 2nd pass |
|------|-------|-----|-------|--------|-------------|---------|----------|
| | | | | | | | |

---

## Daily Log

### Week 1 · Arrays / Strings / Hashing / Two Pointers

| Day | Date | Done (LC#) | Time | P0 done | Note / review |
|-----|------|-----------|------|---------|----------------|
| D1 | 7/7 | | | /3 | hashing + prefix sum |
| D2 | | | | /3 | two pointers |
| D3 | | | | /3 | sliding window |
| D4 | | | | /3 | prefix sum / difference |
| D5 | | | | /2 | strings |
| D6 | | | | /1 | intervals |
| D7 | | | | /3 | week review |

### Week 2 · Lists / Trees / BFS-DFS / Stacks

| Day | Date | Done (LC#) | Time | P0 done | Note / review |
|-----|------|-----------|------|---------|----------------|
| D8  | | | | /3 | list basics |
| D9  | | | | /3 | list advanced |
| D10 | | | | /6 | tree traversal |
| D11 | | | | /3 | tree properties |
| D12 | | | | /3 | stack / monotonic stack |
| D13 | | | | /3 | backtracking |
| D14 | | | | /2 | week review |

### Week 3 · Graphs / Union-Find / DP

| Day | Date | Done (LC#) | Time | P0 done | Note / review |
|-----|------|-----------|------|---------|----------------|
| D15 | | | | /3 | graph traversal |
| D16 | | | | /2 | BFS shortest path |
| D17 | | | | /2 | union-find |
| D18 | | | | /3 | DP intro |
| D19 | | | | /4 | DP paths / subsequences |
| D20 | | | | /3 | DP knapsack |
| D21 | | | | /3 | week review |

### Week 4 · Binary Search / Greedy / Heap / Mock

| Day | Date | Done (LC#) | Time | P0 done | Note / review |
|-----|------|-----------|------|---------|----------------|
| D22 | | | | /3 | binary search |
| D23 | | | | /3 | binary search on answer |
| D24 | | | | /3 | greedy |
| D25 | | | | /3 | heap / topK |
| D26 | | | | /2 | comprehensive |
| D27 | | | | — | mock exam 1 (90 min) |
| D28 | | | | — | mock exam 2 + final review |

---

## Mock Exam Record

| # | Date | Time | Problems | AC | Main loss |
|---|------|------|----------|----|-----------|
| Mock 1 (D27) | | | 3 | /3 | |
| Mock 2 (D28) | | | 3 | /3 | |

---

## Weekly Review (fill each Sunday)

**Week 1 (D7)**
- Hardest problem this week:
- Pattern still not mastered:
- Reminder to carry into next week:

**Week 2 (D14)**
- Hardest problem this week:
- Pattern still not mastered:
- Reminder to carry into next week:

**Week 3 (D21)**
- Hardest problem this week:
- Pattern still not mastered:
- Reminder to carry into next week:

**Week 4 (D28)**
- Hardest problem this week:
- Last things to cram before the exam:
- Overall confidence (1–5):

---

## 题型掌握度记录

启用日期：2026-09-27。以下是等待诊断的初始状态，不是对既往学习的否定。旧 Dashboard 的 Done 与参考实现的 DONE 都不能替代这里的个人证据。

### 状态与提示

| 状态 | 含义 |
| --- | --- |
| 待诊断 | 尚无足够的本轮个人作答证据 |
| 能解释 | 能讲清直接解法、状态或决策、正确性与边界 |
| 原题独立 | 无解法提示，独立实现母题并通过明确标注的验证 |
| 变式独立 | 改变关键条件后仍能独立建模与实现 |
| 保持与限时 | 延迟复测仍能完成，并达到提前约定的计时目标 |

提示等级：H0＝只给题面，未给解法提示；H1＝反例或方向提示；H2＝关键状态或转移提示；H3＝已看完整解答。记录本次最高提示等级，并注明题意澄清与算法提示的区别。

已见过的题即使本次为 H0，也不能算未见变式。对话思路正确不能自动标为代码通过。耗时由学习者计时或可验证日志提供；未知就写“未计时”，不能由聊天间隔推算。

### 题型矩阵

| 题型 | 当前状态 | 母题证据 | 变式证据 | 延迟复测 |
| --- | --- | --- | --- | --- |
| 背包与计数 | 待诊断 | — | — | — |
| 回溯与字符串分段 | 待诊断 | — | — | — |
| 可达性与最少步数 | 待诊断 | — | — | — |
| 前缀和、哈希与连续区间 | 待诊断 | — | — | — |
| 双指针与滑动窗口 | 待诊断 | — | — | — |
| 二分与答案二分 | 待诊断 | — | — | — |
| 其他动态规划 | 待诊断 | — | — | — |
| 贪心与区间 | 待诊断 | — | — | — |
| 栈、单调结构与堆 | 待诊断 | — | — | — |
| 链表与二叉树 | 待诊断 | — | — | — |
| 图、拓扑与并查集 | 待诊断 | — | — | — |
| 字符串解析与矩阵模拟 | 待诊断 | — | — | — |
| 位运算、基础数论与 Trie | 待诊断 | — | — | — |

### 作答证据台账

尚未收到本轮作答，因此不预填题目成绩。每题保留第一次作答，修正版本另存。

| 日期 | 题目标识 | 原题/变式/复测 | 是否已见 | 提示 | 用时 | 结论及验证方式 | 证据 |
| --- | --- | --- | --- | --- | --- | --- | --- |

验证方式必须注明：口述审阅、代码静态审阅、本地测试执行、平台 AC 或学习者自述。没有执行就不写“测试通过”。证据链接指向 [attempts/](attempts/README.md) 的原始作答或 [notes/](notes/README.md) 的复盘记录。

### 本轮工作状态

- 训练计划与记录入口：已在本学习分支建立，是否合入以 PR 状态为准。
- 6 道建模诊断、3 道编码诊断：待作答，不计为完成。
- 新增 LC-377、40、47、131：待对应专题推进后逐题建设，本次没有新增题解。
- LC-518 边界修复与补充题统一校验：后续独立维护项，本次未修改。
