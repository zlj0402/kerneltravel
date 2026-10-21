---
name: wmbus-transport-layer
description: EN 13757-4 Transport Layer (TPL) 详解，包括 Long TPL、Short TPL、Extended Link Layer 的格式和使用场景
metadata: 
  node_type: memory
  type: reference
  originSessionId: current-session
  modified: 2026-10-14T09:49:42.462Z
---

# EN 13757-4 Transport Layer

## 概述

Transport Layer 用于在 CI-field 之后传输应用层信息或管理命令。

## CI-field 与 TPL 类型

| CI 值 | 设计 | TPL Header | 用途 |
|-------|------|------------|------|
| 80h | Transport Layer to device | Long | Other Device → Meter (如 REQ-UD2) |
| 82h | Network management data to device | short | 网络管理命令 |
| 86h | Extended Link Layer to device | Extended | 扩展链路层 |
| 87h | Network management data to device | long | 网络管理命令 |
| 88h | Network management data to device | long | 网络管理命令 |
| 89h | Network management data from device | None | 网络管理响应 |
| 8Ah | Transport Layer from device | **Short** | Meter → Other Device (如 ACK) |
| 8Bh | Transport Layer from device | **Long** | Meter → Other Device (如 ACK) |
| 8Ch | Extended Link Layer from device | Extended | 扩展链路层 |

## 13.3.2 Short Transport Layer (CI = 8Ah)

**用途**: Meter → Other Device 的通信（如 ACK 响应）

**格式** (Table 57):
| CI | ACC | STS | Conf. Field |
|----|-----|-----|-------------|
| 1 byte | 1 byte | 1 byte | 2 bytes |

**特点**: 不包含 Application Layer Address (ALA)，因为假设接收方已知发送方身份。

## 13.3.3 Long Transport Layer (CI = 80h 或 8Bh)

**用途**: 
- Other Device → Meter (CI = 80h，如 REQ-UD2)
- Meter → Other Device (CI = 8Bh，如 ACK)

**格式** (Table 58):
| CI | Ident no. | Manuf. | Ver. | Device Type | ACC | STS | Conf. Field |
|----|-----------|--------|------|-------------|-----|-----|-------------|
| 1 byte | 4 bytes | 2 bytes | 1 byte | 1 byte | 1 byte | 1 byte | 2 bytes |

**特点**: 包含完整的 Application Layer Address (ALA = Ident no. + Manuf. + Ver. + Device Type)

## 关键规则：何时使用 Long vs Short TPL

### 来自 12.5.6 的 NOTE

> **If the meter address differs from the sender address, the meter address will be transmitted after the CI-field using a long Transport Layer; see 13.3.3 and EN 13757-7.**

**解读**:
- 当 **Link Layer Address (LLA) ≠ Application Layer Address (ALA)** 时，必须使用 **Long TPL**
- 当 **LLA = ALA** 时，可以使用 **Short TPL**

### 典型场景分析

#### 场景 1: 直接使用无线电模块的仪表

```
仪表 (LLA = ALA = MTR)
├─ 上行 (Meter → Collector): LLA = MTR, ALA = MTR
│  └─ 可以使用 Short TPL (CI = 8Ah)
└─ 下行 (Collector → Meter): LLA = MTR, ALA = MTR
   └─ 使用 Long TPL (CI = 80h) 携带完整 ALA
```

#### 场景 2: 通过 RF Adapter 的仪表

```
RF Adapter (LLA = COM) + Meter (ALA = MTR)
├─ 上行 (Meter → Collector via RF Adapter): 
│  ├─ LLA = COM (RF Adapter 地址)
│  └─ ALA = MTR (仪表地址)
│  └─ 必须使用 Long TPL 携带 ALA
└─ 下行 (Collector → Meter via RF Adapter):
   ├─ LLA = COM
   └─ ALA = MTR
   └─ 使用 Long TPL (CI = 80h) 携带完整 ALA
```

### Annex E 图示总结

**Figure E.3 (Long TPL)**:
- 场景: Other Device (COM) 通过 RF Adapter (RFA, LLA=RFA) 与 Meter (ALA=MTR) 通信
- LLA ≠ ALA，必须使用 Long TPL
- 上行: CI = 8Bh (Long TPL from device)
- 下行: CI = 80h (Long TPL to device)

**Figure E.4 (Short TPL)**:
- 场景: Other Device (COM, LLA=COM) 直接与 Meter (LLA=ALA=MTR) 通信
- LLA = ALA，可以使用 Short TPL
- 上行: CI = 8Ah (Short TPL from device)
- 下行: CI = 5Bh (SND-UD, 无 TPL)

## 回答用户问题

### 问题 1: Primary 发给 Secondary 时用 Long TPL，那 Secondary 发给 Primary 时呢？

**答案**: 
- 如果 **Secondary (仪表) 的 LLA = ALA**（直接使用无线电模块），可以用 **Short TPL**
- 如果 **Secondary 的 LLA ≠ ALA**（通过 RF Adapter），必须用 **Long TPL**

### 问题 2: 是否可能收到 Short TPL 的帧？

**答案**: **是的**，完全可能！

当以下条件满足时，会收到 Short TPL 帧：
1. 发送方是直接使用无线电模块的仪表（LLA = ALA）
2. 发送方使用 CI = 8Ah (Short TPL from device)

**典型场景**:
- 仪表直接发送 ACK 响应（CI = 8Ah）
- 仪表发送周期性数据（SND-NR, CI = 44h，无 TPL）
- 仪表发送安装请求（SND-IR, CI = 46h，无 TPL）

## Extended Link Layer (CI = 86h / 8Ch)

当使用加密或扩展安全功能时，使用 Extended Link Layer 代替 Transport Layer。
