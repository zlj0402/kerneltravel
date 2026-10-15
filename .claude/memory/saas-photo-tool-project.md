---
name: saas-photo-tool-project
description: 个人 SaaS 项目规划：证件照/图片处理工具，纯前端方案 + 可选 Python AI 后端
metadata:
  node_type: memory
  type: project
  originSessionId: abb916dd-5930-4244-b891-f93f275c8a3d
  modified: 2026-10-15T03:09:16.382Z
---

# 个人 SaaS 项目开发规划

## 📌 项目概述

**项目名称**：证件照助手 / PhotoTool（暂定）  
**核心功能**：一站式图片处理工具，解决证件照上传的痛点需求  
**目标用户**：需要处理证件照的普通用户（办社保卡、签证、考试等）  
**技术理念**：极致简单——打开就用，无需注册，隐私安全

---

## 🎯 核心功能需求

### MVP（最小可用版本）
1. **图片格式转换**：PNG/BMP/WebP → JPG（真正重编码，不是改后缀）
2. **调整像素尺寸**：宽×高，支持锁定宽高比等比例缩放
3. **框选裁剪**：鼠标拖拽选区，支持自由框选 + 固定比例框选（如 358:441）
4. **调整文件大小**：控制压缩后大小（15KB - 10MB），通过调节 JPG quality 实现
5. **DPI 设置**：修改图片 DPI 元数据（72/96/150/300 等）
6. **背景底色**：一键换白底/蓝底/红底，或自定义颜色

### 进阶功能（第二版）
- **去除水印**：简单水印用修复笔刷，复杂水印需 AI
- **证件照模板**：预设常见尺寸（一寸、二寸、签证、社保卡、护照）
- **批量处理**：一次处理多张图片
- **历史记录**：登录后保存处理记录

---

## 🛠️ 技术方案

### 方案 A：纯前端（推荐起步 ✅）

**技术栈**：
- 前端：HTML + Tailwind CSS + 原生 JavaScript（或 Vue 3）
- 图片处理：
  - Fabric.js（画布操作、框选裁剪）
  - pica（高质量缩放）
  - browser-image-compression（压缩控制大小）
  - Canvas API（格式转换、DPI 修改）

**核心原理**：
```javascript
// 格式转换（Canvas API）
const canvas = document.createElement('canvas');
canvas.width = img.width;
canvas.height = img.height;
const ctx = canvas.getContext('2d');
ctx.drawImage(img, 0, 0);
canvas.toBlob(blob => {
    download(blob, 'output.jpg');
}, 'image/jpeg', 0.9);
```

**优点**：
- ✅ 2C2G 服务器毫无压力（纯静态，几乎不消耗资源）
- ✅ 用户图片不上传服务器，隐私安全
- ✅ 速度快 3-5 倍（本地处理，无网络延迟）
- ✅ 开发快，1-2 周出 MVP

**缺点**：
- ❌ AI 功能做不了（去水印、智能抠图需要后端模型）
- ❌ 无法统计用户行为（需要加统计接口）

**浏览器支持的格式**：
- ✅ JPEG、PNG、WebP、BMP、ICO（原生支持）
- ⚠️ GIF（只能导出静态帧）
- ❌ TIFF（需要第三方库，证件照用不到）

---

### 方案 B：前后端结合（进阶）

**技术栈**：
- 前端：Vue 3 + Tailwind
- 后端：Python FastAPI
- 图片处理：
  - 简单操作（裁剪、缩放、格式转换）→ 前端做
  - 复杂操作（AI 去水印、智能抠图）→ 后端做

**后端库**：
- Pillow (PIL)：Python 图像处理标准库
- OpenCV：高级处理（去水印、抠图）
- rembg：开源 AI 去背景模型

**优点**：
- ✅ 能做 AI 功能
- ✅ 处理大文件不卡浏览器

**缺点**：
- ❌ 2C2G 吃紧（特别是 AI 模型）
- ❌ 用户担心隐私（图片上传到服务器）

---

### 折中方案（推荐 ✅）

**先做方案 A（纯前端），后续加后端做 AI**：
1. Week 1：纯前端 MVP 上线
2. Week 2：收集用户反馈
3. Week 3-4：加 Python 后端做 AI 功能（rembg 去背景、OpenCV 去水印）

---

## 🌐 域名选择

### 已查询结果
- ❌ `p2p.com`：已被注册（1995年，续费到2033年）
- ❌ `p2p.tools`：已被注册
- ❌ `pic2pic.com`：可能已注册（RDAP 无返回）
- ✅ `pic2pic.tools`：大概率可注册（nslookup 无记录）
- ✅ `img2img.tools`：大概率可注册
- ✅ `i2i.tools`：大概率可注册

### 推荐域名
1. **首选**：`pic2pic.com`（.com 最权威，需确认是否可注册）
2. **备选**：`pic2pic.tools`（工具属性，简洁）
3. **其他**：
   - `img2img.tools`（技术感强）
   - `imgtool.tools`（直白，SEO 友好）
   - `photoedit.tools`（功能明确）
   - `pixelp2p.com`（品牌感）

**价格参考**：.tools 域名约 ￥50-80/年

**注册渠道**：阿里云 https://wanwang.aliyun.com/domain

---

## 📊 使用统计方案

### 问题
纯前端 SaaS 无法统计用户行为（没有请求发到服务器）

### 解决方案

#### 1. 简单统计接口（推荐起步）
```python
# FastAPI 后端
@app.post("/api/stats")
async def record_stats(request: Request):
    data = await request.json()
    user_id = request.client.host
    feature = data.get("feature")
    # 记录到内存/数据库
    return {"status": "ok"}

@app.get("/api/stats/summary")
async def get_stats():
    return {
        "page_views": 1234,
        "unique_users": 567,
        "total_processed": 890,
        "feature_usage": {"crop": 234, "resize": 345}
    }
```

#### 2. 数据库持久化（生产环境）
- SQLite（单机）或 PostgreSQL（多用户）
- 记录：用户 ID、功能名称、时间戳、元数据

#### 3. 第三方分析工具（最简单）
- Google Analytics / 百度统计 / Umami
- 能看到：访问量、独立访客、用户来源、设备类型
- 自定义事件追踪功能使用

### 管理后台
- 简单 HTML 看板（Chart.js 可视化）
- 显示：总访问量、独立用户、处理图片数、功能使用分布

---

## 🖥️ 服务器规划

### 当前资源：2C2G 阿里云 ECS

**可同时运行的服务**：
- ✅ 静态博客（Hugo / Hexo）：~0 MB
- ✅ 纯前端 SaaS（证件照助手）：~0 MB
- ✅ LiteLLM（AI 网关）：~200MB
- ✅ Nginx：~10MB
- ✅ AdGuard Home（DNS）：~30MB
- ✅ Uptime Kuma（监控）：~100MB
- ✅ Vaultwarden（密码管理）：~50MB
- **总计**：~400MB，还剩 1.6GB 富余

### 系统选择

#### Ubuntu 22.04 LTS（推荐 ✅）
**优点**：
- ✅ 省资源（系统本身 ~300MB）
- ✅ 完全控制，学习价值高
- ✅ 灵活，自动化友好
- ✅ 安全可靠

**部署步骤**：
1. 系统初始化：`apt update && apt upgrade`
2. 安装 Nginx：`apt install nginx`
3. 配置多站点：blog.yourdomain.com / tool.yourdomain.com
4. 申请 SSL 证书：`certbot --nginx`

#### 宝塔面板
**优点**：
- ✅ 上手快，网页点击操作
- ✅ 一键部署 LNMP

**缺点**：
- ❌ 吃资源（面板本身 ~200MB）
- ❌ 学习价值低
- ❌ 不够灵活

**建议**：初期不用宝塔，直接 Ubuntu 原生

---

## 🔐 安全与隐私

### 纯前端方案的优势
- ✅ 用户图片不离开浏览器，隐私安全
- ✅ 无服务器存储，无泄露风险
- ✅ 用户放心上传证件照等私密照片

### 后端统计的隐私保护
- 匿名统计（用 IP 或浏览器指纹，不记录用户信息）
- 明确告知用户"此功能需要上传图片"（AI 功能）
- 处理后立即删除临时文件

---

## 💰 变现思路（可选）

1. **高级功能付费**：AI 去水印、智能抠图 ￥1-5 / 次
2. **去广告**：免费版有广告，付费版无广告
3. **API 接口**：给开发者调用，按次计费
4. **流量主**：接入 Google AdSense / 阿里妈妈广告
5. **引流**：导流到其他服务（比如打印证件照）

---

## 🚀 开发路线图

### Week 1：MVP（核心功能）
- Day 1-2：搭框架，实现上传 + 预览
- Day 3-4：实现裁剪 + 缩放 + 格式转换
- Day 5-6：实现文件大小控制 + DPI 设置 + 背景色
- Day 7：UI 美化 + 测试

### Week 2：上线 + 推广
- Day 1：买域名
- Day 2：部署到服务器 + HTTPS
- Day 3-4：写文档、截图、录 Demo
- Day 5-7：发到 V2EX / 掘金 / 小红书 / GitHub

### Week 3-4：迭代
- 根据用户反馈加功能
- 加证件照预设模板
- 加批量处理
- 考虑加后端做 AI 去水印

---

## 📝 待确认事项

1. **项目名字**：证件照助手 / PhotoTool / 其他？
2. **域名注册**：`pic2pic.com` 还是 `pic2pic.tools`？
3. **技术栈**：纯前端先上，还是直接前后端结合？
4. **AI 功能**：是否需要去水印、智能抠图？
5. **统计需求**：简单统计还是完整用户系统？

---

## 🔗 相关资源

- **域名注册**：https://wanwang.aliyun.com/domain
- **图片处理库**：
  - Fabric.js：http://fabricjs.com/
  - pica：https://github.com/nodeca/pica
  - Pillow：https://pillow.readthedocs.io/
- **FastAPI 文档**：https://fastapi.tiangolo.com/
- **LiteLLM**：https://github.com/BerriAI/litellm

---

**最后更新**：2026-10-15  
**状态**：规划阶段，待讨论确认
