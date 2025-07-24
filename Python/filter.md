

# filter函数

Python中的filter函数是一种内置的高效过滤器，用于从序列中筛选出符合特定条件的元素，生成一个由符合条件元素组成的新列表。filter函数在Python中广泛应用于数据清洗和预处理。

## filter函数基本用法
语法结构
filter(function, iterable)

参数说明:
- function：用于筛选元素的函数，该函数应返回一个布尔值，表示元素是否符合筛选条件。
- iterable：待筛选的序列，可以是列表、元组、集合等可迭代对象。
返回值:
- filter函数返回一个由符合条件的元素组成的新列表。

## 筛选符合条件的元素
```py
# 筛选符合条件的元素
filter函数可以用于从序列中筛选出符合特定条件的元素。例如，从一个列表中筛选出所有偶数：

numbers = [1, 2, 3, 4, 5, 6, 7, 8, 9]  
even_numbers = filter(lambda x: x % 2 == 0, numbers)  
print(list(even_numbers))  # Output: [2, 4, 6, 8]
```
## 数据清洗和预处理
```py
strings = ['hello', '', 'world', None, 'python', 'programming']  
clean_strings = filter(lambda x: x or x.strip(), strings)  
print(list(clean_strings))  # Output: ['hello', 'world', 'python', 'programming']
```
该函数用于判断一个字符串是否为非空字符串或者可以去除空格后的非空字符串。filter函数将这个函数应用于序列中的每个元素，筛选出符合条件的非空字符串元素。

## 与其他函数结合使用
```py
numbers = [1, 2, 3, 4, 5, 6, 7, 8, 9]  
even_numbers         = filter(lambda x: x % 2 == 0, numbers)  
squared_even_numbers = map(lambda x: x ** 2, even_numbers)  
print(list(squared_even_numbers))  # Output: [4, 16, 36, 64]
```
首先使用filter函数筛选出列表中的偶数元素，然后使用map函数将筛选出的元素平方。

## 去除序列中所有值为假的元素
如果将 filter() 函数的第一个参数 function 的值设置为 None，就会默认去除序列中所有值为假的元素，如 None、False、0、’’、()、[] 和 {}等，代码如下：
```py
list1 = ['', False, 1, 0, None, [], 3, 4, [1,23]]
print(list(filter(None, list1)))

```