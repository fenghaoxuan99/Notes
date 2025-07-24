# CSV 文件

​CSV 代表 Comma Separated Values。CSV 文件是一种纯文本文件，用于存储表格和电子表格信息。内容通常是由文本、数字或日期组成的表格。使用将数据存储在表中的程序可以轻松导入和导出 CSV 文件。
通常 CSV 文件的第一行包含表格列标签。后面的每一行表示表中的一行。逗号分隔行中的每个单元格，这就是名称的来源。

## 通过创建 writer 对象写入

```py
import csv

# 数据
person = [('xxx', 18, 193), ('yyy', 18, 182), ('zzz', 19, 185)]
# 表头
header = ['name', 'age', 'height']

with open('person.csv', 'w', encoding='utf-8', newline='') as file_obj:
    # 创建对象
    writer = csv.writer(file_obj)
    # 写表头
    writer.writerow(header)
    # 遍历，将每一行的数据写入csv
    for p in person:
        writer.writerow(p)
```

要注意这个 newline 参数，默认是\n，即换行符，csv 文件中每一行的末尾都是\n，所以在写入的时候，会自动加上\n，导致文件中出现空行。

```py
import csv
# 数据
person = [('xxx', 18, 193), ('yyy', 18, 182), ('zzz', 19, 185)]
# 表头
header = ['name', 'age', 'height']

with open('person.csv', 'w', encoding='utf-8', newline='') as file_obj:
    # 创建对象
    writer = csv.writer(file_obj)
    # 写表头
    writer.writerow(header)
    # 3.写入数据(一次性写入多行)
    writer.writerows(person)
```

## 通过创建 DictWriter 对象写入

```py
import csv

# 数据
person = [
    {'name': 'xxx', 'age': 18, 'height': 193},
    {'name': 'yyy', 'age': 18, 'height': 182},
    {'name': 'zzz', 'age': 19, 'height': 185},
]
# 表头
header = ['name', 'age', 'height']

with open('person.csv', 'w', encoding='utf-8', newline='') as file_obj:
    # 1.创建DicetWriter对象
    dictWriter = csv.DictWriter(file_obj, header)
    # 2.写表头
    dictWriter.writeheader()
    # 3.写入数据(一次性写入多行)
    dictWriter.writerows(person)
```

## 通过 reader()读取

```py
import csv

with open('person.csv', 'r', encoding='utf-8') as file_obj:
    # 1.创建reader对象
    reader = csv.reader(file_obj)
    # 2.遍历进行读取数据
    for r in reader:
        print(r)
```

## 通过 DictReader()读取

```py
import csv

with open('person.csv', 'r', encoding='utf-8') as file_obj:
    # 1.创建reader对象
    dictReader = csv.DictReader(file_obj)
    # 2.遍历进行读取数据
    for r in dictReader:
        print(r)
```
