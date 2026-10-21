---
name: wmbus-annexes
description: EN 13757-4 附录：帧格式示例(Mode S/T/C)、同步消息预测接收、时序图总结、频率分配(868/169 MHz)、AES CTR加密流程
metadata: 
  node_type: memory
  type: reference
  originSessionId: 2ca42aff-aa3e-4575-bd3b-6396bb2d3bee
  modified: 2026-09-24T03:58:03.230Z
---

# EN 13757-4 附录

## Annex A — 868 MHz 频段频率分配

| 频率范围 | 带宽 | 占空比 | 应用 | 功率 |
|----------|------|--------|------|------|
| 868.0~868.6 MHz | 25kHz ch | <1% | SRD | 25 mW |
| 868.6~868.7 MHz | — | <10% | ALARMS | 500 mW |
| 868.7~869.2 MHz | 25kHz ch | 100% | SRD | 500 mW |
| 869.2~869.3 MHz | 25kHz ch | T.b.d | SRD | 25 mW |
| 869.3~869.4 MHz | — | <10% | ALARMS | 100 mW |
| 869.4~869.65 MHz | 25kHz ch | 100% | SRD | 10 mW |
| 869.65~869.7 MHz | — | 100% | SRD | 5 mW |
| 869.7~870 MHz | — | — | ALARMS | 25 mW |

## Annex B — 169 MHz 频段频率分配

- 频段: 168.4~168.8 MHz
- 功率范围: 10~20 dBm
- An1-f3 频段不允许 concentrators
- 占空比: 0.1%（当地时间 00:00-06:00）
- 应用: Assistive Listening Device, Metering Device, Non-specific SRD

---

## Annex C — 帧格式示例

### C.1 Mode S 帧示例

**条件:**
- L = 15 bytes (9+6)
- C-field = 44h (SND-NR, Meter initiative)
- 制造商 "CEN" = 0CAEh
- 制造商编号 = 12345678h, Ver=01h, DevType=07h
- 数据: volume = 876543 litres
- DIF = 0Bh (6 digit BCD instantaneous)
- VIF = 13h (volume in litres)

**Block 1 (10+2 CRC):**
Hex (无CRC): `0F 44 AE 0C 78 56 34 12 01 07`

**Block 2 (1+5+2 CRC):**
Hex (无CRC): `78 0B 13 43 65 87`
- CI=78h (无header, 直接VIF/DIF)
- 数据 BCD LSB first: 43 65 87 → 876543

**完整 hex:**
```
0F 44 AE 0C 78 56 34 12 01 07 44 47 78 0B 13 43 65 87 1E 6D
```

**Manchester 编码:** 每 bit→2 chips, 总 898 chips
Mode S1 chip rate: 32,768 kcps → 传输持续 **27.4 ms**

### C.2 Mode T1 帧示例

**条件:** 同 C.1

**完整 hex (与 Mode S 相同数据):**
```
0F 44 AE 0C 78 56 34 12 01 07 44 47 78 0B 13 43 65 87 1E 6D
```

**6-chip per nibble 编码:** 总 290 chips
Mode T1 chip rate: 100 kcps → 传输持续 **2.9 ms**

### C.3 Mode C1 帧示例 (Format B, 带 ELL)

**条件:**
- Format B, CI = 8Ch
- L = 20 bytes
- C-field = 44h
- CC = 20h (同步帧), ACC = 27h
- 帧 < 128 bytes → 单个块

**完整 hex:**
```
14 44 AE 0C 78 56 34 12 01 07 8C 20 27 78 0B 13 43 65 87 7A C5
```

**NRZ 编码, MSB first:** 总 232 chips
Mode C1 chip rate: 100 kcps → 传输持续 **2.32 ms**

### 帧示例对比

| 特性 | Mode S | Mode T1 | Mode C1 |
|------|--------|---------|---------|
| 编码 | Manchester | 3-out-of-6 | NRZ |
| Chip rate | 32,768 kcps | 100 kcps | 100 kcps |
| 块结构 | 两块(各带CRC) | 两块(各带CRC) | 一块(<128B) |
| 总 chips | 898 | 290 | 232 |
| 传输时间 | 27.4 ms | 2.9 ms | 2.32 ms |
| ELL | 否 | 否 | 是(CC+ACC) |

---

## Annex D — 同步消息预测接收

**目的:** 预测同步表计的下一次传输时间

**推荐方法:** 连续接收 6 个标称传输间隔

**计算示例:**
- 收到 access number 110 和 112 的同步消息
- 时间差: t_110 + t_111 = 1661.563 s
- 错过一条消息（112-110=2）
- 计算标称间隔:
  ```
  t_nom = 1661.563 × 2048 / (2048-46+2048-47) = 850.083 s
  ```
- 整数因子 n = 425
- 下一个传输间隔:
  ```
  t_112 = (1 + (|112-128|-64)/2048) × 850.083 = 830.159 s
  ```

**关键:** 每次收到新同步消息可重新计算 t_nom，补偿**温度漂移**

---

## Annex E — 时序图总结

### 关键缩写
| 缩写 | 含义 |
|------|------|
| LLA | Link Layer Address |
| ALA | Application Layer Address |
| MTR | Meter(表计) |
| COM | Data Collector/Concentrator |
| OMC | Other Meter Collector |
| RFA | Radio Frequency Adapter |

### C-field 命令值汇总

| C-field | 命令 | 说明 |
|---------|------|------|
| 44h | SND-NR | Send No Reply |
| 45h | SND-UD3 | Send User Data 3（多播/广播） |
| 46h | SND-IR | Send Installation Request |
| 43h | SND-UD2 | Send User Data 2（需要立即响应） |
| 40h | SND-NKE | 传输结束 |
| 06h | CNF-IR | Confirm Installation |
| 00h | ACK | 确认 |
| 08h | RSP-UD | Response User Data |
| 5Bh | REQ-UD2 | Request User Data 2 |
| 73h | SND-UD | Send User Data (Other Device→Meter) |

### Figure E.2 — 安装时序
1. Meter 发送 SND-IR (C=46h, CI=7Ah)，带 ACC
2. 周期重复直到收到 CNF-IR 或超时
3. Other Device 接受并响应 CNF-IR (C=06h)
4. 表计收到确认后停止
5. Other Device 随机延迟后发送 Link-reset

### Figure E.3/E.4 — 长短传输层连接
1. Meter: SND-NR (C=44h)
2. Other Device: SND-UD (C=73h)
3. 每次发送后短暂接收窗口 (B=1, A=0)
4. ACK (C=00h) 预定义延迟响应
5. REQ-UD2 (C=5Bh) 读取结果
6. RSP-UD (C=08h) 提供响应
7. SND-NKE (C=40h) 结束

### Figure E.5 — 超时与频繁访问周期
- 成功收到第一个消息后启动连接超时
- 收到第二个消息后超时重启
- 无更多消息时按预定义延迟周期重复最后消息
- 超时停止重复 → FAC 结束

### Figure E.6 — 访问需求
1. Meter 有告警 → 生成访问需求
2. 重复发送直到确认
3. 可通过非周期性传输加速
4. ACD-bit 在 C-field 中通知有 Class 1 告警数据

### Figure E.7 — RF连接(SND-UD2)
- SND-UD2 (C=43h) 用于需要立即响应的场景
- Meter 可直接用期望响应代替 ACK
- SND-NKE (C=40h) 关闭链路

### Figure E.8~E.10 — Mode C 专用
- 使用 Frame Number (FN) 用于 AES 加密
- 支持 Repeater 传输（H=Hop Count）
- 快速/慢速响应延迟由 D-field 控制

### Figure E.11 — 快速/慢速响应延迟
| D值 | 行为 |
|-----|------|
| D=1 | 快速时序 |
| D=0 | 慢速时序 |

- Meter 发送帧时可指定 D=1
- 收到 D=0 后必须使用 D=0
- 收到 D=1 后可自由选择
- Other Device 始终可自由选择

### Figure E.12/E.13 — 多播(SND-UD3)三阶段

**1. Preparation（逐表计）:**
- Meter: SND-NR (C=44h)
- Other Device: SND-UD2 (C=43h) 发送 Prepare 命令
- Meter: RSP-UD (C=08h) 响应
- Other Device: SND-NKE (C=40h) 关闭

**2. Transfer Synchronization（广播）:**
- Other Device: SND-UD3 (C=45h) 广播 Synchronize，含倒计时 t_countdown
- 有精确时钟的 Meter 可跳过, 仅在传输开始时开启接收器
- 其他 Meter 需宽接收窗口捕获同步

**3. Image Transfer（广播）:**
- Other Device 以 t_pace 间隔广播 Transfer
- 未收到的 Meter 记录丢失，继续尝试后续帧
- SND-NKE (C=40h) 终止传输

---

## Annex F — AES-128 Counter Mode 加密流程

### 加密
```
Nonce + Counter → AES Block Cipher(Key) → Keystream
Plaintext XOR Keystream → Ciphertext
```

### 解密
```
Nonce + Counter → AES Block Cipher(Key) → Keystream
Ciphertext XOR Keystream → Plaintext
```

### Initial Counter Block 构成 (16 bytes)
| M(2B) | A(6B) | CC(1B) | SN(4B) | FN(2B) | BC(1B) |
- Nonce = M + A + CC + SN (前 13 bytes)
- Counter = FN(2B) + BC(1B) (3 bytes)
- Counter 每加密 16 bytes (一个 block) 递增

## CRC 多项式 (FT3 per EN 60870-5-1)

**数据链路层 CRC:**
```
x^16 + x^13 + x^12 + x^11 + x^10 + x^8 + x^6 + x^5 + x^4 + x^2 + 1
```
初始值=0, MSB first, 结果取反

**Payload CRC (扩展链路层):**
```
x^16 + x^13 + x^12 + x^11 + x^10 + x^8 + x^6 + x^5 + x^2 + 1
```
初始值=0, 结果取反

注意: 两个 CRC 多项式不同（数据链路层多了 x^4 项）
