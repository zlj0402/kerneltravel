---
name: mbus-scenario-a-vs-b
description: 蓝皮书定义的两种 M-Bus 场景对比：场景 A（DLMS Server 使用专用 M-Bus APDU）vs 场景 B（DLMS Client 使用 DLMS/COSEM APDU）
metadata: 
  node_type: memory
  type: reference
  originSessionId: current-session
  modified: 2026-10-21T14:30:00.000Z
---

# 蓝皮书定义的两种 M-Bus 场景

## 文档来源

- **文档**: DLMS UA 1000-1 (Blue Book)
- **章节**: 4.8 Interface classes for setting up data exchange via M-Bus
- **小节**: 4.8.1 Overview

## 两种场景的定义

### 场景 A：DLMS Server（M-Bus Master）与 M-Bus Slaves 通信

**定义**：
> "a DLMS server hosted by an M-Bus master and exchanging **dedicated M-Bus APDUs** with M-Bus slaves"

**架构**：
```
┌──────────────────┐
│  M-Bus Master    │
│  ┌────────────┐  │
│  │ DLMS Server│  │
│  └────────────┘  │
│       ↓          │
│  专用 M-Bus APDU │  ← 使用 EN 13757 标准
└───────┼──────────┘
        ↓
  ┌─────┴─────┬──────────┬──────────┐
  ↓           ↓          ↓          ↓
┌─────┐   ┌─────┐   ┌─────┐   ┌─────┐
│M-Bus│   │M-Bus│   │M-Bus│   │M-Bus│
│Slave│   │Slave│   │Slave│   │Slave│
└─────┘   └─────┘   └─────┘   └─────┘
```

**特点**：
- ✅ Master 作为 DLMS **Server**
- ✅ 使用**专用 M-Bus APDU**（不是 DLMS/COSEM APDU）
- ✅ **直接使用 EN 13757 标准**
- ✅ 用于设置和管理 M-Bus 媒体
- ✅ 需要实现完整的 M-Bus 协议栈
- ✅ 典型应用: **M-Bus 网关、集中器**

**使用的接口类**：
- M-Bus client (class_id = 72)
- M-Bus master port setup (class_id = 74)
- M-Bus diagnostic (class_id = 77, version = 0)

**协议栈**：
```
├─ DLMS Server（应用层）
├─ 专用 M-Bus APDU
├─ EN 13757-3（传输层）
├─ EN 13757-4（无线链路层）← 必须实现！
└─ EN 13757-4（物理层）← 必须实现！
```

### 场景 B：DLMS Client 与 DLMS Servers（M-Bus Slaves）通信

**定义**：
> "a DLMS client hosted by an M-Bus master and exchanging **DLMS/COSEM APDUs** with DLMS servers hosted by M-Bus slaves"

**架构**：
```
┌──────────────────┐
│  M-Bus Master    │
│  ┌────────────┐  │
│  │ DLMS Client│  │
│  └────────────┘  │
│       ↓          │
│  DLMS/COSEM APDU │  ← M-Bus 只是透明传输
└───────┼──────────┘
        ↓
  ┌─────┴─────┬──────────┬──────────┐
  ↓           ↓          ↓          ↓
┌─────┐   ┌─────┐   ┌─────┐   ┌─────┐
│M-Bus│   │M-Bus│   │M-Bus│   │M-Bus│
│Slave│   │Slave│   │Slave│   │Slave│
│     │   │     │   │     │   │     │
│┌───┐│   │┌───┐│   │└───┘│   │└───┘│
││DLMS││   ││DLMS││   │     │   │     │
││Serv││   ││Serv││   │     │   │     │
││er  ││   ││er  ││   │     │   │     │
│└───┘│   │└───┘│   │     │   │     │
└─────┘   └─────┘   └─────┘   └─────┘
```

**特点**：
- ⚠️ Master 作为 DLMS **Client**
- ⚠️ 使用 **DLMS/COSEM APDU**（通过 M-Bus 传输）
- ⚠️ M-Bus 只是**透明传输层**
- ⚠️ 不需要深入解析 M-Bus 协议
- ✅ 典型应用: **智能电表（每个电表是 DLMS Server）**

**使用的接口类**：
- DLMS server M-Bus port setup (class_id = 76)
- M-Bus slave port setup (class_id = 25)
- Wireless Mode Q channel (class_id = 73)
- M-Bus diagnostic (class_id = 77, version = 0)

**协议栈**：
```
├─ DLMS Client（应用层）
├─ DLMS/COSEM APDU
├─ 绿皮书 10.6（M-Bus profile）
├─ EN 13757-3（传输层）← M-Bus TL + Wrapper（透明传输）
├─ EN 13757-4（无线链路层）← 透明传输，不解析 M-Bus 内容
└─ EN 13757-4（物理层）← 透明传输，只传输比特流
```

## 关键对比

| 方面 | 场景 A | 场景 B |
|------|--------|--------|
| **Master 角色** | DLMS **Server** | DLMS **Client** |
| **APDU 类型** | **专用 M-Bus APDU** | DLMS/COSEM APDU |
| **是否使用 EN 13757** | ✅ **是！直接使用标准** | ⚠️ M-Bus 只是透明传输 |
| **需要实现 M-Bus 协议** | ✅ **是！必须实现完整协议栈** | ⚠️ 需要物理层，但不解析链路层 |
| **需要解析 M-Bus 帧** | ✅ **是！必须解析** | ❌ 不需要，只传递 |
| **需要实现物理层** | ✅ **是！必须实现** | ✅ 需要（传输比特流） |
| **需要实现链路层** | ✅ **是！必须实现** | ❌ 不需要（M-Bus 透明） |
| **需要实现传输层** | ✅ **是！必须实现** | ⚠️ 需要（M-Bus TL + Wrapper） |
| **典型应用** | **M-Bus 网关、集中器** | **智能电表（通过 M-Bus 传输 DLMS）** |
| **GCP 要求** | 不适用 | **必须支持** ✅ |

## 主站实现属于哪个场景？

### 如果你的主站是 M-Bus 网关/集中器（场景 A）

```
你的主站（DLMS Server，M-Bus Master）
    ↓ 专用 M-Bus APDU（EN 13757 标准）
    ↓
M-Bus Slave（水表、气表、热表等）
```

**你需要实现**：
- ✅ 完整的 M-Bus 协议栈（物理层、链路层、传输层）
- ✅ 专用 M-Bus APDU 处理
- ✅ M-Bus 帧解析和构建
- ✅ C-field 命令处理
- ✅ DIF/VIF 数据编码
- ✅ COSEM 对象（class_id = 72, 74, 77）

### 如果你的主站是智能电表管理系统（场景 B）

```
你的主站（DLMS Client，HES/DC）
    ↓ DLMS/COSEM APDU（GET/SET/ACTION）
    ↓
智能电表（DLMS Server，托管在 M-Bus Slave 中）
```

**你需要实现**：
- ✅ DLMS Client（应用层）
- ✅ DLMS/COSEM APDU 处理
- ✅ M-Bus 传输层（绿皮书 10.6 profile）
- ⚠️ M-Bus 物理层（传输比特流）
- ❌ 不需要深入解析 M-Bus 链路层

## 学习路径建议

### 场景 A 的学习路径（M-Bus 网关/集中器）

**阶段 1：M-Bus 物理层（EN 13757-4 Chapter 7-10）**
- Chapter 7: 通用物理层特性
- Chapter 8: 编码方式（Manchester, 3-out-of-6, NRZ）
- Chapter 9: Mode C（Compact）
- Chapter 10: Mode T（Frequent Transmit）

**阶段 2：M-Bus 链路层（EN 13757-4 Chapter 11-13）**
- Chapter 11: Frame Format A/B
- Chapter 12: 时序（同步传输、访问时序、安装）
- Chapter 13: Extended Link Layer

**阶段 3：M-Bus 传输层（EN 13757-3）**
- 传输层协议
- 分段和重组
- C_ITL 处理

**阶段 4：M-Bus 应用层**
- 专用 M-Bus APDU
- DIF/VIF 数据编码
- COSEM 对象（class_id = 72, 74, 77）

### 场景 B 的学习路径（智能电表管理系统）

**阶段 1：DLMS/COSEM 基础**
- 绿皮书（DLMS/COSEM 架构和协议）
- COSEM 对象模型
- DLMS/COSEM 服务（GET/SET/ACTION）

**阶段 2：M-Bus 传输层（绿皮书 10.6）**
- M-Bus profile
- 三级寻址（LLA/C_ITL/SAP）
- 服务映射（MBUS-DATA.req/ind/cnf）

**阶段 3：M-Bus 物理层（基础）**
- 物理层特性（只需要传输比特流）
- 不需要深入理解编码和调制细节

## Note

### 重要理解

1. **场景 A 和场景 B 不是互斥的**
   - 一个系统可能同时包含两种场景
   - 例如：M-Bus 网关（场景 A）连接到 HES/DC（场景 B）

2. **场景 A 更符合 "M-Bus 协议实现"**
   - 需要深入理解 EN 13757 标准
   - 需要实现完整的 M-Bus 协议栈
   - 是真正的 "M-Bus" 开发

3. **场景 B 更符合 "DLMS/COSEM 应用开发"**
   - M-Bus 只是传输层
   - 重点是 DLMS/COSEM 应用层
   - 是 "基于 M-Bus 的 DLMS/COSEM" 开发

4. **GCP 针对的是场景 B**
   - GCP 是智能电表的规范
   - 每个电表是 DLMS Server
   - 使用 DLMS/COSEM APDU
   - M-Bus 只是传输方式之一
