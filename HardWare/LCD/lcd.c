#include "lcd.h"
#include "main.h"
#include "spi.h"

// LCD的画笔颜色
uint16_t POINT_COLOR = 0x0000;

// LCD的背景色
uint16_t BACK_COLOR = 0xFFFF;

// 管理LCD重要参数
_lcd_dev lcddev;

// 通过SPI写8位数据
void LCD_SPI_WriteByte(uint8_t data)
{
    HAL_SPI_Transmit(&hspi2, &data, 1, 1000);
}

// 写命令到LCD
void LCD_WR_REG(uint8_t regval)
{
    LCD_DC_CLR;     // 写命令
    LCD_CS_CLR;     // 选中LCD
    LCD_SPI_WriteByte(regval);
    LCD_CS_SET;     // 取消选中
}

// 写数据到LCD
void LCD_WR_DATA(uint8_t data)
{
    LCD_DC_SET;     // 写数据
    LCD_CS_CLR;     // 选中LCD
    LCD_SPI_WriteByte(data);
    LCD_CS_SET;     // 取消选中
}

// 写16位数据到LCD
void LCD_WR_DATA_16(uint16_t data)
{
    LCD_DC_SET;     // 写数据
    LCD_CS_CLR;     // 选中LCD
    LCD_SPI_WriteByte(data >> 8);
    LCD_SPI_WriteByte(data);
    LCD_CS_SET;     // 取消选中
}

// LCD延时
void LCD_Delay(uint32_t delay)
{
    HAL_Delay(delay);
}

// LCD复位
void LCD_Reset(void)
{
    LCD_RST_CLR;
    LCD_Delay(100);
    LCD_RST_SET;
    LCD_Delay(100);
}

// LCD初始化
void LCD_Init(void)
{
    // 初始化LCD参数
    lcddev.width = 128;
    lcddev.height = 160;
    
    LCD_Reset();  // 硬件复位
    
    // ST7735初始化序列
    LCD_WR_REG(0x11);   // Sleep out
    LCD_Delay(120);
    
    LCD_WR_REG(0xB1);   // Frame rate control
    LCD_WR_DATA(0x01);
    LCD_WR_DATA(0x2C);
    LCD_WR_DATA(0x2D);
    
    LCD_WR_REG(0xB2);   // Frame rate control
    LCD_WR_DATA(0x01);
    LCD_WR_DATA(0x2C);
    LCD_WR_DATA(0x2D);
    
    LCD_WR_REG(0xB3);   // Frame rate control
    LCD_WR_DATA(0x01);
    LCD_WR_DATA(0x2C);
    LCD_WR_DATA(0x2D);
    LCD_WR_DATA(0x01);
    LCD_WR_DATA(0x2C);
    LCD_WR_DATA(0x2D);
    
    LCD_WR_REG(0xB4);   // Column inversion
    LCD_WR_DATA(0x07);
    
    LCD_WR_REG(0xC0);   // Power control
    LCD_WR_DATA(0xA2);
    LCD_WR_DATA(0x02);
    LCD_WR_DATA(0x84);
    
    LCD_WR_REG(0xC1);   // Power control
    LCD_WR_DATA(0xC5);
    
    LCD_WR_REG(0xC2);   // Power control
    LCD_WR_DATA(0x0A);
    LCD_WR_DATA(0x00);
    
    LCD_WR_REG(0xC3);   // Power control
    LCD_WR_DATA(0x8A);
    LCD_WR_DATA(0x2A);
    
    LCD_WR_REG(0xC4);   // Power control
    LCD_WR_DATA(0x8A);
    LCD_WR_DATA(0xEE);
    
    LCD_WR_REG(0xC5);   // VCOM control
    LCD_WR_DATA(0x0E);
    
    LCD_WR_REG(0x20);   // 不显示反转
    
    LCD_WR_REG(0x36);   // Memory access control
    LCD_WR_DATA(0xC8);  // MX, MY, RGB模式
    
    LCD_WR_REG(0x3A);   // Pixel format
    LCD_WR_DATA(0x05);  // 16位像素
    
    LCD_WR_REG(0xE0);   // Gamma (positive)
    LCD_WR_DATA(0x02);
    LCD_WR_DATA(0x1C);
    LCD_WR_DATA(0x07);
    LCD_WR_DATA(0x12);
    LCD_WR_DATA(0x37);
    LCD_WR_DATA(0x32);
    LCD_WR_DATA(0x29);
    LCD_WR_DATA(0x2D);
    LCD_WR_DATA(0x29);
    LCD_WR_DATA(0x25);
    LCD_WR_DATA(0x2B);
    LCD_WR_DATA(0x39);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x01);
    LCD_WR_DATA(0x03);
    LCD_WR_DATA(0x10);
    
    LCD_WR_REG(0xE1);   // Gamma (negative)
    LCD_WR_DATA(0x03);
    LCD_WR_DATA(0x1D);
    LCD_WR_DATA(0x07);
    LCD_WR_DATA(0x06);
    LCD_WR_DATA(0x2E);
    LCD_WR_DATA(0x2C);
    LCD_WR_DATA(0x29);
    LCD_WR_DATA(0x2D);
    LCD_WR_DATA(0x2E);
    LCD_WR_DATA(0x2E);
    LCD_WR_DATA(0x37);
    LCD_WR_DATA(0x3F);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x02);
    LCD_WR_DATA(0x10);
    
    LCD_WR_REG(0x13);   // Normal display on
    
    LCD_WR_REG(0x29);   // Display on
    LCD_Delay(100);
    
    LCD_Clear(WHITE);   // 清屏为白色
    
    LCD_BL_SET;         // 打开背光
}

// LCD开启显示
void LCD_DisplayOn(void)
{
    LCD_WR_REG(0x29);   // Display on
}

// LCD关闭显示
void LCD_DisplayOff(void)
{
    LCD_WR_REG(0x28);   // Display off
}

// 设置光标位置
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
    LCD_WR_REG(0x2A);   // 列地址设置
    LCD_WR_DATA(Xpos >> 8);
    LCD_WR_DATA(Xpos & 0xFF);
    
    LCD_WR_REG(0x2B);   // 行地址设置
    LCD_WR_DATA(Ypos >> 8);
    LCD_WR_DATA(Ypos & 0xFF);
}

// 设置窗口
void LCD_SetWindows(uint16_t xStar, uint16_t yStar, uint16_t xEnd, uint16_t yEnd)
{
    LCD_WR_REG(0x2A);   // 设置X坐标
    LCD_WR_DATA(xStar >> 8);
    LCD_WR_DATA(0xFF & xStar);
    LCD_WR_DATA(xEnd >> 8);
    LCD_WR_DATA(0xFF & xEnd);

    LCD_WR_REG(0x2B);   // 设置Y坐标
    LCD_WR_DATA(yStar >> 8);
    LCD_WR_DATA(0xFF & yStar);
    LCD_WR_DATA(yEnd >> 8);
    LCD_WR_DATA(0xFF & yEnd);

    LCD_WR_REG(0x2C);   // 写入RAM
}

// 准备写GRAM
void LCD_WriteRAM_Prepare(void)
{
    LCD_WR_REG(0x2C);   // 写GRAM
}

// 清屏
void LCD_Clear(uint16_t Color)
{
    uint32_t index = 0;
    uint32_t totalpoint = lcddev.width;
    totalpoint *= lcddev.height;  // 得到总点数
    
    LCD_SetWindows(0, 0, lcddev.width - 1, lcddev.height - 1);
    LCD_DC_SET;     // 写数据
    LCD_CS_CLR;     // 选中LCD
    
    for (index = 0; index < totalpoint; index++)
    {
        LCD_SPI_WriteByte(Color >> 8);
        LCD_SPI_WriteByte(Color);
    }
    
    LCD_CS_SET;     // 取消选中
}

// 画点
void LCD_DrawPoint(uint16_t x, uint16_t y)
{
    LCD_SetCursor(x, y);        // 设置光标位置
    LCD_WriteRAM_Prepare();     // 开始写入GRAM
    LCD_WR_DATA_16(POINT_COLOR);
}