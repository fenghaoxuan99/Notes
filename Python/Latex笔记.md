
<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->
- [插入公式](#插入公式)
- [插入公式](#插入公式)
  - [左对齐例子：](#左对齐例子)
  - [居中对齐例子：](#居中对齐例子)
- [注释](#注释)
- [编号](#编号)
- [自动编号](#自动编号)
- [换行与对齐](#换行与对齐)
  - [换行](#换行)
  - [对齐](#对齐)
- [空格](#空格)
- [上下标](#上下标)
- [绝对值](#绝对值)
- [范数](#范数)
- [括号](#括号)
  - [方法2](#方法2)
- [分式](#分式)
- [根式](#根式)
- [对数](#对数)
- [省略号](#省略号)
- [最值](#最值)
- [方程组和分段函数](#方程组和分段函数)
  - [方程组](#方程组)
  - [分段函数](#分段函数)
- [累加和累乘](#累加和累乘)
- [矢量](#矢量)
  - [例子：](#例子)
  - [例子：](#例子-1)
- [极限](#极限)
- [导数](#导数)
- [偏导](#偏导)
- [梯度](#梯度)
- [积分](#积分)
- [矩阵](#矩阵)
  - [基础矩阵](#基础矩阵)
  - [带括号的矩阵](#带括号的矩阵)
- [使用特殊的matrix](#使用特殊的matrix)
  - [元素省略的矩阵](#元素省略的矩阵)
- [表格](#表格)

<!-- /code_chunk_output -->




## 插入公式
左对齐公式（行中公式）：$数学公式$
居中公式（独立公式）：$$数学公式$$

### 左对齐例子：
$x+y=z$
### 居中对齐例子：
$$x+y=z$$

## 注释
%为单行注释。

例子1：

$$
%第一个极限
\lim_{n \to +\infty} \frac{1}{n(n+1)}
\quad %空一格
and %英文单词and
\quad %空一格
%第2个极限
\lim_{x\leftarrow{example} \infty} \frac{1}{n(n+1)}
$$

## 编号
在公式末尾使用\tag{编号}来实现公式手动编号，大括号内的内容可以自定义。
例子2：
$$
x+y=z
\tag{1}
$$

## 自动编号
使用\begin{equation}和\end{equation}进行公式输入，要同时使用，且编号不能够修改。
例子3：
$$
\begin{equation}
a^2+b^2=c^2
\end{equation}
$$

## 换行与对齐
### 换行
使用\\进行换行，最后一行的\\可写可不写。
例子4：
$$
f(x)=2x+1 \\
=2+1 \\
=3
$$

### 对齐
使用\begin{aligned}进行对齐，&表示对齐位置，一般都在=前面。
例子：
$$
\begin{aligned}
f(x)&=2x+1 \\
&=2+1 \\
&=3
\end{aligned}
$$

## 空格
\quad：空一格
\qquad：空两格

例子：
$$x \quad y \qquad z$$

## 上下标
^表示上标， _ 表示下标。如果上下标的内容多于一个字符，需要用 {}将这些内容括成一个整体。上下标可以嵌套，也可以同时使用。

例子：
$$x^{y^z_w}=(1+{\rm e}^x)^{-2xy^w}$$

## 绝对值
|、\vert、\mid可以表示绝对值。由以下示例可以看出，使用|或\vert效果相同，使用\mid在字母与符号之间的间隔较大，不美观，因此推荐使用|或\vert。

示例：
$|x|$
$\vert x \vert$
$\mid x \mid$

## 范数
使用方法与绝对值相同，只是连续输入2个绝对值符号而已，示例如下。
示例：
$$L_p=||x||_p$$
或
$$L_p=\vert\vert x \vert\vert_p$$

## 括号
==()、[]、|表示符号本身，使用 \{\} 来表示 {}==。当要显示大号的括号或分隔符时，要用 \left 和 \right 命令，如$\left(表达式\right)$

$$f(x,y,z) = 3y^2z \left( 3+\frac{7x+5}{1+y^2} \right)$$

$$
f\left(
   \left[
     \frac{
       1+\left\{x,y\right\}
     }{
       \left(
          \frac{x}{y}+\frac{y}{x}
       \right)
       \left(u+1\right)
     }+a
   \right]^{3/2}
\right)
$$

==有时候要用\left.或\right.进行匹配而不显示本身==。

例子：
$$\left. \frac{ {\rm d}u}{ {\rm d}x} \right| _{x=0}$$

### 方法2
使用\big和\bigg来创建逐级变大的括号，包含 (圆括号)、[方括号]、|绝对值|。

例子：
$$\bigg( \big( ( ) \big) \bigg)$$
$$\bigg[ \big[ [ ] \big] \bigg]$$
$$\bigg| \big| | | \big| \bigg|$$


## 分式
通常使用 \frac {分子} {分母} 命令产生一个分式，分式可嵌套。
如果分式很复杂，亦可使用 分子 \over 分母 命令，此时分式仅有一层。

例子：
$$\frac{a-1}{b-1} \quad and \quad {a+1\over b+1}$$

## 根式
\sqrt [根指数] {被开方数}
注意：缺省根指数时为2

例子：
$$\sqrt{2} \quad and \quad \sqrt[n]{x+y}$$

## 对数
\log_{对数底数}{表达式}
表达式的大括号可省略
$$
\log_{2}{x} \quad and \quad \log_{10}{x} \quad and \quad \log_{x}{y}
$$

 
## 省略号
数学公式中常见的省略号有两种，\ldots 表示与文本底线对齐的横向省略号 … \ldots…，\cdots 表示与文本中线对齐的横向省略号 ⋯ \cdots⋯，\vdots表示纵向省略号 ⋮ \vdots⋮，\ddots表示斜向省略号 ⋱ \ddots⋱。

例子：
$$f(x_1,x_2,\underbrace{\ldots}_{\ ldots} ,x_n) = x_1^2 + x_2^2 + \overbrace{\cdots}^{\rm cdots} + x_n^2$$

## 最值
\max_{下标表达式}{最值表达式}表示最大值，\min_{下标表达式}{最值表达式}表达最小值。
例子：
$$||x||_\infty=\max_{1\leq i\leq n}{|x_i|}$$

## 方程组和分段函数
### 方程组
方程组有2种方式，分别是\begin{aligned}和\begin{cases}方式，&表示对齐位置，推荐使用\begin{cases}方式，使用方法如下：

\begin{aligned}方式：
需配合\left\{使用，可以使方程组根据=对齐。

$$
\left\{
\begin{aligned}
&a+b+c=2 \\
&a-b=4 \\
\end{aligned}
\right.
$$


### 分段函数
分段函数可以通过\begin{cases}方式实现，不同的是方程式和条件之间要用&符号隔开。

例子：
$$
y =
\begin{cases}
\sin(x)       & x<0 \\
x^2 + 2x +4   & 0 \leq x < 1 \\
x^3           & x \geq 1 \\
\end{cases}
$$


## 累加和累乘
使用 \sum_{下标表达式}^{上标表达式}{累加表达式}来输入一个累加。
与之类似，使用 \prod \bigcup \bigcap来分别输入累乘、并集和交集。
此类符号在行内显示时上下标表达式将会移至右上角和右下角。

例子：
$$\sum_{i=1}^n \frac{1}{i^2} \quad and \quad \prod_{i=1}^n \frac{1}{i^2} \quad and \quad \bigcup_{i=1}^{2} R$$


## 矢量
使用 \vec{矢量}来表示一个矢量。
也可以使用 \overrightarrow等命令自定义字母上方的符号。

### 例子：
$$\vec{a} \cdot \vec{b} = 0$$

### 例子：
$$\underleftarrow{xy} \quad and \quad \overleftrightarrow{xy} \quad and \quad \overrightarrow{xy}$$


## 极限
\lim_{变量 \to 表达式} 表达式
如有需求，可以更改 \to 符号至任意符号。

例子：
$$\lim_{n \to +\infty} \frac{1}{n(n+1)} \quad and \quad \lim_{x\leftarrow{example} \infty} \frac{1}{n(n+1)}$$


## 导数
${\rm d}x$或${\text d}x$或$\text{d}x$


## 偏导
$\frac{\partial y}{\partial x}$

## 梯度
$\nabla f(x)$

## 积分
\int_积分下限^积分上限 {被积表达式}
例子：
$$\int_0^1 {x^2} \, {\rm d}x$$


## 矩阵
### 基础矩阵
使用\begin{matrix}…\end{matrix} 这样的形式来表示矩阵，在\begin 与\end 之间加入矩阵中的元素即可。矩阵的行之间使用\\ 分隔，\\表示换行，列之间使用& 分隔，&表示对齐位置。

例子：

$$
\begin{matrix}
1 & x & x^2 \\
1 & y & y^2 \\
1 & z & z^2 \\
\end{matrix}
$$

### 带括号的矩阵
使用\left 与\right 表示括号

如果要对矩阵加括号，可以像上文中提到的一样，使用\left 与\right 配合表示括号符号。

例子：

$$
\left[
\begin{matrix}
1 & x & x^2 \\
1 & y & y^2 \\
1 & z & z^2 \\
\end{matrix}
\right]
$$


## 使用特殊的matrix

带括号的矩阵也可以使用特殊的matrix 。即替换\begin{matrix}…\end{matrix} 中matrix 为pmatrix ，bmatrix ，Bmatrix ，vmatrix , Vmatrix 。

pmatrix：$\begin{pmatrix}1 & 2 \\ 3 & 4\\ \end{pmatrix}$
bmatrix：$\begin{bmatrix}1 & 2 \\ 3 & 4\\ \end{bmatrix}$
Bmatrix：$\begin{Bmatrix}1 & 2 \\ 3 & 4\\ \end{Bmatrix}$
vmatrix：$\begin{vmatrix}1 & 2 \\ 3 & 4\\ \end{vmatrix}$
Vmatrix：$\begin{Vmatrix}1 & 2 \\ 3 & 4\\ \end{Vmatrix}$

### 元素省略的矩阵
可以使用\cdots ：⋯ \cdots⋯，\ddots：⋱ \ddots⋱ ，\vdots：⋮ \vdots⋮ ，来省略矩阵中的元素。

例子：

$$
\begin{pmatrix}
1&a_1&a_1^2&\cdots&a_1^n\\
1&a_2&a_2^2&\cdots&a_2^n\\
\vdots&\vdots&\vdots&\ddots&\vdots\\
1&a_m&a_m^2&\cdots&a_m^n\\
\end{pmatrix}
$$


## 表格
使用\begin{array}{列样式}…\end{array} 这样的形式来创建表格，列样式可以是clr 表示居中，左，右对齐，还可以使用| 表示一条竖线。表格中各行使用\\ 分隔，各列使用& 分隔。使用\hline 在本行前加入一条直线。
例子：      
$$
\begin{array}{|c|l|c|r|}
\hline
n & \text{Left} & \text{Center} & \text{Right} \\
\hline
1 & 0.24 & 1 & 125 \\
\hline
2 & -1 & 189 & -8 \\
\hline
3 & -20 & 2000 & 01+10i \\
\hline
\end{array}
$$.