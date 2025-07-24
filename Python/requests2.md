### `requests.get` 函数的参数详解

`requests.get` 是 Python `requests` 库中用于发送 HTTP GET 请求的核心函数，其参数和返回值的属性和方法如下：

---

#### **一、`requests.get` 的主要参数**

| 参数名                | 类型                  | 作用                                                               | 示例                                                           |
| --------------------- | --------------------- | ------------------------------------------------------------------ | -------------------------------------------------------------- |
| **`url`**             | `str`                 | 请求的目标 URL（必需参数）。                                       | `requests.get("https://api.example.com/data")`                 |
| **`params`**          | `dict` 或 `str`       | 附加到 URL 的查询参数（自动编码为 URL 格式）。                     | `params={"key": "value"}` → `https://api.com?key=value`        |
| **`headers`**         | `dict`                | 自定义请求头（如设置 User-Agent、Authorization 等）。              | `headers={"User-Agent": "Mozilla/5.0"}`                        |
| **`cookies`**         | `dict` 或 `CookieJar` | 附加到请求的 Cookie 信息。                                         | `cookies={"session_id": "abc123"}`                             |
| **`auth`**            | `tuple`               | 基本认证（用户名和密码）。                                         | `auth=("user", "pass")`                                        |
| **`timeout`**         | `float` 或 `tuple`    | 请求超时时间（秒）。若为元组，表示连接和读取超时。                 | `timeout=5` 或 `timeout=(3, 10)`                               |
| **`allow_redirects`** | `bool`                | 是否允许重定向（默认为 `True`）。                                  | `allow_redirects=False`                                        |
| **`proxies`**         | `dict`                | 设置代理服务器（支持 HTTP 和 HTTPS）。                             | `proxies={"http": "http://10.10.1.10:3128"}`                   |
| **`verify`**          | `bool` 或 `str`       | 是否验证 SSL 证书（默认为 `True`）。若为字符串，指定 CA 证书路径。 | `verify=False`（禁用验证）或 `verify="/path/to/cert.pem"`      |
| **`stream`**          | `bool`                | 是否以流模式下载响应内容（用于大文件）。                           | `stream=True` → 需手动关闭连接或使用 `response.iter_content()` |
| **`cert`**            | `str` 或 `tuple`      | 指定客户端证书（路径或 (证书, 密钥) 元组）。                       | `cert=("/path/client.cert", "/path/client.key")`               |

---

#### **二、返回的 `Response` 对象属性**

| 属性名            | 类型                | 说明                                                       |
| ----------------- | ------------------- | ---------------------------------------------------------- |
| **`status_code`** | `int`               | HTTP 状态码（如 200、404、500）。                          |
| **`headers`**     | `dict`              | 响应头（字典格式，键不区分大小写）。                       |
| **`text`**        | `str`               | 响应内容的字符串形式（自动根据编码解码）。                 |
| **`content`**     | `bytes`             | 响应内容的字节形式（原始二进制数据）。                     |
| **`encoding`**    | `str`               | 响应内容的编码（可手动修改以修正解码错误）。               |
| **`url`**         | `str`               | 实际请求的最终 URL（包含重定向后的地址）。                 |
| **`cookies`**     | `RequestsCookieJar` | 服务器返回的 Cookie 信息。                                 |
| **`history`**     | `list`              | 重定向历史记录（列表中的每个元素是一个 `Response` 对象）。 |
| **`request`**     | `PreparedRequest`   | 原始请求对象（查看请求头、方法等信息）。                   |

---

#### **三、`Response` 对象常用方法**

| 方法名                   | 作用                                                      | 示例                                                        |
| ------------------------ | --------------------------------------------------------- | ----------------------------------------------------------- |
| **`json()`**             | 将 JSON 响应解析为 Python 对象（字典或列表）。            | `data = response.json()`                                    |
| **`raise_for_status()`** | 如果状态码为 4xx 或 5xx，抛出 `HTTPError` 异常。          | `response.raise_for_status()`（需配合 `try...except` 使用） |
| **`iter_content()`**     | 以流模式按块读取响应内容（需设置 `stream=True`）。        | `for chunk in response.iter_content(chunk_size=1024): ...`  |
| **`close()`**            | 主动关闭响应连接（通常在使用 `stream=True` 后手动调用）。 | `response.close()`                                          |

---

#### **四、完整示例**

```python
import requests

# 发送带参数的 GET 请求
params = {"page": 1, "limit": 20}
headers = {"User-Agent": "MyApp/1.0"}
response = requests.get(
    url="https://api.example.com/posts",
    params=params,
    headers=headers,
    timeout=5,
    verify=True
)

# 检查状态码并处理响应
if response.status_code == 200:
    print("请求成功！")
    print("响应头:", response.headers)
    print("内容编码:", response.encoding)
    print("JSON 数据:", response.json())
else:
    print(f"请求失败，状态码: {response.status_code}")
    response.raise_for_status()  # 抛出异常（可选）

# 下载文件（流模式）
response = requests.get("https://example.com/large_file.zip", stream=True)
with open("large_file.zip", "wb") as f:
    for chunk in response.iter_content(chunk_size=8192):
        f.write(chunk)
response.close()  # 手动关闭连接
```

---

#### **五、注意事项**

1. **编码问题**：若 `response.text` 出现乱码，可手动设置 `response.encoding = "utf-8"`。
2. **JSON 解析**：若响应内容非 JSON，`response.json()` 会抛出 `JSONDecodeError`。
3. **超时处理**：建议始终设置 `timeout`，避免程序无响应。
4. **资源释放**：使用 `stream=True` 时，需手动调用 `response.close()` 或使用 `with` 语句。

掌握这些参数和返回值特性，可以高效处理 HTTP 请求与响应。

<font color="cyan">dddddd</font>
