
# OpenCV `rectangle()` 函数详解

`rectangle()` 是 OpenCV 中用于在图像上绘制矩形的核心函数，您提供的代码展示了其典型用法。下面我将详细解析每个参数和功能：

## 函数原型

```cpp
void cv::rectangle(
    InputOutputArray img,         // 目标图像
    Point pt1,                    // 矩形左上角点
    Point pt2,                    // 矩形右下角点
    const Scalar& color,          // 矩形颜色
    int thickness = 1,            // 线宽
    int lineType = LINE_8,        // 线型
    int shift = 0                 // 坐标精度
);
```

## 参数详解（针对您的代码）

### 1. `frame` - 目标图像
- **类型**: `cv::Mat`
- **作用**: 要在其上绘制矩形的图像
- **要求**: 通常是 3 通道 (BGR) 或单通道图像
- **注意**: 函数会直接修改这个图像

### 2. `Point(rect_x, rect_y)` - 矩形左上角
- **类型**: `cv::Point`
- **作用**: 定义矩形的起始点 (左上角坐标)
- **坐标系统**: 图像左上角为原点 (0,0)，x 向右增加，y 向下增加
- **示例**: `Point(100, 50)` 表示 x=100, y=50 的位置

### 3. `Point(rect_x + 200, rect_y + rect_size)` - 矩形右下角
- **类型**: `cv::Point`
- **作用**: 定义矩形的对角点 (右下角坐标)
- **计算方式**:
  - 宽度: `200` 像素
  - 高度: `rect_size` 像素
- **注意**: 确保坐标在图像范围内，否则可能绘制不全

### 4. `Scalar(0, 255 * progress, 255 * (1 - progress))` - 矩形颜色
- **类型**: `cv::Scalar`
- **作用**: 定义矩形的颜色 (BGR 格式)
- **颜色分量**:
  - **蓝色(B)**: `0` (固定为0)
  - **绿色(G)**: `255 * progress` (根据进度变化)
  - **红色(R)**: `255 * (1 - progress)` (根据进度变化)
- **颜色变化效果**:
  - 当 `progress = 0.0`: `Scalar(0, 0, 255)` → 红色   🔴
  - 当 `progress = 0.5`: `Scalar(0, 128, 128)` → 黄色 🟡
  - 当 `progress = 1.0`: `Scalar(0, 255, 0)` → 绿色   🟢
- **应用场景**: 常用于表示进度条或状态指示器

### 5. `FILLED` - 填充模式
- **作用**: 指定矩形内部填充方式
- **可选值**:
  - `FILLED` 或 `-1`: 完全填充矩形内部
  - 正整数: 边框线宽 (像素)
- **默认值**: `1` (1像素边框)
- **在您代码中**: 使用 `FILLED` 表示实心矩形

## 完整功能解析

您提供的代码创建了一个**动态颜色变化的填充矩形**：
1. 位置: 从 `(rect_x, rect_y)` 开始
2. 尺寸: 宽度 200 像素，高度 `rect_size` 像素
3. 颜色: 根据 `progress` 值在红色(0.0)到绿色(1.0)之间渐变
4. 样式: 完全填充 (无边框)

## 实际应用示例

### 进度条实现

```cpp
// 创建黑色背景
Mat frame(480, 640, CV_8UC3, Scalar(0, 0, 0));

// 进度值 (0.0 - 1.0)
float progress = 0.75f; 

// 进度条位置和尺寸
int bar_x = 50, bar_y = 200;
int bar_width = 500, bar_height = 40;

// 绘制背景框 (灰色边框)
rectangle(frame, 
          Point(bar_x, bar_y), 
          Point(bar_x + bar_width, bar_y + bar_height),
          Scalar(200, 200, 200), 
          2);

// 绘制进度条 (颜色渐变)
rectangle(frame,
          Point(bar_x, bar_y),
          Point(bar_x + bar_width * progress, bar_y + bar_height),
          Scalar(0, 255 * progress, 255 * (1 - progress)),
          FILLED);
```

### 对象标记

```cpp
// 检测到的人脸区域
Rect face_rect(150, 100, 120, 160);

// 在图像上绘制绿色矩形标记
rectangle(frame,
          face_rect.tl(),      // 左上角点
          face_rect.br(),      // 右下角点
          Scalar(0, 255, 0),   // 绿色
          2,                   // 2像素边框
          LINE_AA);            // 抗锯齿线型
```

## 高级用法

### 1. 带透明度的矩形

```cpp
// 创建矩形ROI
Mat roi = frame(Rect(100, 100, 300, 200));

// 创建带透明度的覆盖层
Mat overlay(roi.size(), CV_8UC3, Scalar(0, 150, 255));
double alpha = 0.3; // 透明度

// 混合叠加
addWeighted(overlay, alpha, roi, 1 - alpha, 0, roi);
```

### 2. 圆角矩形

```cpp
// 自定义圆角矩形函数
void roundedRect(Mat& img, Rect rect, Scalar color, int radius, int thickness) {
    // 绘制四个角
    circle(img, rect.tl() + Point(radius, radius), radius, color, thickness);
    circle(img, Point(rect.x + rect.width - radius, rect.y + radius), radius, color, thickness);
    circle(img, Point(rect.x + radius, rect.y + rect.height - radius), radius, color, thickness);
    circle(img, rect.br() - Point(radius, radius), radius, color, thickness);
    
    // 绘制四条边
    rectangle(img, Rect(rect.x + radius, rect.y, rect.width - 2*radius, rect.height), color, thickness);
    rectangle(img, Rect(rect.x, rect.y + radius, rect.width, rect.height - 2*radius), color, thickness);
}
```

## 性能注意事项

1. **批量绘制**: 避免多次调用 `rectangle()`，尽量一次绘制多个图形
2. **ROI操作**: 对大图像，先提取ROI再绘制可提高性能
3. **抗锯齿选择**:
   - `LINE_8`: 快速但锯齿明显
   - `LINE_AA`: 质量高但稍慢
4. **坐标检查**: 确保坐标在图像范围内，避免崩溃

## 常见错误

1. **坐标顺序错误**:
   ```cpp
   // 错误：左上角在右下角之后
   rectangle(frame, Point(300,200), Point(100,50), ...);
   ```

2. **颜色通道不匹配**:
   ```cpp
   // 错误：灰度图使用BGR颜色
   Mat gray(480,640,CV_8UC1);
   rectangle(gray, ..., Scalar(0,0,255)); // 应使用Scalar(255)
   ```

3. **越界坐标**:
   ```cpp
   // 错误：坐标超出图像范围
   rectangle(frame, Point(-10,-10), Point(700,500), ...);
   ```

`rectangle()` 是 OpenCV 中最常用的绘图函数之一，掌握其用法对于图像标注、UI元素创建和可视化至关重要。您提供的代码展示了如何创建动态颜色变化的进度条效果，这是许多计算机视觉应用中的常见需求。