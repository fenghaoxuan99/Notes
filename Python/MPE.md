<!--
 * @Author: feng 1804831168@qq.com
 * @Date: 2024-07-17 15:35:58
 * @LastEditors: feng 1804831168@qq.com
 * @LastEditTime: 2025-02-12 11:05:28
 * @Description:
 * Copyright (c) 2025 by Feng, All Rights Reserved.
-->

# MPE 使用

## 空格

- &nbsp
  不换行空格，全称是 No-Break Space，它是最常见和我们使用最多的空格，大多数的人可能只接触了 ，它是按下 space 键产生的空格。在 HTML 中，如果你用空格键产生此空格，空格是不会累加的（只算 1 个）。要使用 html 实体表示才可累加，该空格占据宽度受字体影响明显而强烈。
- &ensp
  半角空格，全称是 En Space，en 是字体排印学的计量单位，为 em 宽度的一半。根据定义，它等同于字体度的一半（如 16px 字体中就是 8px）。名义上是小写字母 n 的宽度。此空格传承空格家族一贯的特性：透明的，此空格有个相当稳健的特性，就是其占据的宽度正好是 1/2 个中文宽度，而且基本上不受字体影响。
- &emsp
  全角空格，全称是 Em Space，em 是字体排印学的计量单位，相当于当前指定的点数。例如，1 em 在 16px 的字体中就是 16px。此空格也传承空格家族一贯的特性：透明的，此空格也有个相当稳健的特性，就是其占据的宽度正好是 1 个中文宽度，而且基本上不受字体影响。

## 上标

30^th^

## 下标

H~2~O

## 脚注

Content [^1]
[^1]: Hi! This is a footnote

## 缩略

\*[HTML]: Hyper Text Markup Language
\_[W3C]: World Wide Web Consortium
The HTML specification
is maintained by the W3C.

## Admonitions 插件

!!! note This is the admonition title
This is the admonition body
==**note abstract info tip success question warning failure danger bug example quote**==

版权 (©) — &copy;
注册商标 (®) — &reg;
商标 (™) — &trade;
欧元 (€) — &euro;
左箭头 (←) — &larr;
上箭头 (↑) — &uarr;
右箭头 (→) — &rarr;
下箭头 (↓) — &darr;
度数 (°) — &#176;
圆周率 (π) — &#960;

> :button: **Warning:** Do not push the big red button.

:joy: **Warning:** Do not push the big red button.

:memo: **Note** Sunrises are beautiful.

> :bulb: **Tip:** Remember to appreciate the little things in life.
