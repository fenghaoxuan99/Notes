Python 的 `requests` 库是处理 HTTP 请求的第三方库，因其简洁的 API 和强大的功能被广泛使用。以下是其核心用法和用途的详细说明及示例：

---

### 一、安装与基础使用

```python
pip install requests  # 安装
import requests       # 导入
```

---

### 二、发送 GET 请求

**用途**：获取网页内容或调用 API。

#### 1. 基本 GET 请求

```python
response = requests.get("https://api.example.com/data")
print(response.text)  # 获取响应文本
```

#### 2. 带参数的 GET 请求

```python
params = {"key1": "value1", "key2": "value2"}
response = requests.get("https://api.example.com/data", params=params)
print(response.url)  # 输出完整 URL，如 https://api.example.com/data?key1=value1&key2=value2
```

---

### 三、发送 POST 请求

**用途**：提交表单数据或上传文件到服务器。

#### 1. 提交表单数据

```python
data = {"username": "admin", "password": "123456"}
response = requests.post("https://api.example.com/login", data=data)
print(response.json())  # 解析 JSON 响应
```

#### 2. 提交 JSON 数据

```python
json_data = {"name": "Alice", "age": 30}
response = requests.post("https://api.example.com/users", json=json_data)
```

---

### 四、处理响应

#### 1. 响应内容

```python
response = requests.get("https://api.example.com/data")
print(response.status_code)  # 状态码（如 200）
print(response.headers)      # 响应头（字典格式）
print(response.text)         # 文本内容（如 HTML/XML）
print(response.json())       # 解析 JSON 内容（需响应是 JSON 格式）
```

#### 2. 处理二进制内容（如图片）

```python
response = requests.get("https://example.com/image.jpg")
with open("image.jpg", "wb") as f:
    f.write(response.content)  # 保存二进制内容
```

---

### 五、文件上传

**用途**：上传本地文件到服务器。

```python
files = {"file": open("report.pdf", "rb")}
response = requests.post("https://api.example.com/upload", files=files)
```

---

### 六、高级用法

#### 1. 使用会话（Session）

**用途**：保持跨请求的 Cookie 和配置（如登录状态）。

```python
session = requests.Session()
session.get("https://example.com/login", params={"user": "admin"})  # 保持 Cookie
response = session.get("https://example.com/dashboard")             # 使用同一会话
```

#### 2. 设置超时

```python
try:
    response = requests.get("https://api.example.com/data", timeout=3)  # 3 秒超时
except requests.exceptions.Timeout:
    print("请求超时！")
```

#### 3. 使用代理

```python
proxies = {
    "http": "http://10.10.1.10:3128",
    "https": "http://10.10.1.10:1080",
}
response = requests.get("https://api.example.com/data", proxies=proxies)
```

#### 4. SSL 证书验证

```python
response = requests.get("https://api.example.com/data", verify=False)  # 禁用验证（不推荐）
response = requests.get("https://api.example.com/data", verify="/path/to/cert.pem")  # 自定义证书
```

---

### 七、错误处理

```python
try:
    response = requests.get("https://api.example.com/data")
    response.raise_for_status()  # 如果状态码非 200，抛出 HTTPError
except requests.exceptions.HTTPError as err:
    print(f"HTTP 错误: {err}")
except requests.exceptions.RequestException as err:
    print(f"请求异常: {err}")
```

---

### 八、实际应用场景

1. **调用 RESTful API**：获取天气数据、股票信息等。
2. **网页爬虫**：抓取网页内容（需遵守 `robots.txt`）。
3. **自动化测试**：模拟用户登录、表单提交。
4. **文件下载**：批量下载图片或文档。

---

### 九、注意事项

1. **编码处理**：使用 `response.encoding = 'utf-8'` 解决乱码问题。
2. **JSON 解析**：若响应非 JSON 格式，`response.json()` 会抛出异常。
3. **资源释放**：使用 `with` 语句或手动关闭响应对象（`response.close()`）。

---

通过掌握这些核心用法，你可以高效地使用 `requests` 完成绝大多数 HTTP 交互任务。
