
# HTTPS 中 TLS 与 TCP 握手的关系  

#### **常规流程（普遍情况）**  
1. **TCP 三次握手先行**  
   - HTTPS 基于 TCP 传输协议，必须先完成 TCP 三次握手建立可靠连接。  
   - **过程**：  
     - 客户端 → 服务端：SYN  
     - 服务端 → 客户端：SYN-ACK  
     - 客户端 → 服务端：ACK  

2. **TLS 握手随后**  
   - **TLSv1.2**：需 2-RTT（四次握手），例如基于 RSA 算法的流程：  
     - Client Hello → Server Hello → Certificate → Client Key Exchange → Finished  
   - **TLSv1.3**：仅需 1-RTT（两次握手），效率更高：  
     - Client Hello → Server Hello + Certificate + Finished  

---

#### **特定条件下 TLS 与 TCP 握手可并行**  
需同时满足以下条件：  
✅ **条件 1**：客户端与服务端均启用 **TCP Fast Open (TFO)**  
   - **首次通信**：仍需完整 TCP 三次握手，服务端生成 Cookie 返回客户端缓存。  
   - **后续通信**：客户端在 SYN 报文中携带应用数据和 Cookie，绕过三次握手直接传输数据。  
   - **服务端验证**：  
     - Cookie 有效 → 在 SYN-ACK 中确认数据并立即响应。  
     - Cookie 无效 → 丢弃数据，仅确认 SYN 序列号。  

✅ **条件 2**：使用 **TLSv1.3** 且已完成至少一次通信  
   - TLSv1.3 的 1-RTT 特性允许在 TCP 第三次握手（ACK）时携带 TLS 数据。  
   - **会话恢复机制**（0-RTT）：通过 `pre_shared_key` 和 `early_data` 扩展，在 TCP 连接后立即发送加密消息。  

---

#### **并行流程示例（TFO + TLSv1.3）**  
1. **后续连接建立过程**：  
   - 客户端发送 SYN 报文：携带应用数据 + TLS Client Hello + 缓存的 TFO Cookie。  
   - 服务端验证 Cookie：  
     - 若有效 → 返回 SYN-ACK + TLS Server Hello + 应用响应数据。  
   - 客户端回复 ACK：完成 TLS 握手剩余步骤。  
2. **效果**：  
   - TLS 握手与 TCP 握手在同一个 RTT 内完成。  
   - HTTP 请求可在此过程中一并传输，显著减少延迟。  

---

#### **关键补充说明**  
- **TFO 限制**：仅支持 Linux 内核 ≥ 3.7 的系统，且需双方显式启用。  
- **TLSv1.3 优势**：  
  - 默认 1-RTT 握手，会话恢复可实现 0-RTT。  
  - 增强安全性（移除弱加密算法，如 RSA 密钥交换）。  
- **注意事项**：  
  - 并行握手仅适用于 **非首次通信** 的场景。  
  - 未启用 TFO 或使用 TLSv1.2 时，必须严格遵循“先 TCP 后 TLS”的顺序。  

---

> **结论**：  
> - 常规场景：**“先 TCP 三次握手，再 TLS 握手”正确无误**。  
> - 特殊场景：仅当 **TFO + TLSv1.3 + 非首次连接** 时，两者可并行。  
> - 面试官若否定常规流程，属于技术理解错误；但提出并行可能性需明确前提条件。