---
name: wmbus-extended-link-layer
description: EN 13757-4 扩展链路层(ELL)、CI-field分配、CC-field位定义、会话号、AES-128加密、管理功能(Set/Get/Confirm Radio Parameters)
metadata: 
  node_type: memory
  type: reference
  originSessionId: 2ca42aff-aa3e-4575-bd3b-6396bb2d3bee
  modified: 2026-09-24T03:57:09.499Z
---

# EN 13757-4 扩展链路层与管理功能

## CI-Field 分配 (数据链路层之后的第一字节)

| CI值 | 名称 | TPL-Header | 说明 |
|------|------|------------|------|
| 80h | Transport Layer to device | Short | EN 13757-3 协议(如REQ-UD2) |
| 82h | Network mgmt to device | Short | 网络管理命令(Clause 14) |
| 83h | Network mgmt to device | None | 网络管理命令 |
| 86h | Variable Extended Link Layer | 可变长度 | 可选服务 |
| 87h | Network mgmt from device | Long | 网络管理响应 |
| 88h | Network mgmt from device | Long | 网络管理响应 |
| 89h | Network mgmt from device | None | 网络管理响应 |
| 8Ah | Transport Layer from device | Short | 如ACK |
| 8Bh | Transport Layer from device | Long | 如ACK |
| 8Ch | ELL, 无加密 | 2 bytes | CC + ACC |
| 8Dh | ELL, 有加密 | 8 bytes | CC + ACC + SN + PayloadCRC |
| 8Eh | ELL, 无加密, 含目标地址 | 10 bytes | CC + ACC + M2 + A2 |
| 8Fh | ELL, 有加密, 含目标地址 | 16 bytes | CC + ACC + M2 + A2 + SN + PayloadCRC |

## 扩展链路层 (ELL) 结构

### CI = 8Ch（无加密）
| CI(1B) | CC(1B) | ACC(1B) |

### CI = 8Dh（有加密）
| CI(1B) | CC(1B) | ACC(1B) | SN(4B) | PayloadCRC(2B) |

### CI = 8Eh（无加密, 含目标地址）
| CI(1B) | CC(1B) | ACC(1B) | M2(2B) | A2(6B) |

### CI = 8Fh（有加密, 含目标地址）
| CI(1B) | CC(1B) | ACC(1B) | M2(2B) | A2(6B) | SN(4B) | PayloadCRC(2B) |

### CI = 86h（Variable ELL）
始终存在的字段: CI(1B), CC(1B), ACC(1B), ECL(1B)
可选字段由 ECL 控制:

**ECL (Extended Link Layer Control, 1 byte):**

| Bit | 名称 | 说明 |
|-----|------|------|
| 7 | PLCP | Payload CRC field 存在 |
| 6-5 | — | Reserved (=0) |
| 4 | RXLP | Reception Level 存在 |
| 3-2 | RTD | 00=无; 01=有,分辨率1/256s; 10=有,分辨率2s |
| 1 | SNP | SN field 存在 |
| 0 | MAP | M2 和 A2 field 存在 |

可选字段: M2(2B), A2(6B), SN(4B), RTD(2B), RXL(1B), PayloadCRC(2B)

## CC-Field (Communication Control, 1 byte)

| Bit7 | Bit6 | Bit5 | Bit4 | Bit3 | Bit2 | Bit1 | Bit0 |
|------|------|------|------|------|------|------|------|
| B | D | S | H | P | A | R | X |

- **B (bit7)**: 双向 — 同数据链路层 B-bit
- **D (bit6)**: 响应延迟 — 与 X 位组合决定延迟类型
- **S (bit5)**: 同步 — S=1 为同步帧
- **H (bit4)**: Hop Count — H=0 直接源; H=1 经中继
- **P (bit3)**: 优先级 — P=1 高优先级(仅告警/不频繁数据)
- **A (bit2)**: 可达性 — 同数据链路层 A-bit
- **R (bit1)**: Repeated Access — 用于重复消息(EN 13757-5)
- **X (bit0)**: 扩展延迟 — 与 D 位组合

### 响应延迟组合

| D(bit6) | X(bit0) | 延迟类型 |
|---------|---------|----------|
| 0 | 0 | Slow delay |
| 1 | 0 | Fast delay |
| 0 | 1 | N模式=Extended delay; 其他=RFU |
| 1 | 1 | RFU |

Meter 应在下一帧中返回收到的 D/X 设置。固定网络推荐 D=0; 其他推荐 D=1。

## Session Number Field (SN, 4 bytes)

| Bit 31-29 | Bit 28-4 | Bit 3-0 |
|-----------|----------|---------|
| ENC-field | Time-field | Session-field |

### ENC (3 bits) — 加密选择
| 值 | 方式 |
|----|------|
| 000b | 无加密 |
| 001b | AES-128 Counter Mode |
| 010b~111b | 保留 |

### Time (25 bits)
- 相对分钟计数器
- 最大表示约 **64年**

### Session (4 bits)
- 分钟内的会话编号
- 每分钟最多 **16个** 双向会话
- 每分钟第一个会话 session=0

### 会话规范
- Meter 在每次 SND-NR 或 ACC-NR 中指定新 Session Number
- 双向通信期间 Session Number 不变
- Other Device→Meter 帧中 SN 必须与 Meter 定义的初始 SN 相同
- Payload 未变时 SN 可复用
- SN 至少每 **300秒** 更新一次

## AES-128 Counter Mode 加密

- ENC=001b 时使用标准 AES-128 CTR
- 参考: ISO/IEC 18033-3, NIST SP 800-38A, Annex F
- 不需要数据长度为16的倍数（无额外 padding）
- 加密范围: 从 PayloadCRC-field 到帧末尾（不含链路层 CRC）

### Initial Counter Block (16 bytes)

| M-field(2B) | A-field(6B) | CC-field(1B) | SN-field(4B) | FN(2B) | BC(1B) |
|-------------|-------------|-------------|-------------|--------|--------|

- **FN (Frame Number)**: 会话内帧编号，新 session 重置，FAC 重传时不变
- **BC (Block Counter)**: 帧内加密块编号，每帧从 0 开始
- M-field 和 A-field 包含发送方地址（上下行不同）
- R-field 和 H-field 在 Initial Counter Block 中始终设为 0

### 加密/解密流程
```
加密: Plaintext XOR AES(Key, Nonce||Counter) → Ciphertext
解密: Ciphertext XOR AES(Key, Nonce||Counter) → Plaintext
```
(CTR mode 特性: 加密和解密使用相同操作)

## RTD-Field (Run Time Delay, 可选 2 bytes)

- 无符号整数
- 分辨率: "2s" = 值×2秒; "1/256s" = 值×3.90625毫秒
- 首次传输设为 0
- 每次中继累加延迟（接收最后sync word到重传最后sync word）
- 溢出保存最大值 FFFFh 或选择更高分辨率
- 更新 RTD 时须重新计算 CRC
- RTD 默认不受完整性保护

## RXL-Field (Reception Level, 1 byte)

| Bit7 | Bit6 | Bit5-0 |
|------|------|--------|
| 0=信号电平 | 0=RSSI; 1=Link Margin | RL(0~63) |

### RSSI 编码
| RL值 | 含义 |
|------|------|
| 0 | 无RSSI信息或有线 |
| 1 | ≤-142 dBm |
| 2~62 | -144+(2×RL) dBm (范围 -140~-20 dBm) |
| 63 | ≥-18 dBm |

### Link Margin 编码
| RL值 | 含义 |
|------|------|
| 0 | 无信息或有线 |
| 1 | ≤-10 dB |
| 2~62 | (-11+RL) dB (范围 -9~+51 dB) |
| 63 | ≥+52 dB |

## Payload CRC-Field

- CRC 覆盖帧其余部分（不含链路层 CRC）
- 多项式: x^16 + x^13 + x^12 + x^11 + x^10 + x^8 + x^6 + x^5 + x^4 + x^2 + 1
- 初始值: 0, 最终取反

## Transport Layer（传输层）

### Short Transport Layer (CI=8Ah)
| CI(1B) | ACC(1B) | STS(1B) | Conf.Field(2B) |

### Long Transport Layer (CI=8Bh 或 80h)
| CI(1B) | Ident.no(4B) | Manuf(2B) | Ver(1B) | DevType(1B) | ACC(1B) | STS(1B) | Conf.Field(2B) |

- CI=8Bh: Meter→Other Device
- CI=80h: Other Device→Meter

---

## 链路控制管理功能 (Clause 14)

使用 CI-field: 82h, 83h, 87h, 88h, 89h

| Function Field | 服务 |
|---------------|------|
| 40h | Set Radio Parameters |
| 41h | Get Radio Parameters |
| 42h | Set Limited Radio Parameters |
| 43h | Confirm Radio Parameters |
| 44h~4Eh | Reserved |
| 4Fh | Manufacturer Specific |

### Set Radio Parameters (40h)

**命令格式:**
| F(40h) | SF(1B) | SF2(1B) | Activation time(5B) | Fall-back time(5B) | 条件字段... |

**SF field 位定义:**
| Bit7 | Bit6 | Bit5 | Bit4 | Bit3 | Bit2 | Bit1 | Bit0 |
|------|------|------|------|------|------|------|------|
| MO | FR | PWR | INT | UPCH | DNCH | MAN | RFU |

每位指示对应条件字段是否存在。

**条件字段:**
- Mode (1B): 00h=R, 01h=S, 02h=T, 03h=C, 04h=N, 05h=F
- Frame format (1B): 00h=A, 01h=B
- Tx-power (1B): 00h=Max, 01h~04h=Level1~4(约每级-6dB), 10h=不变, 11h=增加, 12h=减少
- Tx-interval (2B): 标称传输间隔, 增量 2s
- Up-Channel (2B): 字节1=sub-mode, 字节2=channel
- Down-Channel (2B): 同上
- Manuf (3~n B): Man.ID(2B) + Len(1B) + data(nB)

**Activation time**: 激活时间（相对时间），值为0或过去日期=当前会话结束后执行
**Fall-back time**: Gateway 验证切换时间，未收到 Confirm 则回退

**响应格式:**
| F(40h) | SF(1B) | (Power level)(1B) | (Error report)(3B) | (Manuf error) |

### Get Radio Parameters (41h)
- 命令: F(41h) + SF(dummy=00h)
- 响应: 返回当前/待生效的所有参数

### Set Limited Radio Parameters (42h)
- 受限版本，Mode 和 Frame format 不可更改
- SF 中 MO 和 FR 位不能设置

### Confirm Radio Parameters (43h)
- 确认新参数设置成功
- 可禁用或延长 fall-back timer
- SF bit0(TIM): 0=无fall-back time; 1=帧中包含新Fall-back time

### Manufacturer Specific (4Fh)
| F(4Fh) | SF(1B) | Man.ID(2B) | Len(1B) | Man.data(nB) |
