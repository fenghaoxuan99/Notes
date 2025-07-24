# C++网络编程：IP地址转换函数深度解析与最佳实践

## 1. IP地址转换函数概述

在网络编程中，IP地址经常需要在可读字符串格式和二进制网络字节序格式之间相互转换。POSIX标准提供了一系列函数来完成这些转换。

## 2. inet_pton函数详解

### 2.1 函数功能与原型

`inet_pton` (presentation to numeric) 函数将点分十进制(IPv4)或冒号十六进制(IPv6)格式的IP地址转换为二进制网络字节序格式。

```c
#include <arpa/inet.h>

int inet_pton(int af, const char *src, void *dst);
```

### 2.2 参数解析

| 参数 | 类型 | 说明 |
|------|------|------|
| `af` | int | 地址族：AF_INET(IPv4)或AF_INET6(IPv6) |
| `src` | const char* | 源IP地址字符串 |
| `dst` | void* | 存储转换结果的缓冲区 |

### 2.3 返回值说明

- **1**：转换成功
- **0**：输入地址无效
- **-1**：发生错误（设置errno）

### 2.4 使用示例

#### IPv4转换示例
```c
#include <stdio.h>
#include <arpa/inet.h>

void convert_ipv4(const char* ip_str) {
    struct in_addr addr;
    
    if (inet_pton(AF_INET, ip_str, &addr) == 1) {
        printf("IPv4转换成功: 0x%08x\n", ntohl(addr.s_addr));
    } else {
        perror("IPv4转换失败");
    }
}

int main() {
    convert_ipv4("192.168.1.1");
    convert_ipv4("256.0.0.1");  // 无效地址示例
    return 0;
}
```

#### IPv6转换示例
```c
#include <stdio.h>
#include <arpa/inet.h>

void convert_ipv6(const char* ip_str) {
    struct in6_addr addr;
    
    if (inet_pton(AF_INET6, ip_str, &addr) == 1) {
        printf("IPv6转换成功: ");
        for (int i = 0; i < 16; i++) {
            printf("%02x", addr.s6_addr[i]);
            if (i % 2 == 1 && i != 15) printf(":");
        }
        printf("\n");
    } else {
        perror("IPv6转换失败");
    }
}

int main() {
    convert_ipv6("2001:0db8:85a3:0000:0000:8a2e:0370:7334");
    convert_ipv6("2001::db8::1234");  // 无效地址示例
    return 0;
}
```

## 3. 反向转换函数inet_ntop

### 3.1 函数功能与原型

`inet_ntop` (numeric to presentation) 是`inet_pton`的反向操作，将二进制网络字节序地址转换为可读字符串格式。

```c
const char *inet_ntop(int af, const void *src, 
                     char *dst, socklen_t size);
```

### 3.2 参数解析

| 参数 | 类型 | 说明 |
|------|------|------|
| `af` | int | 地址族：AF_INET或AF_INET6 |
| `src` | const void* | 二进制格式的IP地址 |
| `dst` | char* | 存储结果的缓冲区 |
| `size` | socklen_t | 缓冲区大小 |

### 3.3 返回值说明

- 成功：返回指向结果字符串的指针
- 失败：返回NULL（设置errno）

### 3.4 使用示例

```c
#include <stdio.h>
#include <arpa/inet.h>

void binary_to_ipv4(uint32_t binary_ip) {
    char str[INET_ADDRSTRLEN];
    struct in_addr addr;
    addr.s_addr = htonl(binary_ip);
    
    if (inet_ntop(AF_INET, &addr, str, INET_ADDRSTRLEN)) {
        printf("IPv4字符串: %s\n", str);
    } else {
        perror("IPv4转换失败");
    }
}

void binary_to_ipv6(const uint8_t* binary_ip) {
    char str[INET6_ADDRSTRLEN];
    struct in6_addr addr;
    memcpy(addr.s6_addr, binary_ip, 16);
    
    if (inet_ntop(AF_INET6, &addr, str, INET6_ADDRSTRLEN)) {
        printf("IPv6字符串: %s\n", str);
    } else {
        perror("IPv6转换失败");
    }
}

int main() {
    // IPv4示例
    binary_to_ipv4(0xC0A80101);  // 192.168.1.1
    
    // IPv6示例
    uint8_t ipv6_bin[16] = {
        0x20, 0x01, 0x0d, 0xb8, 0x85, 0xa3, 0x00, 0x00,
        0x00, 0x00, 0x8a, 0x2e, 0x03, 0x70, 0x73, 0x34
    };
    binary_to_ipv6(ipv6_bin);
    
    return 0;
}
```

## 4. 传统IPv4转换函数

### 4.1 inet_aton函数

```c
#include <arpa/inet.h>

int inet_aton(const char *cp, struct in_addr *inp);
```

**特点：**
- 仅支持IPv4
- 成功返回1，失败返回0
- 线程安全

**示例：**
```c
struct in_addr addr;
if (inet_aton("192.168.1.1", &addr)) {
    printf("转换成功: 0x%08x\n", addr.s_addr);
}
```

### 4.2 inet_addr函数（已废弃）

```c
in_addr_t inet_addr(const char *cp);
```

**问题：**
- 不能处理255.255.255.255（返回INADDR_NONE）
- 不支持IPv6
- 非线程安全

## 5. 函数对比与选择建议

### 5.1 功能对比表

| 特性 | inet_pton | inet_ntop | inet_aton | inet_addr |
|------|-----------|-----------|-----------|-----------|
| IPv4支持 | ✓ | ✓ | ✓ | ✓ |
| IPv6支持 | ✓ | ✓ | ✗ | ✗ |
| 二进制→文本 | ✗ | ✓ | ✗ | ✗ |
| 文本→二进制 | ✓ | ✗ | ✓ | ✓ |
| 线程安全 | ✓ | ✓ | ✓ | ✗ |
| 错误处理 | 完善 | 完善 | 一般 | 有限 |

### 5.2 选择建议

1. **新项目开发**：统一使用`inet_pton`和`inet_ntop`
   - 支持IPv4/IPv6双协议栈
   - 更好的错误处理
   - 线程安全

2. **维护旧代码**：
   - 逐步替换`inet_addr`为`inet_pton`
   - 保留`inet_aton`但考虑升级

3. **性能关键场景**：
   ```cpp
   // 缓存转换结果示例
   #include <unordered_map>
   #include <string>
   
   std::unordered_map<std::string, uint32_t> ipv4_cache;
   
   uint32_t cached_inet_pton(const std::string& ip_str) {
       auto it = ipv4_cache.find(ip_str);
       if (it != ipv4_cache.end()) {
           return it->second;
       }
       
       struct in_addr addr;
       if (inet_pton(AF_INET, ip_str.c_str(), &addr) == 1) {
           ipv4_cache[ip_str] = addr.s_addr;
           return addr.s_addr;
       }
       throw std::runtime_error("Invalid IP address");
   }
   ```

## 6. 高级应用与错误处理

### 6.1 自定义IP地址验证

```cpp
bool is_valid_ip(const std::string& ip) {
    struct in_addr addr;
    struct in6_addr addr6;
    
    if (inet_pton(AF_INET, ip.c_str(), &addr) == 1) {
        return true;
    }
    
    if (inet_pton(AF_INET6, ip.c_str(), &addr6) == 1) {
        return true;
    }
    
    return false;
}
```

### 6.2 网络字节序处理

```cpp
#include <iostream>
#include <arpa/inet.h>

void print_ip_info(uint32_t network_order_ip) {
    uint32_t host_order = ntohl(network_order_ip);
    std::cout << "网络字节序: 0x" << std::hex << network_order_ip << std::endl;
    std::cout << "主机字节序: 0x" << std::hex << host_order << std::endl;
    
    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &network_order_ip, ip_str, INET_ADDRSTRLEN);
    std::cout << "IP地址: " << ip_str << std::endl;
}
```

### 6.3 综合错误处理示例

```cpp
#include <iostream>
#include <stdexcept>
#include <arpa/inet.h>

class IPConversionError : public std::runtime_error {
public:
    explicit IPConversionError(const std::string& msg) 
        : std::runtime_error(msg) {}
};

uint32_t string_to_ipv4(const std::string& ip_str) {
    struct in_addr addr;
    
    // 检查空字符串
    if (ip_str.empty()) {
        throw IPConversionError("Empty IP address string");
    }
    
    // 尝试转换
    int result = inet_pton(AF_INET, ip_str.c_str(), &addr);
    
    if (result == 1) {
        return addr.s_addr;
    } else if (result == 0) {
        throw IPConversionError("Invalid IPv4 address format: " + ip_str);
    } else {
        throw IPConversionError(std::string("inet_pton failed: ") + strerror(errno));
    }
}

std::string ipv4_to_string(uint32_t ip) {
    char buf[INET_ADDRSTRLEN];
    const char* result = inet_ntop(AF_INET, &ip, buf, INET_ADDRSTRLEN);
    
    if (result == nullptr) {
        throw IPConversionError(std::string("inet_ntop failed: ") + strerror(errno));
    }
    
    return std::string(result);
}
```

## 7. 实际应用案例

### 7.1 网络服务器中的地址处理

```cpp
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>

void handle_client(int sockfd, const sockaddr_in& client_addr) {
    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(client_addr.sin_addr), ip_str, INET_ADDRSTRLEN);
    
    std::cout << "Connection from: " << ip_str 
              << ":" << ntohs(client_addr.sin_port) << std::endl;
    
    // 处理客户端请求...
}

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    
    // 绑定到所有网络接口
    if (inet_pton(AF_INET, "0.0.0.0", &server_addr.sin_addr) != 1) {
        perror("inet_pton failed");
        return 1;
    }
    
    if (bind(sockfd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        return 1;
    }
    
    listen(sockfd, 5);
    
    while (true) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(sockfd, (sockaddr*)&client_addr, &client_len);
        
        if (client_fd < 0) {
            perror("accept failed");
            continue;
        }
        
        handle_client(client_fd, client_addr);
        close(client_fd);
    }
    
    return 0;
}
```

### 7.2 IP地址范围检查

```cpp
#include <arpa/inet.h>
#include <string>

bool is_ip_in_range(const std::string& ip_str, 
                   const std::string& network_str,
                   const std::string& netmask_str) {
    struct in_addr ip, network, netmask;
    
    // 转换IP地址
    if (inet_pton(AF_INET, ip_str.c_str(), &ip) != 1) return false;
    if (inet_pton(AF_INET, network_str.c_str(), &network) != 1) return false;
    if (inet_pton(AF_INET, netmask_str.c_str(), &netmask) != 1) return false;
    
    // 检查是否在同一网络
    return ((ip.s_addr & netmask.s_addr) == (network.s_addr & netmask.s_addr));
}

// 使用示例
bool result = is_ip_in_range("192.168.1.100", "192.168.1.0", "255.255.255.0");
```

## 8. 跨平台注意事项

1. **Windows平台**：
   - 需要包含`<ws2tcpip.h>`而非`<arpa/inet.h>`
   - 需要先调用`WSAStartup`

2. **字节序问题**：
   - 始终使用`htonl`/`ntohl`等函数处理网络字节序
   - 不要假设主机字节序

3. **IPv6兼容性**：
   - 测试时使用各种IPv6格式（压缩、展开、混合等）

## 9. 性能优化技巧

1. **缓存转换结果**：对频繁转换的IP地址使用缓存
2. **批量转换**：处理大量IP时考虑批量操作
3. **避免重复验证**：在数据接收层验证一次即可

```cpp
// 批量转换示例
std::vector<uint32_t> convert_ip_list(const std::vector<std::string>& ip_list) {
    std::vector<uint32_t> result;
    result.reserve(ip_list.size());
    
    struct in_addr addr;
    for (const auto& ip : ip_list) {
        if (inet_pton(AF_INET, ip.c_str(), &addr) == 1) {
            result.push_back(addr.s_addr);
        } else {
            result.push_back(0);  // 用0表示无效地址
        }
    }
    
    return result;
}
```

## 10. 总结与最佳实践

1. **统一使用inet_pton/inet_ntop**：支持双协议栈，更安全可靠
2. **严格错误检查**：总是检查返回值，处理错误情况
3. **注意字节序**：网络字节序与主机字节序的转换
4. **输入验证**：在应用层增加额外的验证逻辑
5. **线程安全**：这些函数本身是线程安全的，但要注意缓冲区的使用
6. **性能考虑**：高频场景考虑缓存或批量处理