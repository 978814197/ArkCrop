# ArkCrop · 基于 Boost 协程的图片**按高度分块**裁剪服务

> 这是一个 **C++ Web 形态的云函数**：通过 Boost.Asio/Beast 提供 HTTP
> 接口，使用协程读写本地图片文件，并将裁剪计算封装为协程在线程池执行。  
> **裁剪规则**：将整张图片 **按高度均分为 N 块**，取第 `start_idx..end_idx`（含）这些块合成的连续垂直区域进行裁剪，保存到指定输出路径。

## ✨ 特性

- **输入极简（仅 5 个参数）**：
  1. `input_path`：图片的本地路径（绝对/相对）。
  2. `output_path`：裁剪后的图片输出路径（包含文件名与扩展名）。
  3. `start_idx`：起始块下标（**0 基**，包含）。
  4. `end_idx`：结束块下标（**0 基**，包含）。
  5. `total_blocks`：总分块数（N）。
- **全链路协程化**：HTTP 收发、文件 I/O 使用 `co_await`；CPU 裁剪在 `boost::asio::thread_pool` 执行，I/O 线程不阻塞。
- **零额外语义**：不做缩放、不做横向切分，只做 **按高度分块的垂直裁剪**。
- **云函数友好**：无状态、可水平扩展、提供 `/healthz` 监控。

## 🧮 裁剪定义（精确定义）

设原图尺寸 `W×H`，总分块数 `N = total_blocks`，起止下标 `s = start_idx`、`e = end_idx`（约束：`0 ≤ s ≤ e < N`）。  
垂直裁剪区间按 **整数边界** 定义：

```
y0 = floor( H * s       / N )
y1 = floor( H * (e + 1) / N )    // 右开左闭 → 实际裁高为 (y1 - y0)
ROI = [x=0 .. W, y=y0 .. y1-1]
```

- 当 `H` 与 `N` 不整除时，**底部块可能更高或更矮**，本定义保证块边界覆盖全高且 **无缝/无重叠**。
- 写入输出图像尺寸为 `W × (y1 - y0)`。

## 🔌 HTTP API（云函数）

### `POST /crop-file`

- **请求体（JSON）**：

```jsonc
{
  "input_path": "data/source/page_001.png",
  "output_path": "data/crops/page_001_s2_e4.png",
  "start_idx": 2,
  "end_idx": 4,
  "total_blocks": 10
}
```

- **响应**：

```json
{
  "ok": true,
  "width": 1654,
  "height": 495,
  "y0": 330,
  "y1": 825,
  "saved": "data/crops/page_001_s2_e4.png"
}
```

- **错误码**：
  - `400` 参数不合法（路径为空、索引越界、`total_blocks < 1` 等）。
  - `404` 输入图片不存在或无法读取。
  - `500` 处理失败（I/O/编码异常）。