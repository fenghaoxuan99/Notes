<!--
 * @Author: feng 1804831168@qq.com
 * @Date: 2024-07-18 20:17:11
 * @LastEditors: feng 1804831168@qq.com
 * @LastEditTime: 2025-02-12 14:33:37
 * @Description:
 * Copyright (c) 2025 by Feng, All Rights Reserved.
-->

@property 把一个 getter 方法变成属性，只需要加上@property 就可以了
@score.setter，负责把一个 setter 方法变成属性赋值，于是，我们就拥有一个可控的属性操作,可写

```py
class Student(object):
    @property
    def score(self):
        return self._score

    @score.setter
    def score(self, value):
        if not isinstance(value, int):
            raise ValueError('score must be an integer!')
        if value < 0 or value > 100:
            raise ValueError('score must between 0 ~ 100!')
        self._score = value
```
