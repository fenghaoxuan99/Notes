
## SYN 攻击详解及防御策略

### 🔥 什么是 SYN 攻击？
SYN 攻击是一种利用 TCP 三次握手缺陷的**拒绝服务攻击（DoS/DDoS）**：
1. 攻击者伪造大量不同 IP 地址的 **SYN 报文**发送到目标服务器
2. 服务器为每个 SYN 创建连接对象，进入 `SYN_RCVD` 状态，并回复 **SYN-ACK**
3. 由于源 IP 是伪造的，服务器**永远收不到 ACK 响应**
4. 这些"半开连接"持续占用服务器的**半连接队列（SYN 队列）**
5. 当队列被占满时，服务器**拒绝新的合法连接请求**，导致服务不可用


### TCP 队列机制（攻击目标）
在 TCP 三次握手中，内核维护两个关键队列：
| **队列类型** | **存储内容** | **触发操作** | **溢出后果** |
|------------|------------|------------|------------|
| **半连接队列 (SYN队列)** | 收到 SYN 后创建的半开连接对象 | 发送 SYN-ACK 后等待 ACK | 新 SYN 被丢弃 |
| **全连接队列 (Accept队列)** | 完成三次握手的连接对象 | 等待应用调用 `accept()` | 新连接被丢弃 |

```c
// 典型服务器代码中的队列设置
int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
// backlog 参数直接影响队列大小
listen(listen_fd, 1024); 
```

---

### 🛡️ 四种防御 SYN 攻击的方法

#### 方法一：调大网络设备缓冲队列
```bash
# 增加网卡接收队列容量（默认1000）
sudo sysctl -w net.core.netdev_max_backlog=10000
```
- **作用**：当网卡接收速度 > 内核处理速度时，提供缓冲空间
- **适用场景**：高流量但非恶意攻击环境

#### 方法二：增大 TCP 半连接队列
需**同时调整**三个参数：
```bash
# 1. 增大SYN队列最大长度（默认128）
sudo sysctl -w net.ipv4.tcp_max_syn_backlog=4096

# 2. 增大全连接队列上限（默认128）
sudo sysctl -w net.core.somaxconn=4096

# 3. 在代码中增大backlog参数
listen(socket_fd, 4096);  // 需重启服务生效
```
> 📌 **关键点**：实际队列大小 = min(backlog, somaxconn)

#### 方法三：启用 SYN Cookie 机制
```bash
# 启用SYN Cookie（推荐设置为1）
sudo sysctl -w net.ipv4.tcp_syncookies=1
```
- **工作原理**：
  1. SYN 队列满时，服务端**不创建半连接**
  2. 通过加密算法生成 **Cookie** 放入 SYN-ACK 的序列号
  3. 合法客户端回 ACK 时携带 Cookie
  4. 服务端验证 Cookie 后直接创建全连接
- **参数说明**：
  - `0`：关闭
  - `1`：队列满时启用（推荐）
  - `2`：强制启用

#### 方法四：减少 SYN-ACK 重传次数
```bash
# 减少半连接等待时间（默认5次）
sudo sysctl -w net.ipv4.tcp_synack_retries=2
```
- **效果**：
  - 重传次数从默认 5 次（约 180 秒）降为 2 次（约 10 秒）
  - 加速释放被占用的半连接资源
- **计算**：超时时间 = 1 + 2 + 4 = 7秒（三次等待）

---

### 🛠️ 综合防御建议
1. **基础配置**：
   ```bash
   echo "net.core.netdev_max_backlog=10000" >> /etc/sysctl.conf
   echo "net.ipv4.tcp_max_syn_backlog=4096" >> /etc/sysctl.conf
   echo "net.core.somaxconn=4096" >> /etc/sysctl.conf
   echo "net.ipv4.tcp_syncookies=1" >> /etc/sysctl.conf
   echo "net.ipv4.tcp_synack_retries=2" >> /etc/sysctl.conf
   sysctl -p  # 应用配置
   ```
   
2. **架构级防护**：
   - 前端部署 **WAF（Web应用防火墙）**
   - 使用 **CDN 服务**分散流量
   - 启用云服务商的 **DDoS 防护**

3. **监控命令**：
   ```bash
   # 查看半连接状态（SYN_RECV）
   netstat -antp | grep SYN_RECV | wc -l
   
   # 监控队列溢出
   watch -n 1 'ss -s | grep -i "synrecv"'
   ```

> ⚠️ **注意**：单一措施效果有限，建议组合使用并定期压力测试。对于大规模 DDoS 攻击，需要基础设施级防护方案。