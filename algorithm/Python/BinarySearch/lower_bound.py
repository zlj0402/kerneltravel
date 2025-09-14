"""
@brief: 以 闭区间，半开半闭，开区间 三种方式，实现 lower_bound
@reference: https://www.bilibili.com/video/BV1AP41137w7?vd_source=d5cc8e9f24da95fe33209d4f0bee23c3&spm_id_from=333.788.videopod.sections
@date: 14/9/2025

@example:
    + Q: 返回有序数组中第一个 ≥ 8 的数的位置，如果所有数 < 8，返回数组长度；

@detail:
    + 三种实现方式
        + [L, R]
        + [L, R)，这里没有实现 (L, R]
        + (L, R)

@postscript:
    + lower_bound 求解的是 >= target 的问题；
    + 当解题条件中，带着整数数组，这样的字眼（默认的非递减）
    + 利用这一整数条件，lower_bound 也可以用于 > < <= 的求解；
        + > target: 转换为 lower_bound(nums, target + 1)，求 >= target + 1, 即等价于 > target；
        + < target: 转换为 lower_bound(nums, target) - 1
        + <= target: > target 求取 idx 之后，idx - 1
"""

from typing import List


# 要求 nums 是非递减的，即 nums[i] <= nums[i + 1]
# 返回最小的满足 nums[i] >= target 的 i
# 如果不存在，返回 len(nums)
# [L, R]
def lower_bound(nums: List[int], target: int) -> int:
    left = 0
    right = len(nums) - 1   # 闭区间 [left, right]
    while left <= right:    # 区间不为空
        mid = left + (right - left) // 2
        if nums[mid] < target:
            left = mid + 1  # [mid + 1, right]
        else:
            right = mid - 1 # [left, mid - 1]
    return left

# [L, R)
def lower_bound2(nums: List[int], target: int) -> int:
    left = 0
    right = len(nums)   # 左闭右开区间 [left, right)
    while left < right: # 区间不为空
        mid = left + (right - left) // 2
        if nums[mid] < target:
            left = mid + 1  # [mid + 1, right)
        else:
            right = mid # [left, mid)
    return left # right

# (L, R)
def lower_bound3(nums: List[int], target: int) -> int:
    left = -1
    right = len(nums)   # 开区间 (left, right)
    while left + 1 < right: # 区间不为空
        mid = left + (right - left) // 2
        if nums[mid] < target:
            left = mid  # (mid, right)
        else:
            right = mid # (left, mid)
    return right