---
name: gcp-wmbus-requirements
description: ACESM GCP (Generic Companion Profile) 中关于 Wireless M-Bus 的使用要求和模式规范
metadata: 
  node_type: memory
  type: reference
  originSessionId: current-session
  modified: 2026-10-21T03:48:17.036Z
---

# ACESM GCP - Wireless M-Bus 要求

## 文档信息

- **文档名称**: DLMS-UA_ACESM-Electricity-Smart-Meter-Profile-Ed-1.V2.0
- **章节**: 6.11.5 Wireless M-Bus (第 48 页 / 第 47/491 页)
- **发布日期**: 2025-10-01

## 强制支持的无线模式

> "The wireless M-Bus is based on the EN 13757-4 wireless meter readout using mode **C1/C2 and T1/T2** (other modes may be supported optionally)."

### 必须支持的模式

- **Mode C1** (Compact, 单向, 100 kbps, NRZ FSK)
- **Mode C2** (Compact, 双向, 100 kbps)
- **Mode T1** (Frequent Transmit, 单向, ~66.7 kbps, 3-out-of-6 编码)
- **Mode T2** (Frequent Transmit, 双向)

### 可选支持的模式

- Mode S (Stationary)
- Mode R (Frequent Receive)
- Mode N (Narrowband VHF, 169 MHz)
- Mode F (Frequent Rx+Tx, 433 MHz)

## 帧格式标准

> "The format class **FT3** of EN 60870-5-1 and the telegram structure according to EN 60870-5-2 is used."

- 使用 FT3 帧格式
- 遵循 EN 60870-5-2 的电报结构

## M-Bus Client 方法支持

### 方法定义

| Method | 名称 | 说明 |
|--------|------|------|
| 1 | `slave_install(data)` | 无效果（保留） |
| 2 | `slave_deinstall(data)` | 卸载 M-Bus 从站设备，为新设备安装做准备 |
| 3 | `capture(data)` | 无效果（保留） |
| 4 | `reset_alarm(data)` | 重置 M-Bus 设备告警状态，消息在射频链路可用时发送 |
| 5 | `synchronise_clock(data)` | 同步 M-Bus 设备时钟与 Client 时钟 |
| 6 | `data_send(data)` | 发送数据到 M-Bus 从站设备 |
| 9 | `transfer_security_information(data)` | 传输加密密钥到 M-Bus 设备，响应时更新 `encryption_key_status` (attribute 14) |

### 单向模式限制

> "The methods described above have **no effect** if the M-Bus device is exclusively operated in an **unidirectional mode (e.g. C1)**"

**解读**:
- 如果设备仅支持单向模式（如 C1），上述方法无效
- 必须支持双向模式（C2 或 T2）才能使用这些功能

## 设备安装流程

遵循 EN 13757-4 标准：
- 使用 **SND-IR** (Installation Request) 和 **CNF-IR** (Installation Confirmation) 数据报
- 参考 EN 13757-4 Annex E Figure E.2 – Installation timing

### 安装过程

1. M-Bus 设备周期性发送 SND-IR 数据报
2. 直到收到 CNF-IR 或超时
3. ACESM（智能电表）通过发送 CNF-IR 接受 M-Bus 设备

## 消息缓冲要求

> "The smart meter shall support the buffering of **at least 10 M-Bus messages**"

**要求**:
- 智能电表必须支持至少缓冲 10 条 M-Bus 消息
- 用于射频链路不可用时的消息暂存

## 选择 C 和 T 模式的原因

**Mode C (Compact)**:
- ✅ 100 kbps 高速率（快速传输）
- ✅ 短帧（<22 ms）
- ✅ 与 Mode T 兼容的前导码设计
- ✅ 适合需要快速响应的场景

**Mode T (Frequent Transmit)**:
- ✅ ~66.7 kbps 中等速率
- ✅ 适合 walk-by/drive-by 抄表
- ✅ 3-out-of-6 编码提供良好的同步特性
- ✅ 每几秒发送短帧（3-8 ms）

**不强制 Mode S/R/N/F 的原因**:
- Mode S：速率较低（16 kbps），适合电池供电固定网络，但不是最优选
- Mode R：仅双向 R2，需要多信道支持
- Mode N：169 MHz 频段，主要用于特定地区
- Mode F：433 MHz 频段，使用较少

## 与绿皮书 (DLMS/COSEM) 的对比

| 特性 | GCP 要求 | 绿皮书 (10.6) |
|------|---------|--------------|
| **必须支持的模式** | **Mode C1/C2, T1/T2** | 未指定（模式无关） |
| **可选模式** | 其他模式可选 | 未提及 |
| **帧格式** | FT3 | FT3（一致） |
| **安装流程** | SND-IR / CNF-IR | 相同（引用 EN 13757-4） |
| **消息缓冲** | ≥10 条 | 未明确 |
| **单向模式限制** | 明确说明单向模式下方法无效 | 未提及 |

## Note

- GCP 是 DLMS UA 的 Generic Companion Profile，针对交流电表（ACESM）
- GCP 明确指定了必须支持的无线模式，而绿皮书是模式无关的
- C 和 T 模式是 868 MHz 频段最常用的模式，覆盖了欧洲和大部分地区的智能计量应用
