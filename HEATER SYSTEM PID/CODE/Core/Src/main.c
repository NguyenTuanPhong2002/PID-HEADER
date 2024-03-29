/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "application.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
CLCD_I2C_Name LCD1;

PIDController pid;

uint32_t encoderCount = 60;
float powerpoint = 1.0f;
uint32_t realValue = 0;
uint8_t dir = 0;
uint64_t timestap = 0;
uint64_t sw_timestap = 0;
uint8_t status = 0;
uint8_t clsmoniter = 0;
double control_signal = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_SPI2_Init(void);
static void MX_RTC_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == GPIO_PIN_2 && (HAL_GetTick() - sw_timestap) > 1000) {
		if (status == 0) {
			status = 1;
			//clsmoniter = 1;
			initTime();
			PID_Init(&pid, 1.0, 0.1, 0.01, encoderCount);
		} else if (status == 1) {
			initTime();
			status = 2;
			clsmoniter = 1;
		} else if (status == 2) {
			status = 0;
			clsmoniter = 1;
		}
		sw_timestap = HAL_GetTick();
	}

	if (status == 0 || status == 1) {

		if ((HAL_GetTick() - timestap) > 100)
			dir = 0;
		if (GPIO_Pin == GPIO_PIN_1 && dir == 0
				|| GPIO_Pin == GPIO_PIN_1 && dir == 1) {
			encoderCount++;
			PID_Init(&pid, 1.0, 0.1, 0.01, encoderCount);
			HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
			dir = 1;
			timestap = HAL_GetTick();
		}
		if (GPIO_Pin == GPIO_PIN_0 && dir == 0
				|| GPIO_Pin == GPIO_PIN_0 && dir == 2) {
			if (encoderCount > 0) {
				encoderCount--;
				PID_Init(&pid, 1.0, 0.1, 0.01, encoderCount);
			}
			HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
			dir = 2;
			timestap = HAL_GetTick();
		}
	}

	if (status == 2) {

		if ((HAL_GetTick() - timestap) > 100)
			dir = 0;
		if (GPIO_Pin == GPIO_PIN_1 && dir == 0
				|| GPIO_Pin == GPIO_PIN_1 && dir == 1) {
			powerpoint = powerpoint + 0.05f;
			//PID_Init(&pid, 1.0, 0.1, 0.01, encoderCount);
			HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
			dir = 1;
			timestap = HAL_GetTick();
		}
		if (GPIO_Pin == GPIO_PIN_0 && dir == 0
				|| GPIO_Pin == GPIO_PIN_0 && dir == 2) {
			if (powerpoint > 0.9f) {
				powerpoint = powerpoint - 0.05f;
				//PID_Init(&pid, 1.0, 0.1, 0.01, encoderCount);
			}
			HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
			dir = 2;
			timestap = HAL_GetTick();
		}
	}
}

int pwmtime;
void pwm_output(int pwm) {
	if (pwm <= 0) {
		HAL_GPIO_WritePin(PWM_GPIO_Port, PWM_Pin, 1);
		HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, 1);
		HAL_Delay(250);
	} else if (pwm >= 100) {
		HAL_GPIO_WritePin(PWM_GPIO_Port, PWM_Pin, 0);
		HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, 0);
		HAL_Delay(250);
	} else {
		pwmtime = 100 - pwm;
		if (pwmtime == 0) {
			pwmtime = 1;
		}
		for (int i = 0; i < 3; i++) {
			HAL_GPIO_WritePin(PWM_GPIO_Port, PWM_Pin, 0);
			HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, 0);
			HAL_Delay(pwm);
			HAL_GPIO_WritePin(PWM_GPIO_Port, PWM_Pin, 1);
			HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, 1);
			HAL_Delay(pwmtime);
		}
	}

}

void powerPID() {

}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
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
	MX_I2C1_Init();
	MX_USART1_UART_Init();
	MX_SPI2_Init();
	MX_RTC_Init();
	MX_TIM2_Init();
	/* USER CODE BEGIN 2 */

	HAL_GPIO_WritePin(PWM_GPIO_Port, PWM_Pin, 1);

	initApp();
	MAX6675_Init();

	mainMenu();
	initTime();

	/* Create the object*/

// char *message = "Hello, UART!\r\n";
// HAL_UART_Transmit(&huart1, (uint8_t *)message, strlen(message),
// 				  HAL_MAX_DELAY);
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
		if (clsmoniter == 1) {
			CLCD_I2C_Clear(&LCD1);
			clsmoniter = 0;
		}

		if (status == 1) {
			updateSetpoint(encoderCount);
			updateStatus(status);

			/* Functions */
			realValue = getTem();
			updateRealValue(realValue);
			control_signal = PID_Update(&pid, realValue);
			if (realValue > encoderCount) {
				pwm_output(-1);
			} else {
				pwm_output(control_signal);
			}
			updateTime();
		} else if (status == 0) {
			mainMenu();
			pwm_output(-1);
			updateSetpoint(encoderCount);
			updateStatus(status);

			/* Functions */
			realValue = getTem();
			updateRealValue(realValue);
			HAL_Delay(200);
		} else if (status == 2) {
			mainPower();
			updatePowerpoint(powerpoint);
			realValue = getTem();
			updateRealValue(realValue);
			pwm_output((int) (powerpoint * 5.8f));
			updateTime();
		}
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI
			| RCC_OSCILLATORTYPE_LSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.LSIState = RCC_LSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
	PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void) {

	/* USER CODE BEGIN I2C1_Init 0 */

	/* USER CODE END I2C1_Init 0 */

	/* USER CODE BEGIN I2C1_Init 1 */

	/* USER CODE END I2C1_Init 1 */
	hi2c1.Instance = I2C1;
	hi2c1.Init.ClockSpeed = 100000;
	hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN I2C1_Init 2 */

	/* USER CODE END I2C1_Init 2 */

}

/**
 * @brief RTC Initialization Function
 * @param None
 * @retval None
 */
static void MX_RTC_Init(void) {

	/* USER CODE BEGIN RTC_Init 0 */

	/* USER CODE END RTC_Init 0 */

	RTC_TimeTypeDef sTime = { 0 };
	RTC_DateTypeDef DateToUpdate = { 0 };

	/* USER CODE BEGIN RTC_Init 1 */

	/* USER CODE END RTC_Init 1 */

	/** Initialize RTC Only
	 */
	hrtc.Instance = RTC;
	hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
	hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
	if (HAL_RTC_Init(&hrtc) != HAL_OK) {
		Error_Handler();
	}

	/* USER CODE BEGIN Check_RTC_BKUP */

	/* USER CODE END Check_RTC_BKUP */

	/** Initialize RTC and set the Time and Date
	 */
	sTime.Hours = 0x0;
	sTime.Minutes = 0x0;
	sTime.Seconds = 0x0;

	if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK) {
		Error_Handler();
	}
	DateToUpdate.WeekDay = RTC_WEEKDAY_MONDAY;
	DateToUpdate.Month = RTC_MONTH_JANUARY;
	DateToUpdate.Date = 0x1;
	DateToUpdate.Year = 0x0;

	if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BCD) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN RTC_Init 2 */

	/* USER CODE END RTC_Init 2 */

}

/**
 * @brief SPI2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI2_Init(void) {

	/* USER CODE BEGIN SPI2_Init 0 */

	/* USER CODE END SPI2_Init 0 */

	/* USER CODE BEGIN SPI2_Init 1 */

	/* USER CODE END SPI2_Init 1 */
	/* SPI2 parameter configuration*/
	hspi2.Instance = SPI2;
	hspi2.Init.Mode = SPI_MODE_MASTER;
	hspi2.Init.Direction = SPI_DIRECTION_2LINES_RXONLY;
	hspi2.Init.DataSize = SPI_DATASIZE_16BIT;
	hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi2.Init.NSS = SPI_NSS_SOFT;
	hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
	hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi2.Init.CRCPolynomial = 10;
	if (HAL_SPI_Init(&hspi2) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN SPI2_Init 2 */

	/* USER CODE END SPI2_Init 2 */

}

/**
 * @brief TIM2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM2_Init(void) {

	/* USER CODE BEGIN TIM2_Init 0 */

	/* USER CODE END TIM2_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };

	/* USER CODE BEGIN TIM2_Init 1 */

	/* USER CODE END TIM2_Init 1 */
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 0;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 65535;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim2) != HAL_OK) {
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM2_Init 2 */

	/* USER CODE END TIM2_Init 2 */

}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void) {

	/* USER CODE BEGIN USART1_Init 0 */

	/* USER CODE END USART1_Init 0 */

	/* USER CODE BEGIN USART1_Init 1 */

	/* USER CODE END USART1_Init 1 */
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART1_Init 2 */

	/* USER CODE END USART1_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	/* USER CODE BEGIN MX_GPIO_Init_1 */
	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, PWM_Pin | LD1_Pin | LD2_Pin | LD3_Pin | LD4_Pin,
			GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, CS_Pin | LED_LCD_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pins : PWM_Pin LD1_Pin LD2_Pin LD3_Pin
	 LD4_Pin */
	GPIO_InitStruct.Pin = PWM_Pin | LD1_Pin | LD2_Pin | LD3_Pin | LD4_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : PB0 PB1 PB2 */
	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : CS_Pin LED_LCD_Pin */
	GPIO_InitStruct.Pin = CS_Pin | LED_LCD_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : BT1_Pin BT2_Pin BT3_Pin */
	GPIO_InitStruct.Pin = BT1_Pin | BT2_Pin | BT3_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);

	HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI1_IRQn);

	HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI2_IRQn);

	/* USER CODE BEGIN MX_GPIO_Init_2 */
	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
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
