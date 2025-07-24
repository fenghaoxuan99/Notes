

# datetime模块

|类或方法	         | 描述|
|:---:              |:---:|
|datetime.date	    |表示一个具体的日期，包括年份、月份和日数|
|datetime.time	    |表示一个具体的时间，包括小时、分钟、秒和微秒|
|datetime.datetime	|表示一个具体的日期和时间，包括日期和时间的所有信息|
|datetime.timedelta	|表示两个日期或时间之间的差距|
|datetime.datetime.today() / datetime.datetime.now()	|返回表示当前日期和时间的对象|
|datetime.datetime.fromtimestamp(ts)	|将时间戳转换为datetime对象|
|datetime.datetime.strftime(format) / datetime.datetime.strptime(date_string, format)	|将datetime对象格式化为字符串，或将字符串解析为datetime对象|


## datetime.date类表示一个具体的日期

|属性	            |描述|
|:---:              |:---:|
|year	            |年份|
|month	            |月份|
|day	            |日数|
|weekday()	        |返回星期几（0表示星期一，6表示星期日）|
|isoweekday()	    |返回ISO规定的星期几（1表示星期一，7表示星期日）|
|isocalendar()	    |返回一个元组，其中包含年份、周数和星期几|
|strftime(format)	|将date对象格式化为字符串|


## datetime.time类表示一个具体的时

|属性	|描述|
|:---:  |:---:|
|hour	|小时|
|minute	|分钟|
|second	|秒|
|microsecond	|微秒|
|strftime(format)	|将time对象格式化为字符串

## datetime.datetime类

|属性	|描述|
|:---:  |:---:|
|year	|年份|
|month	|月份|
|day	|日数|
|hour	|小时|
|minute	|分钟|
|second	|秒|
|microsecond	|微秒|
|timestamp()	|返回时间戳（自1970年以来的秒数）|
|weekday()	    |返回星期几（0表示星期一，6表示星期日）|
|isoweekday()	|返回ISO规定的星期几（1表示星期一，7表示星期日）|
|isocalendar()	|返回一个元组，其中包含年份、周数和星期几
|strftime(format)	|将datetime对象格式化为字符串|

```py
import datetime

s = '2023-05-11 10:30:15'
dt = datetime.datetime.strptime(s, '%Y-%m-%d %H:%M:%S')
print(dt)  # 2023-05-11 10:30:15
```

## datetime.timedelta类表示两个日期或时间之间的差距

|属性	 |描述|
|:---:   |:---:|
|days	        |天数|
|seconds	    |秒数|
|microseconds	|微秒数|
|total_seconds()	|返回总秒数|



## datetime模块还提供了一些其他有用的函数和属性

|函数或属性	|描述|
|:---:|:---:|
|datetime.datetime.today() / datetime.datetime.now()	|返回表示当前日期和时间的对象|
|datetime.datetime.fromtimestamp(ts)	|将时间戳转换为datetime对象|
|datetime.datetime.astimezone(tz=None)	|返回带有时区信息的datetime对象|
|datetime.datetime.utcoffset()	|返回一个datetime对象相对于UTC的偏移量|
|datetime.date.today()	|返回表示当前日期的date对象|
|datetime.datetime.combine(date, time)	|将一个date对象和一个time对象组合成一个datetime对象|


### datetime.datetime.fromtimestamp(ts)使用实例
```py
import datetime

ts1 = datetime.datetime.now()
ts  = datetime.datetime.now().timestamp()
print(ts1)
print(ts)

ts = 1686790215.0 # 时间戳
dt = datetime.datetime.fromtimestamp(ts)

print(dt)  # 2023-05-12 02:30:15
```

