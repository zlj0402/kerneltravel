---
name: wmbus-data-link-layer
description: EN 13757-4 数据链路层详解，包括帧格式A/B、字段定义(L/C/M/A/CI/CRC)、C-field命令码、时序机制(FAC/同步传输/安装)
metadata: 
  node_type: memory
  type: reference
  originSessionId: 2ca42aff-aa3e-4575-bd3b-6396bb2d3bee
  modified: 2026-10-16T01:31:30.761Z
---

# EN 13757-4 数据链路层

## 帧结构总览

数据链路层紧跟前导码和同步模式之后，支持两种帧格式：
- **Format A**: 符合 EN 60870-5-1 FT3 格式，每块最多 16 字节数据
- **Format B**: 扩展格式，支持更大帧（第二块最多 128 字节）

## Format A 帧格式

### 第一块（10+2 字节）
| L-field | C-field | M-field | A-field | CRC |
|---------|---------|---------|---------|-----|
| 1 byte  | 1 byte  | 2 bytes | 6 bytes | 2 bytes |

### 第二块（1+data+2 字节）
| CI-field | Data-field | CRC |
|----------|------------|-----|
| 1 byte   | 15 或 (L-9)mod16 bytes | 2 bytes |

### 可选块（每块 data+2 字节）
| Data-field | CRC |
|------------|-----|
| 16 或 (L-9)mod16 bytes（最后一块）| 2 bytes |

## Format B 帧格式

### 第一块（10 字节，无 CRC）
| L-field | C-field | M-field | A-field |
|---------|---------|---------|---------|
| 1 byte  | 1 byte  | 2 bytes | 6 bytes |

### 第二块（data+2 字节）
| CI-field | Data-field | CRC |
|----------|------------|-----|
| 1 byte   | 115 或 (L-12) bytes | 2 bytes |

### 可选块（仅当帧>128字节时）
| Data-field | CRC |
|------------|-----|
| (L-129) bytes | 2 bytes |

## 多字节字段顺序

- CI-field **之前**: 低字节优先（LSB first）
- CI-field **之后** (CI=80h,82h,83h,86h,88h~8Fh): 低字节优先
- CRC 字节: **高字节优先**（MSB first）

## L-Field (Length)

- **Format A**: L = 后续用户数据字节数（含控制和地址字节，不含CRC）
- **Format B**: L = 后续所有字节数（含所有CRC字节）

## C-Field (Control Field) — 命令码

### 位结构（MSB→LSB）
| Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3-0 |
|-------|-------|-------|-------|---------|
| FCB/ACD | FCV/DFC | RES(=0) | PRM | Function Code |

- PRM=1: 初级站→次级站; PRM=0: 次级站→初级站
- RES 始终为 0

### 初级站发送的功能码（PRM=1）

| 功能码 | 符号 | 方向 | 功能 | 支持模式 |
|--------|------|------|------|----------|
| 0 | SND-NKE | →表计 | 链路复位,清除FCB,结束FAC | S2,T2,C2,R2,N2,F2 |
| 3 | SND-UD/SND-UD2 | →表计 | 发送命令 | S2,T2,C2,R2,N2,F2 |
| 4 | SND-NR | 表计→ | 周期数据(无应答) | S1,T1,C1,N1,F2-m |
| 5 | SND-UD3 | →多接收器 | 广播/多播命令(无回复) | 可选 |
| 6 | SND-IR | 表计→ | 安装请求(手动) | 可选 |
| 7 | ACC-NR | 表计→ | 提供访问机会(无应用数据) | 可选 |
| 8 | ACC-DMD | 表计→ | 访问请求 | 可选 |
| 9 | REQ-UD1 | →表计 | 报警请求(Class 1) | S2,T2,C2,R2,N2,F2 |
| B | REQ-UD2 | →表计 | 数据请求(Class 2) | S2,T2,C2,R2,N2,F2 |

### 次级站发送的功能码（PRM=0）

| 功能码 | 符号 | 功能 | 触发 | 支持 |
|--------|------|------|------|------|
| 0 | ACK | 确认 | SND-UD/ACC-DMD/REQ-UD1 | 强制 |
| 1 | NACK | 否定确认(缓冲区溢出等) | SND-UD/SND-UD2 | 可选 |
| 6 | CNF-IR | 安装确认 | SND-IR | 可选 |
| 8 | RSP-UD | 响应应用数据 | REQ-UD1/REQ-UD2/SND-UD2 | 强制 |

### SND-UD2 特殊行为
当 FCV=1 且 C-field=53h 或 73h 时，表计收到功能码 3 + 清除 FCV-bit (C-field=43h) 时，假设收到 SND-UD + REQ-UD2，须以 RSP-UD 回复。

## M-Field (Manufacturer ID)

- 15位由三个字母(ISO/IEC 646, A~Z)编码
- 编码见 EN 13757-7:2018, 7.5.2
- MSB=0: 硬编码地址（制造商保证全球唯一）
- MSB≠0: 软地址（仅在系统最大传输范围内唯一）

## A-Field (Address)

- 始终包含**发送器地址**
- 上行链路 = 表计地址; 下行链路 = Other Device 地址
- 配合 EN 13757-3: A-field = identification number(4B) + version(1B) + device type(1B)

## CRC-Field

- 多项式: **x^16 + x^13 + x^12 + x^11 + x^10 + x^8 + x^6 + x^5 + x^4 + x^2 + 1**
- 初始值: 0
- 最终取反（complemented）
- 按 FT3 (EN 60870-5-1) 计算
- Format A: 每个 block 独立 CRC
- Format B: ≤128字节单个CRC; >128字节两个CRC

## 时序机制

### 12.6.1 安装消息时序
- 仅在手动事件（按键）后启动
- SND-IR 至少重复 **6次**，间隔 30~60 秒
- 手动启动后 **60分钟**内停止
- Other Device 回复 CNF-IR
- 反馈消息(SND-NKE)须**随机延迟 5~25 秒**（避免碰撞）

### 12.6.2 同步传输

目的: 使电池高效的 Other Device 能预测接收窗口

**标称传输间隔最大值:**

| 模式 | tNOM(max) |
|------|-----------|
| SG | 15 min |
| S | 120 min |
| R, N, F | 24 h |

**精度要求:**
- 静态容差: ≤±1 ppm/天, ≤±40 ppm
- 动态容差(-15~+65°C): +90/-10 ppm
- 动态容差(其他温度): +210/-10 ppm
- 全寿命总绝对容差(-15~+65°C): +130/-50 ppm
- 全寿命总绝对容差(其他): +250/-50 ppm
- 非累积抖动: tNOM<300s → <±1ms; tNOM≥300s → <±3ms

**消息省略规则:**
- 每24小时滑动窗口内可省略不超过 6.25% 的同步消息
- access number 仍须照常递增

**同步消息类型与混合使用:**
- 同步消息必须为 SND-NR、ACC-DMD 或 ACC-NR 三种类型之一（C-field 分别为 44h、48h、47h）
- 必须包含 short/long header 或 CI-field 在 86h 或 8Ch~8Fh 范围内，因为需要 access number
- **ACC-NR 消息可以专门用于维护同步（maintain synchronization）**，即不携带应用数据，仅用于维持时间同步
- **SND-NR、ACC-DMD 和 ACC-NR 三种消息可以混合使用**

**Note:** 
- ACC-NR 用于维护同步的原因：电池高效的 Other Device（如数据集中器、中继器）仅在可预测的短暂时间窗口内开启接收器。为与 Meter 的同步传输保持同步，接收端至少需要收到两条同步消息。ACC-NR 消息可以不携带应用数据，仅作为"心跳"信号维持时间同步，这样 Meter 无需每次都发送完整的周期数据（SND-NR）或响应访问请求（ACC-DMD），节省能量。
- 三种消息可混合使用的原因：协议允许灵活组合——Meter 可以在某些同步周期发送实际数据（SND-NR），在其他周期仅维护同步（ACC-NR），或在需要双向通信时请求访问（ACC-DMD）。这种混合使用不影响接收端的同步计算，因为所有三种消息都包含必需的 access number 和同步标记（sync-bit 或 Synchronized Subfield）。

### 12.6.3 访问时序

**表计可达性 (CC-field 的 B 和 A 位):**

| B | A | 含义 |
|---|---|------|
| 0 | 0 | 不可达 — 单向表计,无访问窗口 |
| 0 | 1 | 暂时不可达 — 此帧后无窗口 |
| 1 | 0 | 有限访问 — 仅短暂访问窗口(电池供电) |
| 1 | 1 | 无限访问 — 至少到下次发送前可访问(市电供电) |

### Frequent Access Cycle (FAC)
- 表计收到对其地址的命令后进入 FAC
- 以 FAC-Transmission delay tTXD 周期性重复最后消息
- 持续到 FAC-Timeout tTO（最后成功接收命令后的时间）
- Other Device 可通过 SND-NKE 提前结束 FAC

## 前向纠错 (FEC) — 可选

### 数据报结构
```
[Preamble+synch] [Frame A/B + CRC] [FEC header] [FEC check data] [Postamble]
```

### FEC Header (16 bits)
三重 5-bit FEC ID（三重化允许位级多数表决纠错）

| FEC ID | 算法 | Header值 |
|--------|------|---------|
| 00h | 保留 | 0000h |
| 01h | Reed Solomon(255,223) | 0421h |
| 18h~1Eh | 制造商特定 | — |
| 1Fh | 保留 | 7FFFh |

### Reed Solomon(255,223)
- Block size: 255 symbols, Symbol size: 8 bits
- 纠错能力: 16 symbols
- 信息块: 223 bytes, 不足补 00
- 生成多项式: CCSDS 131.0-B-2

### FEC 可用性

| 模式 | 单向 | 双向 |
|------|------|------|
| S | Yes | No |
| T | Yes | No |
| R | No | No |
| C | Yes | Yes |
| N | Yes | Yes |
| F | Yes | No |

## 重复/重复消息
简单中继器可能导致接收重复消息。通过 H-bit(Hop Count) 和 R-bit(Repeated Access) 标记。
