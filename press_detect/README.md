# ADC按键长短按检测程序

基于Linux ADC接口的按键检测程序，支持长按/短按识别，适用于RK3568等嵌入式Linux系统。

## 项目简介

这是一个运行在Linux系统上的ADC按键检测程序，通过读取ADC设备文件来检测按键状态，并能够区分短按（<1秒）和长按（≥1秒）操作。

## 功能特性

- ✅ **ADC按键检测**：通过Linux IIO子系统读取ADC值
- ✅ **多按键支持**：支持4个按键（key4, key5, key6, key7）
- ✅ **长短按识别**：自动识别短按（<1秒）和长按（≥1秒）
- ✅ **实时响应**：10ms轮询间隔，响应迅速
- ✅ **优雅退出**：支持Ctrl+C优雅退出，自动清理资源
- ✅ **简洁输出**：只输出必要信息，格式清晰

## 硬件要求

### 支持的开发板
- RK3568核心板
- RK3588核心板
- RK3399核心板
- 其他支持Linux IIO子系统的嵌入式板卡

### 按键电路
程序假设使用**电阻分压式按键电路**：

```
VCC (3.3V) ----+
               |
               R1 (上拉电阻)
               |
ADC输入 -------+
               |
               按键开关
               |
               R2/R3/R4/R5 (不同阻值)
               |
GND -----------+
```

### ADC值映射
| 按键 | ADC值范围 | 典型值 |
|------|-----------|--------|
| 无按键 | > 900 | 1023 |
| key4 | < 50 | 9 |
| key5 | 200-300 | 235 |
| key6 | 400-500 | 457 |
| key7 | 650-750 | 683 |

## 软件要求

### 系统要求
- Linux操作系统
- 支持IIO子系统的内核
- gcc编译器

### 依赖库
- 标准C库（libc）
- POSIX时间库（librt）

## 快速开始

### 1. 克隆项目


### 2. 编译程序
```bash
# 方法1：直接编译
gcc main.c press.c -o press_test

### 3. 运行程序
```bash
# 可能需要root权限访问ADC设备
sudo ./press_test

# 或者
./press_test
```

### 4. 使用程序
```bash
$ ./press_test
当前ADC状态: 1023
key4 短按
key5 长按
key6 短按
key7 长按
^C
程序退出
$
```

## 项目结构

```
press_detect/
├── main.c              # 主程序文件
├── press.c             # 按键检测核心实现
├── press.h             # 头文件
└── README.md           # 项目说明文档
```

## 核心文件说明

### main.c
- 程序入口
- ADC初始化
- 主循环控制
- 信号处理

### press.c
- ADC设备操作
- 按键状态检测
- 长短按时间计算
- 按键释放检测

### press.h
- 函数声明
- 常量定义
- 设备路径定义


### 核心函数

#### `int adc_init(const char* adc_path)`
初始化ADC设备
- **参数**：ADC设备文件路径
- **返回值**：0成功，-1失败

#### `int adc_read_raw(void)`
读取原始ADC值
- **返回值**：ADC数值，-1表示失败

#### `int wait_for_key_press(void)`
等待按键按下
- **返回值**：按键编号(4-7)，-1失败，-2退出

#### `void wait_for_key_release(void)`
等待按键释放并判断长短按
- **功能**：输出"长按"或"短按"

#### `void cleanup_adc(void)`
清理ADC资源
- **功能**：关闭设备文件，释放资源

### 配置常量

```c
// ADC设备路径（根据实际硬件调整）
#define RK3568_ADC_PATH "/sys/bus/iio/devices/iio:device0/in_voltage0_raw"
#define RK3588_ADC_PATH "/sys/bus/iio/devices/iio:device0/in_voltage0_raw"
#define RK3399_ADC_PATH "/sys/bus/iio/devices/iio:device0/in_voltage0_raw"

// 长按时间阈值（毫秒）
const long LONG_PRESS_TIME_MS = 1000;
```

## 配置和定制

### 修改ADC设备路径
根据你的硬件平台，修改`main.c`中的设备路径：
```c
// 示例路径，根据实际情况修改
const char* test_adc_path = "/sys/bus/iio/devices/iio:device0/in_voltage0_raw";
```

### 调整按键ADC值范围
在`detect_key_from_adc()`函数中修改ADC值范围：
```c
if (adc_val < 50) {
    return 4;  // 调整key4的ADC阈值
}
// ... 其他按键
```

### 修改长按时间
在`wait_for_key_release()`函数中修改：
```c
const long LONG_PRESS_TIME_MS = 2000;  // 改为2秒
```


#### 查看实际ADC值
```c
// 在main.c中添加调试代码
while (running) {
    debug_print_adc();  // 显示当前ADC值
    usleep(500000);     // 每500ms显示一次
}
```

#### 检查设备文件
```bash
# 手动读取ADC值
cat /sys/bus/iio/devices/iio:device0/in_voltage0_raw

# 持续监控
watch -n 0.1 'cat /sys/bus/iio/devices/iio:device0/in_voltage0_raw'
```