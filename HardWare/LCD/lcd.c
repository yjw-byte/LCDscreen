#include "lcd.h"
#include "lcdfont.h"
#include "main.h"
#include "spi.h"

// 重新定义LCD相关操作，避免与lcd.c冲突
#define LCD_CMD  0
#define LCD_DATA 1

// 通过SPI写8位数据
void LCD_SPI_WriteByte(uint8_t data)
{
    HAL_SPI_Transmit(&hspi2, &data, 1, 1000);
}

// 写命令到LCD
 void LCD_WR_REG(uint8_t regval)
{
    HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_RESET); // 写命令
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET); // 选中LCD
    LCD_SPI_WriteByte(regval);
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);   // 取消选中
}

// 写数据到LCD
void LCD_WR_DATA(uint8_t data)
{
    HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_SET);   // 写数据
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET); // 选中LCD
    LCD_SPI_WriteByte(data);
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);   // 取消选中
}

// 写16位数据到LCD
void LCD_WR_DATA_16(uint16_t data)
{
    LCD_WR_DATA(data >> 8);
    LCD_WR_DATA(data & 0xFF);
}


// LCD复位
void LCD_Reset(void)
{
    HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_SET);
    HAL_Delay(100);
}

// LCD初始化
void LCD_Init_HAL(void)
{
    // LCD复位
    LCD_Reset();

    // ST7735初始化序列
    LCD_WR_REG(0x11);   // Sleep out
    HAL_Delay(120);

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
    LCD_WR_DATA(0xC0);  // MX, MY, RGB模式

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
    HAL_Delay(100);

    // 清屏为白色
    LCD_Fill(0, 0, 128, 160, WHITE);

    // 打开背光
    HAL_GPIO_WritePin(BL_GPIO_Port, BL_Pin, GPIO_PIN_SET);
}
void LCD_SetWindow(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd)
{
    xStart += 2;      // ST7735 128x160 屏的横向偏移
    xEnd   += 2;
    yStart += 1;      // 纵向偏移
    yEnd   += 1;

    LCD_WR_REG(0x2A);   // CASET
    LCD_WR_DATA(xStart >> 8);
    LCD_WR_DATA(xStart & 0xFF);
    LCD_WR_DATA(xEnd >> 8);
    LCD_WR_DATA(xEnd & 0xFF);

    LCD_WR_REG(0x2B);   // RASET
    LCD_WR_DATA(yStart >> 8);
    LCD_WR_DATA(yStart & 0xFF);
    LCD_WR_DATA(yEnd >> 8);
    LCD_WR_DATA(yEnd & 0xFF);

    LCD_WR_REG(0x2C);   // 开始写 RAM
}

// 设置光标位置
 void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
    LCD_SetWindow(Xpos, Ypos, Xpos, Ypos);
    LCD_WR_REG(0x2C);    // 开始写入GRAM
}

// 设置窗口
void LCD_SetWindows(uint16_t xStar, uint16_t yStar, uint16_t xEnd, uint16_t yEnd)
{
    LCD_SetWindow(xStar, yStar, xEnd, yEnd);
    LCD_WR_REG(0x2C);   // 写入RAM
}

/******************************************************************************
      函数说明：在指定区域填充颜色
      入口数据：xsta,ysta   起始坐标
                xend,yend   终止坐标
                color       要填充的颜色
      返回值：  无
******************************************************************************/
void LCD_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color)
{
    uint16_t i, j;
    LCD_SetWindows(xsta, ysta, xend - 1, yend - 1); // 设置显示范围
    for (i = ysta; i < yend; i++) {
        for (j = xsta; j < xend; j++) {
            LCD_WR_DATA_16(color);
        }
    }
}

/******************************************************************************
      函数说明：在指定位置画点
      入口数据：x,y 画点坐标
                color 点的颜色
      返回值：  无
******************************************************************************/
void LCD_DrawPoint_Color(uint16_t x, uint16_t y, uint16_t color)
{
    LCD_SetWindow(x, y, x, y); // 设置光标位置
    LCD_WR_REG(0x2C);    // 开始写入GRAM
    LCD_WR_DATA_16(color);
}

/******************************************************************************
      函数说明：画线
      入口数据：x1,y1   起始坐标
                x2,y2   终止坐标
                color   线的颜色
      返回值：  无
******************************************************************************/
void LCD_DrawLine_Color(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    delta_x = x2 - x1; // 计算坐标增量
    delta_y = y2 - y1;
    uRow = x1;         // 画线起点坐标
    uCol = y1;
    
    if (delta_x > 0)
        incx = 1; // 设置单步方向
    else if (delta_x == 0)
        incx = 0; // 垂直线
    else {
        incx = -1;
        delta_x = -delta_x;
    }
    
    if (delta_y > 0)
        incy = 1;
    else if (delta_y == 0)
        incy = 0; // 水平线
    else {
        incy = -1;
        delta_y = -delta_y;
    }
    
    if (delta_x > delta_y)
        distance = delta_x; // 选取基本增量坐标轴
    else
        distance = delta_y;
    
    for (t = 0; t < distance + 1; t++) {
        LCD_DrawPoint_Color(uRow, uCol, color); // 画点
        xerr += delta_x;
        yerr += delta_y;
        if (xerr > distance) {
            xerr -= distance;
            uRow += incx;
        }
        if (yerr > distance) {
            yerr -= distance;
            uCol += incy;
        }
    }
}

/******************************************************************************
      函数说明：画矩形
      入口数据：x1,y1   起始坐标
                x2,y2   终止坐标
                color   矩形的颜色
      返回值：  无
******************************************************************************/
void LCD_DrawRectangle_Color(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    LCD_DrawLine_Color(x1, y1, x2, y1, color);
    LCD_DrawLine_Color(x1, y1, x1, y2, color);
    LCD_DrawLine_Color(x1, y2, x2, y2, color);
    LCD_DrawLine_Color(x2, y1, x2, y2, color);
}

/******************************************************************************
      函数说明：画圆
      入口数据：x0,y0   圆心坐标
                r       半径
                color   圆的颜色
      返回值：  无
******************************************************************************/
void Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color)
{
    int a, b;
    a = 0;
    b = r;
    while (a <= b) {
        LCD_DrawPoint_Color(x0 - b, y0 - a, color); // 3
        LCD_DrawPoint_Color(x0 + b, y0 - a, color); // 0
        LCD_DrawPoint_Color(x0 - a, y0 + b, color); // 1
        LCD_DrawPoint_Color(x0 - a, y0 - b, color); // 2
        LCD_DrawPoint_Color(x0 + b, y0 + a, color); // 4
        LCD_DrawPoint_Color(x0 + a, y0 - b, color); // 5
        LCD_DrawPoint_Color(x0 + a, y0 + b, color); // 6
        LCD_DrawPoint_Color(x0 - b, y0 + a, color); // 7
        a++;
        if ((a * a + b * b) > (r * r)) // 判断要画的点是否过远
        {
            b--;
        }
    }
}

/******************************************************************************
      函数说明：显示汉字串
      入口数据：x,y显示坐标
                *s 要显示的汉字串
                fc 字的颜色
                bc 字的背景色
                sizey 字号 可选 16 24 32
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
    while (*s != 0) {
        if (sizey == 12)
            LCD_ShowChinese12x12(x, y, s, fc, bc, sizey, mode);
        else if (sizey == 16)
            LCD_ShowChinese16x16(x, y, s, fc, bc, sizey, mode);
        else if (sizey == 24)
            LCD_ShowChinese24x24(x, y, s, fc, bc, sizey, mode);
        else if (sizey == 32)
            LCD_ShowChinese32x32(x, y, s, fc, bc, sizey, mode);
        else
            return;
        s += 2;
        x += sizey;
    }
}

/******************************************************************************
      函数说明：显示单个12x12汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese12x12(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
    uint8_t i, j, m = 0;
    uint16_t k;
    uint16_t HZnum;      // 汉字数目
    uint16_t TypefaceNum; // 一个字符所占字节大小
    uint16_t x0 = x;
    uint8_t data_buffer[32]; // 缓冲区，用于批量传输
    uint16_t buffer_index = 0;
    
    TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;

    HZnum = sizeof(tfont12) / sizeof(typFNT_GB12); // 统计汉字数目
    for (k = 0; k < HZnum; k++) {
        if ((tfont12[k].Index[0] == *(s)) && (tfont12[k].Index[1] == *(s + 1))) {
            LCD_SetWindows(x, y, x + sizey - 1, y + sizey - 1);
            for (i = 0; i < TypefaceNum; i++) {
                for (j = 0; j < 8; j++) {
                    if (!mode) // 非叠加方式
                    {
                        if (tfont12[k].Msk[i] & (0x01 << j)) {
                            data_buffer[buffer_index++] = fc >> 8;
                            data_buffer[buffer_index++] = fc & 0xFF;
                        } else {
                            data_buffer[buffer_index++] = bc >> 8;
                            data_buffer[buffer_index++] = bc & 0xFF;
                        }
                        
                        // 如果缓冲区满了，就发送数据
                        if (buffer_index >= 32) {
                            LCD_WR_DATA_Array(data_buffer, buffer_index);
                            buffer_index = 0;
                        }
                        
                        m++;
                        if (m % sizey == 0) {
                            m = 0;
                            break;
                        }
                    } else // 叠加方式
                    {
                        if (tfont12[k].Msk[i] & (0x01 << j))
                            LCD_DrawPoint_Color(x, y, fc); // 画一个点
                        x++;
                        if ((x - x0) == sizey) {
                            x = x0;
                            y++;
                            break;
                        }
                    }
                }
            }
            
            // 发送剩余的数据
            if (buffer_index > 0) {
                LCD_WR_DATA_Array(data_buffer, buffer_index);
                buffer_index = 0;
            }
        }
        continue; // 查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }
}

/******************************************************************************
      函数说明：显示单个16x16汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese16x16(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
    uint8_t i, j, m = 0;
    uint16_t k;
    uint16_t HZnum;      // 汉字数目
    uint16_t TypefaceNum; // 一个字符所占字节大小
    uint16_t x0 = x;
    uint8_t data_buffer[32]; // 缓冲区，用于批量传输
    uint16_t buffer_index = 0;
    
    TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
    HZnum = sizeof(tfont16) / sizeof(typFNT_GB16); // 统计汉字数目
    for (k = 0; k < HZnum; k++) {
        if ((tfont16[k].Index[0] == *(s)) && (tfont16[k].Index[1] == *(s + 1))) {
            LCD_SetWindows(x, y, x + sizey - 1, y + sizey - 1);
            for (i = 0; i < TypefaceNum; i++) {
                for (j = 0; j < 8; j++) {
                    if (!mode) // 非叠加方式
                    {
                        if (tfont16[k].Msk[i] & (0x01 << j)) {
                            data_buffer[buffer_index++] = fc >> 8;
                            data_buffer[buffer_index++] = fc & 0xFF;
                        } else {
                            data_buffer[buffer_index++] = bc >> 8;
                            data_buffer[buffer_index++] = bc & 0xFF;
                        }
                        
                        // 如果缓冲区满了，就发送数据
                        if (buffer_index >= 32) {
                            LCD_WR_DATA_Array(data_buffer, buffer_index);
                            buffer_index = 0;
                        }
                        
                        m++;
                        if (m % sizey == 0) {
                            m = 0;
                            break;
                        }
                    } else // 叠加方式
                    {
                        if (tfont16[k].Msk[i] & (0x01 << j))
                            LCD_DrawPoint_Color(x, y, fc); // 画一个点
                        x++;
                        if ((x - x0) == sizey) {
                            x = x0;
                            y++;
                            break;
                        }
                    }
                }
            }
            
            // 发送剩余的数据
            if (buffer_index > 0) {
                LCD_WR_DATA_Array(data_buffer, buffer_index);
                buffer_index = 0;
            }
        }
        continue; // 查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }
}

/******************************************************************************
      函数说明：显示单个24x24汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese24x24(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
    uint8_t i, j, m = 0;
    uint16_t k;
    uint16_t HZnum;      // 汉字数目
    uint16_t TypefaceNum; // 一个字符所占字节大小
    uint16_t x0 = x;
    uint8_t data_buffer[48]; // 缓冲区，用于批量传输
    uint16_t buffer_index = 0;
    
    TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
    HZnum = sizeof(tfont24) / sizeof(typFNT_GB24); // 统计汉字数目
    for (k = 0; k < HZnum; k++) {
        if ((tfont24[k].Index[0] == *(s)) && (tfont24[k].Index[1] == *(s + 1))) {
            LCD_SetWindows(x, y, x + sizey - 1, y + sizey - 1);
            for (i = 0; i < TypefaceNum; i++) {
                for (j = 0; j < 8; j++) {
                    if (!mode) // 非叠加方式
                    {
                        if (tfont24[k].Msk[i] & (0x01 << j)) {
                            data_buffer[buffer_index++] = fc >> 8;
                            data_buffer[buffer_index++] = fc & 0xFF;
                        } else {
                            data_buffer[buffer_index++] = bc >> 8;
                            data_buffer[buffer_index++] = bc & 0xFF;
                        }
                        
                        // 如果缓冲区满了，就发送数据
                        if (buffer_index >= 48) {
                            LCD_WR_DATA_Array(data_buffer, buffer_index);
                            buffer_index = 0;
                        }
                        
                        m++;
                        if (m % sizey == 0) {
                            m = 0;
                            break;
                        }
                    } else // 叠加方式
                    {
                        if (tfont24[k].Msk[i] & (0x01 << j))
                            LCD_DrawPoint_Color(x, y, fc); // 画一个点
                        x++;
                        if ((x - x0) == sizey) {
                            x = x0;
                            y++;
                            break;
                        }
                    }
                }
            }
            
            // 发送剩余的数据
            if (buffer_index > 0) {
                LCD_WR_DATA_Array(data_buffer, buffer_index);
                buffer_index = 0;
            }
        }
        continue; // 查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }
}

/******************************************************************************
      函数说明：显示单个32x32汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese32x32(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
    uint8_t i, j, m = 0;
    uint16_t k;
    uint16_t HZnum;      // 汉字数目
    uint16_t TypefaceNum; // 一个字符所占字节大小
    uint16_t x0 = x;
    uint8_t data_buffer[64]; // 缓冲区，用于批量传输
    uint16_t buffer_index = 0;
    
    TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
    HZnum = sizeof(tfont32) / sizeof(typFNT_GB32); // 统计汉字数目
    for (k = 0; k < HZnum; k++) {
        if ((tfont32[k].Index[0] == *(s)) && (tfont32[k].Index[1] == *(s + 1))) {
            LCD_SetWindows(x, y, x + sizey - 1, y + sizey - 1);
            for (i = 0; i < TypefaceNum; i++) {
                for (j = 0; j < 8; j++) {
                    if (!mode) // 非叠加方式
                    {
                        if (tfont32[k].Msk[i] & (0x01 << j)) {
                            data_buffer[buffer_index++] = fc >> 8;
                            data_buffer[buffer_index++] = fc & 0xFF;
                        } else {
                            data_buffer[buffer_index++] = bc >> 8;
                            data_buffer[buffer_index++] = bc & 0xFF;
                        }
                        
                        // 如果缓冲区满了，就发送数据
                        if (buffer_index >= 64) {
                            LCD_WR_DATA_Array(data_buffer, buffer_index);
                            buffer_index = 0;
                        }
                        
                        m++;
                        if (m % sizey == 0) {
                            m = 0;
                            break;
                        }
                    } else // 叠加方式
                    {
                        if (tfont32[k].Msk[i] & (0x01 << j))
                            LCD_DrawPoint_Color(x, y, fc); // 画一个点
                        x++;
                        if ((x - x0) == sizey) {
                            x = x0;
                            y++;
                            break;
                        }
                    }
                }
            }
            
            // 发送剩余的数据
            if (buffer_index > 0) {
                LCD_WR_DATA_Array(data_buffer, buffer_index);
                buffer_index = 0;
            }
        }
        continue; // 查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }
}

/******************************************************************************
      函数说明：显示单个字符
      入口数据：x,y显示坐标
                num 要显示的字符
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChar(uint16_t x, uint16_t y, uint8_t num, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
    uint8_t temp, sizex, t, m = 0;
    uint16_t i, TypefaceNum; // 一个字符所占字节大小
    uint16_t x0 = x;
    uint8_t data_buffer[32]; // 缓冲区，用于批量传输
    uint16_t buffer_index = 0;
    
    sizex = sizey / 2;
    TypefaceNum = (sizex / 8 + ((sizex % 8) ? 1 : 0)) * sizey;
    num = num - ' '; // 得到偏移后的值
    LCD_SetWindows(x, y, x + sizex - 1, y + sizey - 1); // 设置光标位置
    for (i = 0; i < TypefaceNum; i++) {
        if (sizey == 12)
            temp = ascii_1206[num][i];     // 调用6x12字体
        else if (sizey == 16)
            temp = ascii_1608[num][i];     // 调用8x16字体
        else if (sizey == 24)
            temp = ascii_2412[num][i];     // 调用12x24字体
        else if (sizey == 32)
            temp = ascii_3216[num][i];     // 调用16x32字体
        else
            return;
        for (t = 0; t < 8; t++) {
            if (!mode) // 非叠加模式
            {
                if (temp & (0x01 << t)) {
                    data_buffer[buffer_index++] = fc >> 8;
                    data_buffer[buffer_index++] = fc & 0xFF;
                } else {
                    data_buffer[buffer_index++] = bc >> 8;
                    data_buffer[buffer_index++] = bc & 0xFF;
                }
                
                // 如果缓冲区满了，就发送数据
                if (buffer_index >= 32) {
                    LCD_WR_DATA_Array(data_buffer, buffer_index);
                    buffer_index = 0;
                }
                
                m++;
                if (m % sizex == 0) {
                    m = 0;
                    break;
                }
            } else // 叠加模式
            {
                if (temp & (0x01 << t))
                    LCD_DrawPoint_Color(x, y, fc); // 画一个点
                x++;
                if ((x - x0) == sizex) {
                    x = x0;
                    y++;
                    break;
                }
            }
        }
    }
    
    // 发送剩余的数据
    if (buffer_index > 0) {
        LCD_WR_DATA_Array(data_buffer, buffer_index);
    }
}

/******************************************************************************
      函数说明：显示字符串
      入口数据：x,y显示坐标
                *p 要显示的字符串
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowString(uint16_t x, uint16_t y, const uint8_t *p, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
    while (*p != '\0') {
        LCD_ShowChar(x, y, *p, fc, bc, sizey, mode);
        x += sizey / 2;
        p++;
    }
}

/******************************************************************************
      函数说明：显示数字
      入口数据：m底数，n指数
      返回值：  无
******************************************************************************/
uint32_t mypow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;
    while (n--)
        result *= m;
    return result;
}

/******************************************************************************
      函数说明：显示整数变量
      入口数据：x,y显示坐标
                num 要显示整数变量
                len 要显示的位数
                fc 字的颜色
                bc 字的背景色
                sizey 字号
      返回值：  无
******************************************************************************/
void LCD_ShowIntNum(uint16_t x, uint16_t y, uint16_t num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey)
{
    uint8_t t, temp;
    uint8_t enshow = 0;
    uint8_t sizex = sizey / 2;
    for (t = 0; t < len; t++) {
        temp = (num / mypow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1)) {
            if (temp == 0) {
                LCD_ShowChar(x + t * sizex, y, ' ', fc, bc, sizey, 0);
                continue;
            } else
                enshow = 1;
        }
        LCD_ShowChar(x + t * sizex, y, temp + 48, fc, bc, sizey, 0);
    }
}

/******************************************************************************
      函数说明：显示两位小数变量
      入口数据：x,y显示坐标
                num 要显示小数变量
                len 要显示的位数
                fc 字的颜色
                bc 字的背景色
                sizey 字号
      返回值：  无
******************************************************************************/
void LCD_ShowFloatNum1(uint16_t x, uint16_t y, float num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey)
{
    uint8_t t, temp, sizex;
    uint16_t num1;
    sizex = sizey / 2;
    num1 = num * 100;
    for (t = 0; t < len; t++) {
        temp = (num1 / mypow(10, len - t - 1)) % 10;
        if (t == (len - 2)) {
            LCD_ShowChar(x + (len - 2) * sizex, y, '.', fc, bc, sizey, 0);
            t++;
            len += 1;
        }
        LCD_ShowChar(x + t * sizex, y, temp + 48, fc, bc, sizey, 0);
    }
}

/******************************************************************************
      函数说明：显示图片
      入口数据：x,y起点坐标
                length 图片长度
                width  图片宽度
                pic[]  图片数组
      返回值：  无
******************************************************************************/
void LCD_ShowPicture(uint16_t x, uint16_t y, uint16_t length, uint16_t width, const uint8_t pic[])
{
    uint32_t pixel_count = length * width;
    uint32_t i;
    
    LCD_SetWindows(x, y, x + length - 1, y + width - 1);
    for (i = 0; i < pixel_count; i++) {
        LCD_WR_DATA_16((pic[i * 2] << 8) | pic[i * 2 + 1]);
    }
}

/******************************************************************************
      函数说明：批量写入数据到LCD
      入口数据：data 数据数组指针
                len  数据长度
      返回值：  无
******************************************************************************/
void LCD_WR_DATA_Array(uint8_t *data, uint16_t len)
{
    HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_SET);   // 写数据
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET); // 选中LCD
    HAL_SPI_Transmit(&hspi2, data, len, 1000);
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);   // 取消选中
}
