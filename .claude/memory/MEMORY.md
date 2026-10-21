# Memory Index

## Wireless M-Bus (EN 13757-4:2019)

- [标准概述](wmbus-standard-overview.md) — EN 13757 系列、范围、术语、缩写、版本变更
- [操作模式](wmbus-operation-modes.md) — 六种模式(S/T/R/C/N/F)详细参数：频率、编码、速率、响应延迟、性能等级
- [数据链路层](wmbus-data-link-layer.md) — 帧格式A/B、字段定义(L/C/M/A/CI/CRC)、C-field命令码、时序(FAC/同步/安装)、FEC
- [扩展链路层](wmbus-extended-link-layer.md) — ELL结构、CI-field分配、CC-field位定义、Session Number、AES-128加密、管理功能
- [传输层](wmbus-transport-layer.md) — Transport Layer (TPL): Long TPL vs Short TPL 格式、使用场景、LLA与ALA关系
- [附录](wmbus-annexes.md) — 帧格式示例、同步预测、时序图总结、频率分配(868/169 MHz)、AES CTR流程、CRC多项式
- [学习指南](wmbus-learning-guide.md) — 基于GCP要求的Wireless M-Bus学习路径：Mode C/T重点、单向vs双向差异、协议栈集成
- [阅读顺序](wmbus-reading-order-guide.md) — 协议文档阅读顺序指南：从绿皮书到EN 13757-4，分阶段学习路径
- [场景A vs 场景B](mbus-scenario-a-vs-b.md) — 蓝皮书定义的两种M-Bus场景：场景A（专用M-Bus APDU，使用EN 13757标准）vs 场景B（DLMS/COSEM APDU，M-Bus透明传输）

## 应用规范与配置文件

- [GCP Wireless M-Bus 要求](gcp-wmbus-requirements.md) — ACESM GCP 中关于 Wireless M-Bus 的使用要求：强制支持 Mode C1/C2、T1/T2，帧格式 FT3，安装流程，消息缓冲要求
- [绿皮书 M-Bus Profile](greenbook-wmbus-profile.md) — DLMS/COSEM Green Book 10.6节：有线和无线 M-Bus profile 规范，三级寻址（LLA/C_ITL/SAP），服务映射，帧结构，与 GCP 的区别

## 个人 SaaS 项目规划

- [照片处理工具项目框架](photo-tool-project-framework.md) — 完整技术方案：功能需求(Phase1-3)、技术栈(Vue3+FastAPI)、项目结构、开发计划(Week1-3)、关键决策
- [SaaS 照片处理工具项目](saas-photo-tool-project.md) — 证件照/图片处理工具规划：核心功能、技术方案(纯前端/Python后端)、域名选择、统计方案、服务器部署(2C2G)、变现思路

## 核心原则

- [诚实第一](core-principle-honesty.md) — **最重要原则**：不知道就说不知道，诚实高于一切
