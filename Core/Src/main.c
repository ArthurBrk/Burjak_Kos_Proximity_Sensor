/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "adc.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <math.h>

// Display
#include "c7x10b.h"
#include "adapter.h"
#include "c7x10b_config.h"

// Proximity
#include "i2c_utils.h"
#include "printf.h"
#include "Calculation_PS.h"
/* USER CODE END Includes */
/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// Display
#define DISPLAY_REFRESH_INTERVAL 200 // update interval (ms)

// Proximity
#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))

#define I2C_CHECK_OK(status)    ((status) == HAL_OK)
#define I2C_CHECK_ERROR(status) ((status) != HAL_OK)

#define BIT(x)      (1UL << (x))
#define SET_BITS(reg, bit) ((reg) |= BIT(bit))
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
// Proximity
volatile float g_ps_cm = 0.0f;
static uint8_t TimerCount = 0;
static uint16_t adc_value = 0;
static uint8_t last_ps_cm = 50; //last measured proximity in cm (max 50)
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
static void Read_Proximity_Sensor(void);
static float Get_Proximity_cm(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint16_t get_ADC_Value(){
    uint16_t value = 0;
    HAL_StatusTypeDef status;

    printf("[ADC] Start\r\n");
    status = HAL_ADC_Start(&hadc1);
    if (status != HAL_OK) {
        printf("[ADC] ERROR: Start failed with status=%d\r\n", status);
        return 0;
    }

    printf("[ADC] Waiting for conversion\r\n");
    status = HAL_ADC_PollForConversion(&hadc1, 10);
    if (status != HAL_OK) {
        printf("[ADC] ERROR: Conversion timeout! (status=%d)\r\n", status);
        HAL_ADC_Stop(&hadc1);
        return 0;
    }

    value = (uint16_t)HAL_ADC_GetValue(&hadc1);
    printf("[ADC] Raw value: %u (0x%04X) [Range: 0-4095]\r\n", value, value);

    HAL_ADC_Stop(&hadc1);
    printf("[ADC] Stopped\r\n");

    return value;
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
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
  	// Display
    // WICHTIG: MAX30205 braucht Zeit nach Power-Up
    HAL_Delay(100);
    // Initialize 7x10 B display
    c7x10b_init();
    c7x10b_clearDisplay();

    // Proximity
    // Initialize Proximity Sensor
    PS_StateConfig_ADC(0);

    set_UART(&huart2);
    HAL_TIM_Base_Start_IT(&htim6);
    printf("**********************************");
    printf("System Initialized\r\n");
    printf("**********************************");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  uint32_t last_refresh_time = 0;
  while (1)
  {
	  // Read ADC to configure PS duty cycle
	  adc_value = get_ADC_Value();
	  PS_StateConfig_ADC(adc_value);

	  // Read proximity sensor and get distance in cm
	  float ps_cm_float = Get_Proximity_cm();

	  // Round to nearest integer for display (since drawNumber expects uint8_t 0-99)
	  last_ps_cm = (uint8_t)roundf(ps_cm_float);

	  uint32_t current_time = HAL_GetTick();
	  if(current_time - last_refresh_time >= DISPLAY_REFRESH_INTERVAL)
	  {
		  c7x10b_clearDisplay();
	      if(last_ps_cm < 50)
	      {
	    	  c7x10b_drawNumber(last_ps_cm);
	      }
	      else
	      {
	    	  c7x10b_drawNumber(50);
	      }
	      c7x10b_refreshDisplay();
	      last_refresh_time = current_time;
	  }
	      c7x10b_refreshDisplay();

	      HAL_Delay(30);   // ~30 Hz update rate
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
// Liest PS1_Data und gibt Rohwert aus
static void Read_Proximity_Sensor(void)
{
    HAL_StatusTypeDef status;
    uint8_t ps_buf[2] = {0};

    status = I2C_ReadBytes(&hi2c1, SENSOR_ADDR, 0x08, 1, ps_buf, 2, 100);

    if (I2C_CHECK_ERROR(status)) {
        printf("PS read FAILED, status=%d\r\n", status);
        return;
    }

    uint16_t ps_raw = ((uint16_t)ps_buf[1] << 8) | ps_buf[0];
    float ps_cm = Proximity_Conversion(ps_raw);
    g_ps_cm = ps_cm;
    printf("PS Raw: %u, PS cm: %f\r\n", ps_raw, ps_cm);
}

static float Get_Proximity_cm(void)
{
    Read_Proximity_Sensor();
    return g_ps_cm;
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim){
	if(htim->Instance == TIM6){
		printf("TIM6 trigger \r\n");
		TimerCount++;
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
