# 题面准确性审计（Strict v2）

## 目标

本仓库的题面不是“能帮助做题即可”的摘要，而应准确保留官方题目中会影响以下内容的全部信息：

- 合法输入集合（长度、值域、字符集、排序性、唯一性等）；
- 返回值/输出契约；
- 算法适用前提（例如正数、有序、无重复、一定可达）；
- 特殊保证（唯一答案、可重复使用、不得修改输入、节点一定存在等）。

示例、公司标签和不影响解题契约的展示性文字可以省略，但不能因为代码“碰巧能工作”而省略上述语义。

## 官方基线

仓库保留两套官方来源：

1. `tools/official/lc<N>.txt`：LeetCode 英文官方题面快照，用于跨语言语义核对；
2. `tools/chinese_problem_info.json`：从 LeetCode CN `translatedContent` 抽取的中文题面/提示缓存，用于日常离线生成。

LC-252/253 为 Premium 题，继续使用仓库已经人工核对的经典题意和 `[start,end)` 本地约定。

## 本轮发现的根因

旧的 `docs/problem-review.md` 已经对 106 题做过官方快照复核，但当时主要核对的是 `gen_data.py`。
之后 learner-facing `solution.cpp` 改为直接从 `chinese_problem_info.json` 生成，形成了两个事实源：

```text
official snapshot -> gen_data.py -> 旧复核台账

LeetCode CN cache -> chinese_metadata.py -> solution.cpp
```

因此即使旧台账已经修过某个问题，刷新/切换中文缓存后仍可能回退。LC-704 就出现了这种情况：英文官方题面包含 `target` 值域、唯一元素和升序保证，而当前中文缓存使用了更简化的提示版本。

## Strict v2 事实源

现在生成链路改为：

```text
LeetCode CN cache (可刷新)
        |
        v
tools/statement_overrides.py
  - 跨中英文官方题面的严格语义补充
  - HTML/下标抽取缺陷修复
        |
        v
tools/statement_metadata.py
        |
        v
tools/gen_all.py -> problems/**/solution.cpp
```

`statement_overrides.py` 是持久化人工复核结论，刷新官方中文缓存不会覆盖它。

## 本轮全量复核结论

本轮以原有 106 题官方快照逐题复核结果为基线，重新核对“当前实际生成源”是否仍保留这些结论，并针对字符集/唯一性/排序性/可达性/输入值域等关键输入性质做严格检查。

发现并修正：

| LC | 类型 | 修正 |
|---:|---|---|
| 438 | 输入域精确性 | `s`、`p` 明确为仅包含**小写英文字母**；英文官方题面明确为 `lowercase English letters` |
| 704 | 实质语义回退 | 恢复英文官方的完整四项约束：长度、`nums[i]/target` 严格值域、元素唯一、严格升序 |
| 127 | 抽取缺陷 | 拆开 `s_k == endWord` 与返回要求，恢复清晰的转换序列定义 |
| 207 | 抽取缺陷 | 删除下标 HTML 转换产生的孤立 `_` |
| 236 | 抽取缺陷 | 修复最近公共祖先定义的断裂引号，并保留“节点可为自身祖先” |
| 406 | 抽取缺陷 | 删除 `k_i` 后的孤立 `_` |
| 455 | 抽取缺陷 | 删除 `g[i]` / `s[j]` 后的孤立 `_`，恢复正常句子 |
| 45 | 抽取缺陷 | 将合法跳转条件与“返回最少跳数”拆成独立句子 |

其余题目保持原有 106 题官方复核结论，没有发现需要新增语义 override 的项目。

## 自动门禁

新增 `tools/check_statement_fidelity.py`，对全部 106 个生成题进行离线检查：

- 题号/中文标题必须匹配有效 metadata；
- `题目描述` 必须完整包含有效 description；
- `约束与要求` 必须完整包含有效 constraints；
- 任一生成文件落后于 canonical metadata 都直接失败。

该检查已接入：

```bash
make verify-meta
```

同时 `tools/compare_official.py` 已改为比较**实际 learner-facing metadata**与 `tools/official/lc<N>.txt`，不再比较已经退出生成链路的旧 `gen_data.py` 题面摘要。

## 维护规则

以后刷新官方题面时：

```bash
python3 tools/fetch_chinese_problem_info.py
python3 tools/fetch_official.py --force
python3 tools/compare_official.py
python3 tools/gen_all.py
make verify-meta
```

刷新缓存后不得删除已有 `statement_overrides`，除非重新对照官方题面确认该 override 已经不再需要。
