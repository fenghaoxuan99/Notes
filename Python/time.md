<!--
 * @Author: feng 1804831168@qq.com
 * @Date: 2024-07-10 15:24:18
 * @LastEditors: feng 1804831168@qq.com
 * @LastEditTime: 2025-02-12 14:35:43
 * @Description:
 * Copyright (c) 2025 by Feng, All Rights Reserved.
-->

# time 模块

tm_year: 年
tm_mon: 月，范围为[1,12]
tm_mday: 一个月的第几天，范围为[1,31]
tm_hour: 小时,范围为[0,23]
tm_min: 分, 范围为[0,59]
tm_sec: 秒，范围为[0,59]
tm_wday: 一周中的第几天，范围是[0,6],周一为 0
tm_yday: 一年中的第几天，范围是[1,366]
tm_isdat: 1 代表夏令时

## mktime 函数

mktime()函数将 struct_time（或包含 9 个元素的元组对应于 struct_time）作为参数，并返回自当地时间的纪元以来经过的秒数。

```py
import time

t = (2018, 12, 28, 8, 44, 4, 4, 362, 0)

local_time = time.mktime(t)
print("Local time:", local_time)
```

## time.asctime()

该 asctime()函数将 struct_time（或包含 9 个元素的元组对应于 struct_time）作为参数，并返回表示它的字符串。这是一个实例：

```py
import time

t = (2018, 12, 28, 8, 44, 4, 4, 362, 0)

result = time.asctime(t)
print("Result:", result)
```

## time.strftime()

可以将时间元组定制化输出到时间字符串，其中的时间字符串可以定制格。
|格式控制符 |含义|
|:---: |:---:|
|%Y |四位数的年份，取值范围为 0001~9999,如 1900|
|%m |月份（01~12），例如 10|
|%d |月中的一天（01~31）例如：25|
|%B |本地完整的月份名称，比如 January|
|%b |本地简化的月份名称，比如 Jan|
|%a |本地简化的周日期，Mon~Sun,例如 Wed|
|%A |本地完整周日期，”Monday~Sunday,例如 Wednesday|
|%H |24 小时制小时数(00~23),例如：12|
|%M |分钟数（00~59），例如 26|
|%S |秒（00~59），例如 26|
|%l |12 小时制小时数（01~12），例如：7|
|%p |上下午，取值为 AM 或 PM|

```py
time.strftime("%H:%M:%S",time.localtime()) #返回当前时间的字符串
'01:41:38'

time.strftime("%Y-%m-%d %p %H:%M:%S",time.localtime()) #返回当前日期和时间的字符串
'2022-09-24 AM 01:42:10'
```

## time.strptime(str，format)

time.strptime()可以将字符串时间按照一定时间格式解析为时间元组。这里需要着重和 time.strftime()进行区分，因为这 2 个方法的名字只有 1 个字母不一样。我们可以用一个简单的方法进行区分：time.strftime()中的 f 代表 format，可以记成 str format time(字符串格式的时间) ；time.strptime()的 p 代表 parse，可以记为 str parse time(字符串解析成时间)。下面是本方法的代码举例：

```py
time.strptime('2011-09-11 08:40:22',"%Y-%m-%d %H:%M:%S")
time.struct_time(tm_year=2011, tm_mon=9, tm_mday=11, tm_hour=8, tm_min=40, tm_sec=22, tm_wday=6, tm_yday=254, tm_isdst=-1)
```

```py
import time
timestamp = time.time()

# 自纪元以来经过的秒数
seconds = 1545925769.9618232
local_time = time.ctime(seconds)
print("Local time:", local_time)


local_time = time.localtime()
print("当前时间：", local_time)
print("当前年份：", local_time.tm_year)
print("当前月份：", local_time.tm_mon)
print("当前日期：", local_time.tm_mday)
print("当前小时：", local_time.tm_hour)
print("当前分钟：", local_time.tm_min)
print("当前秒数：", local_time.tm_sec)


local_time = time.localtime()
formatted_time = time.strftime("%Y-%m-%d %H:%M:%S", local_time)
print("当前时间：", formatted_time)
```
