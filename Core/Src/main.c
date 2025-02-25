/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
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
#include "stdbool.h"
#include <stdint.h>
#include <stdio.h>

#include "7SEG.h"
#include "CLCD.h"
#include "variables.h"
#include "initprocess.h"
#include "alarm.h"
#include "clock.h"
#include "timer.h"
#include "stopwatch.h"
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
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim7;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM7_Init(void);
static void MX_TIM2_Init(void);
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
	MX_TIM7_Init();
	MX_TIM2_Init();

	/* Initialize interrupts */
	MX_NVIC_Init();
	/* USER CODE BEGIN 2 */
	Init_Peripherals(&htim7);
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		switch (Watch_Mode) {
		case MODE_CLOCK:
			Clock_basic_operation();
			Clock_display_operation();
			Clock_button_operation();
			break;
		case MODE_ALARM:
			Alarm_basic_operation();
			Alarm_display_operation();
			Alarm_button_operation();
			break;
		case MODE_STOPWATCH:
			Stopwatch_basic_operation();
			Stopwatch_display_operation();
			Stopwatch_button_operation();
			break;
		case MODE_TIMER:
			Timer_basic_operation();
			Timer_display_operation();
			Timer_button_operation();
			break;
		}
		if (Watch_Mode > MODE_TIMER) {
			Watch_Mode = MODE_CLOCK;
		}
		Alarm_Check();
		Init_button_operation();
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
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
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief NVIC Configuration.
 * @retval None
 */
static void MX_NVIC_Init(void) {
	/* TIM7_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(TIM7_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM7_IRQn);
	/* EXTI3_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI3_IRQn);
	/* EXTI4_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI4_IRQn);
	/* EXTI15_10_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
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
	TIM_OC_InitTypeDef sConfigOC = { 0 };

	/* USER CODE BEGIN TIM2_Init 1 */

	/* USER CODE END TIM2_Init 1 */
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 168 - 1;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 1000 - 1;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_Base_Init(&htim2) != HAL_OK) {
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_TIM_PWM_Init(&htim2) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 500;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_ENABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1)
			!= HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM2_Init 2 */

	/* USER CODE END TIM2_Init 2 */
	HAL_TIM_MspPostInit(&htim2);

}

/**
 * @brief TIM7 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM7_Init(void) {

	/* USER CODE BEGIN TIM7_Init 0 */

	/* USER CODE END TIM7_Init 0 */

	TIM_MasterConfigTypeDef sMasterConfig = { 0 };

	/* USER CODE BEGIN TIM7_Init 1 */

	/* USER CODE END TIM7_Init 1 */
	htim7.Instance = TIM7;
	htim7.Init.Prescaler = 84 - 1;
	htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim7.Init.Period = 1000 - 1;
	htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim7) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM7_Init 2 */

	/* USER CODE END TIM7_Init 2 */

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
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOE,
			GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7
					| GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_5, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14,
			GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);

	/*Configure GPIO pins : PE2 PE4 PE5 PE6
	 PE7 PE0 PE1 */
	GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6
			| GPIO_PIN_7 | GPIO_PIN_0 | GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	/*Configure GPIO pin : PE3 */
	GPIO_InitStruct.Pin = GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	/*Configure GPIO pin : PC15 */
	GPIO_InitStruct.Pin = GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : PB0 PB5 */
	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : PD10 PD4 */
	GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	/*Configure GPIO pins : PD12 PD13 PD14 */
	GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	/*Configure GPIO pin : PC6 */
	GPIO_InitStruct.Pin = GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/* USER CODE BEGIN MX_GPIO_Init_2 */
	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void updateBuzzer() {
    if (!buzzer) return;
    if (Press_Time > 0 && Press_Time <= 100) {
        TIM2->ARR = 1000 - 1;
        TIM2->CCR1 = 500;
    } else if (Press_Time >= 700 && Press_Time <= 800) {
        TIM2->ARR = 800 - 1;
        TIM2->CCR1 = 400;
    } else if (Press_Time >= 2500 && Press_Time <= 2600) {
        TIM2->ARR = 600 - 1;
        TIM2->CCR1 = 300;
    } else {
        TIM2->CCR1 = 0;
    }
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
}
void update_Time() {
    if (++clock_time == 1000) {
        clock_time = 0;
        if (++second == 60) {
            second = 0;
            if (++minute == 60) {
                minute = 0;
                if (++hour == 24) {
                    hour = 0;
                    day++;
                }
            }
        }
    }
}
void handleAlarmOrTimer(bool condition, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    if (condition) {
        if (clock_time % 100 < 50) {
            HAL_GPIO_TogglePin(GPIOx, GPIO_Pin);
            if (buzzer){
            	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
            	TIM2->ARR = 1000 - 1; // 주파수 초기값으로 되돌림
            	TIM2->CCR1 = 500; // 듀티 사이클 초기값으로 되돌림
            }
        }
        else {
            if (buzzer){
            	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
            	TIM2->ARR = 1000 - 1; // 주파수 초기값으로 되돌림
            	TIM2->CCR1 = 0; // 듀티 사이클 0으로 설정

            }
        }
    }
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) { //타이머 인터럽트 호출
	if (htim->Instance == TIM7) { //1ms 마다 타이머 인터럽트
		Run_Time++; //실행 시간 증가
		update_Time();
		if ((sw1 || sw2 || sw3 || sw4) && Press_Time <= 2610) {
		   Press_Time++;
		}
		if (stopwatch_running) stopwatch_time++;
		if (timer_start && timer_time_tmp > 0) timer_time_tmp--;
		updateBuzzer();
		handleAlarmOrTimer(alarm_ring && !alarm_setmode, GPIOB, GPIO_PIN_0);
		handleAlarmOrTimer(timer_time_tmp == 0 && timer_start, GPIOD, GPIO_PIN_13);

	}
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) { //외부 인터럽트 호출
	if (GPIO_Pin == GPIO_PIN_3) {
		if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == GPIO_PIN_SET) { // GPIOE 포트의 GPIO_PIN_3 핀이 현재 눌린 상태(높은 전압 상태)인지 확인
			if (sw1 == false) { // 이전 상태는 눌리지 않은 상태
				sw1 = true; // 이전 상태를 현재 상태로 업데이트(스위치가 눌렸음을 표시)
				HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
				Press_Time = 0; // sw1이 눌릴 때 Press_Time 초기화

			}
		}

		else {	// GPIOE 포트의 GPIO_PIN_3 핀이 현재 떼진 상태(낮은 전압 상태)인지 확인
			if (sw1 == true) {	// 이전 상태는 눌린 상태
				sw1 = false; // 이전 상태를 현재 상태로 업데이트(스위치가 떼졌음을 표시)
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
				sw1_released = true; // sw1_released 변수를 true로 설정하여 디바운스 처리 완료 표시
				Press_Time = 0;	// sw1이 떼질 때 Press_Time 초기화


				if (!alarm_ring) {
					if (Press_Mode == PRESS_SHORT && !clock_setmode && !timer_setmode
							&& !alarm_setmode) { //짧게 누르고 수정상태 아니면 모드 변경
						Watch_Mode++;
						mode_changed = true;
					}

					if (Press_Mode == PRESS_SHORT && clock_setmode) { //짧게 누르고 수정상태이면 수정값 증가시키기
						item_select++;
						if (item_select == 6) {
							item_select = 0;
						}
					}

					if (Press_Mode == PRESS_SHORT && timer_setmode) { //짧게 누르고 수정상태이면 수정값 증가시키기
						item_select2++;
						if (item_select2 == 3) {
							item_select2 = 0;
						}
					}

					if (Press_Mode == PRESS_SHORT && alarm_setmode) { //짧게 누르고 수정상태이면 수정값 증가시키기
						item_select3++;
						if (item_select3 == 4) {
							item_select3 = 0;
						}
					}
				}
				if (alarm_ring && !alarm_setmode) {
					alarm_ring = false;
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
				}
			}
		}
	}

	else if (GPIO_Pin == GPIO_PIN_15) {
		if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) == GPIO_PIN_SET) { // GPIOC 포트의 GPIO_PIN_15 핀이 현재 눌린 상태(높은 전압 상태)인지 확인
			if (sw2 == false) { 	// 이전 상태는 눌리지 않은 상태
				sw2 = true; 	// 이전 상태를 현재 상태로 업데이트(스위치가 눌렸음을 표시)
				HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
				Press_Time = 0; 	// sw2이 눌릴 때 Press_Time 초기화
			}
		} else { 	// GPIOC 포트의 GPIO_PIN_15 핀이 현재 떼진 상태(낮은 전압 상태)인지 확인
			if (sw2 == true) { 	// 이전 상태는 눌린 상태
				sw2 = false; 	// 이전 상태를 현재 상태로 업데이트(스위치가 떼졌음을 표시)
				sw2_released = true; //
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
				Press_Time = 0; 	// sw1이 떼질 때 Press_Time 초기화


				if (timer_time_tmp == 0 && timer_start && Watch_Mode != MODE_TIMER) {
					timer_start = !timer_start;
				}
			}
		}
	}

	else if (GPIO_Pin == GPIO_PIN_4) {
		if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_4) == GPIO_PIN_SET) {
			if (sw3 == false) {
				sw3 = true;
				HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
				sw3_pressed = true;
				Press_Time = 0; // sw1이 눌릴 때 Press_Time 초기화
			}
		} else {
			if (sw3 == true) {
				sw3 = false;
				sw3_pressed = false;
				sw3_released = true;
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
				Press_Time = 0;
			}
		}
	}

	else if (GPIO_Pin == GPIO_PIN_10) {
		if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_10) == GPIO_PIN_SET) {
			if (sw4 == false) {
				sw4 = true;
				HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
				Press_Time = 0; // sw1이 눌릴 때 Press_Time 초기화
			}
		} else {
			if (sw4 == true) {
				sw4 = false;
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
				sw4_released = true;
				Press_Time = 0;
			}
		}
	}
}
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
