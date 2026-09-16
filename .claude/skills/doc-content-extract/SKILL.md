---
name: doc-content-extract
description: 当用户要求从 PDF/文档中提取某个章节的文本内容时，请参考此规范；
---

# 指令

1. 提取内容必须**完整**，不能精简，省略或缩写任何段落、列表项、代码块、脚注、注释等；
2. 保持原标题层级结构（如 `###`、`####` 等与原文一致）；
3. 提取的文本**保持原文语言**（通常为英文），除非用户明确要求翻译；
4. 代码块使用对应的语言标记（如 ` ```cmake `、` ```bash `、` ```cpp ` 等）；
5. 脚注、Tip、Note、Warning 等提示块要完整保留，不能丢掉；
6. 列表项（bullet points）要逐条保留，不能合并或省略；
7. 如果用户要求翻译，则在完整提取后，再提供一份中文翻译版本；

## 参考示例

用户请求：提取 1.2 节的文本，保持原标题样式，不要省掉一些；文本保持英文。

正确做法：
1. 从文档中定位 1.2 节的起始和结束位置；
2. 完整提取该节所有子节（1.2.1、1.2.2、...1.2.7）的全部内容；
3. 保持原有的标题层级（###、#### 等）；
4. 保留所有代码块、列表、脚注、Tip/Note；
5. 输出英文原文，不做翻译。

错误做法：
- 只提取了部分子节，省略了 1.2.6 或 1.2.7；
- 把脚注或 Tip 块丢掉了；
- 把代码块简化成了文字描述；
- 自作主张翻译成中文。

## PDF 提取方法

根据环境可用性，选择以下方法之一提取 PDF 内容：

### 方法 1：pdftotext（Poppler 工具）

适用于安装了 poppler-utils 的环境（Linux/macOS/部分 Windows MSYS2）：

```bash
# 提取指定页面范围（页码从 1 开始）
pdftotext -f <起始页> -l <结束页> -layout "document.pdf" -

# 示例：提取第 25-32 页
pdftotext -f 25 -l 32 -layout "document.pdf" output.txt
```

优点：简单直接，支持 `-layout` 保留原始排版
缺点：需要安装 poppler-utils；Windows 环境可能不可用

### 方法 2：Python + PyPDF2

适用于有 Python 环境且安装了 PyPDF2 的情况（跨平台可靠）：

```python
import PyPDF2
import os

# 临时文件放到 .claude/tmp/
os.makedirs('.claude/tmp', exist_ok=True)

pdf_path = r'path/to/document.pdf'
output_path = r'.claude/tmp/extracted_content.txt'

with open(pdf_path, 'rb') as f:
    reader = PyPDF2.PdfReader(f)
    with open(output_path, 'w', encoding='utf-8') as out:
        # PDF 页面索引从 0 开始
        for page_num in range(start_page - 1, end_page):
            if page_num < len(reader.pages):
                page = reader.pages[page_num]
                text = page.extract_text()
                out.write(f'\n=== Page {page_num + 1} ===\n')
                out.write(text)
                out.write('\n')

print(f'Extracted to {output_path}')
```

优点：跨平台可靠，不依赖外部工具；编码处理灵活
缺点：需要 Python 和 PyPDF2 库

## 纯图片 PDF 提取方法（OCR）

当 PDF 是纯图片（无文字层）时，`pdftotext` 和 `PyPDF2` 提取出的文字为空，需要用 OCR 识别。

### 判断是否为纯图片 PDF

```bash
# 用 pdftotext 提取第 1 页，如果输出为空则可能是纯图片 PDF
pdftotext -f 1 -l 1 "document.pdf" -
```

### 所需工具

| 工具 | 用途 | 安装 |
|------|------|------|
| **PyMuPDF** (pymupdf) | 将 PDF 页面转为图片 | `pip install PyMuPDF` |
| **Tesseract OCR** | 图片文字识别 | Windows: 从 [UB-Mannheim/tesseract](https://github.com/UB-Mannheim/tesseract/wiki) 下载安装；Linux: `sudo apt install tesseract-ocr` |

### 提取步骤

#### 步骤 1：PDF 页面转图片 + OCR

```python
import pymupdf
import subprocess
import os

pdf_path = r'path/to/document.pdf'
output_dir = r'.claude/tmp/ocr_pages'
tesseract = r'D:\Tools\Tesseract-OCR\tesseract.exe'  # 按实际路径修改
# Linux: tesseract = 'tesseract'

os.makedirs(output_dir, exist_ok=True)

doc = pymupdf.open(pdf_path)
total_pages = len(doc)

for page_num in range(total_pages):
    page = doc[page_num]
    # 将页面渲染为图片（DPI 200 适合 OCR）
    pix = page.get_pixmap(dpi=200)
    img_path = os.path.join(output_dir, f'page_{page_num+1:03d}.png')
    pix.save(img_path)

    # Tesseract OCR 识别
    txt_base = os.path.join(output_dir, f'page_{page_num+1:03d}')
    subprocess.run([tesseract, img_path, txt_base, '-l', 'eng'],
                   capture_output=True)

    if (page_num + 1) % 10 == 0:
        print(f'Page {page_num+1}/{total_pages} done')

doc.close()
print(f'All {total_pages} pages processed!')
```

#### 步骤 2：合并所有页面文本

```python
import os

output_dir = r'.claude/tmp/ocr_pages'
output_file = r'.claude/tmp/full_text.txt'

with open(output_file, 'w', encoding='utf-8') as out:
    for i in range(1, total_pages + 1):
        txt_file = os.path.join(output_dir, f'page_{i:03d}.txt')
        if os.path.exists(txt_file):
            with open(txt_file, 'r', encoding='utf-8') as f:
                content = f.read()
                out.write(f'\n===== Page {i} =====\n\n')
                out.write(content)
                out.write('\n')

print(f'Written to {output_file}')
```

### Tesseract 语言包

```bash
# 查看已安装的语言
tesseract --list-langs

# Windows: 安装时勾选需要的语言包
# Linux: 安装额外语言包
sudo apt install tesseract-ocr-chi-sim   # 简体中文
sudo apt install tesseract-ocr-chi-tra   # 繁体中文
```

使用指定语言进行 OCR：

```python
# 英文
subprocess.run([tesseract, img_path, txt_base, '-l', 'eng'])

# 中英混合
subprocess.run([tesseract, img_path, txt_base, '-l', 'eng+chi_sim'])
```

### 注意事项

- DPI 建议设为 200，太低识别率低，太高处理慢且文件大；
- 每页生成一个 `.png` 和一个 `.txt`，合并时按页码顺序拼接；
- OCR 对表格、公式、图表的识别效果有限，可能需要人工校对；
- 大文件（>100 页）处理时间较长，建议先试几页确认效果后再批量处理。

## 查找章节位置

PDF 页码通常与印刷页码不一致，需要先定位章节：

1. **查看目录**：提取前几页（通常是目录），搜索章节标题和对应页码
2. **搜索关键词**：在整个 PDF 中搜索章节标题，找到实际 PDF 页码
3. **试错法**：如果知道大致范围，提取相邻几页确认内容

```bash
# 搜索章节标题所在页
pdftotext "document.pdf" - | grep -n "Chapter 5\. Basic Testing"
```

```python
# Python 方式搜索
import PyPDF2
with open('document.pdf', 'rb') as f:
    reader = PyPDF2.PdfReader(f)
    for i, page in enumerate(reader.pages):
        text = page.extract_text()
        if "Chapter 5" in text:
            print(f"Found on PDF page {i + 1}")
```
