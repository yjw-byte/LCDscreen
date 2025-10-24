# STM32F407 LCD显示屏驱动项目

本项目是基于STM32F407微控制器的LCD显示屏驱动程序，使用ST7735驱动芯片控制128x160彩色TFT液晶屏。

## 项目概述

该项目实现了对ST7735驱动的TFT LCD屏幕的基本图形操作功能，包括：
- 基本图形绘制（点、线、矩形、圆形）
- 文字显示（ASCII字符及中文字体支持）
- 图片显示功能
- 颜色填充等基本操作

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
├── CMakeLists.txt          # CMake构建配置
└── README.md               # 项目说明文档
```

## 主要功能

### LCD基本操作
- [LCD_Init_HAL()](file:///D:/software/1-7%20Hal/Clion/f407_LCD/HardWare/LCD/lcd.c#L56-L125) - LCD初始化
- [LCD_Fill()](file:///D:/software/1-7%20Hal/Clion/f407_LCD/HardWare/LCD/lcd.c#L213-L230) - 区域填充指定颜色
- [LCD_DrawPoint_Color()](file:///D:/software/1-7%20Hal/Clion/f407_LCD/HardWare/LCD/lcd.c#L233-L241) - 绘制单个点
- [LCD_DrawLine_Color()](file:///D:/software/1-7%20Hal/Clion/f407_LCD/HardWare/LCD/lcd.c#L244-L275) - 绘制直线
- [LCD_DrawRectangle_Color()](file:///D:/software/1-7%20Hal/Clion/f407_LCD/HardWare/LCD/lcd.c#L278-L294) - 绘制矩形框
- [Draw_Circle()](file:///D:/software/1-7%20Hal/Clion/f407_LCD/HardWare/LCD/lcd.c#L297-L326) - 绘制圆形

### 文字显示
- [LCD_ShowChar()](file:///D:/software/1-7%20Hal/Clion/f407_LCD/HardWare/LCD/lcd.c#L389-L422) - 显示单个字符
- [LCD_ShowString()](file:///D:/software/1-7%20Hal/Clion/f407_LCD/HardWare/LCD/lcd.c#L425-L444) - 显示字符串
- [LCD_ShowChinese()](file:///D:/software/1-7%20Hal/Clion/f407_LCD/HardWare/LCD/lcd.c#L329-L350) - 显示中文字符

### 图片显示
- [LCD_ShowPicture()](file:///D:/software/1-7%20Hal/Clion/f407_LCD/HardWare/LCD/lcd.c#L478-L501) - 显示图片

## 使用方法

1. 克隆或下载本项目
2. 使用STM32CubeIDE或CLion打开项目
3. 根据实际硬件连接修改管脚配置
4. 编译并下载到STM32F407开发板
5. 连接ST7735驱动的LCD屏幕即可运行

## 示例代码

在主函数中，项目演示了LCD的基本用法：

```c
int main(void)
{
    // 系统初始化
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_SPI2_Init();
    
    // 初始化LCD
    LCD_Init_HAL();
    
    // 填充屏幕为白色
    LCD_Fill(0, 0, 128, 160, WHITE);
    
    // 显示图片
    LCD_ShowPicture(65,80,40,40,gImage_1);
    
    // 循环切换背景色
    while (1)
    {
        for(uint8_t i=0;i<sizeof(color_palette)/sizeof(color_palette[0]);i++)
        {
            LCD_Fill(0,0,128,160,color_palette[i]);
            HAL_Delay(500);
        }
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

项目使用CMake构建系统，主要配置在[CMakeLists.txt](file:///D:/software/1-7%20Hal/Clion/f407_LCD/CMakeLists.txt)中定义。需要注意的是，新增的驱动源文件需要手动添加到[target_sources](file:///D:/software/1-7%20Hal/Clion/f407_LCD/CMakeLists.txt#L56-L61)中才能正确链接。

## 注意事项

1. 确保硬件连接正确，特别是SPI接口和控制引脚
2. 根据具体使用的LCD型号可能需要调整初始化参数
3. 如需显示中文，请确保包含相应的中文字库
4. 修改CMakeLists.txt后需重新配置项目

## 许可证

本项目基于STMicroelectronics提供的许可条款，详细信息请参见源码中的LICENSE文件。