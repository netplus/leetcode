"""Canonical Chinese titles for every problem in this workspace.

English titles remain in ``gen_data.py`` as source metadata.  Generated
learner-facing files use this registry so the Chinese naming stays consistent
across solution headers, the study plan, and the statement-review ledger.
"""

CHINESE_TITLES = {
    1: "两数之和",
    3: "无重复字符的最长子串",
    4: "寻找两个正序数组的中位数",
    5: "最长回文子串",
    11: "盛最多水的容器",
    14: "最长公共前缀",
    15: "三数之和",
    19: "删除链表的倒数第 N 个结点",
    20: "有效的括号",
    21: "合并两个有序链表",
    22: "括号生成",
    23: "合并 K 个升序链表",
    28: "找出字符串中第一个匹配项的下标",
    31: "下一个排列",
    33: "搜索旋转排序数组",
    34: "在排序数组中查找元素的第一个和最后一个位置",
    39: "组合总和",
    41: "缺失的第一个正数",
    42: "接雨水",
    45: "跳跃游戏 II",
    46: "全排列",
    53: "最大子数组和",
    55: "跳跃游戏",
    56: "合并区间",
    57: "插入区间",
    62: "不同路径",
    64: "最小路径和",
    70: "爬楼梯",
    76: "最小覆盖子串",
    78: "子集",
    79: "单词搜索",
    84: "柱状图中最大的矩形",
    92: "反转链表 II",
    94: "二叉树的中序遍历",
    98: "验证二叉搜索树",
    102: "二叉树的层序遍历",
    104: "二叉树的最大深度",
    105: "从前序与中序遍历序列构造二叉树",
    124: "二叉树中的最大路径和",
    127: "单词接龙",
    128: "最长连续序列",
    133: "克隆图",
    134: "加油站",
    139: "单词拆分",
    141: "环形链表",
    142: "环形链表 II",
    143: "重排链表",
    144: "二叉树的前序遍历",
    145: "二叉树的后序遍历",
    148: "排序链表",
    151: "反转字符串中的单词",
    152: "乘积最大子数组",
    153: "寻找旋转排序数组中的最小值",
    155: "最小栈",
    160: "相交链表",
    167: "两数之和 II - 输入有序数组",
    179: "最大数",
    198: "打家劫舍",
    199: "二叉树的右视图",
    200: "岛屿数量",
    206: "反转链表",
    207: "课程表",
    209: "长度最小的子数组",
    210: "课程表 II",
    213: "打家劫舍 II",
    215: "数组中的第 K 个最大元素",
    226: "翻转二叉树",
    234: "回文链表",
    236: "二叉树的最近公共祖先",
    238: "除自身以外数组的乘积",
    239: "滑动窗口最大值",
    252: "会议室",
    253: "会议室 II",
    283: "移动零",
    295: "数据流的中位数",
    300: "最长递增子序列",
    303: "区域和检索 - 数组不可变",
    304: "二维区域和检索 - 矩阵不可变",
    312: "戳气球",
    322: "零钱兑换",
    347: "前 K 个高频元素",
    406: "根据身高重建队列",
    410: "分割数组的最大值",
    416: "分割等和子集",
    417: "太平洋大西洋水流问题",
    438: "找到字符串中所有字母异位词",
    455: "分发饼干",
    494: "目标和",
    542: "01 矩阵",
    543: "二叉树的直径",
    547: "省份数量",
    560: "和为 K 的子数组",
    621: "任务调度器",
    684: "冗余连接",
    685: "冗余连接 II",
    695: "岛屿的最大面积",
    704: "二分查找",
    739: "每日温度",
    746: "使用最小花费爬楼梯",
    875: "爱吃香蕉的珂珂",
    990: "等式方程的可满足性",
    994: "腐烂的橘子",
    1011: "在 D 天内送达包裹的能力",
    1109: "航班预订统计",
    1143: "最长公共子序列",
    1248: "统计「优美子数组」",
}


def get_chinese_title(num: int) -> str:
    """Return the canonical learner-facing Chinese title for an LC number."""
    return CHINESE_TITLES[num]


def validate_title_coverage(problems: list[dict]) -> None:
    """Fail if the problem catalogue and Chinese-title registry diverge."""
    expected = {p["num"] for p in problems}
    actual = set(CHINESE_TITLES)
    missing = sorted(expected - actual)
    extra = sorted(actual - expected)
    if missing or extra:
        raise RuntimeError(
            f"Chinese-title coverage mismatch: missing={missing}, extra={extra}"
        )
