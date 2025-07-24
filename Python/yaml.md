
# YAML

YAML（YAML Ain't Markup Language）是一种人类可读的数据序列化语言。它的设计目标是使数据在不同编程语言之间交换和共享变得简单。YAML采用了一种简洁、直观的语法，以易于阅读和编写的方式表示数据结构。

## 基本语法规则
- 大小写敏感
- 使用缩进表示层级关系
- 缩进时不允许使用Tab键，只允许使用空格。
- 缩进的空格数目不重要，只要相同层级的元素左侧对齐即可
- \# 表示注释，从这个字符一直到行尾，都会被解析器忽略。

缩进：YAML使用空格进行缩进，通常是两个空格，不允许使用制表符（Tab）。
键值对：YAML的基本组成元素是键值对，键和值之间用冒号（:）分隔。
数据结构：

- 列表：使用短横线（-）表示列表项。
- 字典：使用冒号（:）后跟一个空格来表示键值对。
- 注释：以井号（#）开始的行是注释。
- 字符串：YAML中的字符串可以不用引号，如果字符串中包含特殊字符或需要换行，可以使用单引号(')或双引号(")。

多行字符串：使用|或>可以表示多行字符串，|保持原样输出，>则折叠空白。
**使用>表示多行字符串：折叠空白，即每行开头和结尾的空白会被删除**

锚点和别名：使用&和*可以创建锚点和别名。

标量类型：YAML支持多种标量类型，如整数、浮点数、布尔值等。

null值：用null、~或!!null表示。

```ruby
# 这是一个注释

person:
  name: John Doe
  age: 30
  is_student: false
  skills:
    - programming
    - cooking
    - painting

# 多行字符串示例
bio: |
  John is a skilled programmer and a great cook.
  He also enjoys painting in his spare time.

# 锚点和别名示例
items:
  - &shoes
    type: shoes
    price: 100
  - *shoes
  - &glasses
    type: glasses
    price: 150
  - *glasses

# 标量类型示例
numbers:
  - 1
  - 3.14
  - 42

# null值示例
nothing: null
```


数组：
```ruby
languages:
 - Ruby
 - Perl
 - Python 

也可以
languages: [ apple, orange, pear ]

companies:
    -
        id: 1
        name: company1
        price: 200W
    -
        id: 2
        name: company2
        price: 500W
        
意思是 companies 属性是一个数组，每一个数组元素又是由 id、name、price 三个属性构成。

数组也可以使用流式(flow)的方式表示：

companies: [{id: 1,name: company1,price: 200W},{id: 2,name: company2,price: 500W}]
```

字符串可以写成多行，从第二行开始，必须有一个单空格缩进。换行符会被转为空格。
```ruby
 string:
- Hello # 字符串默认不使用引号表示
# 使用双引号或者单引号包裹特殊字符
- ' world'
# 单引号和双引号都可以使用，双引号不会对特殊字符转义
- "Hello\nworld"
- 'Hello\nworld'
# 单引号之中如果还有单引号，必须连续使用两个单引号转义
- 'Curder''s iPhone'
# 字符串可以写成多行，从第二行开始，必须有一个单空格缩进，换行符会被转为空格
- Hello
  world
# 多行字符串可以使用 | 保留换行符，也可以使用 > 折叠换行
- |  # 多行字符串可以使用 | 保留换行符
  Hello
  world
- >  # 使用 > 折叠换行
  Hello
  world
- |+  # + 表示保留文字块末尾的换行
  Hello
- |-  # - 表示删除字符串末尾的换行
  Hello
- |  # 字符串之中可以插入 HTML 标记
  <p style="color: red">
    段落
  </p>

```


```ruby
str: 这是一段
  多行
  字符串

多行字符串可以使用|保留换行符，也可以使用>折叠换行。
this: |
  Foo
  Bar
that: >
  Foo
  Bar

  +表示保留文字块末尾的换行，-表示删除字符串末尾的换行。
s1: |
  Foo
s2: |+
  Foo
s3: |-
  Foo
```


## 引用
**锚点&和别名*，可以用来引用。**

```ruby
defaults: &defaults
  adapter:  postgres
  host:     localhost

development:
  database: myapp_development
  <<: *defaults

test:
  database: myapp_test
  <<: *defaults
=====等同于下面的代码==========
defaults:
  adapter:  postgres
  host:     localhost

development:
  database: myapp_development
  adapter:  postgres
  host:     localhost

test:
  database: myapp_test
  adapter:  postgres
  host:     localhost
```
**&用来建立锚点（defaults），<<表示合并到当前数据，*用来引用锚点。**




```py
name: "John"
addr: "长沙"
age: 18
status: null
isStudent: true
fruits:
    - apple
    - banana
    - orange
teacher:
    name: "Alex"
    age: 30
```


## 读取yaml文件
```py
import yaml
 
with open(file="d.yaml", mode="r",encoding='utf-8') as f:
    res = yaml.safe_load(f)
print(res)
```


## python将数据写入yaml文件
```py
import yaml
 
data = {
    "Person": {
        "name": "John",
        "age": 30,
        "address": {
            "street": "123 Main St",
            "city": "Anytown",
            "state": "CA"
        }
    }
}
# 将data变量存储的数据写入YAML文件
with open(file="example.yaml", mode="w") as f:
    yaml.dump(data, f)
```
