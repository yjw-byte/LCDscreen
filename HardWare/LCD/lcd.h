#ifndef __LCD_H
#define __LCD_H

#include "main.h"
#include "spi.h"

// LCD重要参数集
typedef struct
{
    uint16_t width;         // LCD 宽度
    uint16_t height;        // LCD 高度
    uint16_t id;            // LCD ID
    uint8_t dir;            // LCD 方向
    uint16_t wramcmd;       // 开始写GRAM指令
    uint16_t setxcmd;       // 设置x起始坐标指令
    uint16_t setycmd;       // 设置y起始坐标指令
} _lcd_dev;

// LCD参数
extern _lcd_dev lcddev; // 管理LCD重要参数
extern SPI_HandleTypeDef hspi2;

// LCD地址
#define LCD_BASE        ((uint32_t)(0x60000000 | 0x000007FE))
#define LCD             ((LCD_TypeDef*) LCD_BASE)

// LCD命令和数据位
#define LCD_CMD         0       // 写命令
#define LCD_DATA        1       // 写数据

// 定义LCD的GPIO引脚
#define LCD_CS_SET      HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET)
#define LCD_CS_CLR      HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET)

#define LCD_DC_SET      HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_SET)
#define LCD_DC_CLR      HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_RESET)

#define LCD_RST_SET     HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_SET)
#define LCD_RST_CLR     HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_RESET)

#define LCD_BL_SET      HAL_GPIO_WritePin(BL_GPIO_Port, BL_Pin, GPIO_PIN_SET)
#define LCD_BL_CLR      HAL_GPIO_WritePin(BL_GPIO_Port, BL_Pin, GPIO_PIN_RESET)

// 扫描方向定义
#define L2R_U2D         0       // 从左到右,从上到下
#define L2R_D2U         1       // 从左到右,从下到上
#define R2L_U2D         2       // 从右到左,从上到下
#define R2L_D2U         3       // 从右到左,从下到上

#define U2D_L2R         4       // 从上到下,从左到右
#define U2D_R2L         5       // 从上到下,从右到左
#define D2U_L2R         6       // 从下到上,从左到右
#define D2U_R2L         7       // 从下到上,从右到左

#define DFT_SCAN_DIR    L2R_U2D // 默认扫描方向

// 画笔颜色
#define WHITE           0xFFFF
#define BLACK           0x0000
#define BLUE            0x001F
#define BRED            0XF81F
#define GRED            0XFFE0
#define GBLUE           0X07FF
#define RED             0xF800
#define MAGENTA         0xF81F
#define GREEN           0x07E0
#define CYAN            0x7FFF
#define YELLOW          0xFFE0
#define BROWN           0XBC40
#define BRRED           0XFC07
#define GRAY            0X8430

// GUI颜色
#define DARKBLUE        0X01CF
#define LIGHTBLUE       0X7D7C
#define GRAYBLUE        0X5458

#define LIGHTGREEN      0X841F
#define LIGHTGRAY       0XEF5B
#define LGRAY           0XC618

#define LGRAYBLUE       0XA651
#define LBBLUE          0X2B12

// 函数声明
void LCD_Init(void);
void LCD_DisplayOn(void);
void LCD_DisplayOff(void);
void LCD_Clear(uint16_t Color);
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);
void LCD_DrawPoint(uint16_t x, uint16_t y);
uint16_t LCD_ReadPoint(uint16_t x, uint16_t y);
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_SetWindows(uint16_t xStar, uint16_t yStar, uint16_t xEnd, uint16_t yEnd);
void LCD_WriteRAM_Prepare(void);
void LCD_WriteData(uint16_t Data);
void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue);
uint16_t LCD_ReadData(void);
uint16_t LCD_ReadReg(uint8_t LCD_Reg);
void LCD_SetParam(void);
void LCD_SetDirection(uint8_t direction);

// 底层驱动函数
void LCD_SPI_WriteByte(uint8_t data);
void LCD_WR_REG(uint8_t regval);
void LCD_WR_DATA(uint8_t data);
void LCD_WR_DATA_16(uint16_t data);
void LCD_Delay(uint32_t delay);
void LCD_Reset(void);

#endif