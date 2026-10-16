# 题解深化首批：LC300 / LC39

日期：2026-10-16。总控：Issue #4。基线：`282195744fec06b024b89d2eae2730866ffcaee2`。
状态：**改写和局部验证完成，整仓门禁未执行，尚未合并**。

## 实际修改

两题均重写 `visual`、`general_solution`、`limitations`、`generalization`、`core`、`transfer`。
没有替换 C++ 算法，没有更改题面、签名、输入输出适配器，也没有修改个人训练进度。

LC300 的主要因果链是：

```text
全部子序列 → 同一结尾只保留最长 → O(n²) 结尾 DP
→ 同一长度保留最小结尾 → 证明最小结尾有序
→ 先线性定位替换档位 → 再以二分降低定位成本
```

新增 `[3,5,6,2,4]` 反例：最终 `tails=[2,4,6]`，但不是原数组子序列。
新增“只记最长长度”的信息不足例，以及严格递增/不下降、恢复路径、计数变式。

LC39 的主要因果链是：

```text
明确组合身份 → 完整枚举选择序列 → 路径副本排序并用集合查重
→ 发现生成重复表示的开销 → 固定下标不减，让每个组合只有一条生成路径
→ 引入 start → 分别解释可重复使用与排序后 break
```

区分数值排序和固定下标顺序的作用。新增相同剩余金额、不同可选后缀的例子，
以及误传 `i+1`、未排序就 `break` 的可执行反例。

## 源数据接入

原 manual 模块作为 `pedagogy_reasoning_manual_base.py` 原样保留，Git blob 为
`701103715843ad128e3172d74c05ba4e379690e9`。原导入路径保留为聚合入口，仍导出
`MANUAL_REASONING_OVERRIDES` 和 `guide`；新增模块只补入两题，重号即报错。

`refined_data.py` 与 `gen_all.py` 没有改动。两题生成注释采用已有文本块格式做局部重渲染，
本轮没有运行完整 `gen_all.py`，不把局部段落一致视为全量 canonical 合并已经通过。

## 实际执行证据

原文件经 Git blob SHA 核对后用于本地工作副本：

| 文件 | 原 Git blob SHA |
|---|---|
| LC300 solution.cpp | `a5d58f87b89dcd0329f26da7718e889ba35b29bc` |
| LC39 solution.cpp | `3d95aba4e6635627c981915decaab413195e524b` |

重渲染时逐字节核对：题解之前的题面保持不变，从“本地输入输出格式”到文件末尾保持不变。
这包括原 C++ 实现、代码注释、完整 main 和输入输出行为。

运行环境：Python 3、`g++ (Debian 14.2.0-19) 14.2.0`；C++ 使用 `-std=c++14 -O2 -Wall -Wextra`。

执行命令：

```sh
python3 -m py_compile tools/pedagogy_depth_batch01.py tools/pedagogy_reasoning_manual.py
python3 tools/check_pedagogy_depth_batch01.py --cpp
```

实际输出：

```text
PASS Python differential cases: LC300=3285, LC39=748
PASS explanatory examples and 3 deliberately wrong variants rejected
PASS 12 patch/generated sections (focused check, not full canonical merge)
PASS C++14 LC300: 3 adapter examples + 3285 differential cases
PASS C++14 LC39: 3 adapter examples + 748 differential cases
NOT RUN by this checker: full canonical merge, verify-meta, all-repository judges
```

测试域与独立依据：

- LC300：穷举长度 1..7、值来自 {-1,0,1} 的数组，再加 6 个指定输入；
  用全部子序列枚举作为答案依据，与二维循环 DP、尾值方法及仓库 C++ 实现对照。
- LC39：候选取 {2,3,4,5,6} 的所有非空子集，分别正序和逆序，目标 1..12，
  再加 4 个指定输入；以“每个候选的使用次数”独立枚举为依据，与完整序列枚举后去重、
  固定下标回溯及仓库 C++ 实现对照，额外检查输出重复。
- 同一 C++ `Solution` 对象复用多个输入，检查 LC39 成员状态的清理行为。
- 两题原 main 分别执行 3 个题面样例。C++ 对照使用实际 solution.cpp 中的类，
  只在批量测试包装中重命名原 main，没有另写一份冒充仓库实现。
- 3 个故意错误变式：LIS 用非严格边界；LC39 错传 `i+1`；未排序而直接 `break`。
  反例要求错误变式与原题正确答案不同，防止“测试总能通过”却没有区分力。

测试域是有限的。通过对照测试不替代一般正确性证明，也不代表全输入域或整仓都已验收。

## 未执行和未改变的内容

本轮没有完整 Git 工作副本，未运行：

```sh
python3 tools/gen_all.py
python3 tools/check_generated_fidelity.py
make verify-meta
make verify
```

因此导入全量 canonical 层、全仓生成输出和其它题目的编译/判题均不在本轮通过结论内。
首批 PR 应保持 Draft，补齐这些门禁与 PR diff review 后才能合并。

本轮没有重新核验官方远端题面；仅保持已读取基线的题面字节不变。
LC39 现有题头复杂度标签沿用基线，未在本批重新审定其紧确性；后续门禁复核时应连同
源数据中的复杂度描述单独核查，不能用本次教学测试替代复杂度证明。

P2 的下一题为 LC45；P3 的 LC518 边界风险仍未在本批处理。
