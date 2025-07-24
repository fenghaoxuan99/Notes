<!--
 * @Author: feng 1804831168@qq.com
 * @Date: 2024-07-10 11:46:30
 * @LastEditors: feng 1804831168@qq.com
 * @LastEditTime: 2025-02-12 14:31:02
 * @Description:
 * Copyright (c) 2025 by Feng, All Rights Reserved.
-->

## 使用大括号 {} 创建空字典：

my_dict = {}

## 使用键值对创建字典：

my_dict = {'key1': 'value1', 'key2': 'value2', 'key3': 'value3'}

## 使用 dict() 构造函数：

my_dict = dict(key1='value1', key2='value2', key3='value3')

## 使用列表/元组的列表转换为字典：

```py
列表中每个元素是一个包含键值对的元组
my_list = [('key1', 'value1'), ('key2', 'value2'), ('key3', 'value3')]
my_dict = dict(my_list)
```

## 或者使用 zip 函数

```py
keys   = ['key1', 'key2', 'key3']
values = ['value1', 'value2', 'value3']
my_dict = dict(zip(keys, values))
```

## 使用推导式出创建一个列表

```py
# 创建一个简单的字典，键为数字 0 到 4，值为它们的平方
my_dict = {x: x**2 for x in range(5)}
print(my_dict)
# 输出: {0: 0, 1: 1, 2: 4, 3: 9, 4: 16}
```

my_dict = {"a": 1, "b": 2}

# 使用 update() 方法批量添加或更新键值对

```py
my_dict.update({"d": 4, "e": 5})

print(my_dict)
# 输出: {'a': 1, 'b': 5, 'c': 3, 'd': 4, 'e': 5}
```

my_dict = {"a": 1, "b": 2}

# 使用 setdefault() 方法添加键值对，但仅在键不存在时

my_dict.setdefault("f", 6)

print(my_dict)

# 输出: {'a': 1, 'b': 2, 'f': 6}

# 如果键已存在，则不会进行任何操作

```py
my_dict.setdefault("a", 10)

print(my_dict)

# 输出:{'a': 1, 'b': 2, 'f': 6}

my_dict = {"a": 1, "b": 2, "c": 3}
```

# 删除键为 "b" 的键值对，并返回被删除的值

```py
deleted_value = my_dict.pop("b")

print(deleted_value) # 输出: 2
print(my_dict) # 输出: {'a': 1, 'c': 3}
```

## 序号 函数及描述

1 **dict.clear()**
删除字典内所有元素
2 **dict.copy()**
返回一个字典的浅复制
3 **dict.fromkeys(seq[, val])**
创建一个新字典，以序列 seq 中元素做字典的键，val 为字典所有键对应的初始值
4 **dict.get(key, default=None)**
返回指定键的值，如果值不在字典中返回 default 值
5 **dict.has_key(key)**
如果键在字典 dict 里返回 true，否则返回 false。Python3 不支持。
6 **dict.items()**
以列表返回可遍历的(键, 值) 元组数组
7 **dict.keys()**
以列表返回一个字典所有的键
8 **dict.setdefault(key, default=None)**
和 get()类似, 但如果键不存在于字典中，将会添加键并将值设为 default
9 **dict.update(dict2)**
把字典 dict2 的键/值对更新到 dict 里
10 **dict.values()**
以列表返回字典中的所有值
11 **pop(key[,default])**
删除字典给定键 key 所对应的值，返回值为被删除的值。key 值必须给出。 否则，返回 default 值。
12 **popitem()**
返回并删除字典中的最后一对键和值。
