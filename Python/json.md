
# JSON 文件

JSON（JavaScript Object Notation，JavaScript对象表示法）是一种轻量级的数据交换格式，易于人阅读和编写，同时也易于机器解析和生成。它基于JavaScript的一个子集，但JSON是语言无关的，很多编程语言都支持JSON格式。

以下是JSON文件的一些基本格式和语法规则：

1. **数据类型**：JSON支持以下几种数据类型：
   - 字符串（String）： 用双引号`"`括起来。
   - 数字（Number）：   没有引号的数字。
   - 布尔值（Boolean）：`true`或`false`。
   - 空值（Null）：     用`null`表示。
   - 对象（Object）：   用大括号`{}`括起来，包含键值对。
   - 数组（Array）：    用方括号`[]`括起来，可以包含任意类型的数据。

2. **键值对**：JSON中的每个键值对由一个字符串键和一个值组成，键和值之间用冒号（:）分隔。

3. **对象**：对象是一系列键值对的集合，键值对之间用逗号（,）分隔。

4. **数组**：数组是有序的数据集合，数组中的元素可以是任何类型的值。

5. **缩进**：虽然JSON格式不要求缩进，但为了可读性，通常会使用缩进来组织结构。

6. **特殊字符**：字符串中的某些特殊字符需要转义，如双引号`\"`、反斜杠`\\`、控制字符（如换行符`\n`、制表符`\t`）等。

7. **注释**：JSON标准本身不支持注释，但某些实现可能允许使用注释。

8. **编码**：JSON通常使用UTF-8编码。


**JSON数据就是一段字符串而已**，只不过有不同意义的分隔符将其分割开来而已：
1. []中括号代表的是一个数组；
2. {}大括号代表的是一个对象
3. 双引号“”表示的是属性值
4. 冒号：代表的是前后之间的关系，冒号前面是属性的名称，后面是属性的值，这个值可以是基本数据类型，也可以是引用数据类型。



## JSON文件的示例：

```json
{
  "name": "John Doe",
  "age": 30,
  "is_student": false,
  "skills": ["programming", "cooking", "painting"],
  "address": {
    "street": "123 Main St",
    "city": "Anytown",
    "zip": "12345"
  },
  "bio": "John is a skilled programmer and a great cook. He enjoys painting in his spare time."
}
```

例子中：
- `name`、`age`、`is_student`、`skills`、`address`和`bio`是键。
- `"John Doe"`、`30`、`false`、`["programming", "cooking", "painting"]`、JSON对象`{"street": "123 Main St", "city": "Anytown", "zip": "12345"}`和`"John is a skilled programmer and a great cook. He enjoys painting in his spare time."`是对应的值。
- `skills`是一个数组，包含了多个字符串。
- `address`是一个对象，包含了街道、城市和邮编。




## 简单例子1
```py
import json

# 生成JSON数据
data = {
    "name": "Jane Doe",
    "age": 25,
    "skills": ["writing", "designing"]
}

# 将Python字典转换为JSON字符串
json_string = json.dumps(data)
print(json_string)

# 将JSON字符串写入文件
with open('data.json', 'w') as json_file:
    json.dump(data, json_file, indent=4) // 缩进为4个空格

# 从文件读取JSON字符串并解析
with open('data.json', 'r') as json_file:
    parsed_data = json.load(json_file)
    print(parsed_data['name'])  # 输出: Jane Doe
```

## 将自定义Python对象转换为JSON对象

要将对象转换为JSON，我们需要编写一个扩展JSONEncoder的新类。在这个类中，需要实现default()。此方法将具有返回JSON的自定义代码。
以下是Country类的示例编码器。这个类将帮助将Python对象转换为JSON对象：
```py
import json
 
class CountryEncoder(json.JSONEncoder):
     def default(self, o):
     if isinstance(o, Country):
            # JSON object would be a dictionary.
                return {
                "name" : o.name,
                "population": o.population,
                "languages": o.languages
           }
     else:
           # Base class will raise the TypeError.
           return super().default(o)


class Country:
    def __init__(self, name, population, languages):
        self.name = name
        self.population = population
        self.languages = languages

# 创建一个Country对象
country = Country("Germany", 83000000, ["German"])

# 使用自定义的CountryEncoder将Country对象转换为JSON字符串
import json

json_string = json.dumps(country, cls=CountryEncoder)
print(json_string)


```