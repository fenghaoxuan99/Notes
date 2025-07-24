
# cv::VideoWriter 构造函数详解

`cv::VideoWriter` 是 OpenCV 中用于创建和写入视频文件的类，其构造函数用于初始化视频写入器。下面我将详细解释该函数的用法：

## 函数原型

```cpp
cv::VideoWriter::VideoWriter(
    const String& filename, 
    int fourcc, 
    double fps, 
    Size frameSize, 
    bool isColor = true
);
```

## 参数详解

### 1. `filename` (输出文件名)
- **类型**: `const String&`
- **描述**: 要创建的视频文件路径
- **支持格式**: 取决于系统安装的编解码器，常见格式包括：
  - `.mp4` (MPEG-4)
  - `.avi` (AVI)
  - `.mov` (QuickTime)
  - `.mkv` (Matroska)
- **示例**: `"output_video.mp4"`

### 2. `fourcc` (编解码器标识)
- **类型**: `int`
- **描述**: 指定视频编解码器的4字符代码(FourCC)
- **获取方式**: 使用 `VideoWriter::fourcc(char c1, char c2, char c3, char c4)` 函数
- **常用编解码器**:
  - `VideoWriter::fourcc('m', 'p', '4', 'v')` - MPEG-4 编码 (.mp4)
  - `VideoWriter::fourcc('X', '2', '6', '4')` - H.264 编码
  - `VideoWriter::fourcc('X', 'V', 'I', 'D')` - XviD 编码 (.avi)
  - `VideoWriter::fourcc('M', 'J', 'P', 'G')` - Motion-JPEG 编码
  - `VideoWriter::fourcc('P', 'I', 'M', '1')` - MPEG-1 编码
- **注意事项**:
  - 编解码器可用性取决于系统安装的库
  - 如果使用 `-1`，OpenCV 会弹出编解码器选择对话框

### 3. `fps` (帧率)
- **类型**: `double`
- **描述**: 输出视频的帧率（每秒帧数）
- **取值范围**: 通常为 15-60，取决于应用场景
- **示例**: `30.0` (30帧/秒)

### 4. `frameSize` (帧尺寸)
- **类型**: `cv::Size`
- **描述**: 视频帧的尺寸（宽度 × 高度）
- **格式**: `Size(width, height)`
- **注意事项**:
  - 必须与后续写入的帧尺寸完全一致
  - 常见尺寸: `Size(640, 480)`, `Size(1280, 720)`, `Size(1920, 1080)`

### 5. `isColor` (颜色模式) [可选]
- **类型**: `bool`
- **默认值**: `true`
- **描述**: 
  - `true`: 输出彩色视频（3通道 BGR）
  - `false`: 输出灰度视频（1通道）

## 使用注意事项

1. **编解码器兼容性**:
   - 不同系统支持的编解码器可能不同
   - 在Linux上，确保安装相应编码库（如 `libx264-dev`）
   - 如果编解码器不可用，使用 `isOpened()` 检查是否成功初始化

2. **帧尺寸一致性**:
   - 所有写入的帧必须与构造函数中指定的尺寸完全一致
   - 否则写入会失败（但不会报错）

3. **文件扩展名**:
   - 文件扩展名应与编解码器匹配
   - 例如，使用 `'m','p','4','v'` 编解码器时，应使用 `.mp4` 扩展名

4. **资源释放**:
   - 完成写入后必须调用 `release()` 方法关闭视频文件
   - 否则视频文件可能损坏或不完整

## 完整使用示例

```cpp
#include <opencv2/opencv.hpp>
using namespace cv;

int main() {
    // 视频参数
    const String filename = "output.mp4";
    const int fourcc = VideoWriter::fourcc('m', 'p', '4', 'v'); // MP4 编解码器
    const double fps = 30.0;
    const Size frameSize(640, 480);
    const bool isColor = true;

    // 创建 VideoWriter 对象
    VideoWriter writer(filename, fourcc, fps, frameSize, isColor);
    
    // 检查是否成功打开
    if (!writer.isOpened()) {
        std::cerr << "无法创建视频文件！" << std::endl;
        return -1;
    }

    // 写入100帧
    for (int i = 0; i < 100; i++) {
        Mat frame(frameSize, CV_8UC3, Scalar(0, i % 256, 255 - i % 256));
        writer.write(frame); // 写入帧
    }

    // 释放资源
    writer.release();
    std::cout << "视频创建成功: " << filename << std::endl;
    return 0;
}
```

## 常见问题解决

1. **无法创建视频文件**:
   - 检查文件路径是否有效且有写入权限
   - 尝试不同的编解码器（如 `'X','V','I','D'`）
   - 安装必要的编解码器库（Linux: `sudo apt install libx264-dev`）

2. **视频无法播放**:
   - 确保调用了 `release()` 方法
   - 尝试使用不同的媒体播放器（如 VLC）
   - 检查帧尺寸是否一致

3. **编解码器不可用**:
   - 使用 `VideoWriter::fourcc('M','J','P','G')`（通常广泛支持）
   - 尝试 `VideoWriter::fourcc('X','2','6','4')`（H.264）
   - 在Windows上，使用 `0` 弹出编解码器选择对话框

通过正确使用 `cv::VideoWriter` 构造函数，您可以创建各种格式的视频文件，用于视频处理、计算机视觉应用和多媒体开发。