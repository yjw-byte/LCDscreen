/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
#include "pic.h"
#include "lvgl.h"
#include "lv_port_disp.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
uint16_t color_palette[] = {
  0xFFFF, // WHITE
  0x0000, // BLACK
  0x001F, // BLUE
  0xF81F, // BRED
  0xFFE0, // GRED
  0x07FF, // GBLUE
  0xF800, // RED
  0xF81F, // MAGENTA
  0x07E0, // GREEN
  0x7FFF, // CYAN
  0xFFE0, // YELLOW
  0xBC40, // BROWN
  0xFC07, // BRRED
  0x8430, // GRAY
  0x01CF, // DARKBLUE
  0x7D7C, // LIGHTBLUE
  0x5458, // GRAYBLUE
  0x841F, // LIGHTGREEN
  0xC618, // LGRAY
  0xA651, // LGRAYBLUE
  0x2B12  // LBBLUE
};

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// 按钮事件处理函数
static void btn_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {
        static uint8_t cnt = 0;
        cnt++;
        
        /*获取按钮上的标签*/
        lv_obj_t * label = lv_obj_get_child(btn, 0);
        lv_label_set_text_fmt(label, "Button: %d", cnt);
    }
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI2_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
  
  // 初始化LCD
  LCD_Init_HAL();
  HAL_TIM_Base_Start_IT(&htim6); // 启动定时器6，用于LCD延时
  lv_init();
  lv_port_disp_init();
  LCD_Fill(0, 0, 128, 160, WHITE);
  
  // 创建屏幕和基本控件
  lv_obj_t * scr = lv_scr_act();
  lv_obj_set_style_bg_color(scr, lv_color_hex(0x003a57), 0);
  
  // 创建标签
  lv_obj_t * label = lv_label_create(scr);
  lv_label_set_text(label, "Hello LVGL!");
  lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 5);
  
  // 创建按钮
  lv_obj_t * btn = lv_btn_create(scr);
  lv_obj_set_size(btn, 80, 30);
  lv_obj_align(btn, LV_ALIGN_TOP_MID, 0, 25);
  lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, NULL);
  
  // 为按钮添加标签
  lv_obj_t * btn_label = lv_label_create(btn);
  lv_label_set_text(btn_label, "Button");
  lv_obj_center(btn_label);
  
  // 创建进度条
  lv_obj_t * bar1 = lv_bar_create(scr);
  lv_obj_set_size(bar1, 100, 10);
  lv_obj_align(bar1, LV_ALIGN_TOP_MID, 0, 65);
  lv_bar_set_value(bar1, 70, LV_ANIM_OFF);
  
  // 创建滑块
  lv_obj_t * slider = lv_slider_create(scr);
  lv_obj_set_width(slider, 100);
  lv_obj_align(slider, LV_ALIGN_TOP_MID, 0, 85);
  lv_slider_set_value(slider, 50, LV_ANIM_OFF);
  
  // 创建另一个标签显示数值
  lv_obj_t * value_label = lv_label_create(scr);
  lv_label_set_text(value_label, "Value: 50");
  lv_obj_align(value_label, LV_ALIGN_TOP_MID, 0, 105);
  
  // 创建复选框
  lv_obj_t * cb = lv_checkbox_create(scr);
  lv_checkbox_set_text(cb, "Checkbox");
  lv_obj_align(cb, LV_ALIGN_TOP_MID, 0, 125);
  
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  uint32_t last_update = 0;
  int16_t progress = 0;
  int8_t progress_direction = 1;
  
  while (1)
  {
    // 更新进度条动画
    uint32_t current_time = HAL_GetTick();
    if(current_time - last_update > 50) { // 每50ms更新一次
        last_update = current_time;
        
        // 更新进度条
        progress += progress_direction * 2;
        if(progress >= 100) {
            progress = 100;
            progress_direction = -1;
        } else if(progress <= 0) {
            progress = 0;
            progress_direction = 1;
        }
        lv_bar_set_value(bar1, progress, LV_ANIM_OFF);
        
        // 更新数值标签
        lv_label_set_text_fmt(value_label, "Value: %d", lv_slider_get_value(slider));
    }
    
    lv_task_handler();
    HAL_Delay(5);

    /* USER CODE END WHILE */


  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM6) {
    lv_tick_inc(1);  // 每毫秒加 1
  }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */