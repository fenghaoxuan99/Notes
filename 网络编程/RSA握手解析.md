
### **HTTPS RSA 握手解析**  
**核心目标：** 通过 RSA 密钥交换实现安全通信  
**HTTP 风险与 TLS 解决方案：**  
| **风险类型** | **TLS 解决方案**          |
|--------------|--------------------------|
| 窃听风险     | 对称加密通信内容          |
| 篡改风险     | 摘要算法校验数据完整性    |
| 冒充风险     | 数字证书验证服务端身份    |

---

#### **一、TLS 握手流程概览**  
1. **四个阶段**  
   - 客户端发送 `Client Hello`  
   - 服务端回复 `Server Hello` + 证书  
   - 客户端发送预主密钥 + 加密验证  
   - 服务端确认加密通信  
2. **耗时**：2 RTT（往返时延）  
3. **密钥生成材料**：  
   - `Client Random`  
   - `Server Random`  
   - `Pre-Master Secret`  

---

#### **二、详细握手过程**  
##### **1. TLS 第一次握手：Client Hello**  
- **携带参数**：  
  - TLS 版本号  
  - 支持的密码套件列表（如 `TLS_RSA_WITH_AES_128_GCM_SHA256`）  
  - 随机数 `Client Random`  

##### **2. TLS 第二次握手：Server Hello**  
- **服务端操作**：  
  - 确认 TLS 版本与密码套件  
  - 生成随机数 `Server Random`  
  - 发送数字证书（`Server Certificate`）  
- **密码套件解析**：  
  ```  
  TLS_RSA_WITH_AES_128_GCM_SHA256  
  ↑ 密钥交换/签名算法 ↑ 对称加密算法 ↑ 摘要算法  
  ```

##### **3. 客户端验证证书**  
- **证书链验证流程**（以百度证书为例）：  
  ```mermaid
  graph LR
    A[客户端] --> B[baidu.com证书]
    B --> C[中间证书：GlobalSign Organization CA]
    C --> D[根证书：GlobalSign Root CA]
  ```
  - **验证逻辑**：  
    - 用根证书公钥验证中间证书签名  
    - 用中间证书公钥验证服务端证书  
- **根证书来源**：操作系统/浏览器内置  

> **为什么需要证书链？**  
> 隔离根证书风险，避免根证书泄露导致整个信任体系崩溃。

##### **4. TLS 第三次握手：密钥交换**  
- **客户端操作**：  
  1. 生成 `Pre-Master Secret`，用服务端公钥加密后发送（`Client Key Exchange`）  
  2. 发送 `Change Cipher Spec` 通知启用加密  
  3. 发送 `Encrypted Handshake Message`（握手数据摘要的加密验证）  
- **会话密钥生成**：  
  `Master Secret = f(Client Random, Server Random, Pre-Master)`  

##### **5. TLS 第四次握手：服务端确认**  
- 发送 `Change Cipher Spec` + `Encrypted Handshake Message`  
- 双方验证通过后，使用 `Master Secret` 对称加密通信  

---

#### **三、RSA 算法的缺陷**  
- **缺乏前向保密性**：  
  若服务端私钥泄露，历史通信可被解密（因 `Pre-Master` 由公钥加密传输）。  
- **改进方案**：ECDHE 密钥交换（支持前向保密）。  

---

### **关键总结**  
| **阶段**       | **核心操作**                          | **安全作用**               |
|----------------|--------------------------------------|--------------------------|
| 证书验证       | 证书链逐级校验                        | 防服务端身份冒充          |
| 密钥交换       | RSA 加密传输 `Pre-Master`             | 生成对称加密密钥          |
| 加密启用确认   | `Change Cipher Spec` + 握手摘要验证   | 防篡改 + 确认加密通道就绪 |