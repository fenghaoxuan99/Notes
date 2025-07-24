# HTML 在 Markdown 中使用场景：

---

### 1. **文字颜色与背景色**

```html
<span style="color: red;">红色文字</span>
<span style="background-color: yellow; padding: 2px 5px;">高亮文字</span>
```

_适用场景_：强调关键内容或代码注释中的颜色标注。

---

### 2. **自定义对齐方式**

```html
<div style="text-align: center;">居中内容</div>
<div style="text-align: right;">右对齐内容</div>
```

_Markdown 原生仅支持段落左对齐_。

---

### 3. **复杂表格**

Markdown 的表格语法简单，但无法实现合并单元格：

```html
<table>
  <tr>
    <th colspan="2">合并表头</th>
  </tr>
  <tr>
    <td rowspan="2">合并单元格</td>
    <td>内容A</td>
  </tr>
  <tr>
    <td>内容B</td>
  </tr>
</table>
```

---

### 4. **调整图片尺寸与布局**

```html
<img
  src="image.jpg"
  alt="图片描述"
  width="200"
  style="float: right; margin: 10px;"
/>
```

_Markdown 图片语法 `![alt](src)` 无法直接控制尺寸和对齐_。

---

### 5. **折叠内容 (Details/Summary)**

```html
<details>
  <summary>点击展开详情</summary>
  这里是隐藏的内容，支持Markdown语法！
</details>
```

_适用于隐藏长篇代码、附加说明等_。

---

### 6. **内嵌视频与音频**

```html
<video controls width="400">
  <source src="video.mp4" type="video/mp4" />
</video>

<audio controls>
  <source src="audio.mp3" type="audio/mpeg" />
</audio>
```

_Markdown 原生不支持多媒体嵌入_。

---

### 7. **字体与字号控制**

```html
<span style="font-family: 'Arial'; font-size: 14px;">自定义字体</span>
```

_注意：某些平台（如 GitHub）可能限制自定义字体_。

---

### 8. **注释与隐藏内容**

```html
<!-- 这是HTML注释，不会在渲染后显示 -->
```

---

### 9. **分栏布局**

```html
<div style="display: flex; gap: 20px;">
  <div style="flex: 1;">左栏内容</div>
  <div style="flex: 1;">右栏内容</div>
</div>
```

_适合对比展示或并列内容_。

---

### 10. **悬浮提示 (Tooltip)**

```html
<span title="这里是悬浮提示">鼠标悬停查看提示</span>
```

### 1. **文本格式化扩展**

#### **上标与下标**

```html
H<sub>2</sub>O（下标） E = mc<sup>2</sup>（上标）
```

_Markdown 原生不支持上下标_。

#### **键盘输入样式**

```html
按<kbd>Ctrl</kbd>+<kbd>C</kbd>复制
```

_渲染为键盘按键样式_。

---

### 2. **内联框架 (Iframe)**

嵌入外部网页或动态内容：

```html
<iframe
  src="https://example.com"
  width="100%"
  height="300"
  frameborder="0"
  allowfullscreen
>
</iframe>
```

_适用场景_：嵌入地图、在线图表或动态演示。

---

### 3. **锚点跳转**

创建文档内的快速跳转链接：

```html
<!-- 定义锚点 -->
<a id="section1"></a>
<h2>第一部分</h2>

<!-- 跳转链接 -->
[返回顶部](#section1)
```

---

### 6. **响应式图片**

根据屏幕尺寸调整图片大小：

```html
<img src="image.jpg" alt="响应式图片" style="max-wi dth: 100%; height: auto;" />
```

---

### 11. **进度条与计量器**

展示进度或数值范围：

```html
<progress value="75" max="100"></progress>
<meter value="0.6" min="0" max="1">60%</meter>
```

### 13. **元信息标记**

为 SEO 或文档元数据添加隐藏内容：

```html
<meta name="keywords" content="Markdown, HTML, 教程" />
<meta name="description" content="一篇关于 Markdown 与 HTML 结合使用的指南" />
```
