# MySQL优化全攻略：从SQL到架构的深度优化指南

## SQL查询优化（低成本高回报）

### 核心优化策略

1. **索引优化黄金法则**
   - 确保WHERE、JOIN、ORDER BY子句中的字段有索引
   - 遵循"最左前缀"原则设计联合索引
   - 示例：`INDEX(name, age)`可优化：
     ```sql
     SELECT * FROM users WHERE name='张三' AND age>20;
     SELECT * FROM users ORDER BY name, age;
     ```

2. **EXPLAIN深度解读**
   - 重点关注type列（最好到ref/range）
   - 警惕Using filesort和Using temporary
   - 新版本推荐使用`EXPLAIN ANALYZE`获取实际执行数据

3. **避免索引失效的雷区**
   - 隐式类型转换：`WHERE phone=13800138000`（phone是varchar）
   - 函数操作：`WHERE DATE(create_time)='2023-01-01'`
   - 模糊查询：`LIKE '%关键词%'`（前导通配符）

### 高级优化技巧

1. **分页查询优化**
   ```sql
   -- 低效写法
   SELECT * FROM orders LIMIT 1000000, 20;
   
   -- 优化方案（延迟关联）
   SELECT * FROM orders o
   JOIN (SELECT id FROM orders ORDER BY create_time LIMIT 1000000, 20) t
   ON o.id = t.id;
   ```

2. **COUNT优化方案**
   - 精确计数：`SELECT COUNT(*) FROM table FORCE INDEX(primary)`
   - 近似计数：`SHOW TABLE STATUS LIKE 'table_name'`
   - 计数表：维护专用计数表

3. **JOIN优化策略**
   - 小表驱动大表原则
   - 确保关联字段有索引
   - 复杂查询考虑拆分为多个简单查询

## 库表结构优化（中成本持续收益）

### 表设计规范

1. **数据类型最佳实践**
   - 整型：优先TINYINT/SMALLINT
   - 字符串：VARCHAR按需分配，避免CHAR(255)
   - 时间：TIMESTAMP(4字节) vs DATETIME(8字节)

2. **范式与反范式平衡**
   - 3NF保证数据一致性
   - 适度反范式提升查询性能
   - 典型案例：订单表冗余用户姓名

3. **分区表实战**
   ```sql
   -- 按时间范围分区
   CREATE TABLE logs (
     id INT,
     log_time DATETIME,
     content TEXT
   ) PARTITION BY RANGE (TO_DAYS(log_time)) (
     PARTITION p202301 VALUES LESS THAN (TO_DAYS('2023-02-01')),
     PARTITION pmax VALUES LESS THAN MAXVALUE
   );
   ```

### 索引设计进阶

1. **覆盖索引优化**
   ```sql
   -- 普通查询
   SELECT id,name FROM users WHERE age>20;  -- 需要回表
   
   -- 优化为覆盖索引
   ALTER TABLE users ADD INDEX idx_age_name(age, name);
   ```

2. **索引合并策略**
   - `index_merge`优化参数调整
   - 优先使用联合索引而非索引合并

3. **自适应哈希索引**
   - 配置`innodb_adaptive_hash_index`
   - 监控`SHOW ENGINE INNODB STATUS`中的hash searches

## 系统配置优化（需重启生效）

### InnoDB核心参数

| 参数名                      | 推荐值          | 说明                          |
|----------------------------|----------------|-------------------------------|
| innodb_buffer_pool_size     | 物理内存的70%-80% | 数据缓存池                     |
| innodb_log_file_size        | 1-4GB          | 重做日志大小                   |
| innodb_flush_neighbors      | 0(SSD)/1(HDD)  | 刷盘优化                       |
| innodb_read_io_threads      | 4-16           | 读IO线程数                    |
| innodb_write_io_threads     | 4-16           | 写IO线程数                    |

### 全局服务器参数

```ini
[mysqld]
# 连接管理
max_connections=1000
thread_cache_size=50

# 查询缓存(8.0已移除)
query_cache_type=0

# 临时表
tmp_table_size=64M
max_heap_table_size=64M

# 排序优化
sort_buffer_size=4M
join_buffer_size=4M
```

## 硬件优化（高成本方案）

### 升级路线图

1. **存储介质演进**
   - HDD → SATA SSD → NVMe SSD → Optane
   - 随机IOPS提升1000倍

2. **内存扩展**
   - 建议配置：数据热集的1.5倍
   - 监控`Innodb_buffer_pool_reads`指标

3. **CPU选择**
   - 高频核心：OLTP场景
   - 多核心：分析型查询

### 架构级优化

1. **读写分离**
   - 一主多从架构
   - 使用ProxySQL中间件

2. **分库分表**
   - 水平拆分：按ID范围/哈希
   - 垂直拆分：按业务模块

3. **缓存层引入**
   - Redis缓存热点数据
   - 本地缓存(Caffeine)

## 监控与持续优化

### 关键性能指标

```sql
-- 查看慢查询
SELECT * FROM mysql.slow_log ORDER BY start_time DESC LIMIT 10;

-- 索引使用统计
SELECT object_schema, object_name, index_name,
       count_read, count_fetch 
FROM performance_schema.table_io_waits_summary_by_index_usage;

-- 锁等待分析
SELECT * FROM sys.innodb_lock_waits;
```

### 优化周期建议

1. 每日：检查慢查询日志
2. 每周：分析索引使用情况
3. 每月：全面性能评估
4. 每季度：架构评审

通过这四层优化体系的配合实施，可使MySQL性能提升10-100倍，同时保持合理的成本投入。建议从SQL优化开始逐步深入，根据业务需求选择适当的优化层级。