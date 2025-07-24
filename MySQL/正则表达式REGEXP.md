# SQL正则表达式(REGEXP)全面指南

正则表达式(REGEXP)是SQL中强大的模式匹配工具，提供了比LIKE更灵活和精确的字符串匹配能力。本指南将详细介绍SQL中正则表达式的使用方法、语法规则、实际应用和性能优化。

## 1. 正则表达式基础

### 基本语法

不同数据库系统的正则表达式语法略有不同：

```sql
-- MySQL
SELECT * FROM table WHERE column REGEXP 'pattern';

-- PostgreSQL
SELECT * FROM table WHERE column ~ 'pattern';  -- 区分大小写
SELECT * FROM table WHERE column ~* 'pattern'; -- 不区分大小写

-- Oracle
SELECT * FROM table WHERE REGEXP_LIKE(column, 'pattern');

-- SQL Server
SELECT * FROM table WHERE column LIKE '%pattern%'; -- 有限支持
-- 或使用CLR集成扩展正则功能
```

### 与LIKE的区别

| 特性        | LIKE       | REGEXP       |
|------------|------------|--------------|
| 匹配方式    | 简单模式    | 复杂模式      |
| 通配符      | %, _       | 丰富元字符    |
| 性能        | 通常更快    | 通常更慢      |
| 灵活性      | 有限        | 非常灵活      |

## 2. 正则表达式语法详解

### 基本匹配

```sql
-- 匹配包含"apple"的记录
SELECT * FROM products WHERE name REGEXP 'apple';

-- 精确匹配"apple"（使用定位符）
SELECT * FROM products WHERE name REGEXP '^apple$';
```

### 元字符

| 元字符 | 描述                          | 示例                |
|--------|-------------------------------|---------------------|
| .      | 匹配任意单个字符               | 'a.c'匹配"abc"、"a c" |
| ^      | 匹配字符串开头                 | '^a'匹配"apple"     |
| $      | 匹配字符串结尾                 | 'e$'匹配"apple"     |
| *      | 前导字符零次或多次             | 'a*'匹配"", "a", "aa" |
| +      | 前导字符一次或多次             | 'a+'匹配"a", "aa"   |
| ?      | 前导字符零次或一次             | 'a?'匹配"", "a"     |
| {n}    | 前导字符恰好n次                | 'a{2}'匹配"aa"      |
| {n,}   | 前导字符至少n次                | 'a{2,}'匹配"aa", "aaa" |
| {n,m}  | 前导字符n到m次                 | 'a{2,4}'匹配"aa", "aaa", "aaaa" |

### 字符类

```sql
-- 匹配元音字母
SELECT * FROM words WHERE word REGEXP '[aeiou]';

-- 匹配非元音字母
SELECT * FROM words WHERE word REGEXP '[^aeiou]';

-- 匹配数字
SELECT * FROM data WHERE value REGEXP '[0-9]';

-- 匹配字母
SELECT * FROM data WHERE value REGEXP '[a-zA-Z]';
```

### 预定义字符类

| 模式   | 等价于       | 描述                |
|--------|--------------|---------------------|
| \\d    | [0-9]        | 数字                |
| \\D    | [^0-9]       | 非数字              |
| \\w    | [a-zA-Z0-9_] | 单词字符            |
| \\W    | [^a-zA-Z0-9_]| 非单词字符          |
| \\s    | [ \t\r\n\f]  | 空白字符            |
| \\S    | [^ \t\r\n\f] | 非空白字符          |

### 分组和捕获

```sql
-- 匹配重复的单词
SELECT * FROM text WHERE content REGEXP '\\b(\\w+)\\s+\\1\\b';

-- 提取区号和电话号码
SELECT REGEXP_REPLACE(phone, '(\\d{3})(\\d{3})(\\d{4})', '(\\1) \\2-\\3')
FROM contacts;
```

## 3. 数据库特定的正则表达式函数

### MySQL

```sql
-- 基本匹配
SELECT * FROM table WHERE column REGEXP 'pattern';

-- 替换
SELECT REGEXP_REPLACE(column, 'pattern', 'replacement') FROM table;

-- 提取
SELECT REGEXP_SUBSTR(column, 'pattern') FROM table;
```



## 4. 实际应用示例

### 示例1：验证电子邮件格式

```sql
-- MySQL
SELECT email FROM users 
WHERE email REGEXP '^[A-Za-z0-9._%-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,4}$';
```

### 示例2：提取URL中的域名

```sql
-- MySQL
SELECT 
  url,
  REGEXP_SUBSTR(url, 'https?://([^/]+)') AS domain
FROM web_logs;
```

### 示例3：查找重复单词

```sql
-- MySQL
SELECT id, content 
FROM articles 
WHERE content REGEXP '\\b(\\w+)\\s+\\1\\b';
```

### 示例4：格式化电话号码

```sql
-- MySQL
SELECT 
  phone,
  REGEXP_REPLACE(phone, '^(\\d{3})(\\d{3})(\\d{4})$', '(\\1) \\2-\\3') AS formatted_phone
FROM contacts;
```

## 5. 性能优化

### 索引使用

- 正则表达式通常**不能**使用普通索引
- 考虑使用函数索引（如Oracle、PostgreSQL支持）
- 对于复杂模式，考虑使用全文搜索

### 优化技巧

1. **避免前导通配符**：如`.*pattern`效率很低
2. **使用更具体的模式**：越具体越好
3. **考虑预编译正则**：某些数据库支持
4. **限制检查范围**：先缩小数据集再应用正则

### 替代方案

```sql
-- 先使用LIKE缩小范围，再用正则精确匹配
SELECT * FROM logs 
WHERE message LIKE '%error%' 
  AND message REGEXP 'error [0-9]{4}';
```

## 6. 高级正则表达式技巧

### 回溯引用

```sql
-- 查找重复的单词
SELECT content FROM documents
WHERE content REGEXP '\\b(\\w+)\\s+\\1\\b';
```

### 非贪婪匹配

```sql
-- 默认贪婪匹配
SELECT REGEXP_SUBSTR('<b>bold</b> and <i>italic</i>', '<.*>');

-- 非贪婪匹配（MySQL）
SELECT REGEXP_SUBSTR('<b>bold</b> and <i>italic</i>', '<.*?>');

-- 非贪婪匹配（PostgreSQL）
SELECT REGEXP_MATCHES('<b>bold</b> and <i>italic</i>', '<.*?>', 'g');
```

### 条件表达式

```sql
-- Oracle条件正则
SELECT 
  CASE 
    WHEN REGEXP_LIKE(phone, '^\\d{3}-\\d{3}-\\d{4}$') THEN 'Valid'
    ELSE 'Invalid'
  END AS phone_status
FROM customers;
```

## 7. 常见问题解决方案

### 问题1：匹配特殊字符

```sql
-- 匹配包含点号的内容（需要转义）
SELECT * FROM data WHERE value REGEXP '\\.';

-- 匹配包含反斜杠的内容
SELECT * FROM data WHERE value REGEXP '\\\\';
```

### 问题2：多行匹配

```sql
-- PostgreSQL多行模式
SELECT * FROM text WHERE content ~ '(?m)^error';

-- MySQL多行模式（8.0+）
SELECT * FROM text WHERE content REGEXP '(?m)^error';
```

### 问题3：Unicode匹配

```sql
-- PostgreSQL Unicode匹配
SELECT * FROM text WHERE content ~ '\\p{L}'; -- 任何字母

-- MySQL Unicode基本支持
SELECT * FROM text WHERE content REGEXP '[\\x{4e00}-\\x{9fff}]'; -- 中文字符
```

## 8. 跨数据库正则表达式比较

| 功能               | MySQL | PostgreSQL | Oracle | SQL Server |
|--------------------|-------|------------|--------|------------|
| 基本匹配           | ✓     | ✓          | ✓      | 有限       |
| 不区分大小写匹配   | ✓     | ✓          | ✓      | ✓          |
| 替换功能           | 8.0+  | ✓          | ✓      | 有限       |
| 提取子串           | 8.0+  | ✓          | ✓      | 有限       |
| 计数匹配           | ✗     | ✓          | ✓      | ✗          |
| 多行模式           | 8.0+  | ✓          | ✓      | ✗          |
| Unicode支持        | 有限  | 优秀       | 优秀   | 有限       |
