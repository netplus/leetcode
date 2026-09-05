# LeetCode Wiki / doocs 题面基线与校验

本仓库把 `https://leetcode.doocs.org/lc/<num>/` 的中文题面作为学习者可见题目事实的基线。

## 哪些内容属于题目事实

`solution.cpp` 头部的以下四类内容必须与已审核的 doocs baseline 一致：

1. `title`：中文题名；
2. `description`：题目描述、返回契约以及会影响算法判断的边界语义；
3. `examples`：题面 Input / Output / Explanation；
4. `constraints`：输入范围、字符集、数值范围等约束。

算法讲解、图像直觉、公式、不变量、复杂度推导和实现注释属于 pedagogy 层，可以为了学习效果重写；但不能反向修改上述题目事实。

## Canonical 数据层

题面不是直接从每个 `solution.cpp` 维护，而是按下面的优先级生成：

```text
reviewed doocs override
        ↓
Chinese metadata / examples cache
        ↓
offline official snapshot fallback
        ↓
gen_all.py 渲染 solution.cpp
```

相关文件：

- `tools/statement_metadata.py`：组装最终题面 metadata；
- `tools/statement_overrides.py`：保存已经人工核对的 doocs 差异；
- `tools/statement_examples.py`：题面示例及 reviewed example overrides；
- `tools/chinese_problem_info.json`：可刷新的中文题面缓存；
- `tools/gen_all.py`：把 canonical metadata 渲染到各题 `solution.cpp`。

因此发现题面不一致时，不应只手工编辑生成后的 `solution.cpp`。应先修 canonical metadata / override，再重新生成或同步对应文件。

## 本地门禁

```bash
make verify-doocs
```

该命令是**离线、非修改性**检查：逐题读取 canonical metadata，并精确比较所有正式题 `solution.cpp` 中的 title / description / examples / constraints 区块。

它会发现：

- 手工修改生成文件导致的题面漂移；
- canonical 内容已经更新、但 `solution.cpp` 没有同步；
- 旧题面残留在区块中，即使新的 canonical 文本也同时存在；
- 题面 section marker 被删除或破坏；
- 生成文件 EOF 出现额外空行。

`make verify-meta` 依赖 `verify-doocs`，所以完整 metadata 校验自动包含这道门禁。

## 为什么校验不直接联网

`verify-doocs` 不在每次构建时请求网站。原因是 CI / 本地校验需要可重复、稳定，并且网站内容变化必须经过人工确认，不能自动改变学习题库的题意。

因此区分两个动作：

```text
网站发生变化 / 发现差异
        ↓
人工核对 doocs 当前题面
        ↓
更新 cache 或 reviewed override
        ↓
同步生成文件
        ↓
make verify-doocs
```

这里的 `reviewed doocs baseline` 是经过核对后写入仓库的事实基线；`verify-doocs` 负责防止仓库内部再次偏离它。

## Review 原则

核对题面时优先关注会改变解题契约的差异，而不是机械追求逐字一致：

- 返回值到底是什么；
- 是否允许空输入；
- 区间端点是否算重叠；
- 重复元素是否允许；
- 字符集和数值范围；
- 官方示例及其边界行为；
- 中文页与英文页约束不一致时，本仓库 learner-facing baseline 采用 doocs 中文页。

文字可以做轻微排版或可读性规范化，但不能强化、削弱或改变题意。
