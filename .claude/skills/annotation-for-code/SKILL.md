---
name: annotation-for-code
description: 当用户要求代码添加注释时，或者claude自动添加函数声明时，请参考此规范；
---

# 指令

1. 首先，所有注释类型采用英文注释；
2. 给函数/类添加注释时，采用 Doxygen 风格注释；
3. 单行注释，不做风格要求；

## 参考示例

``` C++
/**
  * @brief Finds the length of the longest subsequence that sums to target.
  *
  * Uses 0-1 knapsack DP. dp[i][t] = length of longest subsequence from first i
  * elements with sum t, or -1 if unreachable.
  *
  * @param nums Input vector of positive integers.
  * @param target Target sum to achieve.
  * @return Length of longest subsequence with sum equal to target, or -1 if none exists.
  */
int lengthOfLongestSubsequence(vector<int>& nums, int target) {
    int n = nums.size();

    vector dp(n + 1, vector<int>(target + 1, -1));
    dp[0][0] = 0;

    for (int i = 0; i < n; ++i) {
        for (int t = 0; t <= target; ++t) {
            dp[i + 1][t] = dp[i][t];
            if (t >= nums[i]) {
                int pre = dp[i][t - nums[i]];
                if (pre > -1)
                    dp[i + 1][t] = std::max(dp[i + 1][t], pre + 1);
            }
        }
    }

    return dp[n][target];
}
```
