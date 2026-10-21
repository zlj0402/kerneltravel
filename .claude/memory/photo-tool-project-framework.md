---
name: photo-tool-project-framework
description: 照片处理工具项目的完整技术方案、开发计划和架构设计
metadata:
  node_type: memory
  type: project
  originSessionId: 101cb426-54d7-4316-9465-630368a35216
  modified: 2026-10-20T02:51:03.007Z
---

# 照片处理工具项目 - 技术方案与开发计划

## 项目概述

纯前端 + Python 后端的照片处理 SaaS 工具，支持裁剪、缩放、格式转换、压缩等基础功能，后期扩展去水印、智能抠图等 AI 功能。

## 核心原则

- **前端优先**：能用前端做的，绝不放后端（省服务器资源）
- **后端兜底**：前端做不了的（DPI 修改、批量处理、AI），后端处理
- **渐进式扩展**：先做 MVP，验证用户需求后再加 AI 功能

## 功能需求

### Phase 1：基础功能（MVP，快速上线）

| 功能 | 优先级 | 实现位置 | 说明 |
|------|--------|----------|------|
| 图片上传 | P0 | 前端 | 支持拖拽、点击上传 |
| 图片预览 | P0 | 前端 | 显示原图和基本信息 |
| 裁剪 | P0 | 前端 | 支持自定义比例、自由裁剪 |
| 缩放 | P0 | 前端 | 按尺寸/百分比缩放 |
| 格式转换 | P0 | 前端 | JPEG/PNG/WebP 互转 |
| 质量压缩 | P1 | 前端 | 调整压缩质量 |
| 文件大小控制 | P1 | 前端/后端 | 压缩到指定 KB/MB 以内 |
| 背景色设置 | P1 | 前端 | PNG 透明转纯色背景 |
| DPI 设置 | P1 | 后端 | 修改图片 DPI（证件照需要） |
| 下载 | P0 | 前端 | 下载处理后的图片 |

### Phase 2：高级功能（验证需求后扩展）

| 功能 | 优先级 | 实现位置 | 说明 |
|------|--------|----------|------|
| 批量处理 | P2 | 后端 | 批量转换/压缩多张图片 |
| 去水印 | P2 | 后端 | AI 算法去除水印 |
| 智能抠图 | P2 | 后端 | AI 去除背景 |
| 图片增强 | P3 | 后端 | 锐化、降噪、色彩调整 |
| 证件照模板 | P3 | 前端 | 一寸/二寸/签证照预设 |

### Phase 3：运营功能（商业化）

- 用户系统：登录/注册、历史记录
- 积分/付费：免费次数 + 付费套餐
- CDN 加速：国内访问优化
- 数据统计：使用量、转化率

## 技术架构

### 前端技术栈

```
核心框架：Vue 3（CDN 引入，无需构建）
裁剪库：Cropper.js
HTTP 请求：Axios
样式：Tailwind CSS（快速开发）
部署：静态文件，Nginx 托管
```

**为什么用 Vue 3 CDN 而不是完整项目？**
- 无需 Node.js 构建工具
- 开发速度快
- 适合单页面应用
- 后续可迁移到完整项目

### 后端技术栈

```
框架：FastAPI（现代、高性能）
图像处理：Pillow（基础）+ OpenCV（高级）
AI 功能：rembg（抠图）、opencv-inpaint（去水印）
部署：Docker + Uvicorn
```

**为什么用 FastAPI？**
- 性能好（异步）
- 自动生成 API 文档
- 类型安全
- 易于扩展 AI 功能

### 服务器部署

```
容器：Docker + Docker Compose
反向代理：Nginx
HTTPS：Let's Encrypt
服务器：阿里云（2C2G）
域名：暂时不急，先用服务器 IP 访问
```

## 项目目录结构

```
photo-tool/
├── frontend/                    # 前端代码
│   ├── index.html              # 主页面
│   ├── css/
│   │   ├── style.css           # 自定义样式
│   │   └── tailwind.css        # Tailwind
│   ├── js/
│   │   ├── app.js              # Vue 主应用
│   │   ├── imageProcessor.js   # 前端图像处理
│   │   ├── cropper.js          # 裁剪逻辑
│   │   └── api.js              # 后端 API 调用
│   └── assets/
│       ├── logo.png
│       └── favicon.ico
│
├── backend/                     # 后端代码
│   ├── app/
│   │   ├── __init__.py
│   │   ├── main.py             # FastAPI 入口
│   │   ├── config.py           # 配置（环境变量）
│   │   │
│   │   ├── routers/            # API 路由
│   │   │   ├── __init__.py
│   │   │   ├── image.py        # 基础图像处理
│   │   │   └── ai.py           # AI 功能（后期）
│   │   │
│   │   ├── services/           # 业务逻辑
│   │   │   ├── __init__.py
│   │   │   ├── processor.py    # 图像处理服务
│   │   │   ├── converter.py    # 格式转换
│   │   │   ├── compressor.py   # 压缩优化
│   │   │   └── ai_service.py   # AI 服务（后期）
│   │   │
│   │   └── utils/              # 工具函数
│   │       ├── __init__.py
│   │       └── file.py         # 文件操作
│   │
│   ├── tests/                  # 测试
│   │   └── test_image.py
│   │
│   ├── requirements.txt        # Python 依赖
│   └── Dockerfile              # 后端容器
│
├── nginx/
│   ├── nginx.conf              # Nginx 配置
│   └── ssl/                    # SSL 证书
│
├── docker-compose.yml           # 容器编排
├── .env                         # 环境变量
├── .gitignore
└── README.md                    # 项目说明
```

## 开发计划

### Week 1：基础功能（Phase 1）

| 天 | 任务 | 产出 |
|----|------|------|
| Day 1 | 前端框架搭建 + 上传预览 | 可上传、显示图片 |
| Day 2 | 集成 Cropper.js | 实现裁剪功能 |
| Day 3 | 格式转换 + 压缩 | JPEG/PNG/WebP 转换 |
| Day 4 | 文件大小控制 + 背景色 | 压缩到指定大小 |
| Day 5 | 后端搭建 + DPI 设置 | 后端 API 可用 |
| Day 6 | 前后端联调 | 完整流程跑通 |
| Day 7 | 测试 + Bug 修复 | MVP 上线 |

### Week 2：优化 + 部署

| 天 | 任务 |
|----|------|
| Day 8 | Docker 化部署 |
| Day 9 | 域名 + HTTPS |
| Day 10 | 性能优化（压缩、缓存） |
| Day 11 | 移动端适配 |
| Day 12 | 统计代码接入 |
| Day 13-14 | 灰度测试 + 正式上线 |

### Week 3+：高级功能（Phase 2）

- 去水印功能
- 智能抠图
- 批量处理
- 证件照模板

## 关键决策

- **域名**：暂时不急，先用服务器 IP 访问
- **服务器**：阿里云（2C2G）
- **用户系统**：MVP 不做，后期再加
- **开发顺序**：按建议顺序（基础功能 → 优化部署 → 高级功能）

## 相关链接

- 相关记忆：[[saas-photo-tool-project]]（原始项目规划）
