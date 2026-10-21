---
name: wmbus-operation-modes
description: EN 13757-4 六种无线 M-Bus 操作模式(S/T/R/C/N/F)的详细参数对比，包括频率、编码、速率、响应延迟等
metadata: 
  node_type: memory
  type: reference
  originSessionId: 2ca42aff-aa3e-4575-bd3b-6396bb2d3bee
  modified: 2026-09-24T03:55:30.883Z
---

# EN 13757-4 操作模式详解

## 六种模式总览

| 模式 | 名称 | 频段 | 方向 |
|------|------|------|------|
| S | Stationary | 868 MHz | 单向/双向 |
| T | Frequent Transmit | 868 MHz | 单向/双向 |
| R | Frequent Receive | 868 MHz | 双向(仅R2) |
| C | Compact | 868/869 MHz | 单向/双向 |
| N | Narrowband VHF | 169 MHz | 单向/双向 |
| F | Frequent Rx+Tx | 433 MHz | 双向 |

## 子模式命名规则

- 字母 = 模式类型
- 数字 1 = 单向, 2 = 双向
- 后缀 "-m" = 移动接收设备

## 模式 S — Stationary Mode（868 MHz）

### S1 (单向)
- 频段: 868.0~868.6 MHz, 中心 868.3 MHz
- 编码: Manchester, chip rate = 32,768 kcps
- 长前导码(576 chips), 优化用于电池供电固定接收设备
- 占空比: ≤0.02%, 每天发送数次
- 数据率: 16,384 bps (Manchester 1bit→2chips)
- FSK 频偏: +40~+80 kHz (典型 50 kHz)

### S1-m (单向, 移动接收)
- 同 S1 但短前导码(48 chips)
- 每小时占空比限制 0.02%

### S2 (双向)
- 同 S1 频率, 短前导码(48 chips), 占空比≤1%
- 连续启用或同步接收器
- 响应延迟(O→M): 3~15 ms
- FAC 超时: 25~30 s
- FAC 发送延迟: 1000×N±0.5 ms (N=2,3,5)

### 接收器灵敏度
- HR: -100 ~ -105 dBm (BER<10⁻² 或 PER<0.8)
- LR: -80 dBm, MR: -90 dBm

---

## 模式 T — Frequent Transmit Mode（868 MHz）

### T1 (单向)
- M→O 频段: 868.7~869.2 MHz, 中心 868.95 MHz
- 编码: 3-out-of-6, chip rate = 100 kcps (90~110 kcps)
- 数据率: ~66.7 kbps (f_chip × 2/3)
- 占空比: ≤0.1%, 每几秒发送 3~8 ms 短帧
- 适用于 walk-by / drive-by 抄表

### T2 (双向)
- M→O: 同 T1 (3-out-of-6, 100 kcps)
- O→M: Manchester, 32,768 kcps, 频段 868.0~868.6 MHz
- 响应延迟(O→M): 2~3 ms
- 接收器在发送后短暂启用并锁定

### 3-out-of-6 编码表
每个 4-bit nibble 编码为 6-bit word（0和1各3个，最少2次跳变）：

| NRZ(hex) | 6-bit code | 跳变数 |
|----------|------------|--------|
| 0 | 010110 | 4 |
| 1 | 001101 | 3 |
| 2 | 001110 | 2 |
| 3 | 001011 | 3 |
| 4 | 011100 | 2 |
| 5 | 011001 | 3 |
| 6 | 011010 | 2 |
| 7 | 010011 | 3 |
| 8 | 101100 | 3 |
| 9 | 100101 | 4 |
| A | 100110 | 3 |
| B | 100011 | 2 |
| C | 110100 | 3 |
| D | 110001 | 2 |
| E | 110010 | 3 |
| F | 101001 | 4 |

传输顺序: MSB优先, MSN优先, 字节内MSB优先

### T 模式前导码
序列: `n × (01) 0000111101`, n ≥ 19
- `0101010101b` = capture detect（正常编码中不会出现）
- `00001111b` 和 `11110000b` 在 3-out-of-6 中不会出现 → 用于同步

---

## 模式 R — Frequent Receive Mode（868 MHz，仅 R2）

- 频段: 868.0~868.6 MHz, 信道间隔 60 kHz
- 表计中心频率: 868.030 + n×0.06 MHz (多信道)
- 编码: Manchester, chip rate = 4.8 kcps
- Other Device 中心频率: 868.33 MHz
- 灵敏度(HR): -105 ~ -110 dBm
- 响应延迟(O→M, CI=81h): 3~50 ms
- 响应延迟(M→O): 10~10000 ms
- FAC 发送延迟: 1000×N±0.5 ms (N=5,7,13)
- 前导码: n ≥ 39

---

## 模式 C — Compact Mode（868/869 MHz）

### C1 (单向)
- M→O: 868.7~869.2 MHz, 中心 868.95 MHz
- 编码: NRZ (FSK), chip rate = 100 kcps
- 数据率: 100 kbps (NRZ, 1bit=1chip)
- <22 ms 短数据突发

### C2 (双向)
- M→O: 同 C1 (NRZ FSK, 100 kcps)
- O→M: 869.4~869.65 MHz, 中心 869.525 MHz, NRZ (GFSK), 50 kcps
- FSK 频偏(M→O): ±45 kHz (范围 75%~125%)
- GFSK 频偏(O→M): +25 kHz, BT=0.5
- 快速响应延迟: 100 ms (99.5~100.5)
- 慢速响应延迟: 1000 ms (999.5~1000.5)
- Meter 响应延迟: 同 100 ms / 1000 ms
- 支持 Format A 和 Format B

### C 模式前导码（关键设计：与 T 模式兼容）
前16个chips与T模式前导码+同步字相同，使一个解码器能同时解码T和C模式：
- Format A: `n×(01) 0101010000111101 01010100 11001101` (n=16)
- Format B: `n×(01) 0101010000111101 01010100 00111101` (n=16)
- 后续6个chips = `010101b` → Mode C（T模式的3-out-of-6中不会出现）
- 最后8个chips决定帧格式: `11001101b`=A, `00111101b`=B

### 重复帧机制
Other Device 使用短 85ms 响应延迟（tRR 或 tRR_slow 减 85ms），Meter 帧重复延迟 < 5 ms

---

## 模式 N — Narrowband VHF（169 MHz）

### 子带划分

| 子带 | 频段 | 信道间隔 | 功率 | 占空比 |
|------|------|----------|------|--------|
| A | 169.400~169.475 MHz | 25/50 kHz | 500 mW | 10% |
| B | 169.475~169.4875 MHz | 12.5 kHz | 10 mW | 0.1% |
| C | 169.4875~169.5875 MHz | 12.5/25/50 kHz | 10 mW | 0.001%(夜间0.1%) |
| D | 169.5875~169.8125 MHz | 12.5 kHz | 10 mW | 0.1% |

### 数据速率与调制

| 信道索引 | 子带 | 中心频率 | 间隔 | GFSK速率 | 4GFSK速率 |
|---------|------|---------|------|---------|----------|
| 1 | A | 169.40625+n×0.0125 | 25kHz | 2.4 kbps | — |
| 2 | A | 同上 | 25kHz | 4.8 kbps | — |
| 3 | A | 同上 | 25kHz | 6.4 kbps | — |
| 4 | A | 169.4375 | 50kHz | — | 19.2 kbps |
| 5 | B | 169.48125 | 25kHz | 2.4 kbps | — |
| 6 | B | 同上 | 25kHz | 4.8 kbps | — |
| 7 | C | 169.49375+n×0.0125 | 12.5kHz | 2.4 kbps | — |
| 8 | C | 同上 | 12.5kHz | 4.8 kbps | — |
| 9 | C | 同上 | 12.5kHz | 6.4 kbps | — |
| 10 | D | 169.59375+n×0.0125 | 12.5kHz | 2.4 kbps | — |
| 11 | D | 同上 | 12.5kHz | 4.8 kbps | — |
| 12 | D | 同上 | 12.5kHz | 6.4 kbps | — |
| 13 | D | 169.625+n×0.05 | 50kHz | — | 19.2 kbps |

### GFSK 调制参数
- 相对带宽 BT = 0.5
- 2.4 kbps: 频偏 +2.4 kHz (mod.index 2.0)
- 4.8 kbps: 频偏 +2.4 kHz (mod.index 1.0)

### 4GFSK 调制参数
- 6.4 kbps: 4频偏 +3.2/+2.24/-1.06 (mod.index 1.0)
- 19.2 kbps: 4频偏 +7.2/+5.04/-2.4/-9.36 (mod.index 0.5)
- 2-bit符号映射: 最低频→"01"(A), 第二→"00"(B), 第三→"10"(C), 最高→"11"(D)

### 接收器灵敏度

| 调制 | 数据率 | HR(min/typ) |
|------|--------|-------------|
| GFSK | 2.4 kbps | -115/-123 dBm |
| GFSK | 4.8 kbps | -112/-120 dBm |
| 4GFSK | 6.4 kbps | -109/-112 dBm |
| 4GFSK | 19.2 kbps | -104/-107 dBm |

### 响应延迟
- 快速(O→M): 100 ms
- 慢速: 4.8kbps=1100ms, 6.4kbps=2100ms, 19.2kbps=1100ms
- 扩展(O→M): 5000 ms（Other Device 有时间执行密码运算）

---

## 模式 F — Frequent Rx+Tx（433 MHz）

- 频段: 433.05~434.79 MHz, 中心 433.82 MHz
- 占空比: ≤10%
- 编码: NRZ (FSK), 数据率 = 2.4 kcps
- FSK 频偏: +25.5 kHz (75%~125%)
- 响应延迟(M→O): 8~50000 ms
- 快速响应(O→M): 100 ms, 慢速: 1000 ms
- 灵敏度(HR): -115 ~ -117 dBm
- 前导码: n ≥ 39

---

## 模式参数汇总对比

| 参数 | S | T | R | C | N | F |
|------|---|---|---|---|---|---|
| 频段 | 868.0-868.6 | M:868.7-869.2 / O:868.0-868.6 | 868.0-868.6 | M:868.7-869.2 / O:869.4-869.65 | 169.4-169.8 | 433.05-434.79 |
| M→O 编码 | Manchester | 3-out-of-6 | Manchester | NRZ(FSK) | NRZ(GFSK/4GFSK) | NRZ(FSK) |
| O→M 编码 | Manchester | Manchester | Manchester | NRZ(GFSK) | NRZ(GFSK/4GFSK) | NRZ(FSK) |
| Chip/Data Rate | 32.768k | M:100k / O:32.768k | 4.8k | M:100k / O:50k | 2.4~19.2k | 2.4k |
| 快速响应延迟 | 3-15ms | 2-3ms | 3-50ms | 100ms | 100ms | 100ms |
| FAC超时 | 25-30s | 25-30s | 25-30s | 25-30s | 25-30s | 25-30s |

## 性能等级

### 发射器功率

| 等级 | 说明 | S/T/C | R | N(Sub-A) | F |
|------|------|-------|---|----------|---|
| Lp | 最低 | -5dBm | -5dBm | 0dBm | -5dBm |
| Mp | 中等 | 0dBm | 0dBm | 10dBm | 0dBm |
| Hp | 最高 | +5dBm | +5dBm | +20dBm | +3dBm |

### 接收器灵敏度

| 等级 | S/T/C | R | N | F |
|------|-------|---|---|---|
| LR | -80dBm | -80dBm | -90dBm | -105dBm |
| MR | -90dBm | -90dBm | -100dBm | -110dBm |
| HR | 见各模式表 | 见各模式表 | 见各模式表 | 见各模式表 |
