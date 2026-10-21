---
name: greenbook-wmbus-profile
description: DLMS/COSEM Green Book (Ed.12) 中关于 wired and wireless M-Bus profile 的规范（10.6节）
metadata: 
  node_type: memory
  type: reference
  originSessionId: current-session
  modified: 2026-10-21T03:50:17.934Z
---

# DLMS/COSEM Green Book - Wired and Wireless M-Bus Profile

## 文档信息

- **文档名称**: DLMS/COSEM Architecture and Protocols (Green Book Edition 12)
- **文档编号**: DLMS UA 1000-2 Ed.12 V1.0
- **章节**: 10.6 The wired and wireless M-Bus profile (第 471-495 页)
- **发布日期**: 2025-05-28

## 10.6.1 Scope

> "This subclause 10.6 specifies DLMS/COSEM wired and wireless M-Bus communication profiles for local and neighbourhood networks."

### 范围说明

- 规定本地和邻近网络的有线和无线 M-Bus 通信配置文件
- **不涉及**: 设置和管理 M-Bus 通信通道、M-Bus 网络、注册从站设备和中继器
- **范围限制**: 仅限于与 COSEM 数据模型和 DLMS/COSEM 应用层结合使用的通信协议方面
- 特定于项目的数据结构和数据内容定义可在项目特定的配套规范中提供

### 参考章节

- 10.6.7: M-Bus 帧结构、寻址方案和示例
- 10.6.8: 代表性通信实例的 MSC（消息序列图）

## 10.6.2 Targeted communication environments

### 覆盖的接口

在 IEC 62056-1-0 引入的智能计量架构中，有线和无线 M-Bus 通信配置文件覆盖以下接口：

- **C 接口**: NNAP（Neighborhood Network Access Point）和计量设备之间
- **M 接口**: LNAP（Local Network Access Point）和计量设备之间
- **H1 接口**: 计量设备和简单消费者显示器之间
- **H2 接口**: LNAP 和家庭自动化系统之间

### 设备角色

- 在所有情况下，计量设备作为 **DLMS servers**
- 在 C 和 M 接口，计量设备作为 **M-Bus slaves**，M-Bus master 是 NNAP 或 LNAP
- 在 H1 和 H2 接口，计量设备作为 DLMS server，可以根据有线或无线 M-Bus 的选择以及无线 M-Bus 的操作模式，以 pull 模式或 push 模式运行，作为 M-Bus master 或 M-Bus slave

## 10.6.3 Use of the communication layers for this profile

### 10.6.3.1 底层标准

> "The DLMS/COSEM wired and wireless M-Bus communication profiles for local and neighbourhood networks use the lower-layer protocols specified in the **EN 13757 series**."

### 10.6.3.3.1 Physical layer

> "The physical layer is as specified in **EN 13757-2:2004** (wired, twisted pair based) and in **EN 13757-4:2013** (wireless)."

对于电池供电的 master 和/或少量连接的电表，有线 M-Bus 物理层在 EN 13757-6:2015 中规定（基于双绞线，短距离）。

### 10.6.3.3.2 Link layer

> "The M-Bus link layer is as specified in **EN 13757-2:2004** (wired) and in **EN 13757-4:2013** (wireless)."

**NOTE**: 对于无线抄表，EN 13757-5:2015 支持简单重传（单跳中继）以及路由无线网络，允许扩展传输范围。

### 10.6.3.3.3 Transport layer

> "The M-Bus transport layer is as specified in **EN 13757-3:2018**."

与 10.6.4.6 中规定的 M-Bus wrapper 一起，构成 **DLMS/COSEM M-Bus-based transport layer (TL)**，作为链路层和 DLMS/COSEM AL 之间的适配层。

### M-Bus TL 支持的应用层

M-Bus TL 允许多个应用层在 M-Bus 底层之上共存：

- M-Bus 专用 AL
- DLMS/COSEM AL
- 未来可能指定的其他 AL

使用的应用层由 M-Bus 帧的 **Control Information (CI) field** 选择。在本配置文件规定的通信配置文件中，仅使用 DLMS/COSEM AL。

## 10.6.4 Identification and addressing scheme

### 10.6.4.1 Overview - 三级寻址

有线和无线 M-Bus 通信配置文件提供三级寻址：

#### 1. Link Layer Address (LLA)

- 标识 M-Bus 上的物理设备实体
- 参见 10.6.4.2、10.6.4.3 和 10.6.4.4

#### 2. TL address (C_ITL)

- 作为协议选择器
- 提供有关 TPDU 结构和可用选项/能力的信息
- 在本配置文件中，选择 DLMS/COSEM M-Bus-based TL
- 参见 10.6.4.5

#### 3. M-Bus wrapper

- 包含在 TL 中
- 为 COSEM client 和 server AP 提供寻址
- 参见 10.6.4.6

### 地址三元组

三元组 **{LLA, C_ITL, SAP}** 明确标识一个 AP：

**示例**:
- 当 client AP#1 (Public Client) 想向 server host device #1 中的 server AP#1 (Management LD) 发送 xDLMS APDU 时：
  - LLA = 0x45, C_ITL = 0x61, STSAP = 0x10, DTSAP = 0x01
- 在响应中：
  - LLA = 0x45, C_ITL = 0x7D, STSAP = 0x01, DTSAP = 0x10

### 10.6.4.2 Link Layer Address for wired M-Bus

LLA 在两个通信方向上都携带 M-Bus slave（次级站）的地址。

#### Table 122 - Wired M-Bus Link Layer Addresses

| 值 | 描述 |
|----|------|
| 0x00 | 未配置的从站 |
| 0x01-0xFA | 从站的主地址范围 |
| 0xFB | 保留用于与主 master repeater 的管理通信 |
| 0xFC | 保留 |
| 0xFD | 保留用于二次寻址 |
| 0xFE | 测试和诊断地址 |
| 0xFF | 广播地址 |

**NOTE**: 最近的 EN13757 标准使用以下术语：
- Link Layer Address (LLA) 替代 primary address
- (M-Bus) Application Layer Address (ALA) 替代 secondary address

### 10.6.4.3 Link Layer Address for wireless M-Bus

EN 13757-4:2013 规定了两种数据链路层帧格式：
- **frame format A** (EN 13757-4:2013, 11.3)
- **frame format B** (EN 13757-4:2013, 11.4)

两种情况下，帧都被分为由 CRC 码分隔的块。

**LLA 始终携带发送方（发射器）的地址**，无论帧来自 master 还是 slave。它位于第一个块中。

LLA 由以下元素序列组成：
- Manufacturer identification (M-ID)
- Device Identification No (Serial No.)
- Device Version (VER)
- Device Type (DT)

如果 C_ITL 存在并指示存在 long Extended Link Layer Address (ELLA)，则后续字段包含接收方的地址。它位于第二个块中。

**NOTE**: CI field C_ELL 指示使用带有额外控制服务功能的 Extended Link Layer。

### 10.6.4.4 Link Layer Address for M-Bus broadcast

#### 有线 M-Bus

设备根据 LLA 和 ALA 寻址的能力决定广播或多播寻址：
- 在使用选择性 LLA 的情况下，广播由 LLA = 0xFF 标识
- 在使用 ALA 的情况下（LLA 值 0xFD），适用广播和多播寻址

#### 无线 M-Bus

在无线 M-Bus 网络中，适用广播和多播寻址。

ALA 和无线 LLA 中的地址由以下 4 个元素组成：
- manufacturer identification
- device identification number
- device version
- device type

对于广播或多播寻址，适用以下规则（参见 EN 13757-3:2018）：
- 在 device identification number 元素中，每个单独的数字可以通过通配符 nibble 0xF 进行通配符处理
- manufacturer、version 和 device type 元素可以通过通配符字节 0xFF 进行通配符处理

### 10.6.4.5 Transport layer address

M-Bus 帧可能有多个字段，具体取决于存在的协议层。每个字段携带一个协议层头，payload 由指示该层类型和提供的功能的 **Control Information (CI) field** 引入。

#### Table 123 - DLMS/COSEM M-Bus-based TL C_ITL values

| C_ITL | 描述 |
|-------|------|
| 0x00-0x1F | 不存在 M-Bus data header¹ |
| 0x60 | 存在 Long M-Bus data header，方向 master to slave |
| 0x61 | 存在 Short M-Bus data header，方向 master to slave |
| 0x7C | 存在 Long M-Bus data header，方向 slave to master |
| 0x7D | 存在 Short M-Bus data header，方向 slave to master |

¹ 在这种情况下，分段和重组受到限制。

**必须**支持所有三种 CI field 地址结构选项。

#### C_ITL = 0x00-0x1F 的结构

当 C_ITL = 0x00-0x1F 时，表示不存在 M-Bus data header。在这种情况下，TL 可以提供分段和重组。

**C_ITL 位结构**（无 M-Bus data header 时）：
```
b7 b6 b5 b4 b3 b2 b1 b0
0  0  0  FIN Sequence number
```

- Bits 7, 6, 5 设为 0 表示不存在 M-Bus data header
- Bit 4 (FIN) 指示 TPDU 的数据字段携带 xDLMS APDU 的一部分或完整的 APDU
- Bits 3 to 0 用于序列号。允许序列号翻转，意味着当序列号达到 1111 并且还有段要发送时，下一个段序列号将取值 0000

#### 其他 C_ITL 值

- **C_ITL = 0x61 / 0x7D**: 指示存在 short M-Bus data header，以及消息方向。不支持分段和重组。
- **C_ITL = 0x60 / 0x7C**: 指示存在 long M-Bus data header，以及消息方向。不支持分段和重组。

不同 M-Bus TPDU 结构的使用取决于网络环境以及 M-Bus slave 设备的结构和能力。因此，它们的使用由 **M-Bus slave** 决定。

请求和响应中的 TPDU 格式应相同，除非需要使用分段。

#### Table 124 - CI fields used for link management purposes (仅供参考)

| CI field | 上层 | 描述 |
|----------|------|------|
| 0x80 | Pure TLᵃ | 存在 Long M-Bus data header，to meter |
| 0x8A | Pure TLᵃ | 存在 Short M-Bus data header，from meter |
| 0x8B | Pure TLᵃ | 存在 Long M-Bus data header，from meter |
| 0x8C | ELLᵇ | Short M-Bus ELL without ELLA |
| 0x8E | ELLᵇ | Long M-Bus ELL with ELLA |

ᵃ 此 CI field 值表示消息不用于应用层；不存在 APDU。
ᵇ Extended Link Layer (ELL) 仅用于无线 M-Bus。

### 10.6.4.6 Application addressing extension – M-Bus wrapper

DLMS/COSEM AL 需要识别参与 AA 的伙伴：每个 AA 绑定到一对 client 和 server SAP。

M-Bus wrapper（构成 DLMS/COSEM M-Bus-based TL 的一部分）提供 DLMS client 和 server AP 所需的寻址。

#### Table 125 - Client and server SAPs

**Client SAPs**:
| 值 | 描述 |
|----|------|
| 0x00 | No-station |
| 0x01 | Client Management Process |
| 0x10 | Public Client |
| 0x02-0x0F | 开放用于 client SAP 分配 |
| 0x11-0xFF | 开放用于 client SAP 分配 |

**Server SAPs**:
| 值 | 描述 |
|----|------|
| 0x00 | No-station |
| 0x01 | Management Logical Device |
| 0x02-0x0F | 保留供将来使用 |
| 0x10-0x7E | 开放用于 server SAP 分配 |
| 0xFF | All-station (Broadcast) |

## 10.6.5 Specific considerations and constraints for using certain services within profile

### 10.6.5.1 Overview

通常，有线和无线 M-Bus 介质在使用 DLMS/COSEM AL 服务方面没有区别。

在使用 DLMS/COSEM 提供的功能时，实现者应考虑 M-Bus 介质、电池电源的使用以及设备类型和配置所设置的约束。

#### 高效消息传递机制

为了在数据链路层实现高效的消息传递，在 AA 建立和后续消息交换之间应用以下机制：

- 在每个消息中带有应用数据的 M-Bus 消息应遵循 EN 13757-4:2013 的方案，其中与 SND-UD2 消息结合使用，控制字节码 C = 0x43 允许获取数据而不是 ACK（参见 EN 13757-4:2013, Table 24）
- AA 建立后，所有后续数据交换都遵循应用 SND-UD2 的直接 REQUEST-RESPONSE 机制

**NOTE**: 此机制不适用于无线 M-Bus 中的分片消息（参见 EN 13757-4:2013, Table 24, note c）。

### 10.6.5.2 Application association establishment and release: ACSE services

#### Table 126 - Application associations and data exchange in the M-Bus-based profiles

| Protocol connection parameters | COSEM-OPEN service class | Use | Type of established application association | Service class | Use |
|-------------------------------|-------------------------|-----|-------------------------------------------|---------------|-----|
| 见下文 | Confirmed | Exchange | Confirmed | Confirmed | M-Bus frame |
| 见下文 | Confirmed | Exchange | Confirmed | Unconfirmed | M-Bus frame |
| 见下文 | Unconfirmed | Send AARQ-APDU-s in M-Bus frames | Unconfirmed | Confirmed (not allowed) | - |
| 见下文 | Unconfirmed | Send AARQ-APDU-s in M-Bus frames | Unconfirmed | Unconfirmed | M-Bus frame |

#### COSEM-OPEN 服务参数

**Protocol_Connection_Parameters** 参数应为：
- 协议（配置文件）标识符：有线或无线 M-Bus
- Link Layer Address (LLA)
  - **NOTE**: 在有线 M-Bus 的情况下，这是两个通信方向的 slave 的 LLA；在无线 M-Bus 的情况下，这是发送方/发射器的 LLA
- 在无线 M-Bus 的情况下，如果需要，Extended Link Layer Address ELLA
- M-Bus data header 类型，如果存在，short 或 long M-Bus data header
- server logical device address: COSEM logical device SAP
- client_Id: COSEM client SAP

任何 server（目的地）地址参数都可以包含特殊地址（All-station、No-station 等）。

不使用 User_Information 服务参数。

#### Service_Class 参数

在 M-Bus-based 配置文件中，COSEM-OPEN 服务的 Service_Class 参数链接到 xDLMS InitiateRequest APDU 的 response-allowed 参数：

- 如果 COSEM-OPEN 服务使用 Service_Class == Confirmed 调用，则 response-allowed 参数应设置为 TRUE。期望 server 响应
- 如果使用 Service_Class == Unconfirmed 调用，则 response-allowed 参数应设置为 FALSE。server 不应发回响应
- 使用 COSEM-OPEN 服务（Service_Class == Unconfirmed）和一组逻辑设备地址（例如广播地址）建立 client 和一组逻辑设备之间的未确认 AA

由于底层是无连接的，COSEM-RELEASE 服务的 Use_RLRQ_RLRE 服务参数应设置为 TRUE。

### 10.6.5.3 xDLMS services

#### 10.6.5.3.1 Request-response type services

GET、SET、ACTION 和 ACCESS 服务的 Service_Class 参数链接到 Invoke-Id-And-Priority / Long-Invoke_Id-And-Priority 字段的 service class bit。如果服务使用 Service_Class = Confirmed 调用，则相应的 service class bit 应设置为 1，否则应设置为 0。

**禁止**在 DLMS/COSEM M-Bus-based transport layer 之上建立的未确认 AA 中，以确认方式（Service_Class = Confirmed）请求 xDLMS 服务。Client AL 也会阻止这种情况。接收此类 APDU 的 server 应简单地丢弃它们，或者发回 ConfirmedServiceError APDU，或者如果实现了该功能，发回可选的 ExceptionResponse APDU。

#### 10.6.5.3.2 Unsolicited services

DLMS/COSEM 未请求服务（EventNotification、DataNotification 和 InformationReport）在 M-Bus 网络中的应用取决于 M-Bus 实体的能力和配置，因此应在项目特定的配套规范中规定。

#### 10.6.5.3.3 Broadcast messages

**有线 M-Bus**:
- 支持使用主地址（= link layer address，等于 255）的广播消息
- 由于广播消息无法在链路层确认，可能需要在应用层进行后续验证
- 通常也可使用通配符机制（EN 13757-3:2018，从 EN 13757-4:2013 引用）对辅助地址进行广播和多播

**无线 M-Bus**:
- 因此，在无线 M-Bus 网络中适用广播/多播消息，考虑到从 DLMS 角度来看，如果支持的 M-Bus 底层提供相应功能，则可以通过无线 M-Bus 广播 COSEM APDU

### 10.6.5.4 Security mechanisms

9.2 和 DLMS UA 1000-1 中规定的安全机制可以在没有特定约束的情况下使用。

### 10.6.5.5 Transporting long application messages

有两种可用的机制来传输不适合单个 M-Bus 帧的长消息：

- **TL 提供的分段**（参见 10.6.3.4.3 和 10.6.4.5）。仅当 TPDU 中不存在 M-Bus data header 时，此机制可用（参见 EN13757-1:2014）
- **DLMS/COSEM application layer block transfer**。此机制可用于所有 TPDU 结构

### 10.6.5.6 Media access, bandwidth and timing considerations

对于 DLMS/COSEM 未请求服务，有线和无线 M-Bus 安装中的 M-Bus alarm messaging 可能会由于以下原因而产生一定的延迟：

- 电池供电设备的传输信用耗尽
  - **NOTE**: 作为信息的信用取决于项目特定的配套规范：由于复杂的嵌入，这些不是自由参数，因此在应用中可访问
- 等待下一次传输的访问时间（仅无线 M-Bus）
- 无线电信道干扰（仅无线 M-Bus）
- M-Bus 不支持处理消息优先级

对于无线 M-Bus 网络中的广播，还有一些其他重要方面：

- 能耗取决于各种参数、设备和系统设计，通常不会阻止使用广播
- 可达性对广播和多播至关重要，其中 EN 13757-4:2013, Table 27 指示不同的可达性（无、临时、有限、无限）及设备示例
- 市电供电的电表和通信模块可以无限制地访问（关于占空比！）。另一方面，电池供电的电表会带来一些限制（参见传输信用），但可以在专用的时间窗口内运行以接收消息

## 10.6.6 Communication configuration and management

适用于本国际标准规定的通信配置文件的 COSEM interface classes：

- **DLMS server M-Bus port setup** (class_id = 76)，参见 DLMS UA 1000-1 Part 2 Ed.17:2025, 4.8.6
- **M-Bus slave port setup** (class_id = 25)，参见 DLMS UA 1000-1 Part 2 Ed.17:2025, 4.8.2
- **wireless mode Q channel** (class_id = 73)，参见 DLMS UA 1000-1 Part 2 Ed.17:2025, 4.8.4
- **M-Bus diagnostic** (class_id = 77)，参见 DLMS UA 1000-1 Part 2 Ed.17:2025, 4.8.7

**NOTE**: "wireless mode Q channel" 是 COSEM 接口对象名称，用于配置无线 M-Bus 的 Q 通道（用于传输额外数据，如诊断信息），**不是**指 EN 13757-4 中定义的无线模式（S/T/R/C/N/F）

## 10.6.7 M-Bus frame structures, addressing schemes and examples

### 10.6.7.1 General

本子条款给出了有线和无线 M-Bus 的帧结构和相关寻址方案的概述和示例。

对于传输 xDLMS APDU（Data），只有提供 DLMS/COSEM 应用进程寻址的源 SAP 和目的 SAP 是相关的。M-Bus 帧的所有其他字段由传输层和底层处理。因此，本子条款 10.6.7 是**信息性的**。

### 10.6.7.2.1 Wired M-Bus

有线 M-Bus 是主/从总线，将单个 master 与多个 slave 连接。

master 可以通过 Link Layer Address（LLA，应用于 master 消息的链路层）或 Application Layer Address（ALA）对 slave 寻址。由于 Link Layer Addresses 的范围有限（参见 Table 122），必须在 slave 安装期间分配 LLA。这可以通过服务技术人员手动完成，也可以从 master 自动完成。

ALA 应由制造商分配，并且应在全球范围内唯一（但是，运营商可以在系统内更改它）。

对于通过 ALA 寻址，master 必须发送包含 slave ALA 的特殊 Select-Command。此类 Select-Commands 可以应用通配符。在这种情况下，不会、一个或多个 slave 将被选中。通配符可用于搜索（测试）连接到 M-Bus 的 slave（参见 EN 13757-7:2018 中的 Wild Card Search）。一旦检测到 slave，master 就可以为其分配唯一的 LLA。

这证明了为什么 slave 必须在 long transport layer 中应用 ALA，以便 master 进行自动分配。

对于手动执行的分配，在 slave 响应中使用带有 simple wrapper 的帧就足够了。master 通常应应用带有 simple wrappers 的帧。

**NOTE 1**: 在地址分配过程之后，可以选择禁用 long TL。

### 10.6.7.3 Wireless M-Bus

#### 10.6.7.3.1 Extended Link Layer

对于通过无线 M-Bus 的通信，在每个传输的消息中应使用 short 或 long Extended Link Layer。

无线 M-Bus 使用 access Number (ACC) 来标识旧或新的 M-Bus 帧类型（参见 EN 13757-7:2018, 7.5.5）。因此，无线 M-Bus 协议应为 master 和 slave 提供 access number。需要额外的链路控制位，例如，发出 server 可用性的信号。Extended Link Layer 类型提供不同的服务（参见 EN 13757-4:2013, 12.2）。

电池供电的 slave 无法为 master 提供无限的可用性。这就是为什么访问时间由 slave 自己控制的原因。因此，slave 周期性发送未请求的消息。master 可以在这种传输之后访问 slave。对于在 master 和 server 之间传输的所有帧，应应用 LLA 和 ELLA。对于 slave 的未请求传输，仅需要 LLA。

Figure 189、Figure 190 和 Figure 191 显示了根据 EN 13757-4:2013 的无线 M-Bus 帧结构，并应用了 EN 13757-4:2013, 11.3 中规定的 frame format A。

**帧结构类型**:
- Figure 189: Wireless M-Bus frame structure with short ELL, no M-Bus data header
- Figure 190: Wireless M-Bus frame structure with long ELL, no M-Bus data header
- Figure 191: Wireless M-Bus frame structure with long ELL and long M-Bus data header

**NOTE**: Link Layer Address (LLA) 始终包含发射器地址（无线电单元）。Extended Link Layer Address (ELLA) 始终包含接收器地址。

ELLA 仅在具有 long Extended Link Layer 的帧中可用。

Application Layer Address (ALA) 始终包含 meter application address（slave 实体的 M-Bus 地址）。ALA 仅在 CI-Fields 指示 M-Bus long data header 的帧中可用。

没有或带有 short transport layers 的帧仅在 LLA 足够时使用。

链路层地址（LLA + ELLA）是由制造商分配的硬编码地址，运营商不应更改。如果使用外部通信适配器，则需要将 ALA 分配给此适配器。

#### 10.6.7.3.2 Transport layer

当多个 slave 共享一个无线电单元时，ELLA 不足以明确寻址专用的 slave。在这种情况下，应使用 long transport layer。ALA 包含预期的 slave 地址。

否则，可以使用 C_ITL 字段值为 0x10 的 wrapper（但不使用分段）。

### 10.6.7.4 Encoding example: Data-Notification carrying daily billing data

#### 10.6.7.4.1 Overview

本子条款显示了 server 使用 "Compact data" interface class 发送每日账单数据的编码示例。数据使用 DataNotification 服务（参见 9.3.10）通过无线 M-Bus 使用 frame format A 推送。

"Compact data" interface class（参见 DLMS UA 1000-1）允许将元数据与数据分离。这大大减少了开销，这对于电池供电设备和受限的通信介质（如无线 M-Bus）至关重要。

该示例还显示了与底层相关的信息编码。

#### 10.6.7.4.2 Example: Daily billing data

Table 127 显示了捕获到 "Compact data" 对象的 compact_buffer 属性的每日账单数据以及 template_id。

**Table 127 - Example: Daily billing data**

| Data | class_id | Logical name attribute_id | Size (bytes) | Type |
|------|----------|--------------------------|--------------|------|
| Template_id | 62 | 0-0:66.0.0.255 | 4 | 1 unsigned |
| Unix time | 1 | 0-0:1.1.0.255 | 2 | 4 double-long-unsigned |
| Operating status | 1 | 0-0:96.5.0.255 | 2 | 1 unsigned |
| Error register | 1 | 0-0:97.97.0.255 | 2 | 1 unsigned |
| Total index | 3 | 7-0:13.83.1.255 | 2 | 4 double-long-unsigned |
| Index F1 | 3 | 7-0:13.83.1.255 | 2 | 4 double-long-unsigned |
| Index F2 | 3 | 7-0:13.83.1.255 | 2 | 4 double-long-unsigned |
| Index F3 | 3 | 7-0:13.83.1.255 | 2 | 4 double-long-unsigned |
| Activity calendar name | 20 | 0-0:13.0.0.255 | 2 | 6 octet-string |
| Event counter | 1 | 0-0:96.15.1.255 | 2 | 2 long-unsigned |

在这里，开销仅为 1 字节，标识模板。

Figure 192 显示了在不应用加密和应用认证加密的情况下传输 Data-Notification APDU 的示例。

## 10.6.8 Message sequence charts

本子条款显示了一些使用有线和无线 M-Bus 配置文件在 DLMS client 和 server 之间通信的消息序列图。它们代表了根据头和有线/无线连接的所有 C_ITL 值地址结构选项。

- **Figure 193**: MSC for the COSEM-OPEN service for wired M-Bus, none M-Bus header
- **Figure 194**: MSC for the GET service for wired M-Bus, none M-Bus header

## Note

### 绿皮书的定位

- 绿皮书是 DLMS/COSEM 应用层规范
- 它定义的是如何在 M-Bus 之上运行 DLMS/COSEM 协议
- 它关注的是：应用层寻址、服务映射、消息序列、安全机制等
- **绿皮书不规定具体使用哪种无线模式（S/T/R/C/N/F）**

### 模式选择的职责

- 具体使用哪种无线模式是物理层和链路层的选择
- 这取决于：项目需求（电池寿命、数据速率、覆盖范围）、法规要求（频段、占空比）、设备能力、网络拓扑（walk-by/drive-by/固定网络）
- 应在项目规范（project-specific companion specification）中定义

### 与 GCP 的区别

- **GCP (Generic Companion Profile)** 明确指定了必须支持的无线模式（Mode C1/C2, T1/T2）
- **绿皮书** 是模式无关的（mode-agnostic），不指定具体模式
- 绿皮书只规定必须遵循 EN 13757-4 标准，以及如何使用 M-Bus 帧结构承载 DLMS/COSEM APDU
