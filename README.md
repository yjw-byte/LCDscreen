# STM32F407 LCD显示屏驱动项目

本项目是基于STM32F407微控制器的LCD显示屏驱动程序，使用ST7735驱动芯片控制128x160彩色TFT液晶屏。

## 项目概述

该项目实现了对ST7735驱动的TFT LCD屏幕的基本图形操作功能，并成功移植了LVGL图形库，包括：
- 基本图形绘制（点、线、矩形、圆形）
- 文字显示（ASCII字符及中文字体支持）
- 图片显示功能
- 颜色填充等基本操作
- LVGL图形库的完整移植和应用

项目采用STM32 HAL库进行开发，并通过SPI接口与LCD屏幕通信。

## 硬件连接

| LCD引脚 | STM32F407连接 |
|--------|--------------|
| SCL(SCK) | SPI2 SCK (PB13) |
| SDA(MOSI) | SPI2 MOSI (PB15) |
| RES(RST) | GPIO (PB12) |
| DC | GPIO (PB14) |
| CS | GPIO (PB10) |
| BL | GPIO (PB11) |

## 软件架构

```
├── Core                    # STM32 HAL核心代码
│   ├── Inc                 # 头文件
│   └── Src                 # 源文件
├── HardWare                # 硬件驱动代码
│   └── LCD                 # LCD驱动实现
│       ├── lcd.c           # LCD驱动主要实现
│       ├── lcd.h           # LCD驱动头文件
│       ├── lcdfont.h       # 字体数据
│       └── pic.h           # 图片数据
├── lvgl-8.0.2-core         # LVGL图形库源码
│   ├── examples\porting    # LVGL移植示例代码
│   │   ├── lv_port_disp.c  # 显示驱动移植代码
│   │   └── lv_port_disp.h  # 显示驱动头文件
│   └── src                 # LVGL核心源码
├── CMakeLists.txt          # CMake构建配置
└── README.md               # 项目说明文档
```

## 主要功能

### LCD基本操作
- [LCD_Init_HAL()](file://D:\software\1-7%20Hal\Clion\F407\1-3%20f407_LCD%20-%20lvgl\HardWare\LCD\lcd.c#L56-L125) - LCD初始化
- [LCD_Fill()](file://D:\software\1-7%20Hal\Clion\F407\1-3%20f407_LCD%20-%20lvgl\HardWare\LCD\lcd.c#L213-L230) - 区域填充指定颜色
- [LCD_DrawPoint_Color()](file://D:\software\1-7%20Hal\Clion\F407\1-3%20f407_LCD%20-%20lvgl\HardWare\LCD\lcd.c#L233-L241) - 绘制单个点
- [LCD_DrawLine_Color()](file://D:\software\1-7%20Hal\Clion\F407\1-3%20f407_LCD%20-%20lvgl\HardWare\LCD\lcd.c#L244-L275) - 绘制直线
- [LCD_DrawRectangle_Color()](file://D:\software\1-7%20Hal\Clion\F407\1-3%20f407_LCD%20-%20lvgl\HardWare\LCD\lcd.c#L278-L294) - 绘制矩形框
- [Draw_Circle()](file://D:\software\1-7%20Hal\Clion\F407\1-3%20f407_LCD%20-%20lvgl\HardWare\LCD\lcd.c#L297-L326) - 绘制圆形

### 文字显示
- [LCD_ShowChar()](file://D:\software\1-7%20Hal\Clion\F407\1-3%20f407_LCD%20-%20lvgl\HardWare\LCD\lcd.c#L389-L422) - 显示单个字符
- [LCD_ShowString()](file://D:\software\1-7%20Hal\Clion\F407\1-3%20f407_LCD%20-%20lvgl\HardWare\LCD\lcd.c#L425-L444) - 显示字符串
- [LCD_ShowChinese()](file://D:\software\1-7%20Hal\Clion\F407\1-3%20f407_LCD%20-%20lvgl\HardWare\LCD\lcd.c#L329-L350) - 显示中文字符

### 图片显示
- [LCD_ShowPicture()](file://D:\software\1-7%20Hal\Clion\F407\1-3%20f407_LCD%20-%20lvgl\HardWare\LCD\lcd.c#L478-L501) - 显示图片

### LVGL图形界面
- 完整的LVGL 8.0.2移植
- 支持标签、按钮、进度条、滑块等基本控件
- 定时器驱动的动画效果
- 128x160分辨率优化显示

## 移植步骤

### 1. 配置LVGL库

1. 将LVGL源码放入项目目录，如`lvgl-8.0.2-core`文件夹
2. 修改[Core/Src/main.c](file:///D:/software/1-7%20Hal/Clion/F407/1-3%20f407_LCD%20-%20lvgl/Core/Src/main.c)文件，添加LVGL相关头文件包含：
   ```c
   #include "lvgl.h"
   #include "lv_port_disp.h"
   ```
3. 在[main()](file://D:\software\1-7%20Hal\Clion\F407\1-3%20f407_LCD%20-%20lvgl\Core\Src\main.c#L84-L155)函数中初始化LVGL：
   ```c
   lv_init();
   lv_port_disp_init();
   ```

**AI提示词**: "请帮我配置LVGL库，需要在main.c中添加LVGL头文件包含并初始化LVGL"

### 2. 配置lv_conf.h

1. 复制`lvgl-8.0.2-core/lv_conf_template.h`为`lvgl-8.0.2-core/lv_conf.h`
2. 修改以下关键配置：
   ```c
   #define LV_COLOR_DEPTH     16        // 与屏幕一致的16位色彩深度
   #define LV_MEM_CUSTOM      1         // 使用标准malloc/free
   #define LV_MEMCPY_MEMSET_STD    1    // 使用标准memcpy/memset
   #define LV_SPRINTF_CUSTOM   1        // 使用标准sprintf
   ```

**AI提示词**: "请帮我配置lv_conf.h文件，设置颜色深度为16位，启用标准库函数支持"

### 3. 实现显示驱动移植

1. 复制`lvgl-8.0.2-core/examples/porting/lv_port_disp_template.c`为`lvgl-8.0.2-core/examples/porting/lv_port_disp.c`
2. 复制`lvgl-8.0.2-core/examples/porting/lv_port_disp_template.h`为`lvgl-8.0.2-core/examples/porting/lv_port_disp.h`
3. 修改[lv_port_disp.h](file:///D:/software/1-7%20Hal/Clion/F407/1-3%20f407_LCD%20-%20lvgl/lvgl-8.0.2-core/examples/porting/lv_port_disp.h)，添加函数声明：
   ```c
   void lv_port_disp_init(void);
   ```
4. 修改[lv_port_disp.c](file:///D:/software/1-7%20Hal/Clion/F407/1-3%20f407_LCD%20-%20lvgl/lvgl-8.0.2-core/examples/porting/lv_port_disp.c)：
   - 设置屏幕分辨率：
     ```c
     #define MY_DISP_HOR_RES 128
     #define MY_DISP_VER_RES 160
     ```
   - 实现[disp_init()](file://D:\software\1-7%20Hal\Clion\F407\1-3%20f407_LCD%20-%20lvgl\lvgl-8.0.2-core\examples\porting\lv_port_disp.c#L123-L126)函数调用LCD初始化
   - 实现[disp_flush()](file://D:\software\1-7%20Hal\Clion\F407\1-3%20f407_LCD%20-%20lvgl\lvgl-8.0.2-core\examples\porting\lv_port_disp.c#L139-L152)函数使用LCD驱动函数绘制区域

**AI提示词**: "请帮我实现LVGL显示驱动移植，设置屏幕分辨率为128x160，实现显示刷新函数"

### 4. 更新LCD驱动头文件

在[lcd.h](file:///D:/software/1-7%20Hal/Clion/F407/1-3%20f407_LCD%20-%20lvgl/HardWare/LCD/lcd.h)中添加LVGL需要的函数声明：
```c
void LCD_SetWindows(uint16_t xStar, uint16_t yStar, uint16_t xEnd, uint16_t yEnd);
void LCD_WR_DATA_16(uint16_t data);
```

**AI提示词**: "请帮我更新lcd.h头文件，添加LCD_SetWindows和LCD_WR_DATA_16函数声明"

### 5. 配置CMake构建系统

修改[CMakeLists.txt](file:///D:/software/1-7%20Hal/Clion/F407/1-3%20f407_LCD%20-%20lvgl/CMakeLists.txt)文件：
1. 添加LVGL源文件包含：
   ```cmake
   file(GLOB_RECURSE LVGL_SOURCES lvgl-8.0.2-core/src/*.c)
   ```
2. 添加源文件到编译目标：
   ```cmake
   target_sources(${CMAKE_PROJECT_NAME} PRIVATE
       HardWare/LCD/lcd.c
       lvgl-8.0.2-core/examples/porting/lv_port_disp.c
       ${LVGL_SOURCES}
   )
   ```
3. 添加包含路径：
   ```cmake
   target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
       HardWare/LCD
       lvgl-8.0.2-core
       lvgl-8.0.2-core/src
       lvgl-8.0.2-core/examples/porting
   )
   ```

**AI提示词**: "请帮我配置CMakeLists.txt文件，添加LVGL源文件和包含路径"

### 6. 配置定时器中断

1. 配置TIM6为1ms中断用于LVGL任务调度
2. 在中断服务函数中调用：
   ```c
   void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
   {
     if (htim->Instance == TIM6) {
       lv_tick_inc(1);  // 每毫秒加 1
     }
   }
   ```

**AI提示词**: "请帮我配置定时器中断用于LVGL任务调度，在TIM6中断中调用lv_tick_inc函数"

### 7. 在主循环中处理LVGL任务

在主循环中定期调用：
```c
lv_task_handler();
HAL_Delay(5);
```

**AI提示词**: "请帮我修改main函数主循环，添加LVGL任务处理代码"

## 常见问题及解决方案

### 1. 链接错误：undefined reference to `lv_tick_inc'

**问题原因**：未包含LVGL的HAL层源文件
**解决方案**：确保CMakeLists.txt中包含了所有LVGL源文件

### 2. 链接错误：undefined reference to `lv_port_disp_init'

**问题原因**：函数声明缺失或条件编译未启用
**解决方案**：
1. 确保[lv_port_disp.h](file:///D:/software/1-7%20Hal/Clion/F407/1-3%20f407_LCD%20-%20lvgl/lvgl-8.0.2-core/examples/porting/lv_port_disp.h)中声明了函数
2. 确保[lv_port_disp.c](file:///D:/software/1-7%20Hal/Clion/F407/1-3%20f407_LCD%20-%20lvgl/lvgl-8.0.2-core/examples/porting/lv_port_disp.c)和[lv_port_disp.h](file:///D:/software/1-7%20Hal/Clion/F407/1-3%20f407_LCD%20-%20lvgl/lvgl-8.0.2-core/examples/porting/lv_port_disp.h)中的条件编译开关设置为1

### 3. 显示异常或控件无法显示

**问题原因**：屏幕分辨率或颜色深度配置错误
**解决方案**：
1. 检查[lv_conf.h](file:///D:/software/1-7%20Hal/Clion/F407/1-3%20f407_LCD%20-%20lvgl/lvgl-8.0.2-core/lv_conf.h)中的配置：
   ```c
   #define LV_COLOR_DEPTH     16
   ```
2. 检查[lv_port_disp.c](file:///D:/software/1-7%20Hal/Clion/F407/1-3%20f407_LCD%20-%20lvgl/lvgl-8.0.2-core/examples/porting/lv_port_disp.c)中的分辨率设置：
   ```c
   #define MY_DISP_HOR_RES 128
   #define MY_DISP_VER_RES 160
   ```

## 使用方法

1. 克隆或下载本项目
2. 使用STM32CubeIDE或CLion打开项目
3. 根据实际硬件连接修改管脚配置
4. 编译并下载到STM32F407开发板
5. 连接ST7735驱动的LCD屏幕即可运行

## 示例代码

在主函数中，项目演示了LCD的基本用法和LVGL图形界面：

```c
int main(void)
{
    // 系统初始化
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_SPI2_Init();
    MX_TIM6_Init();
    
    // 初始化LCD
    LCD_Init_HAL();
    HAL_TIM_Base_Start_IT(&htim6); // 启动定时器6，用于LVGL任务调度
    
    // 初始化LVGL
    lv_init();
    lv_port_disp_init();
    
    // 创建LVGL控件示例
    lv_obj_t * label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "Hello World!");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    
    // 循环处理LVGL任务
    while (1)
    {
        lv_task_handler();
        HAL_Delay(5);
    }
}
```

## 颜色定义

项目中预定义了常用的颜色常量：
- WHITE (0xFFFF)
- BLACK (0x0000)
- BLUE (0x001F)
- RED (0xF800)
- GREEN (0x07E0)
- 等多种颜色...

## 构建配置

项目使用CMake构建系统，主要配置在[CMakeLists.txt](file:///D:/software/1-7%20Hal/Clion/F407/1-3%20f407_LCD%20-%20lvgl/CMakeLists.txt)中定义。需要注意的是，新增的驱动源文件需要手动添加到[target_sources](file:///D:/software/1-7%20Hal/Clion/F407/1-3%20f407_LCD%20-%20lvgl/CMakeLists.txt#L51-L58)中才能正确链接。

## 注意事项

1. 确保硬件连接正确，特别是SPI接口和控制引脚
2. 根据具体使用的LCD型号可能需要调整初始化参数
3. 如需显示中文，请确保包含相应的中文字库
4. 修改CMakeLists.txt后需重新配置项目
5. LVGL对内存有一定要求，确保MCU有足够RAM运行图形界面

## 优化与学习方向
1. 采用 DMA + 双缓冲 解决撕裂
2. 优化LVGL内存管理，提升性能
3. 学习更多LVGL高级控件和动画效果
4. 学习lvgl库设计思路，体会LVGL的优秀设计：
   - 模块化设计
   - 可配置性强
   - 丰富的控件和主题支持
   - 硬件抽象性，移植只需要那几个就OK了


## 许可证

本项目基于STMicroelectronics提供的许可条款，详细信息请参见源码中的LICENSE文件。