
### 💡示例代码：

```cpp
putText(img,
        format("[Tracker] Loss Rate: %0.3f %%", static_cast<float>(myTracker.get_id_switch_rate() * 100)),
        Point(0, 60), 0, 0.6, Scalar(255, 0, 0), 2, LINE_AA);
```


## ✅ 1. 函数原型：

```cpp
void cv::putText(InputOutputArray img,
                 const String& text,
                 Point org,
                 int fontFace,
                 double fontScale,
                 Scalar color,
                 int thickness = 1,
                 int lineType = LINE_8,
                 bool bottomLeftOrigin = false );
```


## ✅ 2. 各个参数详解：

### 🔹`img`

> 类型：`cv::Mat&`
> 表示要绘制文字的图像对象（可以是一帧视频图像）。

* 在你的代码中：`img` 就是那张被绘制文字的图像。

---

### 🔹`format(...)`

 用于格式化字符串，类似 `printf` 风格。

* `"[Tracker] Loss Rate: %0.3f %%"` 是模板字符串，其中 `%0.3f` 表示保留小数点后3位。
* `myTracker.get_id_switch_rate()` 是你自己的跟踪器返回的 ID switch rate（目标切换率）。
* 乘以 `100` 是将其从小数转换为百分比。
* 最后 `static_cast<float>(...)` 保证格式正确。

👀 **例子输出**：

```cpp
"[Tracker] Loss Rate: 1.234 %"
```


### 🔹`Point(0, 60)`

 表示文字绘制的起始点（左下角坐标），单位是像素，`(x, y)` 格式。

* `(0, 60)` 表示文字起始点在图像左上角往下60个像素处。

---

### 🔹`0`：`fontFace`

> 字体类型，OpenCV 提供几个预定义字体：

| 值   | 名称                     | 说明               |
| --- | ---------------------- | ---------------- |
| 0   | `FONT_HERSHEY_SIMPLEX` | 常用的简单字体（默认）      |
| 1   | `FONT_HERSHEY_PLAIN`   | 比较小巧、简洁          |
| 2   | `FONT_HERSHEY_DUPLEX`  | 类似 `SIMPLEX`，更平滑 |
| ... | ...                    | ...              |

在你这行代码里，`0` 就是 `FONT_HERSHEY_SIMPLEX`。

---

### 🔹`0.6`：`fontScale`

> 字体缩放因子。`1.0` 是标准大小，`0.6` 表示较小的字体。

---

### 🔹`Scalar(255, 0, 0)`：颜色

> 文本颜色，格式为 BGR（三通道）。

* `Scalar(255, 0, 0)` 表示 **蓝色**（OpenCV 默认是 BGR 而不是 RGB）。

---

### 🔹`2`：`thickness`

> 文字线条粗细。数值越大，字体越粗。

---

### 🔹`LINE_AA`：抗锯齿线型

> 抗锯齿模式，可以使字体更平滑。

* 可选值：

  * `LINE_8`：默认线型
  * `LINE_AA`：抗锯齿，更平滑显示
  * `LINE_4`：低质量线型

---

## ✅ 总结用途：

这行代码的作用是：

> 📌 在图像左上角（下移60像素处）显示追踪器的目标切换率（ID Switch Rate），例如：
> `[Tracker] Loss Rate: 1.234 %`
> 文字为蓝色，字体为简洁样式，字号较小。

---

## 🧪 示例代码演示：

```cpp
#include <opencv2/opencv.hpp>
using namespace cv;

int main() {
    Mat img = Mat::zeros(200, 400, CV_8UC3);  // 创建黑色背景图像
    float switchRate = 0.01234;  // 示例 ID 切换率

    putText(img,
            format("[Tracker] Loss Rate: %0.3f %%", switchRate * 100),
            Point(0, 60),
            FONT_HERSHEY_SIMPLEX,
            0.6,
            Scalar(255, 0, 0),
            2,
            LINE_AA);

    imshow("Result", img);
    waitKey(0);
    return 0;
}
```

